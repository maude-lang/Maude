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
//      Implementation for class FloatDagNode.
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
#include "floatSymbol.hh"
#include "floatDagNode.hh"

FloatDagNode::FloatDagNode(FloatSymbol* symbol, double value)
  : NA_DagNode(symbol),
    value((value == 0.0) ? 0.0 : value)  // don't allow IEEE-754 -0.0
{
}

size_t
FloatDagNode::getHashValue()
{
  Int64 bitPattern = *(reinterpret_cast<const Int64*>(&value));
  return hash(symbol()->getHashValue(), bitPattern ^ (bitPattern >> 32));
}

int
FloatDagNode::compareArguments(const DagNode* other) const
{
  double otherValue = static_cast<const FloatDagNode*>(other)->value;
  return (value == otherValue) ? 0 : ((value > otherValue) ? 1 : -1);
}
 
void
FloatDagNode::overwriteWithClone(DagNode* old)
{
  FloatDagNode* d = new(old) FloatDagNode(safeCast(FloatSymbol*, symbol()), value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
FloatDagNode::makeClone()
{
  FloatDagNode* d = new FloatDagNode(safeCast(FloatSymbol*, symbol()), value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}
