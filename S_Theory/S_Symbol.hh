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
//	Class for symbols belonging to the successor theory.
//
#ifndef _S_Symbol_hh_
#define _S_Symbol_hh_
#include "symbol.hh"

class S_Symbol : public Symbol
{
public:
  S_Symbol(int id, const Vector<int>& strategy = standard, bool memoFlag = false);
  //
  //	Member functions required by theory interface.
  //
  Term* makeTerm(const Vector<Term*>& args);
  DagNode* makeDagNode(const Vector<DagNode*>& args);
  void computeBaseSort(DagNode* subject);
  void normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context);
  bool eqRewrite(DagNode* subject, RewritingContext& context);  
  DagNode* ruleRewrite(DagNode* subject, RewritingContext& context);
  void stackArguments(DagNode* subject,
		      Vector<RedexPosition>& stack,
		      int parentIndex);
  //
  //    Member function overiding default handling.
  //
  void compileOpDeclarations();
  void finalizeSortInfo();
  //
  //	Member function specific to S_Symbol.
  //
  bool mightCollapseToOurSymbol(const Term* subterm) const;

private:
  struct SortPath
  {
    Vector<int> sortIndices;
    int leadLength;
  };

  void computePath(int sortIndex, SortPath& path);
  void memoStrategy(MemoTable::SourceSet& from,
		    DagNode* subject,
		    RewritingContext& context);
  
  Vector<SortPath> sortPathTable;
};

#endif
