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
//      Class for bag of pointers to terms occurring in patterns or right
//	hand sides that can be reused in building right hand sides to enable
//	common subexpression sharing both within rhs and lhs->rhs.
//
#ifndef _termBag_hh_
#define _termBag_hh_
#include <set>

class TermBag
{
public:
  void insertMatchedTerm(Term* term, bool eagerContext);
  void insertBuiltTerm(Term* term, bool eagerContext);
  Term* findTerm(Term* term, bool eagerContext);

private:
  struct LtTerm
  {
    bool
    operator()(Term* const& t1, Term* const& t2) const
    {
      return t1->compare(t2) < 0;
    }
  };

  typedef set<Term*, LtTerm> TermSet;

  TermSet termsUsableInEagerContext;
  TermSet termsUsableInLazyContext;
};

#endif
