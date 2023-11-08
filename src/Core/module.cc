/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class Module.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "sortConstraint.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortBdds.hh"
#include "memoMap.hh"
#include "module.hh"
#include "strategyDefinition.hh"
#include "rewriteStrategy.hh"

//	variable class definitions
#include "variableTerm.hh"

Module::Module(int id)
  : NamedEntity(id)
{
  status = OPEN;
  sortBdds = 0;
  minimumSubstitutionSize = 1;
  memoMap = 0;
}

Module::~Module()
{
  DebugAdvisory("destructing module " << this);
  delete sortBdds;
  //
  //	This is delicate; first we lose any root pointers to
  //	dag nodes that might contain pointers to our symbols.
  //
  clearMemo();
  reset();
  //
  //	Then we delete everything except for the symbols.
  //
  for (Sort* s : sorts)
    delete s;
  for (ConnectedComponent* c : connectedComponents)
    delete c;
  for (SortConstraint* sc : sortConstraints)
    delete sc;
  for (Equation* eq : equations)
    delete eq;
  for (Rule* rl : rules)
    delete rl;
  for (StrategyDefinition* sd : strategyDefinitions)
    delete sd;
  for (RewriteStrategy* rs : strategies)
    delete rs;
  //
  //	And finally it is safe to delete our symbols.
  //
#if 1
  for (Symbol* s : symbols)
    delete s;
#else
  //
  //	Trample over Symbols rather than deleting them, in order
  //	to look for bugs.
  //
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; ++i)
    {
      void* p = symbols[i];
      cerr << "deleting " << symbols[i] << " at "<< p << endl;
      memset(p, -1, sizeof(Symbol));
    }
#endif
}


const SortBdds*
Module::getSortBdds()
{
  if (sortBdds == 0)
    sortBdds = new SortBdds(this);
  return sortBdds;
}

void
Module::insertSortConstraint(SortConstraint* sortConstraint)
{
  Assert(status < THEORY_CLOSED, "bad status");
  sortConstraint->setModuleInfo(this, sortConstraints.length());
  sortConstraints.append(sortConstraint);
}

void
Module::checkSortConstraint(SortConstraint* sortConstraint)
{
  Assert(status < THEORY_CLOSED, "bad status");
  sortConstraint->check();
}

void
Module::insertEquation(Equation* equation)
{
  Assert(status < THEORY_CLOSED, "bad status");
  equation->setModuleInfo(this, equations.length());
  equations.append(equation);
}

void
Module::checkEquation(Equation* equation)
{
  Assert(status < THEORY_CLOSED, "bad status");
  equation->check();
}

void
Module::insertRule(Rule* rule)
{
  Assert(status < THEORY_CLOSED, "bad status");
  rule->setModuleInfo(this, rules.length());
  rules.append(rule);
}

void
Module::checkRule(Rule* rule)
{
  Assert(status < THEORY_CLOSED, "bad status");
  rule->check();
}

void
Module::insertStrategy(RewriteStrategy* strategy)
{
  Assert(status < THEORY_CLOSED, "bad status");
  strategy->setModuleInfo(this, strategies.length());
  strategies.append(strategy);
}

void
Module::insertStrategyDefinition(StrategyDefinition* sdef)
{
  Assert(status < THEORY_CLOSED, "bad status");
  sdef->setModuleInfo(this, strategyDefinitions.length());
  strategyDefinitions.append(sdef);
  sdef->check();
}

void
Module::closeSortSet()
{
  Assert(status == OPEN, "bad status");
  //
  //	We can't use a range-based for loop here because the number
  //	of sorts will grow as we add error sorts, invalidating iterators.
  //	
  int nrSorts = sorts.length();
  for (int i = 0; i < nrSorts; ++i)
    {
      Sort* s = sorts[i];
      if (s->component() == 0)
        {
          ConnectedComponent* c = new ConnectedComponent(s);
          c->setModuleInfo(this, connectedComponents.length());
          connectedComponents.append(c);
        }
    }
  status = SORT_SET_CLOSED;
}

void
Module::closeSignature()
{
  Assert(status == SORT_SET_CLOSED, "bad status");
  status = SIGNATURE_CLOSED;
}

void
Module::closeFixUps()
{
  Assert(status == SIGNATURE_CLOSED, "bad status");
  status = FIX_UPS_CLOSED;
  //
  //	First deal with any inter-symbol dependences until
  //	we hit stability.
  //	This is currently used to normalize identities.
  //
  bool somethingChanged;
  do
    {
      somethingChanged = false;
      for (Symbol* s : symbols)
	{
	  if (s->interSymbolPass())
	    somethingChanged = true;
	}
    }
  while (somethingChanged);
  //
  //	Then deal with things want to be done just after
  //	any inter-symbol dependences have stabilized.
  //	This is currently used to normalize other terms
  //	that get attached to symbols.
  //
  for (Symbol* s : symbols)
    s->postInterSymbolPass();
}

