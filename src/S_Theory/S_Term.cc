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
//	Implementation for class S_Term.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "S_Theory.hh"

//      core class definitions
#include "variableTerm.hh"
#include "rewritingContext.hh"
#include "equation.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//	variable class definitions
#include "variableTerm.hh"

//	S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_Term.hh"
#include "S_ArgumentIterator.hh"
#include "S_LhsAutomaton.hh"
#include "S_RhsAutomaton.hh"

S_Term::S_Term(S_Symbol* symbol, const mpz_class& number, Term* arg)
  : Term(symbol),
    number(number),
    arg(arg)
{
  Assert(arg != 0, "null arg");
}

S_Term::S_Term(const S_Term& original, SymbolMap* map)
  : Term(map == 0 ? original.symbol() : map->translate(original.symbol())),
    number(original.number)
{
  arg = original.arg->deepCopy(map);
}

RawArgumentIterator*
S_Term::arguments()
{
  return new S_ArgumentIterator(arg);
}

void 
S_Term::deepSelfDestruct()
{
  arg->deepSelfDestruct();
  delete this;
}

Term*
S_Term::deepCopy2(SymbolMap* map) const
{
  return new S_Term(*this, map);
}

Term*
S_Term::normalize(bool full, bool& changed)
{
  Assert(arg != 0, "null arg");
  arg = arg->normalize(full, changed);
  Assert(arg != 0, "null arg after normalize");
  if (arg->symbol() == symbol())
    {
      S_Term* a = safeCast(S_Term*, arg);
      number += a->number;
      arg = a->arg;
      delete a;
    }
  setHashValue(hash(symbol()->getHashValue(),
		    arg->getHashValue(),
		    mpz_tdiv_ui(number.get_mpz_t(), INT_MAX)));
  return this;
}

int
S_Term::compareArguments(const Term* other) const
{
  const S_Term* t = safeCast(const S_Term*, other);
  int r = cmp(number, t->number);
  return (r == 0) ? arg->compare(t->arg) : r;
}

int
S_Term::compareArguments(const DagNode* other) const
{
  const S_DagNode* d = safeCast(const S_DagNode*, other);
  int r = cmp(number, d->getNumber());
  return (r == 0) ? arg->compare(d->getArgument()) : r;
}

void
S_Term::findEagerVariables(bool atTop, NatSet& eagerVariables) const
{
  S_Symbol* s = symbol();
  if (atTop ? s->eagerArgument(0) : s->evaluatedArgument(0))
    arg->findEagerVariables(false, eagerVariables);

}

void
S_Term::markEagerArguments(int nrVariables,
			   const NatSet& eagerVariables,
			   Vector<int>& problemVariables)
{
  S_Symbol* s = symbol();
  if (s->eagerArgument(0))
    arg->markEager(nrVariables, eagerVariables, problemVariables);
}

DagNode*
S_Term::dagify2()
{
  return new S_DagNode(symbol(), number, arg->dagify());
}

void
S_Term::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  arg->analyseConstraintPropagation(boundUniquely);
}

void
S_Term::insertAbstractionVariables(VariableInfo& variableInfo)
{
  arg->insertAbstractionVariables(variableInfo);
  bool honorsGroundOutMatch = arg->honorsGroundOutMatch();
  if (dynamic_cast<VariableTerm*>(arg) == 0 && symbol()->mightCollapseToOurSymbol(arg))
    {
      abstractionVariableIndex = variableInfo.makeProtectedVariable();
      honorsGroundOutMatch = false;
      DebugAdvisory("Introduced abstraction variable for " <<
		    arg << " in " << this << '.');
    }
  else
    abstractionVariableIndex = NONE;
  setHonorsGroundOutMatch(honorsGroundOutMatch);
}

LhsAutomaton*
S_Term::compileLhs2(bool matchAtTop,
		    const VariableInfo& variableInfo,
		    NatSet& boundUniquely,
		    bool& subproblemLikely)
{
  S_LhsAutomaton* a = new S_LhsAutomaton(symbol(), number, matchAtTop);
  subproblemLikely = a->addSubpattern(arg,
				      abstractionVariableIndex,
				      variableInfo,
				      boundUniquely);
  return a;
}

void
S_Term::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (ground())
    return;
  S_Symbol* s = symbol();
  if (!atTop)
    availableTerms.insertMatchedTerm(this, eagerContext);
  arg->findAvailableTerms(availableTerms,
			  eagerContext && (atTop ? s->eagerArgument(0) :
					   s->evaluatedArgument(0)));
}

int
S_Term::compileRhs2(RhsBuilder& rhsBuilder,
		    VariableInfo& variableInfo,
		    TermBag& availableTerms,
		    bool eagerContext)
{
  S_Symbol* s = symbol();
  int source = arg->compileRhs(rhsBuilder,
			       variableInfo,
			       availableTerms,
			       eagerContext && s->eagerArgument(0));
  //
  //	Need to flag last use  source.
  //
  variableInfo.useIndex(source);

  int destination = variableInfo.makeConstructionIndex();
  S_RhsAutomaton* automaton = new S_RhsAutomaton(s, number, source, destination);
  rhsBuilder.addRhsAutomaton(automaton);
  return destination;
}

#ifdef DUMP
void
S_Term::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{S_Term}\n";
  ++indentLevel;
  dumpCommon(s, variableInfo, indentLevel);
  s << Indent(indentLevel) << "number = " << number << '\n';
  s << Indent(indentLevel) << "argument:\n";
  arg->dump(s, variableInfo, indentLevel + 1);
  s << Indent(indentLevel - 1) << "End{S_Term}\n";
}
#endif
