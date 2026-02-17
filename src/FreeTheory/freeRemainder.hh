/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
#include "freeOccurrence.hh"
#include "rewritingContext.hh"
#include "equation.hh"

class FreeRemainder : private FreeLhsStructs
{
  NO_COPYING(FreeRemainder);

public:
  enum Speed
    {
      //
      //	To qualify for "fast" treatment the associated equation must:
      //	(1) have a lhs that parses into a non-error sort
      //	(2) have only free symbols in lhs
      //	(3) be left linear
      //	(4) be unconditional
      //	(5) have no "problem" variables (ones which need their bindings copied to avoid
      //	    eager evaluation of lazy subterm)
      //	(6) have the sort of each variable qualify with fastGeqSufficient()
      //	To qualify for "super-fast", additionally each variable must have a sort that
      //	is the unique maximal user sort in its component which must be error-free.
      //
      //	We pick these values so we can just do a sign check at runtime and
      //	also for a convenient lowest() function.
      //
      SUPER_FAST = -1,
      SLOW = 0,
      FAST = 1
    };
  
  static Speed lowest(Speed s1, Speed s2) { return static_cast<Speed>(s1 & s2); }

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

  bool generalMatchReplace(DagNode* subject,
			   RewritingContext& context,
			   Vector<DagNode**>& stack) const;
  bool fastMatchReplace(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;
  bool slowMatchReplace(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;

  bool isOwise() const;
  //
  //	Stuff for stack machine execution
  //
  bool fastCheckAndBind(DagNode** binding, Vector<DagNode**>& stack) const;
  bool generalCheckAndBind(DagNode** binding, Vector<DagNode**>& stack) const;
  bool slowCheckAndBind(DagNode** binding, Vector<DagNode**>& stack) const;
  Instruction* getFirstInstruction() const;
  Speed getSpeed() const;
  
#ifdef DUMP
  void dump(ostream& s, int indentLevel = 0);
#endif

private:
  bool slowMatchReplace2(DagNode* subject,
			RewritingContext& context,
			Vector<DagNode**>& stack) const;
  Speed speed;
  const bool foreign;  // remainder consists of a foreign equation that might collapse
                       // into the free theory
  Vector<FreeVariable> freeVariables;
  Equation* const equation;  // equation we are a remainder of
  Vector<BoundVariable> boundVariables;
  Vector<GroundAlien> groundAliens;
  Vector<NonGroundAlien> nonGroundAliens;

  //Instruction* firstInstruction;
};

inline FreeRemainder::Speed
FreeRemainder::getSpeed() const
{
  return speed;
}

inline bool
FreeRemainder::isOwise() const
{
  return equation->isOwise();
}

inline bool 
FreeRemainder::fastMatchReplace(DagNode* subject,
				RewritingContext& context,
				Vector<DagNode**>& stack) const
{
  if (!(RewritingContext::getTraceStatus()))
    {
      if (speed < 0)
	{
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  for (const FreeVariable& i : freeVariables)
	    {
	      DagNode* d = stackBase[i.position][i.argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      context.bind(i.varIndex, d);
	    }
	}
      else
	{
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  for (const FreeVariable& i : freeVariables)
	    {
	      DagNode* d = stackBase[i.position][i.argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      if (d->fastLeq(i.sort))
		context.bind(i.varIndex, d);
	      else
		return false;
	    }
	}
      equation->getRhsBuilder().replace(subject, context);
      context.incrementEqCount();
      MemoryCell::okToCollectGarbage();
      return true;
    }
  return slowMatchReplace(subject, context, stack);
}

inline bool 
FreeRemainder::generalMatchReplace(DagNode* subject,
				RewritingContext& context,
				Vector<DagNode**>& stack) const
{
  //
  //	We handle general case, including the "super-fast" and "fast" cases.
  //	
  if (!(RewritingContext::getTraceStatus()))
    {
      if (speed < 0)
	{
	  //
	  //	Super-fast case: bind variables without sort check.
	  //
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  for (const FreeVariable& i : freeVariables)
	    {
	      DagNode* d = stackBase[i.position][i.argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      context.bind(i.varIndex, d);
	    }
	}
      else if (speed > 0)
	{
	  //
	  //	Fast case: bind variables after fast sort check.
	  //
	  Vector<DagNode**>::const_iterator stackBase = stack.begin();
	  for (const FreeVariable& i : freeVariables)
	    {
	      DagNode* d = stackBase[i.position][i.argIndex];
	      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	      if (d->fastLeq(i.sort))
		context.bind(i.varIndex, d);
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
FreeRemainder::fastCheckAndBind(DagNode** binding, Vector<DagNode**>& stack) const
{
  //
  //	We only handle the "super-fast" and "fast" cases.
  //
  Vector<DagNode**>::const_iterator stackBase = stack.begin();
  if (speed < 0)
    {
      //
      //	Super-fast case: bind variables without sort check.
      //
      for (const FreeVariable& i : freeVariables)
	{
	  DagNode* d = stackBase[i.position][i.argIndex];
	  Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	  Assert(d->leq(i.sort), "super-fast case fails sort check");
	  binding[i.varIndex] = d;
	}
    }
  else
    {
      Assert(speed > 0, "neither super-fast nor fast case");
      //
      //	Fast case: bind variables after fast sort check.
      //
      for (const FreeVariable& i : freeVariables)
	{
	  DagNode* d = stackBase[i.position][i.argIndex];
	  Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	  if (d->fastLeq(i.sort))
	    binding[i.varIndex] = d;
	  else
	    return false;
	}
    }
  return true;
}

inline bool
FreeRemainder::generalCheckAndBind(DagNode** binding, Vector<DagNode**>& stack) const
{
  //
  //	Just like fastCheckAndBind() except that it calls slowCheckAndBind()
  //	if the remainder is not "super-fast" or "fast".
  //	This extra test, and especially the possibility of a function call, is
  //	undesirable in tight code where fast-ness has already been determined,
  //	hence the existence of fastCheckAndBind().
  //
  Vector<DagNode**>::const_iterator stackBase = stack.begin();
  if (speed < 0)
    {
      //
      //	Super-fast case: bind variables without sort check.
      //
      for (const FreeVariable& i : freeVariables)
	{
	  DagNode* d = stackBase[i.position][i.argIndex];
	  Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	  Assert(d->leq(i.sort), "super-fast case fails sort check");
	  binding[i.varIndex] = d;
	}
    }
  else if (speed > 0)
    {
      //
      //	Fast case: bind variables after fast sort check.
      //
      for (const FreeVariable& i : freeVariables)
	{
	  DagNode* d = stackBase[i.position][i.argIndex];
	  Assert(d->getSortIndex() != Sort::SORT_UNKNOWN, "missing sort information");
	  if (d->fastLeq(i.sort))
	    binding[i.varIndex] = d;
	  else
	    return false;
	}
    }
  else
    {
      //
      //	Remainder is neither "super-fast" nor "fast". Call out-lined code to
      //	do the most general thing.
      //
      return slowCheckAndBind(binding, stack);
    }
  return true;
}

inline Instruction*
FreeRemainder::getFirstInstruction() const
{
  return equation->getInstructionSequence();  // HACK - eventually we should snap this pointer to avoid the extra dereference
}
 
#endif
