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
//	Implementation for class FreeRemainder.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "fullCompiler.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "subproblem.hh"

//      core class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "rewritingContext.hh"
#include "equation.hh"
#include "subproblemSequence.hh"
#include "subproblemAccumulator.hh"

//	full compiler class definitions
#include "compilationContext.hh"

//	free theory class definitions
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeDagNode.hh"
#include "freeOccurrence.hh"
#include "freeLhsAutomaton.hh"
#include "freeRhsAutomaton.hh"
#include "freeTerm.hh"
#include "freeRemainder.hh"

FreeRemainder::FreeRemainder(Equation* eqn,
			     const Vector<FreeOccurrence>& freeSymbols,
			     const Vector<FreeOccurrence>& freeVars,
			     const Vector<FreeOccurrence>& boundVars,
			     const Vector<FreeOccurrence>& gndAliens,
			     const Vector<FreeOccurrence>& nonGndAliens,
			     const Vector<int>& bestSequence,
			     const Vector<LhsAutomaton*>& subAutomata,
			     const Vector<int>& slotTranslation)
  : freeVariables(freeVars.length()),
    boundVariables(boundVars.length()),
    groundAliens(gndAliens.length()),
    nonGroundAliens(nonGndAliens.length())
{
  //
  //	Preliminary deternimation of whether remainder will qualify
  //	for "fast" runtime treatment
  //
  fast = !(eqn->hasCondition());
  foreign = false;
  equation = eqn;
  {
    //
    //  Variables that will be unbound.
    //
    int nrFreeVariables = freeVars.length();
    for (int i = 0; i < nrFreeVariables; i++)
      {
        const FreeOccurrence& oc = freeVars[i];
        FreeTerm* parent = safeCast(FreeTerm*, freeSymbols[oc.position()].term());
	Assert(parent->getSlotIndex() != NONE,
	       "bad slot for " << parent << " in " << eqn);
        VariableTerm* v = safeCast(VariableTerm*, oc.term());
        freeVariables[i].position = slotTranslation[parent->getSlotIndex()];
        freeVariables[i].argIndex = oc.argIndex();
        freeVariables[i].varIndex = v->getIndex();
	Sort* sort = v->getSort();
	if (fast > 0 && !(sort->errorFreeMaximal()))
	  fast = - fast;
        freeVariables[i].sort = sort;
      }
  }
  {
    //
    //	Pseudo variables for left to right sharing.
    //
    int nrFreeSymbols = freeSymbols.length();
    for (int i = 0; i < nrFreeSymbols; i++)
      {
	const FreeOccurrence& oc = freeSymbols[i];
	FreeTerm* f = safeCast(FreeTerm*, oc.term());
	int index = f->getSaveIndex();
	if (index != NONE)
	  {
	    FreeTerm* parent = safeCast(FreeTerm*, freeSymbols[oc.position()].term());
	    Assert(parent->getSlotIndex() != NONE,
		   "bad slot for " << parent << " in " << eqn);
	    int j = freeVariables.length();
	    freeVariables.expandTo(j + 1);
	    freeVariables[j].position = slotTranslation[parent->getSlotIndex()];
	    freeVariables[j].argIndex = oc.argIndex();
	    freeVariables[j].varIndex = index;
	    freeVariables[j].sort = f->getComponent()->sort(0);  // use ERROR sort
	  }
      }
  }
  {
    //
    //  Variables that will be bound
    //
    int nrBoundVariables = boundVars.length();
    for (int i = 0; i < nrBoundVariables; i++)
      {
        const FreeOccurrence& oc = boundVars[i];
        FreeTerm* parent = safeCast(FreeTerm*, freeSymbols[oc.position()].term());
	Assert(parent->getSlotIndex() != NONE,
	       "bad slot for " << parent << " in " << eqn);
        VariableTerm* v = safeCast(VariableTerm*, oc.term());
        boundVariables[i].position = slotTranslation[parent->getSlotIndex()];
        boundVariables[i].argIndex = oc.argIndex();
        boundVariables[i].varIndex = v->getIndex();
	fast = false;
      }
  }
  {
    //
    //  Ground alien subterms
    //
    int nrGroundAliens = gndAliens.length();
    for (int i = 0; i < nrGroundAliens; i++)
      {
        const FreeOccurrence& oc = gndAliens[i];
        FreeTerm* parent = safeCast(FreeTerm*, freeSymbols[oc.position()].term());
	Assert(parent->getSlotIndex() != NONE,
	       "bad slot for " << parent << " in " << eqn);
        groundAliens[i].position = slotTranslation[parent->getSlotIndex()];
        groundAliens[i].argIndex = oc.argIndex();
        groundAliens[i].alien = oc.term();
	fast = false;
      }
  }
  {
    //
    //  Non-ground alien subterms
    //
    int nrNonGroundAliens = nonGndAliens.length();
    for (int i = 0; i < nrNonGroundAliens; i++)
      {
        const FreeOccurrence& oc = nonGndAliens[bestSequence[i]];
        FreeTerm* parent = safeCast(FreeTerm*, freeSymbols[oc.position()].term());
	Assert(parent->getSlotIndex() != NONE,
	       "bad slot for " << parent << " in " << eqn);
        nonGroundAliens[i].position = slotTranslation[parent->getSlotIndex()];
        nonGroundAliens[i].argIndex = oc.argIndex();
        nonGroundAliens[i].automaton = subAutomata[i];
	fast = false;
      }
  }
}

