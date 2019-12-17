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
//	Implementation for class CUI_Term.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "CUI_Theory.hh"

//      interface class definitions
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "equation.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//	variable class definitions
#include "variableTerm.hh"

//	CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_Term.hh"
#include "CUI_ArgumentIterator.hh"
#include "CUI_LhsAutomaton.hh"
#include "CUI_RhsAutomaton.hh"

#include "CUI_LhsCompiler.cc"

CUI_Term::CUI_Term(CUI_Symbol* symbol, const Vector<Term*>& arguments)
  : Term(symbol),
    argArray(2)
{
  Assert(arguments.length() == 2, "bad number of arguments for " << symbol);
  argArray[0] = arguments[0];
  argArray[1] = arguments[1];
}

CUI_Term::CUI_Term(const CUI_Term& original, CUI_Symbol* symbol, SymbolMap* translator)
  : Term(symbol),
    argArray(2)
{
  argArray[0] = original.argArray[0]->deepCopy(translator);
  argArray[1] = original.argArray[1]->deepCopy(translator);
}

RawArgumentIterator*
CUI_Term::arguments()
{
  return new CUI_ArgumentIterator(argArray);
}

void
CUI_Term::deepSelfDestruct()
{
  argArray[0]->deepSelfDestruct();
  argArray[1]->deepSelfDestruct();
  delete this;
}

Term*
CUI_Term::deepCopy2(SymbolMap* translator) const
{
  CUI_Symbol* s = symbol();
  if (translator != 0)
    {
      Symbol* s2 = translator->translate(s);
      if (s2 == 0)
	return translator->translateTerm(this);
      s = dynamic_cast<CUI_Symbol*>(s2);
      if (s == 0)
	{
	  Vector<Term*> args(2);
	  args[0] = argArray[0]->deepCopy(translator);
	  args[1] = argArray[1]->deepCopy(translator);
	  return s2->makeTerm(args);
	}
    }
  return new CUI_Term(*this, s, translator);
}

Term*
CUI_Term::collapseTo(int argNr)
{
  argArray[1 - argNr]->deepSelfDestruct();
  Term* r = argArray[argNr];
  delete this;
  return r;
}

Term*
CUI_Term::normalize(bool full, bool& changed)
{
  argArray[0] = argArray[0]->normalize(full, changed);
  bool subtermChanged;
  argArray[1] = argArray[1]->normalize(full, subtermChanged);
  if (subtermChanged)
    changed = true;

  CUI_Symbol* s = symbol();
  Term* identity = s->getIdentity();
  if (identity != 0)
    {
      if (s->leftId() && identity->equal(argArray[0]))
	{
	  changed = true;
	  return collapseTo(1);
	}
      if (s->rightId() && identity->equal(argArray[1]))
	{
	  changed = true;
	  return collapseTo(0);
	}
    }
  if (s->comm() || s->idem())
    {
      int r = argArray[0]->compare(argArray[1]);
      if (s->idem() && r == 0)
	{
	  changed = true;
	  return collapseTo(0);
	}
      if (s->comm() && r > 0)
	{
	  changed = true;
	  Term* t = argArray[0];
	  argArray[0] = argArray[1];
	  argArray[1] = t;
	}
    }
  unsigned int hashValue = symbol()->getHashValue();
  hashValue = hash(hashValue, argArray[0]->getHashValue());
  setHashValue(hash(hashValue, argArray[1]->getHashValue()));
  return this;
}

int
CUI_Term::compareArguments(const Term* other) const
{
  const Vector<Term*>& argArray2 = static_cast<const CUI_Term*>(other)->argArray;
  int r = argArray[0]->compare(argArray2[0]);
  if (r != 0)
    return r;
  return argArray[1]->compare(argArray2[1]);
}

int
CUI_Term::compareArguments(const DagNode* other) const
{
  const CUI_DagNode* d = static_cast<const CUI_DagNode*>(other);
  int r = argArray[0]->compare(d->argArray[0]);
  if (r != 0)
    return r;
  return argArray[1]->compare(d->argArray[1]);
}

