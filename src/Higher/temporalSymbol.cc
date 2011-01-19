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
//      Implementation for class TemporalSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//	temporal class definitions
#include "logicFormula.hh"

//      higher class definitions
#include "temporalSymbol.hh"

TemporalSymbol::TemporalSymbol(int id, int arity)
  : FreeSymbol(id, arity)
{
  trueSymbol = 0;
  falseSymbol = 0;
  notSymbol = 0;
  nextSymbol = 0;
  andSymbol = 0;
  orSymbol = 0;
  untilSymbol = 0;
  releaseSymbol = 0;
}

bool
TemporalSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, trueSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, falseSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, notSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, nextSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, andSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, orSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, untilSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, releaseSymbol, Symbol*);
  return FreeSymbol::attachSymbol(purpose, symbol);
}

void
TemporalSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  TemporalSymbol* orig = safeCast(TemporalSymbol*, original);
  COPY_SYMBOL(orig, trueSymbol, map, Symbol*);
  COPY_SYMBOL(orig, falseSymbol, map, Symbol*);
  COPY_SYMBOL(orig, notSymbol, map, Symbol*);
  COPY_SYMBOL(orig, nextSymbol, map, Symbol*);
  COPY_SYMBOL(orig, andSymbol, map, Symbol*);
  COPY_SYMBOL(orig, orSymbol, map, Symbol*);
  COPY_SYMBOL(orig, untilSymbol, map, Symbol*);
  COPY_SYMBOL(orig, releaseSymbol, map, Symbol*);
  FreeSymbol::copyAttachments(original, map);
}

void
TemporalSymbol::getSymbolAttachments(Vector<const char*>& purposes,
				     Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, trueSymbol);
  APPEND_SYMBOL(purposes, symbols, falseSymbol);
  APPEND_SYMBOL(purposes, symbols, notSymbol);
  APPEND_SYMBOL(purposes, symbols, nextSymbol);
  APPEND_SYMBOL(purposes, symbols, andSymbol);
  APPEND_SYMBOL(purposes, symbols, orSymbol);
  APPEND_SYMBOL(purposes, symbols, untilSymbol);
  APPEND_SYMBOL(purposes, symbols, releaseSymbol);
  FreeSymbol::getSymbolAttachments(purposes, symbols);
}

int
TemporalSymbol::build(LogicFormula& formula, DagNodeSet& propositions, DagNode* dagNode) const
{
  Symbol* s = dagNode->symbol();
  if (s == trueSymbol)
    return formula.makeOp(LogicFormula::LTL_TRUE);
  if (s == falseSymbol)
    return formula.makeOp(LogicFormula::LTL_FALSE);
  if (s == notSymbol || s == nextSymbol)
    {
      LogicFormula::Op op = (s == notSymbol) ? LogicFormula::NOT : LogicFormula::NEXT;
      DagArgumentIterator i(*dagNode);
      if (i.valid())
	{
	  int a = build(formula, propositions, i.argument());
	  if (a != NONE &&
	      (op != LogicFormula::NOT || formula.getOp(a) == LogicFormula::PROPOSITION))
	    return formula.makeOp(op, a);
	}
      return NONE;
    }
  if (s == andSymbol || s == orSymbol)
    {
      LogicFormula::Op op = (s == andSymbol) ? LogicFormula::AND : LogicFormula::OR;
      DagArgumentIterator i(*dagNode);
      if (i.valid())
	{
	  int a = build(formula, propositions, i.argument());
	  i.next();
	  if (a != NONE && i.valid())
	    {
	      int b;
	      for (;;)
		{
		  b = build(formula, propositions, i.argument());
		  if (b == NONE)
		    break;
		  a = formula.makeOp(op, a, b);
		  i.next();
		  if (!(i.valid()))
		    return a;
		}
	    }
	}
      return NONE;
    }
  if (s == untilSymbol || s == releaseSymbol)
    {
      LogicFormula::Op op = (s == untilSymbol) ? LogicFormula::UNTIL : LogicFormula::RELEASE;
      DagArgumentIterator i(*dagNode);
      if (i.valid())
	{
	  int a = build(formula, propositions, i.argument());
	  i.next();
	  if (a != NONE && i.valid())
	    {
	      int b = build(formula, propositions, i.argument());
	      if (b != NONE)
		return formula.makeOp(op, a, b);
	    }
	}
      return NONE;
    }
  int propIndex = propositions.dagNode2Index(dagNode);
  if (propIndex == NONE)
    {
      propIndex = propositions.cardinality();
      propositions.insert(dagNode);
    }
  return formula.makeProp(propIndex);
}

DagNode*
TemporalSymbol::conjunct(Vector<DagNode*>& args) const
{
  int last = args.length() - 1;
  if (last < 0)
    return trueSymbol->makeDagNode(args);

  static Vector<DagNode*> pair(2);
  pair[1] = args[last];
  for (int i = last - 1; i >= 0; --i)
    {
      pair[0] = args[i];
      pair[1] = andSymbol->makeDagNode(pair);
    }
  return pair[1];
}