FreeRemainder::FreeRemainder(Equation* eqn)
{
  fast = false;
  foreign = true;
  equation = eqn;
}

FreeRemainder::~FreeRemainder()
{
  int nrNonGroundAliens = nonGroundAliens.length();
  for (int i = 0; i < nrNonGroundAliens; i++)
    delete nonGroundAliens[i].automaton;
}

local_inline bool 
FreeRemainder::slowMatchReplace2(DagNode* subject,
				 RewritingContext& context,
				 Vector<DagNode**>& stack) const
{
  Subproblem* subproblem = 0;
  if (foreign)
    {
      if(!(equation->getLhsAutomaton()->match(subject,  context, subproblem)))
	return false;
      if (subproblem != 0 && subproblem->solve(true, context) == false)
	{
	  delete subproblem;
	  return false;
	}
    }
  else
    {
      Vector<DagNode**>::const_iterator stackBase =
	const_cast<const Vector<DagNode**>&>(stack).begin();
      //
      //	Bind free variables
      //
      if (!freeVariables.isNull())
	{
	  Vector<FreeVariable>::const_iterator i = freeVariables.begin();
	  Vector<FreeVariable>::const_iterator e = freeVariables.end();
	  do
	    {
	      DagNode* d = stackBase[i->position][i->argIndex];
	      if (!(d->leq(i->sort)))
		return false;
	      context.bind(i->varIndex, d);
	    }
	  while (++i != e);
	}
      //
      //	Check bound variables
      //
      if (!boundVariables.isNull())
	{
	  Vector<BoundVariable>::const_iterator i = boundVariables.begin();
	  Vector<BoundVariable>::const_iterator e = boundVariables.end();
	  do
	    {
	      if (!(stackBase[i->position][i->argIndex]->equal(context.value(i->varIndex))))
		return false;
	    }
	  while (++i != e);
	}
      //
      //	Match ground aliens
      //
      if (!groundAliens.isNull())
	{
	  Vector<GroundAlien>::const_iterator i = groundAliens.begin();
	  Vector<GroundAlien>::const_iterator e = groundAliens.end();
	  do
	    {
	      if (!(i->alien->equal(stackBase[i->position][i->argIndex])))
		return false;
	    }
	  while (++i != e);
	}
      //
      //	Match non-ground aliens
      //
      if (!nonGroundAliens.isNull())
	{
	  Assert(nonGroundAliens.size() > 0, "no nonGroundAliens");
	  SubproblemAccumulator subproblems;
	  Vector<NonGroundAlien>::const_iterator i = nonGroundAliens.begin();
	  Vector<NonGroundAlien>::const_iterator e = nonGroundAliens.end();
	  do
	    {
	      if (i->automaton->match(stackBase[i->position][i->argIndex], context, subproblem))
		subproblems.add(subproblem);
	      else
		return false;
	    }
	  while (++i != e);
	  //
	  //	find a first solution to subproblem
	  //
	  subproblem = subproblems.extractSubproblem();
	  if (subproblem != 0 && subproblem->solve(true, context) == false)
	    {
	      delete subproblem;
	      return false;
	    }
	}
    }
  //
  //	Check condition
  //
  if (equation->hasCondition())
    {
      //
      //	The rewriting we do to evaluate our condition may well overwrite
      //	the stack; but the stack must be preserved to allow us to handle
      //	the next remainder if the condition fails or rewriting is aborted.
      //
      Vector<DagNode**> savedStack(stack);
      bool r = equation->checkCondition(subject, context, subproblem);
      stack = savedStack;	// cannot swap because FreeNet may have pointers into stack
      if (!r)
	{
	  delete subproblem;
	  return false;
	}
    }
  //
  //	Do replacement
  //
  {
    bool trace = RewritingContext::getTraceStatus();
    if (trace)
      {
	context.tracePreEqRewrite(subject, equation, RewritingContext::NORMAL);
	if (context.traceAbort())
	  {
	    delete subproblem;
	    return false;
	  }
      }
    equation->getRhsBuilder().replace(subject, context);
    context.incrementEqCount();
    if (trace)
      context.tracePostEqRewrite(subject);
  }
  delete subproblem;
  return true;
}

