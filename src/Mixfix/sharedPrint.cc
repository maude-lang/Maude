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
//	Functions shared between 2 or more pretty printers.
//

bool
MixfixModule::ambiguous(int iflags)
{
  //
  //	If there is an operator with the same name and domain kinds then
  //	disambiguation must be down at the currently level.
  //
  if (iflags & DOMAIN_OVERLOADED)
    return true;
  //
  //	If the operator is nullary and has the same lexical syntax as built-in representation, we
  //	need to check if at least one set of the appropriate built-ins is in play.
  //
  if (iflags & PSEUDOS)  // only nullary operators get PSEUDO flags
    {
      if (iflags & PSEUDO_VARIABLE)  // e.g. X:Foo
	return true;
      else if (iflags & PSEUDO_NAT)  // e.g. 42
	return !(kindsWithSucc.empty());
      else if (iflags & PSEUDO_NEG)  // e.g. -42
	return !(kindsWithMinus.empty());
      else if (iflags & PSEUDO_ZERO)  // 0 is the only example
	return !(kindsWithZero.empty());
      else if (iflags & PSEUDO_RAT)  // e.g. -1/2
	return !(kindsWithDivision.empty());
      else if (iflags & PSEUDO_FLOAT)  // e.g. 1.0
	return !(floatSymbols.empty());
      else if (iflags & PSEUDO_STRING)  // e.g. "a"
	return !(stringSymbols.empty());
      else if (iflags & PSEUDO_QUOTED_IDENTIFIER)  // e.g. 'a
	return !(quotedIdentifierSymbols.empty());
    }
  return false;
}

bool
MixfixModule::rangeOfArgumentsKnown(int iflags, bool rangeKnown, bool rangeDisambiguated)
{
  //
  //	If we don't have any ad hoc overloading of the operator, then we know
  //	the kind of each argument.
  //
  if (!(iflags & ADHOC_OVERLOADED))
    return true;
  //
  //	If the operator can be uniquely distinguished by its range and name
  //	despite ad hoc overloading, then if we know are range or we inserted
  //	range disambiguation again we know the kind of each argument.
  //
  if (!(iflags & RANGE_OVERLOADED) && (rangeKnown || rangeDisambiguated))
    return true;
  //
  //	The operator is ad hoc overloaded and the environment does not
  //	determine the operator uniquely. We will assume that the kind of
  //	all arguments will need to be unambigous to correctly determine the
  //	operator, and thus the range of each argument is unknown. This is simple
  //	but unduly conservative.
  //
  return false;
}

void
MixfixModule::prefix(ostream& s, bool needDisambig, const char* color)
{
  if (needDisambig)
    s << '(';
  if (color != 0)
    s << color;
}
