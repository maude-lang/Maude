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
//      Class for variable symbols.
//
#ifndef _variableSymbol_hh_
#define _variableSymbol_hh_
#include "symbol.hh"

class VariableSymbol : public Symbol
{
  NO_COPYING(VariableSymbol);

public:
  VariableSymbol(int id);

  Term* makeTerm(const Vector<Term*>& args);
  DagNode* makeDagNode(const Vector<DagNode*>& args);
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void computeBaseSort(DagNode* subject);
  void normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context);
  Term* termify(DagNode* dagNode);
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

  bool isStable() const;
  bool determineGround(DagNode* dagNode);
  //
  //	Hash cons stuff.
  //
  DagNode* makeCanonical(DagNode* original, HashConsSet* /* hcs */);
  DagNode* makeCanonicalCopy(DagNode* original, HashConsSet* /* hcs */);
  //
  //	VariableSymbol specific functions.
  //
  Sort* getSort();
};

inline Sort*
VariableSymbol::getSort()
{
  //
  //	Temporary hack until sorts mechanism revised.
  //
  const Vector<OpDeclaration>& s = getOpDeclarations();
  Assert(s.length() == 1, "s.length() != 1");
  const Vector<Sort*>& v = s[0].getDomainAndRange();
  Assert(v.length() == 1, "v.length() != 1");
  return v[0];
}

#endif
