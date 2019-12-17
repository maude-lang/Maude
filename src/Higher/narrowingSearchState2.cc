/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2016 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingSearchState2.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "higher.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "rhsAutomaton.hh"
#include "rule.hh"
#include "freshVariableGenerator.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "variantUnificationProblem.hh"
#include "narrowingSearchState2.hh"

bool
NarrowingSearchState2::allVariablesBelongToIncomingFamily()
{
  int nrVariables = variableInfo.getNrVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableDagNode* v = variableInfo.index2Variable(i);
      if (!freshVariableGenerator->belongsToFamily(v->id(), incomingVariableFamily))
	return false;
    }
  return true;
}

NarrowingSearchState2::NarrowingSearchState2(RewritingContext* context,
					     const Vector<DagNode*>& blockerDagsArg,
					     FreshVariableGenerator* freshVariableGenerator,
					     int incomingVariableFamily,
					     int flags,
					     int minDepth,
					     int maxDepth)
  : context(context),
    blockerDags(blockerDagsArg),  // shallow copy
    freshVariableGenerator(freshVariableGenerator),
    incomingVariableFamily(incomingVariableFamily),
    module(context->root()->symbol()->getModule())
{
  ruleIndex = -1;  // not yet started
  incompleteFlag = false;
  unificationProblem = 0;
  reverseMapping = 0;

  DagNode* dagToNarrow = context->root();
  newContext = context;  // unless we do a renaming
  //
  //	Each variable occurring in the target dag needs an index
  //	different from any that occurs in a variable occurring in a
  //	rule lhs.
  //
  //	Indexing the variables will convert any persistent representations into
  //	regular representations suitable for unification and instantiation.
  //
  int firstTargetSlot = module->getMinimumSubstitutionSize();
  dagToNarrow->indexVariables(variableInfo, firstTargetSlot);
  int nrVariablesInDagToNarrow = variableInfo.getNrVariables();
  //
  //	We also index variables in blockerDags.
  //
  int nrBlockerDags = blockerDags.size();
  for (int i = 0; i < nrBlockerDags; ++i)
    blockerDags[i]->indexVariables(variableInfo, firstTargetSlot);
  
  if (!allVariablesBelongToIncomingFamily())
    {
      //
      //	We saw at least one variable that didn't belong to incomingVariableFamily.
      //	We need to do a full renaming for safety, since those other variables may
      //	occur in a rule used for narrowing.
      //
      //	This renaming will preserve the slots we just assigned by indexVariables.
      //	We also need to build freshVariableInfo so VariantUnificationProblem
      //	will be able to recover slots from just the fresh variable names.
      //
      //	We also build a substitution, reverseMapping, that takes us from slots to
      //	the original variables names, so we can build a replacement context using
      //	original variable names.
      //
      int nrVariables = variableInfo.getNrVariables();
      int nrSlots = firstTargetSlot + nrVariables;
      Substitution s(nrSlots);
      //
      //	We only build a reverse mapping for variables that actually occur in the dag
      //	we're going to narrow; variables that only occur in blocker dags don't count.
      //
      int nrSlotsForDagToNarrow = firstTargetSlot + nrVariablesInDagToNarrow;
      reverseMapping = new Substitution(nrSlotsForDagToNarrow);
      reverseMapping->clear(nrSlotsForDagToNarrow);
      //
      //	Make a renaming for each variable.
      //
      for (int i = 0; i < nrVariables; ++i)
	{
	  int slotNr = firstTargetSlot + i;
	  Sort* sort = safeCast(VariableSymbol*, variableInfo.index2Variable(i)->symbol())->getSort();
	  VariableDagNode* v = new VariableDagNode(freshVariableGenerator->getBaseVariableSymbol(sort),
						   freshVariableGenerator->getFreshVariableName(i, incomingVariableFamily),
						   slotNr);
	  s.bind(slotNr, v);
	  if (i < nrVariablesInDagToNarrow)
	    {
	      //
	      //	We only track variables that appear in the dag we're going to narrow
	      //	even though all variables need to be renamed.
	      //
	      reverseMapping->bind(slotNr, variableInfo.index2Variable(i));
	      DebugSave(j, freshVariableInfo.variable2Index(v));
	      Assert(j == i, "indexing clash " << i << " vs " << j);
	    }
	}
      //
      //	Make a copy of the dag we want to narrow, with variable replacements.
      //
      DebugAdvisory("old dagToNarrow = " << dagToNarrow);
      if (DagNode* renamedDagToNarrow = dagToNarrow->instantiate(s))  // just in case dagToNarrow was ground
	dagToNarrow = renamedDagToNarrow;
      DebugAdvisory("new dagToNarrow = " << dagToNarrow);
      newContext = context->makeSubcontext(dagToNarrow);
      //
      //	Likewise renaming variables in blockerDags.
      //
      for (int i = 0; i < nrBlockerDags; ++i)
	{
	  if (DagNode* b = blockerDags[i]->instantiate(s))
	    blockerDags[i] = b;
	}
    }
  //
  //	Forget any variables that only occur in blocker dags.
  //	This avoids generating bindings for variables that don't occur in the dag
  //	we're going to narrow. It also means that the caller won't see these in
  //	getVariableInfo().
  //
  variableInfo.forgetAllBut(nrVariablesInDagToNarrow);
  //
  //	Make a PositionState object to traverse it.
  //
  positionState = new PositionState(dagToNarrow, flags, minDepth, maxDepth);
}

