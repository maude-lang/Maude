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
//      Implementation of match compilation for A, AUl, AUr and AU theories.
//

struct AU_Term::CP_Sequence
{
  Vector<Bool> sequence;
  NatSet bound;
  int cardinality;
  int firstFlex;
};

local_inline bool
AU_Term::unitVariable(VariableTerm* vt, int index) const
{
  if (vt == 0)
    return false;
  AU_Symbol* s = symbol();
  Sort* vs = vt->getSort();
  return (s->sortBound(vs) == 1 && !(idPossible(index) && s->takeIdentity(vs)));
}

void
AU_Term::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  CP_Sequence bestSequence;
  findConstraintPropagationSequence(boundUniquely, bestSequence);
  boundUniquely = bestSequence.bound;  // deep copy
  if (bestSequence.sequence.length() == argArray.length() - 1)
    {
      AU_Symbol* s = symbol();
      //
      //	Lone variable or abstracted term in flex part. We
      //	can only guarentee unique binding if our top symbol
      //	does not have a one sided identity.
      //
      if (!(s->oneSidedId()))
	argArray[bestSequence.firstFlex].term->analyseConstraintPropagation(boundUniquely);
    }
}

LhsAutomaton*
AU_Term::compileLhs2(bool matchAtTop,
		     const VariableInfo& variableInfo,
		     NatSet& boundUniquely,
		     bool& subproblemLikely)
{
  AU_Symbol* s = symbol();
  bool oneSidedIdentity = s->oneSidedId();
  LhsAutomaton* uniqueCollapseAutomaton = 0;
  if (uniqueCollapseSubtermIndex != NONE)
    {
      //
      //        Maybe we should add the variable(s) which will be bound to identity
      //        to this local bound uniquely.
      //
      NatSet local(boundUniquely);
      bool spl;
      uniqueCollapseAutomaton = argArray[uniqueCollapseSubtermIndex].term->
        compileLhs(matchAtTop, variableInfo, local, spl);
    }
  AU_LhsAutomaton* a = new AU_LhsAutomaton(s,
					   matchAtTop,
					   !(collapseSymbols().empty()),
					   uniqueCollapseAutomaton,
					   variableInfo.getNrProtectedVariables());
  subproblemLikely = false;
  int leftPos = 0;
  int rightPos = argArray.length() - 1;
  if (!matchAtTop)
    {
      //
      //	Build rigid part.
      //
      CP_Sequence bestSequence;
      findConstraintPropagationSequence(boundUniquely, bestSequence);
      int nrRigid = bestSequence.sequence.length();
      for (int i = 0; i < nrRigid; i++)
	{
	  bool leftEnd = bestSequence.sequence[i];
	  int j = leftEnd ? leftPos++ : rightPos--;
	  Term* t = argArray[j].term;
	  VariableTerm* v = dynamic_cast<VariableTerm*>(t);
	  if (v != 0)
	    {
	      a->addRigidVariable(v, leftEnd, idPossible(j));
	      boundUniquely.insert(v->getIndex());
	    }
	  else if (t->ground())
	    a->addRigidGroundAlien(t, leftEnd);
	  else
	    {
	      bool spl;
	      LhsAutomaton* subAutomaton =
		t->compileLhs(false, variableInfo, boundUniquely, spl);
	      a->addRigidNonGroundAlien(subAutomaton, leftEnd);
	      subproblemLikely = subproblemLikely || spl;
	    }
	}
      Assert(boundUniquely == bestSequence.bound, "bound clash");
      //
      //	Build flex part (special cases).
      //
      int nrFlex = rightPos - leftPos + 1;
      if (nrFlex == 0)
	{
	  a->complete(AU_LhsAutomaton::GROUND_OUT);
	  return a;
	}
      if (nrFlex == 1 && !oneSidedIdentity)
	{
	  Assert(!matchAtTop, "shouldn't match at top");
	  Tuple& t = argArray[leftPos];
	  if (t.abstractionVariableIndex == NONE)
	    {
	      VariableTerm* vt = dynamic_cast<VariableTerm*>(t.term);
	      a->addFlexVariable(vt, UNDEFINED, idPossible(leftPos));
	      boundUniquely.insert(vt->getIndex());
	      a->complete((s->sortStructure(vt->getSort()) ==
			   AssociativeSymbol::PURE_SORT) ?
			  AU_LhsAutomaton::FAST_LONE_VARIABLE :
			  AU_LhsAutomaton::LONE_VARIABLE);
	    }
	  else
	    {
	      bool matchOurIdentity = t.matchOurIdentity && idPossible(leftPos);
	      Assert(t.collapseToOurSymbol || matchOurIdentity,
		     "should not use abstraction variable");
	      bool spl;
	      a->addFlexAbstractionVariable
		(t.abstractionVariableIndex,
		 t.term->getComponent()->sort(Sort::ERROR_SORT),
		 t.collapseToOurSymbol ? UNBOUNDED : 1,
		 matchOurIdentity,
		 false,
		 t.term->compileLhs(false, variableInfo, boundUniquely, spl));
	      subproblemLikely = subproblemLikely || spl;
	      a->complete(AU_LhsAutomaton::LONE_VARIABLE);
	    }
	  return a;
	}
    }
  //
  //	Build flex part (general case).
  //
  //	Greedy matcher:
  //	(1) does not check to see if matched portion is in error sort;
  //	(2) cannot handle the intricacies involved in one sided identity theories;
  //	(3) does not check that enough has been matched if extension present.
  //
  bool greedy = !oneSidedIdentity &&
    (!matchAtTop || collapseSymbols().empty());
  int fixedLengthBlockStart = NONE;
  for (int i = leftPos; i <= rightPos; i++)
    {
      Tuple& t = argArray[i];
      greedy = greedy && t.abstractionVariableIndex == NONE &&
	t.term->greedySafe(variableInfo, boundUniquely);
      //
      //	Check if thing we match will be of fixed size.
      //
      bool fixedSize = (t.abstractionVariableIndex == NONE);
      VariableTerm* vt = dynamic_cast<VariableTerm*>(t.term);
      if (vt != 0)
	{
	  Sort* vs = vt->getSort();
	  bool ip = idPossible(i);
	  bool ti = s->takeIdentity(vs);
	  bool awkward = matchAtTop && !ip && ti;
	  greedy = greedy && !awkward;
	  if (((matchAtTop || ip) && ti) || s->sortBound(vs) > 1)
	    {
	      fixedSize = false;
	      greedy = greedy &&
		(boundUniquely.contains(vt->getIndex()) ||
		 s->sortStructure(vs) >= AssociativeSymbol::LIMIT_SORT);
	    }
	}
      //
      //	If not fixed size, see if we ended a fixed size block; then add
      //	flex variable or flex abstraction variable. Otherwise see if we need
      //	to start a fixed length block.
      //
      if (!fixedSize)
	{
	  if (fixedLengthBlockStart != NONE)
	    {
	      bool spl;
	      addFixedLengthBlock(a, fixedLengthBlockStart,
				  i - fixedLengthBlockStart,
				  variableInfo, boundUniquely, spl);
	      greedy = greedy && !spl;
	      fixedLengthBlockStart = NONE;
	    }
	  bool ip = idPossible(i);
	  if (t.abstractionVariableIndex == NONE)
	    a->addFlexVariable(vt, UNDEFINED, ip);
	  else
	    {
	      bool matchOurIdentity = ip && t.matchOurIdentity;
	      bool awkward = matchAtTop && !ip && t.matchOurIdentity;
	      Assert(t.collapseToOurSymbol || matchOurIdentity || awkward,
		     "should not use abstraction variable");
	      NatSet local(boundUniquely);
	      bool spl;
	      a->addFlexAbstractionVariable
		(t.abstractionVariableIndex,
		 t.term->getComponent()->sort(Sort::ERROR_SORT),
		 t.collapseToOurSymbol ? UNBOUNDED : 1,
		 matchOurIdentity,
		 awkward,
		 t.term->compileLhs(false, variableInfo, local, spl));
	    }
	}
      else
	{
	  if (fixedLengthBlockStart == NONE)
	    fixedLengthBlockStart = i;
	}
    }
  //
  //	Handle any remaining fixed length block.
  //
  if (fixedLengthBlockStart != NONE)
    {
      Assert(matchAtTop, "ended flex part with fixed length block");
      bool spl;
      addFixedLengthBlock(a, fixedLengthBlockStart,
			  argArray.length() - fixedLengthBlockStart,
			  variableInfo, boundUniquely, spl);
      greedy = greedy && !spl;
    }
  //
  //	Finish up by decideding on a match strategy.
  //
  if (!greedy)
    subproblemLikely = true;
  a->complete(greedy ? AU_LhsAutomaton::GREEDY : AU_LhsAutomaton::FULL);
  return a;
}

