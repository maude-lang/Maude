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
//	Class for matching whats left of a pattern after free symbols have
//	been matched (could we derive FreeLhsAutomaton from this?).
//
#ifndef _freeRemainder_hh_
#define _freeRemainder_hh_
#include "freeLhsStructs.hh"

class FreeRemainder : private FreeLhsStructs
{
  NO_COPYING(FreeRemainder);

public:
  FreeRemainder(Equation* eqn,
		const Vector<FreeOccurrence>& freeSymbols,
		const Vector<FreeOccurrence>& freeVars,
		const Vector<FreeOccurrence>& boundVars,
		const Vector<FreeOccurrence>& gndAliens,
		const Vector<FreeOccurrence>& nonGndAliens,
		const Vector<int>& bestSequence,
		const Vector<LhsAutomaton*>& subAutomata,
		const Vector<int>& slotTranslation);
  FreeRemainder(Equation* eqn);  // trivial FreeRemainder for foreign equations
  ~FreeRemainder();

  bool fastMatchReplace(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;
  bool slowMatchReplace(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;

  bool isOwise() const;

#ifdef DUMP
  void dump(ostream& s, int indentLevel = 0);
#endif

private:
  bool slowMatchReplace2(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;
  //
  //	To qualify for "fast" treatment the associated equation must:
  //	(1) have a lhs that parses into a non-error sort
  //	(2) have only free symbols in lhs
  //	(3) be left linear
  //	(4) be unconditional
  //	(5) have no "problem" variables (ones which need their bindings copied to avoid
  //	    eager evaluation of lazy subterm)
  //	To qualify for "super-fast", additionally each variable must have a sort that
  //	is the unique maximal user sort in its component which must be error-free.
  //	
  //
  Byte fast;  // > 0 super-fast; < 0 fast; = 0 slow
  const bool foreign;  // remainder consists of a foreign equation that might collapse into free theory
  Vector<FreeVariable> freeVariables;
  Equation* const equation;  // equation we are a remainder of
  Vector<BoundVariable> boundVariables;
  Vector<GroundAlien> groundAliens;
  Vector<NonGroundAlien> nonGroundAliens;
};

inline bool 
FreeRemainder::fastMatchReplace(DagNode* subject,
				RewritingContext& context,
				Vector<DagNode**>& stack) const
{
  if (!(RewritingContext::getTraceStatus()))
    {
      if (fast > 0)
	{
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  FOR_EACH_CONST(i, Vector<FreeVariable>, freeVariables)
	    {
	      DagNode* d = stackBase[i->position][i->argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      context.bind(i->varIndex, d);
	    }
	}
      else if (fast < 0)
	{
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  FOR_EACH_CONST(i, Vector<FreeVariable>, freeVariables)
	    {
	      DagNode* d = stackBase[i->position][i->argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      if (d->leq(i->sort))
		context.bind(i->varIndex, d);
	      else
		return false;
	    }
	}
      else
	goto slow;
      equation->getRhsBuilder().replace(subject, context);
      context.incrementEqCount();
      MemoryCell::okToCollectGarbage();
      return true;
    }
slow:
  return slowMatchReplace(subject, context, stack);
}

inline bool
FreeRemainder::isOwise() const
{
  return equation->isOwise();
}

#endif
