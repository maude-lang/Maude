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
//	Class for terms containing strings.
//
#ifndef _stringTerm_hh_
#define _stringTerm_hh_
#include "ropeStuff.hh"
#include "NA_Term.hh"

class StringTerm : public NA_Term
{
public:
  StringTerm(StringSymbol* symbol, const crope& value);

  Term* deepCopy2(SymbolMap* map) const;
  Term* normalize(bool full, bool& changed);
  int compareArguments(const Term* other) const;
  int compareArguments(const DagNode* other) const;
  void overwriteWithDagNode(DagNode* old) const;
  NA_DagNode* makeDagNode() const;
  //
  //	Needed because we have hidden data.
  //
  Term* instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator);

  const crope& getValue() const;

private:
  const crope value;
};


inline const crope&
StringTerm::getValue() const
{
  return value;
}

#endif
