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
//      Implementation for class SatSolverSymbol.
//
#include <list>

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
//#include "substitution.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//	temporal class definitions
#include "logicFormula.hh"
#include "genBuchiAutomaton.hh"

//      higher class definitions
#include "satSolverSymbol.hh"

SatSolverSymbol::SatSolverSymbol(int id)
  : TemporalSymbol(id, 1)
{
  formulaListSymbol = 0;
  nilFormulaListSymbol = 0;
  modelSymbol = 0;
}

bool
SatSolverSymbol::attachData(const Vector<Sort*>& opDeclaration,
			    const char* purpose,
			    const Vector<const char*>& data)
{
  NULL_DATA(purpose, SatSolverSymbol, data);
  return  TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
SatSolverSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, formulaListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, nilFormulaListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, modelSymbol, Symbol*);
  return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
SatSolverSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, falseTerm);
  return TemporalSymbol::attachTerm(purpose, term);
}

void
SatSolverSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  SatSolverSymbol* orig = safeCast(SatSolverSymbol*, original);
  COPY_SYMBOL(orig, formulaListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, nilFormulaListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, modelSymbol, map, Symbol*);

  COPY_TERM(orig, falseTerm, map);
  TemporalSymbol::copyAttachments(original, map);
}

void
SatSolverSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				       Vector<const char*>& purposes,
				       Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, SatSolverSymbol);
  TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
SatSolverSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, formulaListSymbol);
  APPEND_SYMBOL(purposes, symbols, nilFormulaListSymbol);
  APPEND_SYMBOL(purposes, symbols, modelSymbol);
  TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
SatSolverSymbol::getTermAttachments(Vector<const char*>& purposes,
				       Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, falseTerm);
  TemporalSymbol::getTermAttachments(purposes, terms);
}

void
SatSolverSymbol::postInterSymbolPass()
{
  PREPARE_TERM(falseTerm);
  TemporalSymbol::postInterSymbolPass();
}

void
SatSolverSymbol::reset()
{
  falseTerm.reset();  // so false dag can be garbage collected
  TemporalSymbol::reset();  // parents reset() tasks
}

bool
SatSolverSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");

  FreeDagNode* f = safeCast(FreeDagNode*, subject);
  DagNode* formulaDag = f->getArgument(0);
  formulaDag->reduce(context);

  DagNodeSet propositions;
  LogicFormula formula;
  int top = build(formula, propositions, formulaDag);
  if (top == NONE)
    {
      IssueAdvisory("LTL formula " << QUOTE(formulaDag) <<
		    " did not reduce to a valid negative normal form.");
      return TemporalSymbol::eqRewrite(subject, context);
    }
#ifdef TDEBUG
  cout << "top = " << top << endl;
  formula.dump(cout);
#endif

  GenBuchiAutomaton gba(&formula, top);
  list<Bdd> leadIn;
  list<Bdd> cycle;
  DagNode* resultDag = gba.satSolve(leadIn, cycle) ?
    makeModel(propositions, leadIn, cycle) :
    falseTerm.getDag();
  int nrStates = gba.getNrStates();
  int nrFairnessSets = gba.getNrFairnessSets();
  Verbose("SatSolverSymbol: Generalized Buchi automaton has " <<
	  nrStates << " state" << pluralize(nrStates) << " and " <<
	  nrFairnessSets << " fairness set" << pluralize(nrFairnessSets) << '.');
  return context.builtInReplace(subject, resultDag);
}

DagNode*
SatSolverSymbol::makeModel(const DagNodeSet& propositions,
			   const list<Bdd>& leadIn,
			   const list<Bdd>& cycle) const
{
  static Vector<DagNode*> args(2);
  args[0] = makeFormulaList(propositions, leadIn);
  args[1] = makeFormulaList(propositions, cycle);
  return modelSymbol->makeDagNode(args);
}

DagNode*
SatSolverSymbol::makeFormulaList(const DagNodeSet& propositions,
				 const list<Bdd>& bddList) const
{
  static Vector<DagNode*> args;
  args.contractTo(0);
  list<Bdd>::const_iterator e = bddList.end();
  for (list<Bdd>::const_iterator i = bddList.begin(); i != e; ++i)
    args.append(makeFormula(propositions, *i));
  int length = args.length();
  if (length == 0)
    return nilFormulaListSymbol->makeDagNode(args);
  else if (length == 1)
    return args[0];
  else
    return formulaListSymbol->makeDagNode(args);
}

DagNode*
SatSolverSymbol::makeFormula(const DagNodeSet& propositions,
			     const Bdd& formula) const
{
#ifdef TDEBUG
  BddUser::dump(cout,  formula);
  cout << endl;
#endif

  static Vector<DagNode*> args;
  args.contractTo(0);

  Bdd pi = formula.extractPrimeImplicant();
#ifdef TDEBUG
  BddUser::dump(cout,  pi);
  cout << endl;
#endif
  Assert(pi != bdd_false(), "bad formula");
  while (pi != bdd_true())
    {
      int var = bdd_var(pi);
      Bdd lo = bdd_low(pi);
      if (lo == bdd_false())
	{
	  args.append(propositions.index2DagNode(var));
	  pi = bdd_high(pi);
	}
      else
	{
	  Assert(bdd_high(pi) == bdd_false(), "bad formula");
	  args.append(negate(propositions.index2DagNode(var)));
	  pi = lo;
	}
    }
  return conjunct(args);
}
