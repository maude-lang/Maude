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
//      Class for symbols for built in LTL satisfiability solving.
//
#ifndef _satSolverSymbol_hh_
#define _satSolverSymbol_hh_
#include "temporalSymbol.hh"
#include "cachedDag.hh"

class SatSolverSymbol : public TemporalSymbol
{
  NO_COPYING(SatSolverSymbol);

public:
  SatSolverSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  void postInterSymbolPass();
  void reset();

private:
  DagNode* makeModel(const DagNodeSet& propositions,
		     const list<Bdd>& leadIn,
		     const list<Bdd>& cycle) const;
  DagNode* makeFormulaList(const DagNodeSet& propositions,
			   const list<Bdd>& bddList) const;
  DagNode* makeFormula(const DagNodeSet& propositions,
		       const Bdd& formula) const;

  //
  //	Symbols needed for returning models.
  //
  Symbol* formulaListSymbol;
  Symbol* nilFormulaListSymbol;
  Symbol* modelSymbol;

  CachedDag falseTerm;
};

#endif
