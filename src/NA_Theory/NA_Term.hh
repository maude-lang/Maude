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
//	Abstract base class for terms in the NA theory.
//
#ifndef _NA_Term_hh_
#define _NA_Term_hh_
#include "term.hh"

class NA_Term : public Term
{
public:
  NA_Term(NA_Symbol* symbol);
  //
  //    Functions required by theory interface.
  //
  //	The following functions are left to the derived class to handle:
  //
  //	int compareArguments(const Term* other) const
  //	int compareArguments(const DagNode* other) const
  //
  RawArgumentIterator* arguments();
  void deepSelfDestruct();
  void findEagerVariables(bool atTop, NatSet& eagerVariables) const;
  void analyseConstraintPropagation(NatSet& BoundUniquely) const;
  LhsAutomaton* compileLhs2(bool matchAtTop,
                           const VariableInfo& variableInfo,
                           NatSet& boundUniquely,
                           bool& subproblemLikely);
  void markEagerArguments(int nrVariables,
                          const NatSet& eagerVariables,
                          Vector<int>& problemVariables);
  DagNode* dagify2();

  void findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop);
  int compileRhs2(RhsBuilder& rhsBuilder,
		  VariableInfo& variableInfo,
		  TermBag& availableTerms,
		  bool eagerContext);

  //
  //	The following pure virtual functions are particular to NA_Term
  //	and must be defined by the derived class.
  //
  virtual void overwriteWithDagNode(DagNode* old) const = 0;
  virtual NA_DagNode* makeDagNode() const = 0;
};

#endif
