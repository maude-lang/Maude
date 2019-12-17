/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class BranchSymbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
 
//      core class definitions
#include "argumentIterator.hh"
#include "rewritingContext.hh"
 
//      free theory class definitions
#include "freeDagNode.hh"
 
//      built in class definitions
#include "bindingMacros.hh"
#include "branchSymbol.hh"

BranchSymbol::BranchSymbol(int id, int nrArgs)
  : FreeSymbol(id, nrArgs)
{
}

BranchSymbol::~BranchSymbol()
{
  int nrTestTerms = testTerms.length();
  for (int i = 0; i < nrTestTerms; i++)
    testTerms[i]->deepSelfDestruct();
}

bool
BranchSymbol::attachData(const Vector<Sort*>& opDeclaration,
			 const char* purpose,
			 const Vector<const char*>& data)
{
  NULL_DATA(purpose, BranchSymbol, data);
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
BranchSymbol::attachTerm(const char* purpose, Term* term)
{
  int index = atoi(purpose);
  if (index > 0)
    {
      int len = testTerms.length();
      if (len < index)
	{
	  testTerms.resize(index);
	  for (; len < index; len++)
	    testTerms[len] = 0;
	}
      if (testTerms[index - 1] == 0)
	{
	  testTerms[index - 1] = term;
	  return true;
	}
      else
	{
	  DebugAdvisory("BranchSymbol::attachTerm(): " << index << " slot already contains " << testTerms[index - 1]);
	}
    }
  return FreeSymbol::attachTerm(purpose, term);
}

void
BranchSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  BranchSymbol* orig = safeCast(BranchSymbol*, original);
  int nrTerms = orig->testTerms.length();
  int len = testTerms.length();
  if (len < nrTerms)
    {
      testTerms.resize(nrTerms);
      for (; len < nrTerms; len++)
	testTerms[len] = 0;
    }

  for (int i = 0; i < nrTerms; i++)
    {
      if (testTerms[i] == 0)
	{
	  if (Term* t = orig->testTerms[i])
	    testTerms[i] = t->deepCopy(map);
	}
    }
  FreeSymbol::copyAttachments(original, map);
}

void
BranchSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				 Vector<const char*>& purposes,
				 Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, BranchSymbol);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
BranchSymbol::getTermAttachments(Vector<const char*>& purposes,
				 Vector<Term*>& terms)
{
  static Vector<string> numbers;
  int nrTerms = testTerms.length();
  int nrNumbers = numbers.length();
  if (nrNumbers < nrTerms)
    {
      numbers.resize(nrTerms);
      for (; nrNumbers < nrTerms; nrNumbers++)
	numbers[nrNumbers] = int64ToString(nrNumbers + 1);
    }
  for (int i = 0; i < nrTerms; i++)
    {
      if (Term* t = testTerms[i])
	{
	  purposes.append(numbers[i].c_str());
	  terms.append(t);
	}
    }
  FreeSymbol::getTermAttachments(purposes, terms);
}

bool
BranchSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  FreeDagNode* f = static_cast<FreeDagNode*>(subject);
  DagNode *e = f->getArgument(0);
  e->reduce(context);

  int nrTerms = testTerms.length();
  for (int i = 0; i < nrTerms; i++)
    {
      if(testTerms[i]->equal(e))
	return context.builtInReplace(subject, f->getArgument(i + 1));
    }
  //
  //	First argument failed to match any of our test terms. We now need to reduce
  //	all other arguments.
  //
  for (int i = 1; i <= nrTerms; i++)
    f->getArgument(i)->reduce(context);
  return FreeSymbol::eqRewrite(subject, context);
}

void
BranchSymbol::compileOpDeclarations()
{
  //
  //	Add fake declarations to encode our sort structure.
  //
  const Vector<Sort*>& baseDecl = getOpDeclarations()[0].getDomainAndRange();
  int declSize = baseDecl.size();
  Vector<Sort*> domainAndRange(declSize);
  domainAndRange[0] = baseDecl[0];
  ConnectedComponent* kind =  baseDecl[1]->component();
  int nrSorts = kind->nrSorts();
  for (int i = 1; i < nrSorts; ++i)
    {
      Sort* sort = kind->sort(i);
      //
      //	Add a declaration
      //	  firstSort sort sort ... sort -> sort
      //
      for (int j = 1; j < declSize; ++j)
	domainAndRange[j] = sort;
      addOpDeclaration(domainAndRange, false);  // should never be a ctor
    }
  FreeSymbol::compileOpDeclarations();
}

bool
BranchSymbol::rangeSortNeverLeqThan(Sort* /* sort */)
{
  return false;
}

bool
BranchSymbol::rangeSortAlwaysLeqThan(Sort* /* sort */)
{
  return false;
}

bool
BranchSymbol::domainSortAlwaysLeqThan(Sort* /* sort */, int /* argNr */)
{
  return false;
}

void
BranchSymbol::stackArguments(DagNode* subject,
			     Vector<RedexPosition>& stack,
			     int parentIndex,
			     bool respectFrozen,
			     bool eagerContext)
{
  //
  //	We need to define this because we have a builtin strategy. We stack
  //	the first argument as eager and the rest as lazy.
  //
  const NatSet& frozen = getFrozen();
  FreeDagNode* f = safeCast(FreeDagNode*, subject);
  DagNode* d = f->getArgument(0);
  if (!(respectFrozen && frozen.contains(0)) && !(d->isUnstackable()))
    stack.append(RedexPosition(d, parentIndex, 0, eagerContext));
  int nrTerms = testTerms.length();
  for (int i = 1; i <= nrTerms; i++)
    {
      d = f->getArgument(i);
      if (!(respectFrozen && frozen.contains(i)) && !(d->isUnstackable()))
	stack.append(RedexPosition(d, parentIndex, i, false));
    }
}
