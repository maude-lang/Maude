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
//      Implementation for class QuotedIdentifierDagNode.
//
 
//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      front end class definitions
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"
#include "token.hh"

QuotedIdentifierDagNode::QuotedIdentifierDagNode(QuotedIdentifierSymbol* symbol,
						 int idIndex)
  : NA_DagNode(symbol),
    idIndex(idIndex)
{
}

size_t
QuotedIdentifierDagNode::getHashValue()
{
  return hash(symbol()->getHashValue(), idIndex);
}

int
QuotedIdentifierDagNode::compareArguments(const DagNode* other) const
{
  int otherIndex = static_cast<const QuotedIdentifierDagNode*>(other)->idIndex;
  if (idIndex == otherIndex)
    return 0;
  return strcmp(Token::name(idIndex), Token::name(otherIndex));
}
 
void
QuotedIdentifierDagNode::overwriteWithClone(DagNode* old)
{
  QuotedIdentifierDagNode* d =
    new(old) QuotedIdentifierDagNode(safeCast(QuotedIdentifierSymbol*, symbol()),
				     idIndex);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
QuotedIdentifierDagNode::makeClone()
{
  QuotedIdentifierDagNode* d =
    new QuotedIdentifierDagNode(safeCast(QuotedIdentifierSymbol*, symbol()),
				idIndex);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}
