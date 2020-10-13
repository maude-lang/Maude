/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class VariantMatchingProblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"
#include "freshVariableGenerator.hh"
#include "narrowingVariableInfo.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "variantFolder.hh"
#include "variantMatchingProblem.hh"

VariantMatchingProblem::VariantMatchingProblem(VariantFolder* pattern,
					       RewritingContext* subject,
					       FreshVariableGenerator* freshVariableGenerator)
  : pattern(pattern),
    subject(subject),
    freshVariableGenerator(freshVariableGenerator)
{
  checkedVariablesInSubject = false;
  indexOfLastUsedVariant = -1;
  matcher = 0;
  subproblem = 0;
  currentVariant = 0;
}

VariantMatchingProblem::~VariantMatchingProblem()
{
  delete matcher;
  delete subproblem;
  delete subject;
}

void
VariantMatchingProblem::checkVariablesInSubject()
{
  //
  //	We examine any variables that appear in the subject. We're treating
  //	them as constants but because we need fresh variables we need to worry
  //	about any fresh variable names that appear here to avoid confusion.
  //
  NarrowingVariableInfo variableInfo;
  subject->root()->indexVariables(variableInfo, 0);
  int nrVariables = variableInfo.getNrVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      int variableName = variableInfo.index2Variable(i)->id();
      int index;
      int family;
      if (freshVariableGenerator->isFreshVariableName(variableName, index, family) && family == 0)
	{
	  //
	  //	This is one of the # variables that we use so we must avoid it.
	  //
	  indicesToAvoid.insert(index);
	}
    }
  checkedVariablesInSubject = true;
}

void
VariantMatchingProblem::markReachableNodes()
{
  for (DagNode* i : currentMatcher)
    i->mark();
}

bool
VariantMatchingProblem::findNextMatcher()
{
  if (subproblem != 0 && subproblem->solve(false, *matcher))
    {
      //
      //	We found another solution to the matching problem with the
      //	last used variant.
      //
    }
  else
    {
      //
      //	Delete any old results.
      //
    nextVariant:
      delete matcher;
      delete subproblem;
      matcher = 0;
      subproblem = 0;
      const VariableInfo* variableInfo;
      currentVariant = pattern->findNextVariantThatMatches(indexOfLastUsedVariant,
							   subject->root(),
							   variableInfo,
							   matcher,
							   subproblem);
      if (currentVariant == 0)
	return false;  // no more variants match
      if (subproblem != 0 && !(subproblem->solve(true, *matcher)))
	goto nextVariant;
      //
      //	Need to worry about unbound entries in matcher - should get fresh variables.
      //
      int nrFreeVariables = variableInfo->getNrRealVariables();
      int variableIndex = 0;
      for (int i = 0; i < nrFreeVariables; ++i)
	{
	  if (matcher->value(i) == 0)
	    {
	      //
	      //	We have a free variable in the variant substitution that did
	      //	not appear in the variant term, and hence did not get a value
	      //	when the variant term was matched against the subject.
	      //	This happens when we do variant narrowing with non-regular variant
	      //	equations such as:
	      //	  eq X xor X = 0 .
	      //	Here whatever unified with X, including any variables it might
	      //	contain, will disappear from the new variant term.
	      //	We treat such variables as fresh variables.
	      //	We always use # family for such variables.
	      //
	      //	We need to check what # variables appear in subject so we can
	      //	avoid them.
	      //
	      if (!checkedVariablesInSubject)
		checkVariablesInSubject();
	      
	      Term* variableInVariantSubstitution = variableInfo->index2Variable(i);
	      Symbol* variableSymbol = variableInVariantSubstitution->symbol();
	      //
	      //	Find a # variable that doesn't appear in the subject.
	      //
	      while (indicesToAvoid.find(variableIndex) != indicesToAvoid.end())
		++variableIndex;
	      int freshName = freshVariableGenerator->getFreshVariableName(variableIndex, 0);
	      ++variableIndex;
	      VariableDagNode* v = new VariableDagNode(variableSymbol, freshName, i);
	      matcher->bind(i, v);
	    }
	}
    }
  //
  //	Apply matcher to the current variant substitution to create the current variant matcher.
  //
  int nrPatternVariables = currentVariant->size() - 1;
  currentMatcher.resize(nrPatternVariables);
  for (int i = 0; i < nrPatternVariables; ++i)
    {
      DagNode* d = (*currentVariant)[i];
      DagNode* inst = d->instantiate(*matcher);
      currentMatcher[i] = (inst == 0) ? d : inst;
    }

  return true;
}
