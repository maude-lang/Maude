/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class VariableTerm.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//	core class definitions
#include "variableInfo.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "substitution.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "variableDagNode.hh"
#include "variableLhsAutomaton.hh"

VariableTerm::VariableTerm(VariableSymbol* symbol, int name)
  : Term(symbol),
    NamedEntity(name)
{
  index = UNDEFINED;
}

RawArgumentIterator*
VariableTerm::arguments() const
{
  return 0;
}

void
VariableTerm::deepSelfDestruct()
{
  delete this;
}

Term*
VariableTerm::deepCopy2(SymbolMap* translator) const
{
  VariableSymbol* vs = safeCastNonNull<VariableSymbol*>((translator == 0 ? symbol() : translator->translate(symbol())));
  VariableTerm* vt = new VariableTerm(vs, id());
  //
  //	Usually we need the index to be UNDEFINED so that statement analysis works; but if the term is part
  //	of an op to term mapping we may need the index preserved.
  //
  if (translator != 0 && translator->preserveVariableIndices())
    vt->index = index;
  return vt;
}

Term*
VariableTerm::instantiate2(const Vector<Term*>& varBindings, SymbolMap* translator)
{
  return varBindings[index]->deepCopy(translator);
}

Term*
VariableTerm::normalize(bool /* full */, bool& changed)
{
  changed = false;
  setHashValue(hash(symbol()->getHashValue(), id()));
  return this;
}

int
VariableTerm::compareArguments(const Term* other) const
{
  return id() - safeCastNonNull<const VariableTerm*>(other)->id();
}

int
VariableTerm::compareArguments(const DagNode* other) const
{
  return id() - safeCastNonNull<const VariableDagNode*>(other)->id();
}

void
VariableTerm::findEagerVariables(bool /* atTop */, NatSet& eagerVariables) const
{
  eagerVariables.insert(index);
}

void
VariableTerm::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  boundUniquely.insert(index);
}

LhsAutomaton*
VariableTerm::compileLhs2(bool matchAtTop,
			  const VariableInfo& variableInfo,
			  NatSet& boundUniquely,
			  bool& subproblemLikely)
{
  boundUniquely.insert(index);
  subproblemLikely = false;
  bool copyToAvoidOverwriting = matchAtTop /* && variableInfo.rhsVariables().contains(index) */;
  return new VariableLhsAutomaton(index, getSort(), copyToAvoidOverwriting);
}

void
VariableTerm::markEagerArguments(int nrVariables,
				 const NatSet& eagerVariables,
				 Vector<int>& problemVariables)
{
}

DagNode*
VariableTerm::dagify2()
{
  return new VariableDagNode(symbol(), id(), index);
}

bool
VariableTerm::subsumes(const Term* other, bool sameVariableSet) const
{
  //
  //	If "this" and "other" are using the same set of variables
  //	then a variable can subsume itself.
  //
  if (sameVariableSet &&
      symbol() == other->symbol() &&
      id() == safeCastNonNull<const VariableTerm*>(other)->id())
    return true;
  //
  //	Otherwise a variable must be linear and have large enough
  //	sort.
  //
  return !(occursInContext().contains(index)) &&
    getComponent()->leq(other->getSortIndex(), getSortIndex());
}

int
VariableTerm::partialCompareUnstable(const Substitution& partialSubstitution,
				     DagNode* other) const
{
  DagNode* d = partialSubstitution.value(index);
  if (d == 0)
    return Term::UNKNOWN;
  int r = d->compare(other);
  if (r < 0)
    return LESS;
  if (r > 0)
    return GREATER;
  return EQUAL;
}

void
VariableTerm::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (!atTop)
    availableTerms.insertMatchedTerm(this, eagerContext);
}

int 
VariableTerm::compileRhs2(RhsBuilder& /* rhsBuilder */,
			  VariableInfo& /* variableInfo */,
			  TermBag& /* availableTerms */,
			  bool /* eagerContext */)
{
  CantHappen("should never be called");
  return 0;
}