NarrowingSearchState2::~NarrowingSearchState2()
{
  bool gcVarGen = positionState->getFlags() & GC_VAR_GEN;
  //
  //	Stuff we created.
  //
  delete reverseMapping;
  delete unificationProblem;
  delete positionState;
  if (newContext != context)
    delete newContext;
  //
  //	Stuff we took responsibility for deleting.
  //
  if (gcVarGen)
    delete freshVariableGenerator;
  delete context;
}

void
NarrowingSearchState2::markReachableNodes()
{
  //
  //	Protect blocker dags; don't relying on VariantSearch object within
  //	any particular VariantUnificationProblem.
  //
  FOR_EACH_CONST(i, Vector<DagNode*>, blockerDags)
    (*i)->mark();
}

bool
NarrowingSearchState2::findNextNarrowing()
{
  if (ruleIndex > -1)
    {
      //
      //	Already underway; check for another unifier between the current position and
      //	with the current rule.
      //
      bool nextUnifier = unificationProblem->findNextUnifier();
      if (context != newContext)
	context->transferCountFrom(*newContext);
      if (nextUnifier)
	return true;  // need to make substitution from variant unifier
      incompleteFlag |= unificationProblem->isIncomplete();
      delete unificationProblem;
    }
  else
    {
      //
      //	First call - find an initial position.
      //
      if (!(positionState->findNextPosition()))
	return false;
    }
  //
  //	Move to the next rule.
  //
  ++ruleIndex;
  //
  //	Check if we are allowed to use nonexecutable rules. A nonexecutable rule may have
  //	unbound variables in the rhs which we will bind to fresh variables.
  //
  bool allowNonexec = positionState->getFlags() & ALLOW_NONEXEC;
  //
  //	Check whether we should use the original variableInfo or the congruent one
  //	we created with fresh variables.
  //
  const NarrowingVariableInfo& varInfo = (freshVariableInfo.getNrVariables() == 0) ? variableInfo : freshVariableInfo;
  //
  //	Look at all positions (respecting depth bounds and, depending on flags, frozenness) in state dag.
  //
  do
    {
      DagNode* d = positionState->getDagNode();
      DebugAdvisory("trying to narrow at " << d);
      if (dynamic_cast<VariableDagNode*>(d) == 0)  // only consider non-variable positions
	{
	  ConnectedComponent* kind = d->symbol()->rangeComponent();
	  //
	  //	Look at all rules with the correct kind; since we are using variant unification, we can't
	  //	easily know in advance which rule lhs might unify.
	  //
	  const Vector<Rule*>& rules = module->getRules();
	  for (int nrRules = rules.length(); ruleIndex < nrRules; ruleIndex++)
	    {
	      Rule* rl = rules[ruleIndex];
	      if (!(rl->hasCondition())  &&  // we don't attempt narrowing with conditional rules
		  (allowNonexec || !(rl->isNonexec())) &&  // check executability
		  rl->isNarrowing() &&  // check narrowing attribute
		  rl->getLhs()->symbol()->rangeComponent() == kind)  // check kind
		{
		  DebugAdvisory("trying rule " << ruleIndex << " " << rl << " at " << d);
		  unificationProblem = new VariantUnificationProblem(newContext,
								     blockerDags,
								     rl,
								     d,
								     varInfo,
								     freshVariableGenerator,
								     incomingVariableFamily);
		  bool nextUnifier = unificationProblem->findNextUnifier();
		  if (context != newContext)
		    context->transferCountFrom(*newContext);
		  if (nextUnifier)
		    return true;
		  incompleteFlag |= unificationProblem->isIncomplete();
		  delete unificationProblem;
		}
	    }
	}
      ruleIndex = 0;
    }
  while (positionState->findNextPosition());
  unificationProblem = 0;
  return false;
}

DagNode*
NarrowingSearchState2::getNarrowedDag(DagNode*& replacement, DagNode*& replacementContext) const
{
  Rule* r = getRule();
  Substitution& s = unificationProblem->getSolution();
  replacement =  r->getRhsBuilder().construct(s);
  //
  //	It's possible r could have a bare rhs and thus replacement could be a dagNode
  //	that is the binding of variable that appears in the larger dag being narrowed.
  //	Thus we might have replacement appearing multiple times in the larger narrowed dag.
  //	This would screw up context generation, which relies on a unique DagNode* marking
  //	the hole. Thus for safety we make a clone and use that.
  //
  replacement = replacement->makeClone();
  //
  //	Need to clear unused entries in solution that we may have touched during rhs build
  //	to avoid confusing unification algorithm.
  //
  int nrSlots = module->getMinimumSubstitutionSize();
  for (int i = r->getNrProtectedVariables(); i < nrSlots; ++i)
    s.bind(i,0);
  //
  //	In order to return the context at when the narrowing happened we need to
  //	make a copy of the original dag, with the replacement clone at the location of the
  //	narrowing.
  //
  //	One complexity is we may have renamed the variables in the dag we actually traversed
  //	in order to avoid variable clashes. If that happened, we use the reverseMapping
  //	to renaming them back when we rebuild the context.
  //
  replacementContext = reverseMapping ?
    positionState->rebuildAndInstantiateDag(replacement, *reverseMapping, nrSlots, nrSlots + variableInfo.getNrVariables() - 1) :
    positionState->rebuildDag(replacement).first;
  //
  //	Finally we compute the actual dag created by the narrowing step by rebuilding
  //	using the unifier.
  //
  return positionState->rebuildAndInstantiateDag(replacement, s, nrSlots, nrSlots + variableInfo.getNrVariables() - 1);
}
