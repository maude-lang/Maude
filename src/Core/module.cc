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
#include "module.hh"

//	variable class definitions
#include "variableTerm.hh"

Module::Module(int id)
  : NamedEntity(id)
{
  status = OPEN;
}

void
Module::insertSortConstraint(SortConstraint* sortConstraint)
{
  Assert(status < THEORY_CLOSED, "bad status");
  sortConstraint->setModuleInfo(this, sortConstraints.length());
  sortConstraints.append(sortConstraint);
  sortConstraint->check();
}

void
Module::insertEquation(Equation* equation)
{
  Assert(status < THEORY_CLOSED, "bad status");
  equation->setModuleInfo(this, equations.length());
  equations.append(equation);
  equation->check();
}

void
Module::insertRule(Rule* rule)
{
  Assert(status < THEORY_CLOSED, "bad status");
  rule->setModuleInfo(this, rules.length());
  rules.append(rule);
  rule->check();
}

Module::~Module()
{
  //
  //	This is delicate; first we lose any root pointers to
  //	dag nodes that might contain pointers to our symbols.
  //
  clearMemo();
  reset();
  //
  //	Then we delete everything except for the symbols.
  //
  int nrSorts = sorts.length();
  for (int i = 0; i < nrSorts; i++)
    delete sorts[i];
  int nrComponents = connectedComponents.length();
  for (int i = 0; i < nrComponents; i++)
    delete connectedComponents[i];
  int nrSortConstraints = sortConstraints.length();
  for (int i = 0; i < nrSortConstraints; i++)
    delete sortConstraints[i];
  int nrEquations = equations.length();
  for (int i = 0; i < nrEquations; i++)
    delete equations[i];
  int nrRules = rules.length();
  for (int i = 0; i < nrRules; i++)
    delete rules[i];
  //
  //	And finally it is safe to delete our symbols.
  //
  int nrSymbols = symbols.length();
#if 1
  for (int i = 0; i < nrSymbols; i++)
    delete symbols[i];
#else
  for (int i = 0; i < nrSymbols; i++)
    {
      void* p = symbols[i];
      cerr << "deleting " << symbols[i] << " at "<< p << endl;
      memset(p, -1, sizeof(Symbol));
    }
#endif
}

