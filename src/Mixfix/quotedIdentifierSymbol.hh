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
//	Class for quoted identifier symbols.
//
#ifndef _quotedIdentifierSymbol_hh_
#define _quotedIdentifierSymbol_hh_
#include "NA_Symbol.hh"

class QuotedIdentifierSymbol : public NA_Symbol
{
public:
  QuotedIdentifierSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);

  void fillInSortInfo(Term* subject);
  void computeBaseSort(DagNode* subject);
  bool isConstructor(DagNode* subject);
  void compileOpDeclarations();
  Term* termify(DagNode* dagNode);

private:
  Sort* determineSort(int idIndex);

  Sort* baseSort;
  Sort* constantSort;
  Sort* variableSort;
  Sort* sortSort;
  Sort* kindSort;
};

#endif
