/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2025 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class BinarySymbol.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "sortConstraint.hh"
#include "argumentIterator.hh"

//	variable class definitions
#include "variableSymbol.hh"

BinarySymbol::BinarySymbol(int id, bool memoFlag, Term* identity)
  : Symbol(id, 2, memoFlag),
    identityTerm(identity)
{
  cyclicIdentity = (identity == 0) ? 0 : UNDECIDED;
  unequalLeftIdCollapse = false;
  unequalRightIdCollapse = false;
}

bool
BinarySymbol::interSymbolPass()
{
  return identityTerm.getTerm() == 0 ? false : identityTerm.normalize();
}

void
BinarySymbol::reset()
{
  identityTerm.reset();  // so identity dag can be garbage collected
  Symbol::reset();  // default reset() tasks
}

void
BinarySymbol::setPermuteStrategy(const Vector<int>& userStrategy)
{
  int stratLen = userStrategy.length();
  if (stratLen == 0)
    {
      permuteStrategy = EAGER;
      setStrategy(userStrategy, 2, isMemoized());
      return;
    }
  permuteStrategy = LAZY;
  Vector<int> modifiedStrategy;
  bool seenZero = false;
  for (int i = 0; i < stratLen; i++)
    {
      int a = userStrategy[i];
      if (a == 0)
        seenZero = true;
      else
        {  
          if (seenZero)
            {
              permuteStrategy = SEMI_EAGER;
              modifiedStrategy.append(0);
            }
          else
            permuteStrategy = EAGER;
          modifiedStrategy.append(1);
          modifiedStrategy.append(2);
          break;
        }
    }
  modifiedStrategy.append(0);
  setStrategy(modifiedStrategy, 2, isMemoized()); 
}

void
BinarySymbol::setPermuteFrozen(const NatSet& frozen)
{
  //
  //	Must freeze both arguments or neither for permutative operator.
  //
  if (frozen.contains(0) == frozen.contains(1))
    Symbol::setFrozen(frozen);
  else
    {
      NatSet newFrozen;
      newFrozen.insert(0);
      newFrozen.insert(1);
      Symbol::setFrozen(newFrozen);
    }
}

bool
BinarySymbol::isStable() const
{
  return identityTerm.getTerm() == 0;
}

void
BinarySymbol::commutativeSortCompletion()
{
  Assert(domainComponent(0) == domainComponent(1),
	 "Commutative operator " << this <<
	 " has its arguments in different sort components");
  Vector<Sort*> newDecl(3);
  //
  //	Note that the number of declarations may increase as
  //	we process them, so we can't use iterators.
  //
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  Index nrOpDecls = opDecls.length();
  for (Index i = 0; i < nrOpDecls; ++i)  // look at all original declarations
    {
      const Vector<Sort*>& iDecl = opDecls[i].getDomainAndRange();
      if (iDecl[0] == iDecl[1])
	continue;  // declaration is symmetric
      bool iCtor = opDecls[i].isConstructor();
      //
      //	Check to see if a declaration with the argument sorts
      //	exchanged exists.
      //
      for (Index j = opDecls.length() - 1; j >= 0; --j)
	{
	  if (j != i)
	    {
	      const Vector<Sort*>& jDecl = opDecls[j].getDomainAndRange();
	      if (iDecl[0] == jDecl[1] &&
		  iDecl[1] == jDecl[0] &&
		  iDecl[2] == jDecl[2] &&
		  iCtor == opDecls[j].isConstructor())
		goto nextOpDecl;
	    }
	}
      newDecl[0] = iDecl[1];
      newDecl[1] = iDecl[0];
      newDecl[2] = iDecl[2];
      addOpDeclaration(newDecl, iCtor);
    nextOpDecl:
      ;
    }
}

void
BinarySymbol::processIdentity()
{
  Term* id = identityTerm.getTerm();
  if (id == 0)
    return;

  VariableInfo vi;
  id->indexVariables(vi);
  WarningCheck(id->occursBelow().empty(),
	       *id << ": identity element " << QUOTE(id) <<
	       " for operator " << QUOTE(this) << " contains variables.");
  id->symbol()->fillInSortInfo(id);
  int index = id->getSortIndex();  // is this valid?
  Assert(index != Sort::SORT_UNKNOWN, "unknown sort for identity element");
  WarningCheck(index != Sort::ERROR_SORT,
	       *id << ": identity element " << QUOTE(id) <<
	       " for operator " << QUOTE(this) << " has error sort.");
  identityTerm.prepare();
}

bool
BinarySymbol::mightMatchOurIdentity(const Term* subterm) const
{
  Term* id = identityTerm.getTerm();
  if (id == 0)
    return false;
  if (id->equal(subterm))
    return true;  // this can happen if we only have left (right) identity
  Symbol* idTopSymbol = id->symbol();
  //
  //    First examine subterm.
  //
  Symbol* s = subterm->symbol();
  if (s == idTopSymbol && !(subterm->ground()))
    return true;
  VariableSymbol* vs = dynamic_cast<VariableSymbol*>(s);
  if (vs != 0 && id->leq(vs->getSort()))
    return true;
  //
  //    Second examine what it might collapse to.
  //
  const PointerSet& cs = subterm->collapseSymbols();
  int nrSymbols = cs.cardinality();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s2 = static_cast<Symbol*>(cs.index2Pointer(i));
      if (s2 == idTopSymbol)
        return true;
      VariableSymbol* vs2 = dynamic_cast<VariableSymbol*>(s2);
      if (vs2 != 0 && id->leq(vs2->getSort()))
        return true;
    }
  return false;
}

