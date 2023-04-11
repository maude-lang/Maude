/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for managing a collection of variable bindings.
//
//	In the strategy language, variable are binded in the matching of the
//	main pattern of the matchrew strategy and in strategy calls.
//
//	Variable names may be task local.
//
#ifndef _variableBindingsManager_hh_
#define _variableBindingsManager_hh_

#include "rootContainer.hh"
#include "substitution.hh"
#include <queue>

class VariableBindingsManager
{
public:
  typedef int ContextId;

  enum Constants
  {
    EMPTY_CONTEXT = -1
  };

  VariableBindingsManager(int substitutionSize);
  ~VariableBindingsManager();

  //
  // Opens a subcontext.
  //
  // There two alternative functions. In any of them, the contextSpec specifies
  // the origin of the context variables, where a positive index always means
  // that the variable value comes from the given substitution.
  //

  //
  // Opens a strategy call context. All variables come form the substitution.
  //
  ContextId openContext(const Substitution& match,
			const Vector<int>& contextSpec);

  //
  // Opens a static context inside expressions (for matchrew). Negative indices
  // (in absolute value less one) refer to indices in the parent context.
  //
  ContextId openContext(ContextId parent, const Substitution& subst, const Vector<int>& contextSpec);

  //
  // Closes a context, freeing its resources.
  //
  void closeContext(ContextId ctx);

  //
  // Instantiates a DAG node in a context.
  //
  DagNode* instantiate(ContextId ctx, DagNode* original) const;

  //
  // Builds a pair of vectors of variables and values to set the initial
  // substitution in SearchState and its subtypes. An index translation
  // maps indices in the search pattern (vinfo) to indices in the given
  // context.
  //
  void buildInitialSubstitution(ContextId ctx,
				VariableInfo& vinfo,
				const Vector<std::pair<int, int> >& indexTranslation,
				Vector<Term*>& vars,
				Vector<DagRoot*>& values) const;

  //
  // Get all values in the given context, in the order of the specification
  // provided when it was opened.
  //
  const Vector<DagNode*>& getValues(ContextId id) const;

private:
  struct Entry : RootContainer
  {
    Entry() {link();}
    ~Entry() {unlink();}

    Vector<DagNode*> values;

    void markReachableNodes();
    void init(size_t nrVars);
    void free();
  };

  // Table of contexts by its context identifier
  Vector<Entry*> contextTable;
  // Queue of free cells (after some context are closed)
  std::queue<int> freeCells;
  // 
  // Cached substitution
  //
  // In the instantiate method, a Substitution is required to instantiate the
  // terms, so we build it from the given context table entry. Trying to reuse
  // that work when the same context is used multiple consecutive times, we
  // keep the substitution cached.
  //
  mutable Substitution substitution;
  mutable ContextId currentContext;

  static const Vector<DagNode*> emptyVector;
};

inline void
VariableBindingsManager::closeContext(ContextId ctx)
{
  if (ctx != EMPTY_CONTEXT)
    {
      // Performs the cell deletion
      contextTable[ctx]->free();
      freeCells.push(ctx);

      if (currentContext == ctx)
	currentContext = EMPTY_CONTEXT;
    }
}

inline const Vector<DagNode*>&
VariableBindingsManager::getValues(ContextId ctx) const
{
  return ctx == EMPTY_CONTEXT ? emptyVector
			      : contextTable[ctx]->values;
}

inline void
VariableBindingsManager::Entry::free()
{
  values.clear();
  //  unlink();
}

#endif
