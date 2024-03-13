/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class MinusSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"

//      interface class definitions
#include "term.hh"
#include "argumentIterator.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeNet.hh"
#include "freeDagNode.hh"

//      built in stuff
#include "bindingMacros.hh"
#include "succSymbol.hh"
#include "numberOpSymbol.hh"
#include "minusSymbol.hh"

MinusSymbol::MinusSymbol(int id)
  : NumberOpSymbol(id, 1)
{
}

bool
MinusSymbol::attachData(const Vector<Sort*>& opDeclaration,
			const char* purpose,
			const Vector<const char*>& data)
{
  if (strcmp(purpose, "MinusSymbol") == 0)
    {
      if (!data.empty())
	return false;
      Vector<const char*> t(1);
      t[0] = "-";
      return NumberOpSymbol::attachData(opDeclaration, "NumberOpSymbol", t);
    }
  return NumberOpSymbol::attachData(opDeclaration, purpose, data);
}

void
MinusSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				Vector<const char*>& purposes,
				Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, MinusSymbol);
  //
  //	Need to bypass NumberOpSymbol::getDataAttachments() in order to
  //	avoid getting a NumberOpSymbol id hook.
  //
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

DagNode*
MinusSymbol::makeNegDag(const mpz_class& integer)
{
  Assert(integer < 0, "not negative");
  Vector<DagNode*> arg(1);
  arg[0] = getSuccSymbol()->makeNatDag(- integer);  // would like to avoid copy
  return makeDagNode(arg);
}

bool
MinusSymbol::isNeg(const DagNode* dagNode) const
{
  Assert(dagNode->symbol() == this, "symbol mismatch");
  DagNode* a = safeCast(const FreeDagNode*, dagNode)->getArgument(0);
  return a->symbol() == getSuccSymbol() && getSuccSymbol()->isNat(a);
}

const mpz_class&
MinusSymbol::getNeg(const DagNode* dagNode, mpz_class& result) const
{
  Assert(dagNode->symbol() == this, "symbol mismatch");
  result = - getSuccSymbol()->getNat(safeCast(const FreeDagNode*, dagNode)->getArgument(0));
  return result;
}

DagNode*
MinusSymbol::makeIntDag(const mpz_class& integer)
{
  return (integer >= 0) ? getSuccSymbol()->makeNatDag(integer) :
    makeNegDag(integer);
}

Term*
MinusSymbol::makeIntTerm(const mpz_class& integer)
{
  if (integer >= 0)
    return getSuccSymbol()->makeNatTerm(integer);
  Vector<Term*> arg(1);
  arg[0] = getSuccSymbol()->makeNatTerm(- integer);  // would like to avoid copy
  return makeTerm(arg);
}

bool
MinusSymbol::isNeg(const Term* term) const
{
  Assert(term->symbol() == this, "symbol mismatch");
  ArgumentIterator i(*term);
  Term* t = i.argument();
  return t->symbol() == getSuccSymbol() && getSuccSymbol()->isNat(t);
}

const mpz_class&
MinusSymbol::getNeg(const Term* term, mpz_class& result) const
{
  Assert(term->symbol() == this, "symbol mismatch");
  ArgumentIterator i(*term);
  result = - getSuccSymbol()->getNat(i.argument());
  return result;
}

bool
MinusSymbol::getSignedInt64(const DagNode* dagNode, Int64& value) const
{
  if (dagNode->symbol() == this)
    {
      const FreeDagNode* f = safeCast(const FreeDagNode*, dagNode);
      if (getSuccSymbol()->getSignedInt64(f->getArgument(0), value))
	{
	  value = -value;
	  return true;
	}
      return false;
    }
  return getSuccSymbol()->getSignedInt64(dagNode, value);
}
