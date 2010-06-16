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
//      Class for symbols for built in branch (if-then-else-fi type) operations.
//
#ifndef _branchSymbol_hh_
#define _branchSymbol_hh_
#include "freeSymbol.hh"

class BranchSymbol : public FreeSymbol
{
public:
  BranchSymbol(int id, int nrArgs);
  ~BranchSymbol();

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getTermAttachments(Vector<const char*>& purposes,
			  Vector<Term*>& terms);
  //
  //	Built in equational rewriting semantics and strategy.
  //
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void stackArguments(DagNode* subject,
		      Vector<RedexPosition>& stack,
		      int parentIndex);
  //
  //	We need to insert some fake declarations to encode our sort
  //	structure and we disable sort based optimizations.
  //
  void compileOpDeclarations();
  bool rangeSortNeverLeqThan(Sort* sort);
  bool rangeSortAlwaysLeqThan(Sort* sort);
  bool domainSortAlwaysLeqThan(Sort* sort, int argNr);

private:
  Vector<Term*> testTerms;
};

#endif
