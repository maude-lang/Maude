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
//      Implementation for class FloatTerm.
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
#include "term.hh"

//	core class definitions
#include "symbolMap.hh"

//	built in class definitions
#include "floatSymbol.hh"
#include "floatTerm.hh"
#include "floatDagNode.hh"

FloatTerm::FloatTerm(FloatSymbol* symbol, double value)
  : NA_Term(symbol),
    value((value == 0.0) ? 0.0 : value)  // don't allow IEEE-754 -0.0
{
}

Term*
FloatTerm::deepCopy2(SymbolMap* map) const
{
  return new FloatTerm(static_cast<FloatSymbol*>(map == 0 ? symbol() :
						 map->translate(symbol())),
		       value);
}

Term*
FloatTerm::normalize(bool /* full */, bool& changed)
{
  changed = false;
  Int64 bitPattern = *(reinterpret_cast<const Int64*>(&value));
  setHashValue(hash(symbol()->getHashValue(), bitPattern ^ (bitPattern >> 32)));
  return this;
}

int
FloatTerm::compareArguments(const Term* other) const
{
  double otherValue = static_cast<const FloatTerm*>(other)->value;
  //
  //	Straight forward subtraction might fail for values near +- 2^63
  //
  return (value == otherValue) ? 0 : ((value > otherValue) ? 1 : -1);
}

int
FloatTerm::compareArguments(const DagNode* other) const
{
  double otherValue = static_cast<const FloatDagNode*>(other)->value;
  //
  //	Straight forward subtraction might fail for values near +- 2^63
  //
  return (value == otherValue) ? 0 : ((value > otherValue) ? 1 : -1);
}

void
FloatTerm::overwriteWithDagNode(DagNode* old) const
{
  (void) new(old) FloatDagNode(static_cast<FloatSymbol*>(symbol()), value);
}

NA_DagNode*
FloatTerm::makeDagNode() const
{
  return new FloatDagNode(static_cast<FloatSymbol*>(symbol()), value);
}
