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
//      Implementation for class CUI_NumberOpSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "CUI_Theory.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"

//      built in stuff
#include "bindingMacros.hh"
#include "succSymbol.hh"
#include "CUI_NumberOpSymbol.hh"

CUI_NumberOpSymbol::CUI_NumberOpSymbol(int id, Axioms axioms)
  : CUI_Symbol(id, standard, false, axioms)
{
  op = NONE;
  succSymbol = 0;
}

bool
CUI_NumberOpSymbol::attachData(const Vector<Sort*>& opDeclaration,
			       const char* purpose,
			       const Vector<const char*>& data)
{
  BIND_OP(purpose, CUI_NumberOpSymbol, op, data);
  return CUI_Symbol::attachData(opDeclaration, purpose, data);
}

bool
CUI_NumberOpSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, succSymbol, SuccSymbol*);
  return CUI_Symbol::attachSymbol(purpose, symbol);
}

void
CUI_NumberOpSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  CUI_NumberOpSymbol* orig = safeCast(CUI_NumberOpSymbol*, original);
  op = orig->op;
  COPY_SYMBOL(orig, succSymbol, map, SuccSymbol*);
  CUI_Symbol::copyAttachments(original, map);
}

void
CUI_NumberOpSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				       Vector<const char*>& purposes,
				       Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "CUI_NumberOpSymbol";
  data.resize(nrDataAttachments + 1);
  data[nrDataAttachments].resize(1);
  const char*& d = data[nrDataAttachments][0];
  switch (op)
    {
    CODE_CASE(d, 's', 'd', "sd")
    default:
      CantHappen("bad number op");
    }
  CUI_Symbol::getDataAttachments(opDeclaration, purposes, data);
}

void
CUI_NumberOpSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, succSymbol);
  CUI_Symbol::getSymbolAttachments(purposes, symbols);
}

bool
CUI_NumberOpSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  CUI_DagNode* d = safeCast(CUI_DagNode*, subject);
  bool specialEval = true;
  //
  //	Evaluate our arguments and check that they are both numbers.
  //
  for (int i = 0; i < 2; i++)
    {
      DagNode* a = d->getArgument(i);
      a->reduce(context);
      if (!(succSymbol != 0 && succSymbol->isNat(a)))
	specialEval = false;
    }
  if (specialEval)
    {
      const mpz_class& a0 = succSymbol->getNat(d->getArgument(0));
      const mpz_class& a1 = succSymbol->getNat(d->getArgument(1));
      mpz_class r;
      switch (op)
	{
	case CODE('s', 'd'):
	  {
	    r = abs(a0 - a1);
	    break;
	  }
	default:
	  CantHappen("bad number op");
	}
      return succSymbol->rewriteToNat(subject, context, r);
    }
  return CUI_Symbol::eqRewrite(subject, context);
}