void
CUI_Term::findEagerVariables(bool atTop, NatSet& eagerVariables) const
{
  CUI_Symbol* s = symbol();
  if (atTop ? s->eagerArgument(0) : s->evaluatedArgument(0))
    argArray[0]->findEagerVariables(false, eagerVariables);
  if (atTop ? s->eagerArgument(1) : s->evaluatedArgument(1))
    argArray[1]->findEagerVariables(false, eagerVariables);
}

void
CUI_Term::markEagerArguments(int nrVariables,
			     const NatSet& eagerVariables,
			     Vector<int>& problemVariables)
{
  CUI_Symbol* s = symbol();
  if (s->eagerArgument(0))
    argArray[0]->markEager(nrVariables, eagerVariables, problemVariables);
  if (s->eagerArgument(1))
    argArray[1]->markEager(nrVariables, eagerVariables, problemVariables);
}

DagNode*
CUI_Term::dagify2()
{
  CUI_DagNode* d = new CUI_DagNode(symbol());
  d->argArray[0] = argArray[0]->dagify();
  d->argArray[1] = argArray[1]->dagify();
  return d;
}

void
CUI_Term::analyseCollapses2()
{
  Term* t0 = argArray[0];
  Term* t1 = argArray[1];
  t0->analyseCollapses();
  t1->analyseCollapses();
  CUI_Symbol* topSymbol = symbol();
  id0CollapsePossible = false;
  if (topSymbol->leftId() && topSymbol->mightMatchOurIdentity(t0))
    {
      addCollapseSymbol(t1->symbol());
      addCollapseSymbols(t1->collapseSymbols());
      id0CollapsePossible = true;
    }
  id1CollapsePossible = false;
  if (topSymbol->rightId() && topSymbol->mightMatchOurIdentity(t1))
    {
      addCollapseSymbol(t0->symbol());
      addCollapseSymbols(t0->collapseSymbols());
      id1CollapsePossible = true;
    }
  idemCollapsePossible = false;
  if (topSymbol->idem() && !(t0->matchIndependent(t1)))
    {
      PointerSet symbols;
      Term::commonSymbols(argArray, symbols);
      if (!(symbols.empty()))
	{
	  addCollapseSymbols(symbols);
	  idemCollapsePossible = true;
	}
    }
}

#ifdef DUMP
void
CUI_Term::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{CUI_Term}\n";
  ++indentLevel;
  dumpCommon(s, variableInfo, indentLevel);
  s << Indent(indentLevel) << "id0CollapsePossible = " << id0CollapsePossible <<
    "\tid1CollapsePossible = " << id1CollapsePossible <<
    "\tidemCollapsePossible = " << idemCollapsePossible << '\n';
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  argArray[0]->dump(s, variableInfo, indentLevel);
  argArray[1]->dump(s, variableInfo, indentLevel);
  s << Indent(indentLevel - 2) << "End{CUI_Term}\n";
}
#endif

void
CUI_Term::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (ground())
    return;
  CUI_Symbol* s = symbol();
  if (!atTop)
    availableTerms.insertMatchedTerm(this, eagerContext);
  argArray[0]->findAvailableTerms(availableTerms,
				  eagerContext && (atTop ? s->eagerArgument(0) :
						   s->evaluatedArgument(0)));
  argArray[1]->findAvailableTerms(availableTerms,
				  eagerContext && (atTop ? s->eagerArgument(1) :
						   s->evaluatedArgument(1)));
}

int
CUI_Term::compileRhs2(RhsBuilder& rhsBuilder,
		      VariableInfo& variableInfo,
		      TermBag& availableTerms,
		      bool eagerContext)
{
  CUI_Symbol* s = symbol();
  CUI_RhsAutomaton* automaton = new CUI_RhsAutomaton(s);
  int i0 = argArray[0]->compileRhs(rhsBuilder, variableInfo, availableTerms,
				   eagerContext && s->eagerArgument(0));
  int i1 = argArray[1]->compileRhs(rhsBuilder, variableInfo, availableTerms,
				   eagerContext && s->eagerArgument(1));
  //
  //	Need to flag last use of each source.
  //
  variableInfo.useIndex(i0);
  variableInfo.useIndex(i1);

  int index = variableInfo.makeConstructionIndex();
  automaton->close(i0, i1, index);
  rhsBuilder.addRhsAutomaton(automaton);
  return index;
}
