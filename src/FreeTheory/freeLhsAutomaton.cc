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
//      Implementation for class FreeLhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//	core class definitions
#include "variableInfo.hh"
#include "subproblemSequence.hh"
#include "subproblemAccumulator.hh"
#include "substitution.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	free theory class definitions
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeRhsAutomaton.hh"
#include "freeOccurrence.hh"
#include "freeTerm.hh"
#include "freeOccurrence.hh"
#include "freeDagNode.hh"
#include "freeLhsAutomaton.hh"

FreeLhsAutomaton::FreeLhsAutomaton(const Vector<FreeOccurrence>& freeSymbols,
				   const Vector<FreeOccurrence>& uncertainVars,
				   const Vector<FreeOccurrence>& boundVars,
				   const Vector<FreeOccurrence>& gndAliens,
				   const Vector<FreeOccurrence>& nonGndAliens,
				   const Vector<int>& bestSequence,
				   const Vector<LhsAutomaton*>& subAutomata)
: freeSubterms(freeSymbols.length() - 1),    // skip topSymbol
  uncertainVariables(uncertainVars.length()),
  boundVariables(boundVars.length()),
  groundAliens(gndAliens.length()),
  nonGroundAliens(nonGndAliens.length())
{
  {
    //
    //	Free symbol skeleton
    //
    FreeTerm* topTerm = static_cast<FreeTerm*>(freeSymbols[0].term());
    topSymbol = topTerm->symbol();
    topTerm->setSlotIndex(0);
    int slotNr = 1;
    int nrFreeSymbols = freeSymbols.length();
    for (int i = 1; i < nrFreeSymbols; i++)  // skip topSymbol
      {
	const FreeOccurrence& oc = freeSymbols[i];
	FreeTerm* parent = static_cast<FreeTerm*>(freeSymbols[oc.position()].term());
	FreeTerm* term = static_cast<FreeTerm*>(oc.term());
	Symbol* symbol = term->symbol();
	freeSubterms[i - 1].position = parent->getSlotIndex();
	freeSubterms[i - 1].argIndex = oc.argIndex();
	freeSubterms[i - 1].symbol = symbol;
	freeSubterms[i - 1].saveIndex = term->getSaveIndex();
	if (symbol->arity() > 0)
	  term->setSlotIndex(slotNr++);
      }
    stack.expandTo(slotNr);
  }
  {
    //
    //	Variables that may be bound
    //
    int nrUncertainVariables = uncertainVars.length();
    for (int i = 0; i < nrUncertainVariables; i++)
      {
	const FreeOccurrence& oc = uncertainVars[i];
	FreeTerm* parent = static_cast<FreeTerm*>(freeSymbols[oc.position()].term());
	VariableTerm* v = safeCast(VariableTerm*, oc.term());
        uncertainVariables[i].position = parent->getSlotIndex();
	uncertainVariables[i].argIndex = oc.argIndex();
	uncertainVariables[i].varIndex = v->getIndex();
	uncertainVariables[i].sort = v->getSort();
      }
  }
  {
    //
    //	Variables that will be bound
    //
    int nrBoundVariables = boundVars.length();
    for (int i = 0; i < nrBoundVariables; i++)
      {
	const FreeOccurrence& oc = boundVars[i];
	FreeTerm* parent = static_cast<FreeTerm*>(freeSymbols[oc.position()].term());
	VariableTerm* v = safeCast(VariableTerm*, oc.term());
        boundVariables[i].position = parent->getSlotIndex();
	boundVariables[i].argIndex = oc.argIndex();
	boundVariables[i].varIndex = v->getIndex();
      }
  }
  {
    //
    //	Ground alien subterms
    //
    int nrGroundAliens = gndAliens.length();
    for (int i = 0; i < nrGroundAliens; i++)
      {
	const FreeOccurrence& oc = gndAliens[i];
	FreeTerm* parent = static_cast<FreeTerm*>(freeSymbols[oc.position()].term());
	groundAliens[i].position = parent->getSlotIndex();
	groundAliens[i].argIndex = oc.argIndex();
	groundAliens[i].alien = oc.term();
      }
  }
  {
    //
    //	Non-ground alien subterms
    //
    int nrNonGroundAliens = nonGndAliens.length();
    for (int i = 0; i < nrNonGroundAliens; i++)
      {
	const FreeOccurrence& oc = nonGndAliens[bestSequence[i]];
	FreeTerm* parent = static_cast<FreeTerm*>(freeSymbols[oc.position()].term());
	nonGroundAliens[i].position = parent->getSlotIndex();
	nonGroundAliens[i].argIndex = oc.argIndex();
	nonGroundAliens[i].automaton = subAutomata[i];
      }
  }
}

FreeLhsAutomaton::~FreeLhsAutomaton()
{
  int nrNonGroundAliens = nonGroundAliens.length();
  for (int i = 0; i < nrNonGroundAliens; i++)
    delete nonGroundAliens[i].automaton;
}

