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
//      Class for symbols for built in sort test operations.
//
#ifndef _sortTestSymbol_hh_
#define _sortTestSymbol_hh_
#include "freeSymbol.hh"

class SortTestSymbol : public FreeSymbol
{
public:
  SortTestSymbol(int id,
		 Sort* testSort,
		 FreeSymbol* leq,
		 FreeSymbol* nleq,
		 bool eager);
 
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  bool eager() const;
  const Sort* sort() const;
  //
  //	We don't accept or compile any equations.
  //
  bool acceptEquation(Equation* equation);
  void compileEquations();

private:
  static const Vector<int>& makeLazyStrategy();

  Sort* cmpSort;
  FreeSymbol* leqResult;
  FreeSymbol* notLeqResult;
  bool eagerFlag;
};

inline bool
SortTestSymbol::eager() const
{
  return eagerFlag;
}

inline const Sort*
SortTestSymbol::sort() const
{
  return cmpSort;
}

#endif
