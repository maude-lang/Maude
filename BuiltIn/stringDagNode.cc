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
//      Implementation for class StringDagNode.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      built in class definitions
#include "stringSymbol.hh"
#include "stringDagNode.hh"

StringDagNode::StringDagNode(StringSymbol* symbol, const crope& value)
  : NA_DagNode(symbol),
    value(value)
{
  //Assert(sizeof(crope) <= DagNode::nrWords * sizeof(DagNode::Word),
  // cerr << "crope too big for internal storage");  // HACK
  setCallDtor();  // need our dtor called when garbage collected to destruct crope
}

size_t
StringDagNode::getHashValue()
{
  int hashValue = 0;
  for (crope::const_iterator i(value.begin()); i != value.end(); i++)
    hashValue = (hashValue << 1) + *i;
  return hash(symbol()->getHashValue(), hashValue);
}

int
StringDagNode::compareArguments(const DagNode* other) const
{
  return value.compare(static_cast<const StringDagNode*>(other)->value);
}
 
void
StringDagNode::overwriteWithClone(DagNode* old)
{
  StringDagNode* d = new(old) StringDagNode(safeCast(StringSymbol*, symbol()), value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
StringDagNode::makeClone()
{
  StringDagNode* d = new StringDagNode(safeCast(StringSymbol*, symbol()), value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}
