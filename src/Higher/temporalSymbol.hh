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
//      Abstract base class for symbols that need to manipulate LTL formulae.
//
#ifndef _temporalSymbol_hh_
#define _temporalSymbol_hh_
#include "freeSymbol.hh"
#include "dagNodeSet.hh"
#include "bddUser.hh"

class TemporalSymbol : public FreeSymbol
{
  NO_COPYING(TemporalSymbol);

public:
  TemporalSymbol(int id, int arity);

  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);

protected:
  int build(LogicFormula& formula, DagNodeSet& propositions, DagNode* dagNode) const;
  // DagNode* formula2Dag(Bdd formula, DagNodeSet& propositions) const;
  DagNode* negate(DagNode* original) const;
  DagNode* conjunct(Vector<DagNode*>& args) const;

private:
  //
  //	Symbols needed for LTL formulae.
  //
  Symbol* trueSymbol;
  Symbol* falseSymbol;
  Symbol* notSymbol;
  Symbol* nextSymbol;
  Symbol* andSymbol;
  Symbol* orSymbol;
  Symbol* untilSymbol;
  Symbol* releaseSymbol;
};

#endif
