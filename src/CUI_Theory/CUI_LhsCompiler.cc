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
//      Implementation of match compilation for theories made from C, Ul, Ur and I axioms.
//

bool
CUI_Term::betterToReverseOrder(NatSet& boundUniquely) const
{
  Term* t0 = argArray[0];
  Term* t1 = argArray[1];
  NatSet forward(boundUniquely);
  t0->analyseConstraintPropagation(forward);
  t1->analyseConstraintPropagation(forward);
  NatSet reverse(boundUniquely);
  t1->analyseConstraintPropagation(reverse);
  t0->analyseConstraintPropagation(reverse);
  if (reverse.cardinality() > forward.cardinality())
    {
      boundUniquely = reverse;  // deep copy
      return true;
    }
  boundUniquely = forward;  // deep copy
  return false;
}

void
CUI_Term::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  //
  //	If any collapse is possible then constraint propagation analysis
  //	becomes extraordinarily complicated; subtle counter-examples
  //	show that we can't guarantee anything in most cases.
  //	Thus we don't bother doing anything.
  //
  if (id0CollapsePossible || id1CollapsePossible || idemCollapsePossible)
    return;
  Term* t0 = argArray[0];
  Term* t1 = argArray[1];
  if (t0->ground() || t0->willGroundOutMatch(boundUniquely) || t0->equal(t1))
    t1->analyseConstraintPropagation(boundUniquely);
  else if (t1->ground() || t1->willGroundOutMatch(boundUniquely))
    t0->analyseConstraintPropagation(boundUniquely);
  else if (!(symbol()->comm()) || t0->matchIndependent(t1))
    (void) betterToReverseOrder(boundUniquely);
}

LhsAutomaton*
CUI_Term::compileLhs2(bool matchAtTop,
		     const VariableInfo& variableInfo,
		     NatSet& boundUniquely,
		     bool& subproblemLikely)
{
  //
  //	Check to see if greedy matching is safe.
  //
  bool greedy = greedySafe(variableInfo, boundUniquely);
  int flags = greedy ? CUI_LhsAutomaton::GREEDY_MATCH_OK : 0;

  Term* t0 = argArray[0];
  Term* t1 = argArray[1];
  bool comm = symbol()->comm();
  bool uniqueBranch = false;
  bool subtermsEqual = t0->equal(t1);
  bool swap = false;
  //
  //	We need to decide whether to swap our arguments for matching
  //	and whether we have a unique branch.
  //
  if (id0CollapsePossible || id1CollapsePossible || idemCollapsePossible)
    {
      //
      //	Collapse case.
      //
      if (comm &&
	  dynamic_cast<VariableTerm*>(t0) != 0 &&
	  dynamic_cast<VariableTerm*>(t1) == 0)
	swap = true;  // match variable last to heuristically improve substitution
      else if (matchAtTop && idemCollapsePossible && t1->ground())
	{
	  Assert(!(t0->ground()),
		 "can't have idem collapse with two ground terms");
	  swap = true;
	}
    }
  else
    {
      //
      //	Non-collapse case.
      //
      if (t0->ground() || t0->willGroundOutMatch(boundUniquely) || subtermsEqual)
	uniqueBranch = true;
      else if (t1->ground() || t1->willGroundOutMatch(boundUniquely))
	{
	  uniqueBranch = true;
	  swap = true;
	}
      else if (!comm || t0->matchIndependent(t1))
	{
	  uniqueBranch = true;
	  NatSet local(boundUniquely);
	  swap = betterToReverseOrder(local);
	}
      else if (dynamic_cast<VariableTerm*>(t0) != 0 && dynamic_cast<VariableTerm*>(t1) == 0)
	swap = true;  // match variable last to heuristically improve substitution
      if (uniqueBranch)
	flags |= CUI_LhsAutomaton::UNIQUE_BRANCH;
    }
  //
  //	Now we decide on whether to match forward and/or reverse.
  //
  if (!comm || (t0->stable() && t1->stable() && t0->symbol() != t1->symbol()))
    flags |= swap ? CUI_LhsAutomaton::REVERSE : CUI_LhsAutomaton::FORWARD;
  else
    {
      flags |= CUI_LhsAutomaton::FORWARD;
      if (symbol()->idem())
	flags |= CUI_LhsAutomaton::REVERSE;
      else if (!subtermsEqual)
	flags |= CUI_LhsAutomaton::CONDITIONAL_REVERSE;
    }
  //
  //	Swap arguments and enter collapse info into flags.
  //
  if (swap)
    {
      Term* t = t0;
      t0 = t1;
      t1 = t;
      if (id0CollapsePossible)
	flags |= CUI_LhsAutomaton::ID1_COLLAPSE;
      if (id1CollapsePossible)
	flags |= CUI_LhsAutomaton::ID0_COLLAPSE;
    }
  else
    {
      if (id0CollapsePossible)
	flags |= CUI_LhsAutomaton::ID0_COLLAPSE;
      if (id1CollapsePossible)
	flags |= CUI_LhsAutomaton::ID1_COLLAPSE;
    }
  if (idemCollapsePossible)
    flags |= CUI_LhsAutomaton::IDEM_COLLAPSE;
  //
  //	Construct CUI automaton and determine likelihood that it will return a
  //	subproblem at match time.
  //
  CUI_LhsAutomaton* a =
    new CUI_LhsAutomaton(symbol(),
			 matchAtTop,
			 static_cast<CUI_LhsAutomaton::Flags>(flags),
			 variableInfo.getNrProtectedVariables());
  bool spl = a->addSubpattern(t0, variableInfo, boundUniquely);
  spl = a->addSubpattern(t1, variableInfo, boundUniquely) || spl;
  subproblemLikely = spl || (!uniqueBranch && !greedy);
  return a;
}