void
AU_Term::addFixedLengthBlock(AU_LhsAutomaton* a,
			     int blockStart,
			     int blockLength,
			     const VariableInfo& variableInfo,
			     const NatSet& boundUniquely,
			     bool& subproblemLikely)
{
  //
  //	For each possible shift factor sh from 1 to blockLength - 1 we
  //	find the index of the rightmost pattern p such that if the p matches
  //	some subject s (possibly with a subproblem that may or not be soluble),
  //	the pattern q that is sh places to the left of p will fail early
  //	on s, thus ruling out a shift of sh during matching.
  //
  Vector<int> largestIndexThatFails(blockLength);
  for (int shift = 1; shift < blockLength; shift++)
    {
      largestIndexThatFails[shift] = -1;  // default; shift is never ruled out
      for (int i = blockLength - 1; i >= shift; i--)
	{
	  int b = blockStart + i;
	  Term* p = argArray[b].term;  // assume p matched subject
	  Term* q = argArray[b - shift].term;  // q will get p's subject after a shift
	  if (q->earlyMatchFailOnInstanceOf(p))
	    {
	      largestIndexThatFails[shift] = i;
	      break;
	    }
	}
    }
  //
  //	For each pattern p we find the smallest shift that is not ruled out
  //	when a match for that pattern fails and matches for all the patterns
  //	to the right of it succeed (modulo a possible subproblem).
  //
  subproblemLikely = false;
  for (int i = 0; i < blockLength; i++)
    {
      int b = blockStart + i;
      Term* p = argArray[b].term;
      int shift = 1;
      for (; shift < blockLength; shift++)
	{
	  //
	  //	A shift can be ruled out because a match to the right
	  //	of p;
	  //
	  if (i < largestIndexThatFails[shift])
	    continue;
	  //
	  //	Or because the p is more general than the pattern
	  //	that will get it's subject after the shift. Here we need
	  //	to be careful because variables bound by an external agency
	  //	can invalidate subsumption. We rely on the convention that
	  //	the external agency adds any variables that it might bind
	  //	to the set of condition variables.
	  //
	  if (i >= shift &&
	      p->occursBelow().disjoint(variableInfo.getConditionVariables()) &&
	      p->subsumes(argArray[b - shift].term, true))
	    continue;
	  else
	    break;
	}
      VariableTerm* v = dynamic_cast<VariableTerm*>(p);
      if (v != 0)
	a->addFlexVariable(v, shift, false);
      else if (p->ground())
	a->addFlexGroundAlien(p, shift);
      else
	{
	  NatSet local(boundUniquely);
	  bool spl;
	  LhsAutomaton* subAutomaton =
	    p->compileLhs(false, variableInfo, local, spl);
	  a->addFlexNonGroundAlien(subAutomaton, shift);
	  subproblemLikely = subproblemLikely || spl;
	}
    }
}

