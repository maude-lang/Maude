/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA.

*/

//
//      Implementation of a class for managing a collection of variable bindings with sharing.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	others
#include "variableInfo.hh"
#include "substitution.hh"
#include "dagRoot.hh"
#include "cachedDag.hh"
#include "rewritingContext.hh"
#include "variableDagNode.hh"
#include "dagArgumentIterator.hh"

//	strategy language class definitions
#include "variableBindingsManager.hh"

typedef VariableBindingsManager::ContextId ContextId;

// Static empty vector
const Vector<DagNode*> VariableBindingsManager::emptyVector;

VariableBindingsManager::VariableBindingsManager(int substitutionSize)
  : contextShare(DeepComparison(*this)),
    contextTable(0),
    substitution(substitutionSize, 0),
    currentContext(EMPTY_CONTEXT)
{
  // A empty binding can be made to make usage easier
}

VariableBindingsManager::~VariableBindingsManager()
{
  for (Entry* entry : contextTable)
    delete entry;
}

ContextId
VariableBindingsManager::openContext(const Substitution& subst,
				     const Vector<int>& contextSpec,
				     void* filter)
{
  // The position of contextTable where we will write the entry
  int insertionPoint;

  if (!freeCells.empty())
    {
      insertionPoint = freeCells.front();
      freeCells.pop();
    }
  else
    {
      insertionPoint = contextTable.length();
      contextTable.expandBy(1);
      contextTable[insertionPoint] = new Entry;
      contextTable[insertionPoint]->shareRef = contextShare.end();
    }

  // The number of variables in the context
  size_t nrVars = contextSpec.length();
  contextTable[insertionPoint]->init(nrVars);

  Vector<DagNode*>& values = contextTable[insertionPoint]->values;

  for (size_t i = 0; i < nrVars; i++)
    values[i] = subst.value(contextSpec[i]);

  // Checks if the context already exists
  if (filter != nullptr)
    {
      ShareSet::const_iterator alreadyExist = contextShare.find(make_pair(filter, insertionPoint));

      if (alreadyExist != contextShare.end())
	{
	  closeContext(insertionPoint);
	  contextTable[alreadyExist->second]->usersCount++;
	  insertionPoint = alreadyExist->second;
	}
      else
	contextTable[insertionPoint]->shareRef =
	  contextShare.insert(make_pair(filter, insertionPoint)).first;
    }

  return insertionPoint;
}

ContextId VariableBindingsManager::openContext(ContextId parent,
					       const Substitution& match,
					       const Vector<int>& contextSpec)
{
  // The position of contextTable where we will write the entry
  int insertionPoint;

  if (!freeCells.empty())
    {
      insertionPoint = freeCells.front();
      freeCells.pop();
    }
  else
    {
      insertionPoint = contextTable.length();
      contextTable.expandBy(1);
      contextTable[insertionPoint] = new Entry;
      contextTable[insertionPoint]->shareRef = contextShare.end();
    }

  // The number of variables in the context
  size_t nrVars = contextSpec.length();
  contextTable[insertionPoint]->init(nrVars);

  Vector<DagNode*>& values = contextTable[insertionPoint]->values;

  for (size_t i = 0; i < nrVars; i++)
    values[i] = contextSpec[i] >= 0
		    ? match.value(contextSpec[i])
		    : contextTable[parent]->values[-contextSpec[i] - 1];

  return insertionPoint;
}

DagNode*
VariableBindingsManager::instantiate(ContextId ctx, DagNode* original) const
{
  // Note: this function is not reentrant

  // No variables in the context
  if (ctx == EMPTY_CONTEXT)
    return original;

  // Prepares the substitution if needed
  if (currentContext != ctx)
    {
      const Vector<DagNode*>& values = contextTable[ctx]->values;
      size_t nrVars = values.length();

      for (size_t i = 0; i < nrVars; i++)
	substitution.bind(i, values[i]);

      currentContext = ctx;
    }

  return original->instantiate(substitution, true);  // passing true for safety
}

void
VariableBindingsManager::buildInitialSubstitution(ContextId ctx,
						  VariableInfo& vinfo,
						  const Vector<std::pair<int, int> >& indexTranslation,
						  Vector<Term*>& vars,
						  Vector<DagRoot*>& values) const
{
  size_t nrVars = indexTranslation.length();
  vars.resize(nrVars);
  values.resize(nrVars);

  for (size_t i = 0; i < nrVars; i++)
    {
      vars[i] = vinfo.index2Variable(indexTranslation[i].first)->deepCopy();
      values[i] = new DagRoot(contextTable[ctx]->values[indexTranslation[i].second]);
    }
}

void
VariableBindingsManager::Entry::markReachableNodes()
{
  for (DagNode* dag : values)
    dag->mark();
}

inline void
VariableBindingsManager::Entry::init(size_t nrVars)
{
  usersCount = 1;
  values.expandTo(nrVars);
  //  link();
}

inline
VariableBindingsManager::DeepComparison::DeepComparison(VariableBindingsManager& vbm)
  : vbm(vbm)
{
}

bool VariableBindingsManager::DeepComparison::operator()(const VariableBindingsManager::SearchEntry& lhs,
							 const VariableBindingsManager::SearchEntry& rhs) const
{
  // Compares the filter
  if (lhs.first < rhs.first)	return true;
  if (lhs.first > rhs.first)	return false;

  // Then, we compare the entries
  const Vector<DagNode*> &lhsEntries = vbm.contextTable[lhs.second]->values;
  const Vector<DagNode*> &rhsEntries = vbm.contextTable[rhs.second]->values;

  size_t nrEntries = lhsEntries.length();
  if (nrEntries < rhsEntries.size())		return true;
  else if (nrEntries > rhsEntries.size())	return false;

  for (size_t i = 0; i < nrEntries; i++)
    {
      int comparison = lhsEntries[i]->compare(rhsEntries[i]);

      if (comparison < 0)	return true;
      else if (comparison > 0)	return false;
    }

  return false;
}