bool 
FreeRemainder::slowMatchReplace(DagNode* subject,
				RewritingContext& context,
				Vector<DagNode**>& stack) const
{
  context.clear(equation->getNrProtectedVariables());
  bool r = slowMatchReplace2(subject, context, stack);
  context.finished();
  MemoryCell::okToCollectGarbage();
  return r;
}

#ifdef DUMP
void
FreeRemainder::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{FreeRemainder}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "fast = " << fast << "\tforeign = " << foreign << '\n';
  s << Indent(indentLevel) << "freeVariables:\n";
  ++indentLevel;
  int nrRealVariables = equation->getNrRealVariables();
  for (int i = 0; i < freeVariables.length(); i++)
    {
      int index = freeVariables[i].varIndex;
      s << Indent(indentLevel) << "position = " << freeVariables[i].position <<
        "\targIndex = " << freeVariables[i].argIndex <<
        "\tvarIndex = " << index;
      if (index < nrRealVariables)  // might be pseudo-variable
	s << " \""  << equation->index2Variable(index) << '"';
      else
	s << " (pseudo)";
      s << "\tsort = " << freeVariables[i].sort << '\n';
    }

  s << Indent(indentLevel - 1) << "boundVariables:\n";
  for (int i = 0; i < boundVariables.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << boundVariables[i].position <<
        "\targIndex = " << boundVariables[i].argIndex <<
        "\tvarIndex = " << boundVariables[i].varIndex <<
	" \"" << equation->index2Variable(boundVariables[i].varIndex)  << "\"\n";
    }

  s << Indent(indentLevel - 1) << "groundAliens:\n";
  for (int i = 0; i < groundAliens.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << groundAliens[i].position <<
        "\targIndex = " << groundAliens[i].argIndex <<
        "\talien = " << groundAliens[i].alien << '\n';
    }

  s << Indent(indentLevel - 1) << "nonGroundAliens:\n";
  for (int i = 0; i < nonGroundAliens.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << nonGroundAliens[i].position <<
        "\targIndex = " << nonGroundAliens[i].argIndex <<
        "\tautomaton =\n";
      nonGroundAliens[i].automaton->dump(s, *equation, indentLevel + 1);
    }

  s << Indent(indentLevel - 2) << "End{FreeRemainder}\n";
}
#endif
