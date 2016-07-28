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
//	Class for terms containing SMT numbers.
//
#ifndef _SMT_NumberTerm_hh_
#define _SMT_NumberTerm_hh_
#include <gmpxx.h>
#include "NA_Term.hh"

class SMT_NumberTerm : public NA_Term
{
public:
  SMT_NumberTerm(SMT_NumberSymbol* symbol, const mpq_class& value);

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

  const mpq_class& getValue() const;

private:
  mpq_class value;
};

inline const mpq_class&
SMT_NumberTerm::getValue() const
{
  return value;
}

#endif
