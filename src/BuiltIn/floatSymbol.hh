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
//	Class for floating point number symbols.
//
#ifndef _floatSymbol_hh_
#define _floatSymbol_hh_
#include "NA_Symbol.hh"

class FloatSymbol : public NA_Symbol
{
public:
  FloatSymbol(int id);

  void fillInSortInfo(Term* subject);
  void computeBaseSort(DagNode* subject);
  void compileOpDeclarations();
  bool isConstructor(DagNode* subject);
  bool rewriteToFloat(DagNode* subject,
		      RewritingContext& context,
		      double result);
  Term* termify(DagNode* dagNode);

private:
  Sort* sort;
  Sort* finiteSort;
};

#endif