void
Module::closeTheory()
{
  DebugInfo("semi-compiling " << this);
  //
  //	This is where most of the work for analyzing and (semi-)compiling
  //	a module is done.
  //
  Assert(status == FIX_UPS_CLOSED, "bad status");
  status = THEORY_CLOSED;
  //
  //	First compile the sort declarations for each operator.
  //	We could have done this much earlier but since it is often
  //	the most time consuming part of compilation and produces
  //	voluminous sort tables we postpone it we are actually going to
  //	do some useful work in the module.
  //
  for (Symbol* s : symbols)
    s->compileOpDeclarations();
  //
  //	Then decide in which components the error sort can actually be
  //	produced. This is needed for efficient compilation of sort tests
  //	in pattern matching.
  //
  {
    bool changed;
    do
      {
	changed = false;
	for (Symbol* s : symbols)
	  {
	    ConnectedComponent* c = s->rangeComponent();
	    if (c->errorFree() && s->canProduceErrorSort())
	      {
		c->errorSortSeen();
		changed = true;
		DebugAdvisory("sort " << c->sort(0) << " corrupted by " << s);
	      }
	  }
      }
    while (changed);  // iterate until fix point
  }
  //
  //	Then do another pass over the symbols to handle things
  //	that depend on the sort tables.
  //	This is currently used for identity sort computation.
  //
  for (Symbol* s : symbols)
    s->postOpDeclarationPass();
  //
  //	Deal with any sort constraints.
  //	We need to have computed to sort of each identity at this point
  //	so that we can analyseCollapses() on sort contraint patterns.
  //
  indexSortConstraints();
  for (Symbol* s : symbols)
    s->orderSortConstraints();
  //
  //	Certain sort properties relative to a symbol can only be determined after 
  //	the existance of sort constraints applicable to that symbol are known.
  //
  for (Symbol* s : symbols)
    s->finalizeSortInfo();
  //
  //	Now index equations and rules and compile statements.
  //
  indexEquations();
  indexRules();
  for (Symbol* s : symbols)
    {
      s->compileSortConstraints();
      s->compileEquations();
      s->compileRules();
    }
  //
  //	Index and compile strategy definitions.
  //
  for (StrategyDefinition* sd : strategyDefinitions)
    {
      if (!(sd->isBad() || sd->isNonexec()))
	{
	  sd->getStrategy()->addDefinition(sd);
	  sd->preprocess();
	  sd->compile();
	}
    }
}

void
Module::stackMachineCompile()
{
  Assert(status >= THEORY_CLOSED, "bad status");
  if (status == STACK_MACHINE_COMPILED)
    return;
  status = STACK_MACHINE_COMPILED;
  for (Equation* e : equations)
    e->stackMachineCompile();
}

void
Module::indexSortConstraints()
{
  for (SortConstraint* sc : sortConstraints)
    {
      if (!(sc->isBad()))
	{
	  sc->preprocess();  // nonexec mbs need to be preprocessed
	  if (!(sc->isNonexec()))
	    {
	      Term* lhs = sc->getLhs();
	      bool noCollapse = lhs->collapseSymbols().empty();
	      AdvisoryCheck(noCollapse,
			    *lhs << ": collapse at top of " << QUOTE(lhs) <<
			    " may cause it to match more than you expect.");
	      if (noCollapse && dynamic_cast<VariableTerm*>(lhs) == 0)
		lhs->symbol()->offerSortConstraint(sc);
	      else
		{
		  for (Symbol* s : symbols)
		    s->offerSortConstraint(sc);
		}
	    }
	}
    }
}

void
Module::indexEquation(Equation* eq)
{
  eq->preprocess();
  if (!(eq->isNonexec()))
    {
      Term* lhs = eq->getLhs();
      lhs->computeMatchIndices();
      bool noCollapse = lhs->collapseSymbols().empty();
      AdvisoryCheck(noCollapse,
		    *lhs << ": collapse at top of " << QUOTE(lhs) <<
		    " may cause it to match more than you expect.");
      if (noCollapse && dynamic_cast<VariableTerm*>(lhs) == 0)
	lhs->symbol()->offerEquation(eq);
      else
	{
	  for (Symbol* s : symbols)
	    s->offerEquation(eq);
	}
    }
}

