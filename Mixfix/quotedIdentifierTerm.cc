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
//      Implementation for class QuotedIdentifierTerm.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "stringTable.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "symbolMap.hh"

//	front end class definitions
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "token.hh"


QuotedIdentifierTerm::QuotedIdentifierTerm(QuotedIdentifierSymbol* symbol, int idIndex)
  : NA_Term(symbol),
    idIndex(idIndex)
{
}

Term*
QuotedIdentifierTerm::deepCopy2(SymbolMap* map) const
{
  return new QuotedIdentifierTerm(static_cast<QuotedIdentifierSymbol*>(map == 0 ? symbol() :
								       map->translate(symbol())),
				  idIndex);
}

Term*
QuotedIdentifierTerm::normalize(bool /* full */, bool& changed)
{
  changed = false;
  setHashValue(hash(symbol()->getHashValue(), idIndex));
  return this;
}

int
QuotedIdentifierTerm::compareArguments(const Term* other) const
{
  return idIndex - static_cast<const QuotedIdentifierTerm*>(other)->idIndex;
}

int
QuotedIdentifierTerm::compareArguments(const DagNode* other) const
{
  return idIndex - static_cast<const QuotedIdentifierDagNode*>(other)->getIdIndex();
}

void
QuotedIdentifierTerm::overwriteWithDagNode(DagNode* old) const
{
  (void) new(old) QuotedIdentifierDagNode(static_cast<QuotedIdentifierSymbol*>(symbol()), idIndex);
}

NA_DagNode*
QuotedIdentifierTerm::makeDagNode() const
{
  return new QuotedIdentifierDagNode(static_cast<QuotedIdentifierSymbol*>(symbol()), idIndex);
}