void
BinarySymbol::leftIdentitySortCheck()
{
  Term* id = identityTerm.getTerm();
  const ConnectedComponent* component = rangeComponent();
  Assert(component == domainComponent(1),
	 "operator with left identity " << this <<
	 " has right argument and range in different sort components");
  Assert(id->getComponent() == domainComponent(0),
	 "operator " << this <<
	 " has left identity and left argument is different sort components");
  int nrSorts = component->nrSorts();
  //
  //	Check that all collapses are to less or equal sorts.
  //
  int step = traverse(0, id->getSortIndex());
  for (int i = 0; i < nrSorts; i++)
    {
      const Sort* resultSort = component->sort(traverse(step, i));
      if (resultSort->index() != i && !unequalLeftIdCollapse)
	{
	  Verbose("op " << this << " left-identity collapse from " <<
		  resultSort << " to " << component->sort(i) <<
		  " is unequal.");
	  unequalLeftIdCollapse = true;
	}
      DebugInfo("op " << this << " sort " << resultSort <<
		  " collapse to " << component->sort(i) <<
		  " unequalLeftIdCollapse = " << unequalLeftIdCollapse);
      WarningCheck(leq(i, resultSort),
		   "sort declarations for operator " << QUOTE(this) <<
		   " with left identity " << QUOTE(id) <<
		   " can cause collapse from sort " << QUOTE(resultSort) <<
		   " to " << QUOTE(component->sort(i)) <<
		   " (collapsing to a larger or incomparable sort is illegal).");
    }
}

void
BinarySymbol::rightIdentitySortCheck()
{
  Term* id = identityTerm.getTerm();
  const ConnectedComponent* component = rangeComponent();
  Assert(component == domainComponent(0),
	 "operator with right identity " << this <<
	 " has left argument and range in different sort components");
  Assert(id->getComponent() == domainComponent(1),
	 "operator " << this <<
	 " has right identity and right argument is different sort components");
  int nrSorts = component->nrSorts();
  //
  //	Check all collapses are to less or equal sorts.
  //
  int idIndex = id->getSortIndex();
  for (int i = 0; i < nrSorts; i++)
    {
      const Sort* resultSort = component->sort(traverse(traverse(0, i), idIndex));
      if (resultSort->index() != i && !unequalRightIdCollapse)
	{
	  Verbose("op " << this << " right-identity collapse from " <<
		  resultSort << " to " << component->sort(i) <<
		  " is unequal.");
	  unequalRightIdCollapse = true;
	}   
      DebugInfo("op " << this << " sort " << resultSort <<
		  " collapse to " << component->sort(i) <<
		  " unequalRightIdCollapse = " << unequalRightIdCollapse);
      WarningCheck(leq(i, resultSort),
		   "sort declarations for operator " << QUOTE(this) <<
		   " with right identity " << QUOTE(id) <<
		   " can cause collapse from sort " << QUOTE(resultSort) <<
		   " to sort " << QUOTE(component->sort(i)) <<
		   " (collapsing to a larger or incomparable sort is illegal).");
    }
}

void
BinarySymbol::idempotentSortCheck()
{
  const ConnectedComponent* component = rangeComponent();
  Assert(domainComponent(0) == component && domainComponent(1) == component,
	 "Idempotent operator " << this <<
	 " has a domain sort in a different connected component from its range sort");
  int nrSorts = component->nrSorts();
  for (int i = 1; i < nrSorts; i++)
    {
      const Sort* resultSort = component->sort(traverse(traverse(0, i), i));
      WarningCheck(leq(i, resultSort),
		   "sort declarations for idempotent operator " << QUOTE(this) <<
		   " can cause collapse from sort " << QUOTE(resultSort) <<
		   " to sort " << QUOTE(component->sort(i)) <<
		   " (collapsing to a larger or incomparable sort is illegal).");
    } 
}

bool
BinarySymbol::lookForCycle(Term* term, NatSet& examinedIds) const
{
  DebugAdvisory("BinarySymbol::lookForCycle() looking at " << term);
  //
  //	Check if we've cycled back.
  //
  Symbol* s = term->symbol();
  if (s == this)
    return true;
  //
  //	If we've reached symbol which has an identity we haven't explored, see
  //	if we can reach a cycle through it.
  //
  if (BinarySymbol* bs = dynamic_cast<BinarySymbol*>(s))
    {
      if (Term* id = bs->getIdentity())
	{
	  int index = bs->getIndexWithinModule();
	  if (!examinedIds.contains(index))
	    {
	      examinedIds.insert(index);
	      if (lookForCycle(id, examinedIds))
		return true;
	    }
	}
    }
  //
  //	Finally explore the arguments.
  //
  for (ArgumentIterator a(*term); a.valid(); a.next())
    {
      if (lookForCycle(a.argument(), examinedIds))
	return true;
    }
  return false;
}
