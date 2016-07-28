/*

    This file is part of the Maude 2 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class FloatTerm.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "symbolMap.hh"

//	built in class definitions
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberTerm.hh"
#include "SMT_NumberDagNode.hh"

SMT_NumberTerm::SMT_NumberTerm(SMT_NumberSymbol* symbol, const mpq_class& value)
  : NA_Term(symbol),
    value(value)
{
}

Term*
SMT_NumberTerm::deepCopy2(SymbolMap* map) const
{
  return new SMT_NumberTerm(static_cast<SMT_NumberSymbol*>(map == 0 ? symbol() :
							   map->translate(symbol())),
		       value);
}

Term*
SMT_NumberTerm::instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator)
{
  return new SMT_NumberTerm(safeCast(SMT_NumberSymbol*, translator->findTargetVersionOfSymbol(symbol())), value);
}

Term*
SMT_NumberTerm::normalize(bool /* full */, bool& changed)
{
  changed = false;
  setHashValue(hash(symbol()->getHashValue(),
		    mpz_tdiv_ui(value.get_num_mpz_t(), INT_MAX),
		    mpz_tdiv_ui(value.get_den_mpz_t(), INT_MAX)));
  return this;
}

int
SMT_NumberTerm::compareArguments(const Term* other) const
{
  const mpq_class& otherValue = static_cast<const SMT_NumberTerm*>(other)->value;
  return (value == otherValue) ? 0 : ((value > otherValue) ? 1 : -1);
}

int
SMT_NumberTerm::compareArguments(const DagNode* other) const
{
  const mpq_class& otherValue = *(static_cast<const SMT_NumberDagNode*>(other)->value);
  return (value == otherValue) ? 0 : ((value > otherValue) ? 1 : -1);
}

void
SMT_NumberTerm::overwriteWithDagNode(DagNode* old) const
{
  (void) new(old) SMT_NumberDagNode(static_cast<SMT_NumberSymbol*>(symbol()), value);
}

NA_DagNode*
SMT_NumberTerm::makeDagNode() const
{
  return new SMT_NumberDagNode(static_cast<SMT_NumberSymbol*>(symbol()), value);
}
