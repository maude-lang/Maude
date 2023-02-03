/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Abstract class for statement transformers.
//
#ifndef _statementTransformer_hh_
#define _statementTransformer_hh_

class StatementTransformer
{
public:
  enum class Outcome
    {
     NOT_APPLICABLE,	// transformation not relevant
     NOT_NEEDED,	// transformation relevant but needed
     NOT_TRANSFORMED,	// transformation encounter problem; statement unchanged
     TRANSFORMED	// statement transformed
    };

  virtual Outcome transformSortConstraint(SortConstraint* sortConstraint) = 0;
  virtual Outcome transformEquation(Equation* equation) = 0;
  virtual Outcome transformRule(Rule* rule) = 0;
};

#endif