void
Module::indexEquations()
{
  //
  //	First index non-owise equations.
  //
  for (Equation* eq : equations)
    {
      if (!(eq->isBad()) && !(eq->isOwise()))
	indexEquation(eq);
    }
  //
  //	Then index owise equations.
  //
  for (Equation* eq : equations)
    {
      if (!(eq->isBad()) && eq->isOwise())
	indexEquation(eq);
    }
}

void
Module::indexRules()
{
  for (Rule* rl : rules)
    {
      if (!(rl->isBad()))
	{
	  rl->preprocess();
	  Term* lhs = rl->getLhs();
	  bool noCollapse = lhs->collapseSymbols().empty();
	  AdvisoryCheck(noCollapse,
			*lhs << ": collapse at top of " << QUOTE(lhs) <<
			" may cause it to match more than you expect.");
	  if (noCollapse && dynamic_cast<VariableTerm*>(lhs) == 0)
	    lhs->symbol()->offerRule(rl);
	  else
	    {
	      for (Symbol* s : symbols)
		s->offerRule(rl);
	    }
	}
    }
}

void
Module::insertLateSymbol(Symbol* s)
{
  Assert(status >= SIGNATURE_CLOSED, "bad status");
  s->setModuleInfo(this, symbols.length());
  symbols.append(s);

  if (status < FIX_UPS_CLOSED)
    return;  // nothing extra to do
  //
  //	closeFixUps() stuff:
  //	Late symbols are not allowed to affect other symbols so we
  //	don't iterate the interSymbolPass() pass.
  //
  s->interSymbolPass();
  s->postInterSymbolPass();

  if (status < THEORY_CLOSED)
    return;
  //
  //	closeTheory() stuff:
  //	Late symbols are not allowed to create new error sorts so we
  //	don't do the canProduceErrorSort() pass.
  //
  s->compileOpDeclarations();
  s->postOpDeclarationPass();

  //
  //	See if any existing sort constraint could apply to late symbol.
  //
  for (SortConstraint* sc : sortConstraints)
    {
      if (!(sc->isBad() || sc->isNonexec()))
	{
	  Term* lhs = sc->getLhs();
	  if (dynamic_cast<VariableTerm*>(lhs) != 0 || !(lhs->collapseSymbols().empty()))
	    s->offerSortConstraint(sc);
	}
    }
  s->orderSortConstraints();
  s->finalizeSortInfo();
  //
  //	See if any existing equations could apply to late symbol.
  //
  for (Equation* eq : equations)
    {
      if (!(eq->isBad() || eq->isNonexec()) && !(eq->isOwise()))
	{
	  Term* lhs = eq->getLhs();
	  if (dynamic_cast<VariableTerm*>(lhs) != 0 || !(lhs->collapseSymbols().empty()))
	    s->offerEquation(eq);
	}
    }
  for (Equation* eq : equations)
    {
      if (!(eq->isBad() || eq->isNonexec()) && eq->isOwise())
	{
	  Term* lhs = eq->getLhs();
	  if (dynamic_cast<VariableTerm*>(lhs) != 0 || !(lhs->collapseSymbols().empty()))
	    s->offerEquation(eq);
	}
    } 
  //
  //	See if any existing rules could apply to late symbol.
  //
  for (Rule* rl : rules)
    {
      if (!(rl->isBad()))
	{
	  Term* lhs = rl->getLhs();
	  if (dynamic_cast<VariableTerm*>(lhs) != 0 || !(lhs->collapseSymbols().empty()))
	    s->offerRule(rl);
	}
    }
  //
  //	Now make sure anything we indexed under late symbol is compiled
  //
  s->compileSortConstraints();
  s->compileEquations();
  s->compileRules();
}

void
Module::clearMemo()
{
  delete memoMap;
  memoMap = 0;
}

MemoMap*
Module::getMemoMap()
{
  if (memoMap == 0)
    memoMap = new MemoMap;
  return memoMap;
}

void
Module::reset()
{
  for (Symbol* s : symbols)
    s->reset();
}

void
Module::resetRules()
{
  for (Symbol* s : symbols)
    s->resetRules();
}

#ifdef DUMP
void
Module::dump(ostream& s)
{
  s << "Module name: " << this << '\n';
  int nrComp = connectedComponents.length();
  for (int i = 0; i < nrComp; i++)
    {
      s << "Connected Component " << i << '\n';
      connectedComponents[i]->dump(s);
      s << '\n';
    }
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; i++)
    {
      s << "Symbol " << i << "\n";
      symbols[i]->dump(s);
      s << '\n';
    }
}
#endif