bool
FreeLhsAutomaton::match(DagNode* subject,
			Substitution& solution,
			Subproblem*& returnedSubproblem,
			ExtensionInfo* /* extensionInfo */)
{
  if (subject->symbol() != topSymbol)
    return false;
  if (topSymbol->arity() == 0)
    {
      returnedSubproblem = 0;
      return true;
    }
  Vector<DagNode**>::iterator stackBase = stack.begin();
  *stackBase = static_cast<FreeDagNode*>(subject)->argArray();
  if (!freeSubterms.isNull())
    {
      //
      //	Match free symbol skeleton.
      //
      Vector<DagNode**>::iterator stackPointer = stackBase;
      const Vector<FreeSubterm>& t = freeSubterms;
      Vector<FreeSubterm>::const_iterator i = t.begin();
      Vector<FreeSubterm>::const_iterator e = t.end();
      do
	{
	  DagNode* d = stackBase[i->position][i->argIndex];
	  if (d->symbol() != i->symbol)
	    return false;
	  if (i->saveIndex != NONE)
	    solution.bind(i->saveIndex, d);
	  if (i->symbol->arity() != 0)
	    *(++stackPointer) = static_cast<FreeDagNode*>(d)->argArray();
	}
      while (++i != e);
    }
  if (!uncertainVariables.isNull())
    {
      //
      //	Either bind or check uncertain variables.
      //
      const Vector<FreeVariable>& t = uncertainVariables;
      Vector<FreeVariable>::const_iterator i = t.begin();
      Vector<FreeVariable>::const_iterator e = t.end();
      do
	{
	  DagNode* d = stackBase[i->position][i->argIndex];
	  int v = i->varIndex;
	  DagNode* b = solution.value(v);
	  if (b == 0)
	    {
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN,
		     "missing sort information (2) for " << d);
	      if (d->leq(i->sort))
		solution.bind(v, d);
	      else
		return false;
	    }
	  else
	    {
	      if (!(d->equal(b)))
		return false;
	    }
	}
      while (++i != e);
    }
  if (!boundVariables.isNull())
    {
      //
      //	Check bound variables
      //
      const Vector<BoundVariable>& t = boundVariables;
      Vector<BoundVariable>::const_iterator i = t.begin();
      Vector<BoundVariable>::const_iterator e = t.end();
      do
	{
	  if (!(stackBase[i->position][i->argIndex]->equal(solution.value(i->varIndex))))
	    return false;
	}
      while (++i != e);
    }
  if (!groundAliens.isNull())
    {
      //
      //	Match ground aliens
      //
      const Vector<GroundAlien>& t = groundAliens;
      Vector<GroundAlien>::const_iterator i = t.begin();
      Vector<GroundAlien>::const_iterator e = t.end();
      do
	{
	  if (!(i->alien->equal(stackBase[i->position][i->argIndex])))
	    return false;
	}
      while (++i != e);
    }
  if (!nonGroundAliens.isNull())
    {
      //
      //	Match non-ground aliens
      //
      Assert(nonGroundAliens.size() > 0, "no nrNonGroundAliens");
      SubproblemAccumulator subproblems;
      const Vector<NonGroundAlien>& t = nonGroundAliens;
      Vector<NonGroundAlien>::const_iterator i = t.begin();
      Vector<NonGroundAlien>::const_iterator e = t.end();
      do
	{
	  Subproblem* subproblem;
	  if (i->automaton->match(stackBase[i->position][i->argIndex], solution, subproblem))
	    subproblems.add(subproblem);
	  else
	    return false;
	}
      while (++i != e);
      returnedSubproblem = subproblems.extractSubproblem();
    }
  else
    returnedSubproblem = 0;
  return true;
}

#ifdef DUMP
void
FreeLhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{FreeLhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "topSymbol = \"" << topSymbol << "\"\n";

  s << Indent(indentLevel) << "freeSubterms:\n";
  ++indentLevel;
  for (int i = 0; i < freeSubterms.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << freeSubterms[i].position <<
	"\targIndex = " << freeSubterms[i].argIndex <<
	"\tsymbol = \"" << freeSubterms[i].symbol <<
	"\"\tsaveIndex = " << freeSubterms[i].saveIndex << '\n';
    }

  s << Indent(indentLevel - 1) << "uncertainVariables:\n";
  for (int i = 0; i < uncertainVariables.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << uncertainVariables[i].position <<
	"\targIndex = " << uncertainVariables[i].argIndex <<
	"\tvarIndex = " << uncertainVariables[i].varIndex <<
	" \"" << variableInfo.index2Variable(uncertainVariables[i].varIndex) << '"';
      s << "\tsort = " << uncertainVariables[i].sort << '\n';
    }

  s << Indent(indentLevel - 1) << "boundVariables:\n";
  for (int i = 0; i < boundVariables.length(); i++)
    {
      s << Indent(indentLevel) << "position = " << boundVariables[i].position <<
	"\targIndex = " << boundVariables[i].argIndex <<
	"\tvarIndex = " << boundVariables[i].varIndex <<
	" \"" << variableInfo.index2Variable(boundVariables[i].varIndex)  << '"';
      s << '\n';
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
      nonGroundAliens[i].automaton->dump(s, variableInfo, indentLevel + 1);
    }

  s << Indent(indentLevel - 2) << "End{FreeLhsAutomaton}\n";
}
#endif
