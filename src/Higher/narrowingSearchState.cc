/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class NarrowingSearchState.
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

//	core class definitions
#include "rewritingContext.hh"
#include "rhsAutomaton.hh"
#include "rule.hh"
#include "freshVariableGenerator.hh"

//	variable class definitions
#include "variableDagNode.hh"

//	higher class definitions
#include "narrowingUnificationProblem.hh"
#include "narrowingSearchState.hh"

NarrowingSearchState::NarrowingSearchState(RewritingContext* context,
					   FreshVariableGenerator* freshVariableGenerator,
					   int label,
					   int flags,
					   int minDepth,
					   int maxDepth)
  : PositionState(context->root(), flags, minDepth, maxDepth),
    context(context),
    freshVariableGenerator(freshVariableGenerator),    
    label(label),
    withExtension(maxDepth >= 0)
{
  ruleIndex = -1;

  DagNode* target = context->root();
  Module* module = target->symbol()->getModule();
  int firstTargetSlot = module->getMinimumSubstitutionSize();

  context->root()->indexVariables(variableInfo, firstTargetSlot);
  //cout << context->root() << " has " << variableInfo.getNrVariables() << " variables\n";
  /*
  int nrVariables = variableInfo.getNrVariables();
  for (int i = 0; i < nrVariables; ++i)
    {
      VariableDagNode* v = variableInfo.index2Variable(i);
      cout << i << '\t' << static_cast<DagNode*>(v) << '\t' << v->getIndex() << endl;
    }
  */
  unificationProblem = 0;
  noFurtherPositions = false;
}

NarrowingSearchState::~NarrowingSearchState()
{
  delete unificationProblem;
  delete context;
}

bool
NarrowingSearchState::findNextNarrowing()
{
  if (ruleIndex > -1)
    {
      //
      //	Already underway; check for another unifier between the current position and
      //	with the current rule.
      //
      if (unificationProblem->findNextUnifier())
	return true;
      delete unificationProblem;
    }
  else
    {
      //
      //	First call - find an initial position.
      //
      if (!findNextPosition())
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
  bool allowNonexec = getFlags() & ALLOW_NONEXEC;
  do
    {
      //
      //	We don't consider the possibility that our current position,
      //	getDagNode(), might collapse into another theory under unification.
      //
      // //cout << " at " << getDagNode() << endl;
      const Vector<Rule*>& rules = getDagNode()->symbol()->getRules();
      for (int nrRules = rules.length(); ruleIndex < nrRules; ruleIndex++)
	{
	  Rule* rl = rules[ruleIndex];
	  if (!(rl->hasCondition())  &&  // we don't attempt narrowing with conditional rules
	      (allowNonexec || !(rl->isNonexec())) &&  // check executability
	      (label == UNDEFINED || rl->getLabel().id() == label))  // check label
	    {
	      ////cout << "trying rule " << ruleIndex << " " << rl << " at " << getDagNode() << endl;
	      unificationProblem = new NarrowingUnificationProblem(rl,
								   getDagNode(),
								   variableInfo,
								   freshVariableGenerator);
	      if (unificationProblem->findNextUnifier())
		{
		  noFurtherPositions = getFlags() & SINGLE_POSITION;
		  return true;
		}
	      delete unificationProblem;
	    }
	}
      ruleIndex = 0;
    }
  while (!noFurtherPositions && findNextPosition());
  unificationProblem = 0;
  return false;
}

Rule*
NarrowingSearchState::getRule() const
{
  return (getDagNode()->symbol()->getRules())[ruleIndex];
}

DagNode*
NarrowingSearchState::getNarrowedDag(DagNode*& replacement) const
{
  Rule* r = getRule();
  Substitution& s = unificationProblem->getSolution();
  replacement =  r->getRhsBuilder().construct(s);
  //
  //	Need to clear unused entries in solution that we may have touched to avoid confusing
  //	unification algorithm.
  //
  int nrSlots = r->getModule()->getMinimumSubstitutionSize();
  for (int i = r->getNrProtectedVariables(); i < nrSlots; ++i)
    s.bind(i,0);
  
  return rebuildAndInstantiateDag(replacement, s).first;
}

const Substitution&
NarrowingSearchState::getSubstitution() const
{
  return unificationProblem->getSolution();
}
