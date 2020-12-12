/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingSearchState3.
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
#include "narrowingSearchState3.hh"

NarrowingSearchState3::NarrowingSearchState3(RewritingContext* context,
					     Substitution* accumulatedSubstitution,
					     FreshVariableGenerator* freshVariableGenerator,
					     int incomingVariableFamily,
					     int flags,
					     int minDepth,
					     int maxDepth,
					     int variantFlags)
  : PositionState(context->root(), flags, minDepth, maxDepth),
    context(context),
    accumulatedSubstitution(accumulatedSubstitution),
    freshVariableGenerator(freshVariableGenerator),
    incomingVariableFamily(incomingVariableFamily),
    variantFlags(variantFlags),
    module(context->root()->symbol()->getModule())
{
  ruleIndex = -1;  // not yet started
  incompleteFlag = false;
  unificationProblem = 0;
  //
  //	Need to index the dag we're narrowing and the bindings
  //	in the accumulated substitution using a common scheme since
  //	the variant unifiers we find are going to be accumulated.
  //	The indices must start above those used by statements in the module.
  //
  DagNode* dagToNarrow = context->root();
  int firstTargetSlot = module->getMinimumSubstitutionSize();
  dagToNarrow->indexVariables(variableInfo, firstTargetSlot);
  int nrBindings = accumulatedSubstitution->nrFragileBindings();
  for (int i = 0; i < nrBindings; ++i)
    accumulatedSubstitution->value(i)->indexVariables(variableInfo, firstTargetSlot);
}

NarrowingSearchState3:: ~NarrowingSearchState3()
{
  delete unificationProblem;
  if (getFlags() & GC_VAR_GEN)
    delete freshVariableGenerator;
  delete context;
}

bool
NarrowingSearchState3::findNextNarrowing()
{
  if (ruleIndex > -1)
    {
      //
      //	Already underway; check for another unifier between the current position and
      //	with the current rule.
      //
      bool nextUnifier = unificationProblem->findNextUnifier();
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
      if (!(findNextPosition()))
	return false;
    }
  //
  //	Move to the next rule, at current position; if we are forced to move to another
  //	position we will start again at 0.
  //
  ++ruleIndex;
  //
  //	Check if we are allowed to use nonexecutable rules. A nonexecutable rule may have
  //	unbound variables in the rhs which we will bind to fresh variables.
  //
  bool allowNonexec = getFlags() & ALLOW_NONEXEC;
  //
  //	Look at all positions (respecting depth bounds and, depending on flags, frozenness) in state dag.
  //
  do
    {
      DagNode* d = getDagNode();
      DebugAdvisory("trying to narrow at " << d);
      if (dynamic_cast<VariableDagNode*>(d) == 0)  // only consider non-variable positions
	{
	  ConnectedComponent* kind = d->symbol()->rangeComponent();
	  //
	  //	Look at all rules with the correct kind; since we are using variant unification, we can't
	  //	easily know in advance which rule lhs might unify with.
	  //
	  const Vector<Rule*>& rules = module->getRules();
	  for (int nrRules = rules.length(); ruleIndex < nrRules; ++ruleIndex)
	    {
	      Rule* rl = rules[ruleIndex];
	      if (!(rl->hasCondition())  &&  // we don't attempt narrowing with conditional rules
		  (allowNonexec || !(rl->isNonexec())) &&  // check executability
		  rl->isNarrowing() &&  // check narrowing attribute
		  rl->getLhs()->symbol()->rangeComponent() == kind)  // check kind
		{
		  DebugAdvisory("trying rule " << ruleIndex << " " << rl << " at " << d);
		  unificationProblem = new VariantUnificationProblem(context,
								     blockerDags,
								     rl,
								     d,
								     variableInfo,
								     freshVariableGenerator,
								     incomingVariableFamily,
								     variantFlags);
		  bool nextUnifier = unificationProblem->findNextUnifier();
		  if (nextUnifier)
		    return true;
		  incompleteFlag |= unificationProblem->isIncomplete();
		  delete unificationProblem;
		}
	    }
	}
      ruleIndex = 0;
    }
  while (findNextPosition());
  unificationProblem = 0;
  return false;
}

DagNode*
NarrowingSearchState3::getNarrowedDag(DagNode*& replacement, DagNode*& replacementContext) const
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
  replacementContext = rebuildDag(replacement).first;
  //
  //	Finally we compute the actual dag created by the narrowing step by rebuilding
  //	using the unifier.
  //
  return rebuildAndInstantiateDag(replacement, s, nrSlots, nrSlots + variableInfo.getNrVariables() - 1);
}

Substitution*
NarrowingSearchState3::makeAccumulatedSubstitution()
{
  int nrBindings = accumulatedSubstitution->nrFragileBindings();
  Substitution* newAccumulatedSubstitution = new Substitution(nrBindings);
  Substitution& s = unificationProblem->getSolution();
  for (int i = 0; i < nrBindings; ++i)
    {
      DagNode* oldBinding = accumulatedSubstitution->value(i);
      DagNode* newBinding = oldBinding->instantiate(s, true);  // do we need to maintainInvariants?
      newAccumulatedSubstitution->bind(i, (newBinding == 0) ? oldBinding : newBinding);
    }
  return newAccumulatedSubstitution;
}