void
Module::closeSortSet()
{
  Assert(status == OPEN, "bad status");
  int nrSorts = sorts.length();
  for (int i = 0; i < nrSorts; i++)
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
  int nrSymbols = symbols.length();
  //
  //	First deal with any inter-symbol dependences until
  //	we hit stability.
  //	This is currently used to normalize identities.
  //
  bool somethingChanged;
  do
    {
      somethingChanged = false;
      for (int i = 0; i < nrSymbols; i++)
	{
	  if (symbols[i]->interSymbolPass())
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
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->postInterSymbolPass();
}

void
Module::closeTheory()
{
  //
  //	This is where most of the work for analyzing and (semi-)compiling
  //	a module is done.
  //
  Assert(status == FIX_UPS_CLOSED, "bad status");
  status = THEORY_CLOSED;
  int nrSymbols = symbols.length();
  //
  //	First compile the sort declarations for each operator.
  //	We could have done this much earlier but since it is often
  //	the most time consuming part of compilation and produces
  //	voluminous sort tables we postpone it we are actually going to
  //	do some useful work in the module.
  //
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->compileOpDeclarations();
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
	for (int i = 0; i < nrSymbols; i++)
	  {
	    Symbol* s = symbols[i];
	    ConnectedComponent* c = s->rangeComponent();
	    if (c->errorFree() && s->canProduceErrorSort())
	      {
		c->errorSortSeen();
		changed = true;
		//	      cout << c->sort(0) << " corrupted by " << s << '\n';
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
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->postOpDeclarationPass();
  //
  //	Deal with any sort constraints.
  //	We need to have computed to sort of each identity at this point
  //	so that we can analyseCollapses() on sort contraint patterns.
  //
  indexSortConstraints();
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->orderSortConstraints();
  //
  //	Certain sort properties relative to a symbol can only be determined after 
  //	the existance of sort constraints applicable to that symbol are known.
  //
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->finalizeSortInfo();
  //
  //	Now index equations and rules and compile statements.
  //
  indexEquations();
  indexRules();
  for (int i = 0; i < nrSymbols; i++)
    {
      Symbol* s = symbols[i];
      s->compileSortConstraints();
      s->compileEquations();
      s->compileRules();
      // s->finalizeSymbol();
    }
}

void
Module::indexSortConstraints()
{
  int nrSortConstraints = sortConstraints.length();
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSortConstraints; i++)
    {
      SortConstraint* sc = sortConstraints[i];
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
		  for(int j = 0; j < nrSymbols; j++)
		    symbols[j]->offerSortConstraint(sc);
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
      bool noCollapse = lhs->collapseSymbols().empty();
      AdvisoryCheck(noCollapse,
		    *lhs << ": collapse at top of " << QUOTE(lhs) <<
		    " may cause it to match more than you expect.");
      if (noCollapse && dynamic_cast<VariableTerm*>(lhs) == 0)
	lhs->symbol()->offerEquation(eq);
      else
	{
	  int nrSymbols = symbols.length();
	  for(int i = 0; i < nrSymbols; i++)
	    symbols[i]->offerEquation(eq);
	}
    }
}

void
Module::indexEquations()
{
  int nrEquations = equations.length();
  //
  //	First index non-owise equations.
  //
  for (int i = 0; i < nrEquations; i++)
    {
      Equation* eq = equations[i];
      if (!(eq->isBad()) && !(eq->isOwise()))
	indexEquation(eq);
    }
  //
  //	Then index owise equations.
  //
  for (int i = 0; i < nrEquations; i++)
    {
      Equation* eq = equations[i];
      if (!(eq->isBad()) && eq->isOwise())
	indexEquation(eq);
    }
}

void
Module::indexRules()
{
  int nrRules = rules.length();
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrRules; i++)
    {
      Rule* rl = rules[i];
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
	      for(int j = 0; j < nrSymbols; j++)
		symbols[j]->offerRule(rl);
	    }
	}
    }
}

void
Module::insertLateSymbol(Symbol*s)
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
  int nrSortConstraints = sortConstraints.length();
  for (int i = 0; i < nrSortConstraints; i++)
    {
      SortConstraint* sc = sortConstraints[i];
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
  int nrEquations = equations.length();
  for (int i = 0; i < nrEquations; i++)
    {
      Equation* eq = equations[i];
      if (!(eq->isBad() || eq->isNonexec()) && !(eq->isOwise()))
	{
	  Term* lhs = eq->getLhs();
	  if (dynamic_cast<VariableTerm*>(lhs) != 0 || !(lhs->collapseSymbols().empty()))
	    s->offerEquation(eq);
	}
    } 
  for (int i = 0; i < nrEquations; i++)
    {
      Equation* eq = equations[i];
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
  int nrRules = rules.length();
  for (int i = 0; i < nrRules; i++)
    {
      Rule* rl = rules[i];
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
  // s->finalizeSymbol();
}

void
Module::clearMemo()
{
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->clearMemo();
}

void
Module::reset()
{
  int nrSymbols = symbols.length();
  for (int i = 0; i < nrSymbols; i++)
    symbols[i]->reset();
}

void
Module::resetRules()
{
  FOR_EACH_CONST(i, Vector<Symbol*>, symbols)
    (*i)->resetRules();
}

void
Module::saveHiddenState()
{
  FOR_EACH_CONST(i, Vector<Symbol*>, symbols)
    (*i)->saveHiddenState();
}

void
Module::restoreHiddenState()
{
  FOR_EACH_CONST(i, Vector<Symbol*>, symbols)
    (*i)->restoreHiddenState();
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
