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
//	Class for symbols belonging to the AC and ACU theories.
//
#ifndef _ACU_Symbol_hh_
#define _ACU_Symbol_hh_
#include "associativeSymbol.hh"

class ACU_Symbol : public AssociativeSymbol
{
public:
  ACU_Symbol(int id,
	     const Vector<int>& strategy = standard,
	     bool memoFlag = false,
	     Term* identity = 0,
	     bool useTree = true);
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
  Term* termify(DagNode* dagNode);
  //
  //	Member functions overiding default handling.
  //
  void compileOpDeclarations();
  void postOpDeclarationPass();
  //
  //	Unification stuff.
  //
  void computeGeneralizedSort(const SortBdds& sortBdds,
			      const Vector<int>& realToBdd,
			      DagNode* subject,
			      Vector<Bdd>& generalizedSort);
  void computeGeneralizedSort2(const SortBdds& sortBdds,
			       const Vector<int>& realToBdd,
			       DagNode* subject,
			       Vector<Bdd>& outputBdds);
  UnificationSubproblem* makeUnificationSubproblem();
  int unificationPriority() const;
  bool canResolveTheoryClash();
  //
  //	Hash cons stuff.
  //
  DagNode* makeCanonical(DagNode* original, HashConsSet* hcs);
  DagNode* makeCanonicalCopy(DagNode* original, HashConsSet* hcs);
  //
  //	Member functions special to ACU_Symbol.
  //
  bool useTree() const;

protected:
  //
  //	For use in derived classes that override eqRewrite() so
  //	they don't have to known about the complexities of normalization.
  //	Only supports standard strategy; does not support memoization.
  //
  bool reduceArgumentsAndNormalize(DagNode* subject, RewritingContext& context);
  //
  //	Alow derived classes to make our dag nodes with resonable efficiency.
  //
  DagNode* makeDagNode(const Vector<DagNode*>& args,
		       const Vector<int>& multiplicities);

private:
  static bool normalize(DagNode* subject, RewritingContext& context);
  static bool copyReduceSubtermsAndNormalize(DagNode* subject, RewritingContext& context);
  bool rewriteAtTop(DagNode* subject, RewritingContext& context);
  bool rewriteAtTopNoOwise(DagNode* subject, RewritingContext& context);
  bool complexStrategy(DagNode* subject, RewritingContext& context);
  bool memoStrategy(MemoTable::SourceSet& from, DagNode* subject, RewritingContext& context);

  bool useTreeFlag;
};

inline bool
ACU_Symbol::useTree() const
{
  return useTreeFlag;
}

#endif
