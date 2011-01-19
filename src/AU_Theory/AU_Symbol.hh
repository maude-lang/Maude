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
//	Class for symbols belonging to the A, AUl, AUr and AU theories.
//
#ifndef _AU_Symbol_hh_
#define _AU_Symbol_hh_
#include "associativeSymbol.hh"

class AU_Symbol : public AssociativeSymbol
{
public:
  AU_Symbol(int id,
	    const Vector<int>& strategy,
	    bool memoFlag,
	    bool leftId = false,
	    bool rightId = false,
	    Term* identity = 0);
  //
  //    Member functions required by theory interface.
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
  //    Member functions overiding default handling.
  //
  void postOpDeclarationPass();
  void compileEquations();
  //
  //	Member functions particular to AU_Symbol.
  //
  bool leftId() const;
  bool rightId() const;
  bool oneSidedId() const;
  bool useDeque() const;
  int calculateNrSubjectsMatched(DagNode* d,
				 bool leftEnd,  // match starts at left extreme
				 bool rightEnd,  // match end at right extreme
				 bool& nasty);
  //
  //	Hash cons stuff.
  //
  DagNode* makeCanonical(DagNode* original, HashConsSet* hcs);
  DagNode* makeCanonicalCopy(DagNode* original, HashConsSet* hcs);

private:
  bool rewriteAtTop(AU_DagNode* subject, RewritingContext& context);
  bool rewriteAtTopNoOwise(AU_DagNode* subject, RewritingContext& context);
  bool complexStrategy(AU_DagNode* subject, RewritingContext& context);
  bool memoStrategy(MemoTable::SourceSet& from,
		    DagNode* subject,
		    RewritingContext& context);
  void copyAndReduceSubterms(AU_DagNode* subject, RewritingContext& context);

  const Bool leftIdFlag;
  const Bool rightIdFlag;
  const Bool oneSidedIdFlag;
  //
  //	Deque represention of arguments can be used if there is no
  //	one-sided identity, no fancy strategy and there are no equations
  //	at the top.
  //
  Bool useDequeFlag;
};

inline bool
AU_Symbol::leftId() const
{
  return leftIdFlag;
}

inline bool
AU_Symbol::rightId() const
{
  return rightIdFlag;
}

inline bool
AU_Symbol::oneSidedId() const
{
  return oneSidedIdFlag;
}

inline bool
AU_Symbol::useDeque() const
{
  return useDequeFlag;
}

#endif
