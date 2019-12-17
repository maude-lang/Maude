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
//	Class for symbols belonging to theories made from C, Ul, Ur and I axioms.
//
#ifndef _CUI_Symbol_hh_
#define _CUI_Symbol_hh_
#include "binarySymbol.hh"

class CUI_Symbol : public BinarySymbol
{
public:
  enum Axioms
  {
    COMM = 1,		// commutativity
    LEFT_ID = 2,	// left identity
    RIGHT_ID = 4,	// right identity
    IDEM = 8		// idempotence
  };

  CUI_Symbol(int id,
	     const Vector<int>& strategy,
	     bool memoFlag,
	     Axioms axioms,
	     Term* identity = 0);
  //
  //	Member functions required by theory interface.
  //
  Term* makeTerm(const Vector<Term*>& args);
  DagNode* makeDagNode(const Vector<DagNode*>& args);
  void computeBaseSort(DagNode* subject);
  void normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context);
  bool eqRewrite(DagNode* subject, RewritingContext& context);  
  void stackArguments(DagNode* subject,
		      Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen,
		      bool eagerContext);
  Term* termify(DagNode* dagNode);
  //
  //	Member functions overiding default handling.
  //
  void compileOpDeclarations();
  void postOpDeclarationPass();
  void setFrozen(const NatSet& frozen);
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
  //	Member functions specific to CUI_Symbol.
  //
  bool comm() const;
  bool leftId() const;
  bool rightId() const;
  bool idem() const;

private:
  bool memoStrategy(MemoTable::SourceSet& from, DagNode* subject, RewritingContext& context);

  Axioms axioms;
};

inline bool
CUI_Symbol::comm() const
{
  return axioms & COMM;
}

inline bool
CUI_Symbol::leftId() const
{
  return axioms & LEFT_ID;
}

inline bool
CUI_Symbol::rightId() const
{
  return axioms & RIGHT_ID;
}

inline bool
CUI_Symbol::idem() const
{
  return axioms & IDEM;
}

#endif