void
AU_Term::findConstraintPropagationSequence(const NatSet& boundUniquely,
					   CP_Sequence& bestSequence) const
{
  Vector<Bool> currentSequence;
  bestSequence.cardinality = -1;
  findConstraintPropagationSequence(currentSequence, boundUniquely,
				    0, argArray.length() - 1, bestSequence);
}

void
AU_Term::findConstraintPropagationSequence(const Vector<Bool>& currentSequence,
					   const NatSet& boundUniquely,
					   int leftPos,
					   int rightPos,
					   CP_Sequence& bestSequence) const
{
  if (leftPos <= rightPos)
    {
      //
      //	Try to grow search tree.
      //
      //	(1) If left or right term is not abstracted and grounds out
      //	    match it next and don't consider other possibilities.
      //
      const Tuple& lt = argArray[leftPos];
      bool leftBad = lt.collapseToOurSymbol || (lt.matchOurIdentity && idPossible(leftPos));
      if (!leftBad && boundUniquely.contains(lt.term->occursBelow()))
	{
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(true);
	  findConstraintPropagationSequence(newSequence, boundUniquely, leftPos + 1,
					    rightPos, bestSequence);
	  return;
	}
      const Tuple& rt = argArray[rightPos];
      bool rightBad = (leftPos == rightPos) || rt.collapseToOurSymbol ||
	(rt.matchOurIdentity && idPossible(rightPos));
      if (!rightBad && boundUniquely.contains(rt.term->occursBelow()))
	{
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(false);
	  findConstraintPropagationSequence(newSequence, boundUniquely, leftPos,
					    rightPos - 1, bestSequence);
	  return;
	}
      //
      //	(2) If left or right term is a variable of unit sort which cannot take
      //	    identity match it next and don't consider other possibilities.
      //
      VariableTerm* ltVar = dynamic_cast<VariableTerm*>(lt.term);
      if (unitVariable(ltVar, leftPos))
	{
	  
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(true);
	  NatSet newBound(boundUniquely);
	  newBound.insert(ltVar->getIndex());
	  findConstraintPropagationSequence(newSequence, newBound, leftPos + 1,
					    rightPos, bestSequence);
	  return;
	}
      VariableTerm* rtVar = dynamic_cast<VariableTerm*>(rt.term);
      if (leftPos < rightPos && unitVariable(rtVar, rightPos))
	{
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(false);
	  NatSet newBound(boundUniquely);
	  newBound.insert(rtVar->getIndex());
	  findConstraintPropagationSequence(newSequence, newBound, leftPos,
					    rightPos - 1, bestSequence);
	  return;
	}
      //
      //	If left or right term is a non-abstracted alien then consider
      //	matching it next but also consider other possibilities.
      //
      bool growth = false;
      if (!leftBad && ltVar == 0)
	{
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(true);
	  NatSet newBound(boundUniquely);
	  lt.term->analyseConstraintPropagation(newBound);
	  findConstraintPropagationSequence(newSequence, newBound, leftPos + 1,
					    rightPos, bestSequence);
	  growth = true;
	}
      if (!rightBad && rtVar == 0)
	{
	  Vector<Bool> newSequence(currentSequence);
	  newSequence.append(false);
	  NatSet newBound(boundUniquely);
	  rt.term->analyseConstraintPropagation(newBound);
	  findConstraintPropagationSequence(newSequence, newBound, leftPos,
					    rightPos - 1, bestSequence);
	  growth = true;
	}
      if (growth)
	return;
    }
  int n = boundUniquely.cardinality();
  if (n > bestSequence.cardinality ||
      (n == bestSequence.cardinality &&
       currentSequence.length() > bestSequence.sequence.length()))
    {
      bestSequence.sequence = currentSequence;  // deep copy
      bestSequence.bound = boundUniquely;  // deep copy
      bestSequence.cardinality = n;
      bestSequence.firstFlex = leftPos <= rightPos ? leftPos : NONE;
    }
}
