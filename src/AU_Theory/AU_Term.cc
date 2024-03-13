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
//	Implementation for class AU_Term.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "AU_Persistent.hh"
#include "AU_Theory.hh"

//      core class definitions
#include "variableTerm.hh"
#include "rewritingContext.hh"
#include "equation.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//	variable class definitions
#include "variableTerm.hh"

//	AU persistent class definitions
#include "AU_DequeIter.hh"

//	AU theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_DequeDagNode.hh"
#include "AU_Term.hh"
#include "AU_ArgumentIterator.hh"
#include "AU_LhsAutomaton.hh"
#include "AU_RhsAutomaton.hh"

//	our stuff
#include "AU_LhsCompiler.cc"

AU_Term::AU_Term(AU_Symbol* symbol, const Vector<Term*>& arguments)
: Term(symbol), argArray(arguments.length())
{
  int nrArgs = arguments.length();
  Assert(nrArgs >= 2, "insufficient arguments");
  for (int i = 0; i < nrArgs; i++)
    argArray[i].term = arguments[i];
}

AU_Term::AU_Term(const AU_Term& original, AU_Symbol* symbol, SymbolMap* translator)
  : Term(symbol),
    argArray(original.argArray.length())
{
  int nrArgs = original.argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i].term = original.argArray[i].term->deepCopy(translator);
}

RawArgumentIterator*
AU_Term::arguments() const
{
  return new AU_ArgumentIterator(argArray);
}

void
AU_Term::deepSelfDestruct()
{
  for (const Tuple& t : argArray)
    t.term->deepSelfDestruct();
  delete this;
}

Term*
AU_Term::deepCopy2(SymbolMap* translator) const
{
  AU_Symbol* s = symbol();
  if (translator != 0)
    {
      Symbol* s2 = translator->translate(s);
      if (s2 == 0)
	{
	  int nrArgs = argArray.length();
	  if (nrArgs == 2)
	    return translator->translateTerm(this);
	  //
	  //	Tricky situtation - we have to use translateTerm() since
	  //	we are translating to a term but we have more than 2
	  //	2 arguments. We resolve it by creating a temporary
	  //	expanded term.
	  //
	  Vector<Term*> args(2);
	  args[0] = argArray[0].term;
	  for (int i = 1; i < nrArgs; ++i)
	    {
	      args[1] = argArray[i].term;
	      args[0] = new AU_Term(s, args);
	    }
	  Term* t = args[0];
	  Term* r = translator->translateTerm(t);
	  for (int i = 1; i < nrArgs; ++i)
	    {
	      Term* n = safeCast(AU_Term*, t)->argArray[0].term;
	      delete t;
	      t = n;
	    }
	  return r;
	}
      s = dynamic_cast<AU_Symbol*>(s2);
      if (s == 0)
	{
	  //
	  //	Another tricky situation - we are translating to a non-AU_Symbol.
	  //
	  Vector<Term*> args(2);
	  args[0] = argArray[0].term->deepCopy(translator);
	  int nrArgs = argArray.length();
	  for (int i = 1; i < nrArgs; ++i)
	    {
	      args[1] = argArray[i].term->deepCopy(translator);
	      args[0] = s2->makeTerm(args);
	    }
	  return args[0];
	}
    }
  return new AU_Term(*this, s, translator);
}

Term*
AU_Term::normalize(bool full, bool& changed)
{
  changed = false;
  AU_Symbol* s = symbol();
  int nrArgs = argArray.length();
  //
  //	Pass 1: normalize arguments and calculate number of extra arguments that
  //	will result from flattening.
  //
  int expansion = 0;
  for (int i = 0; i < nrArgs; i++)
    {
      bool subtermChanged;
      Term* t = argArray[i].term->normalize(full, subtermChanged);
      if (subtermChanged)
	changed = true;
      argArray[i].term = t;
      if (full && t->symbol() == s)
	expansion += safeCast(AU_Term*, t)->argArray.length() - 1;
    }
  //
  //	Pass 2: flatten at the top.
  //
  if (expansion > 0)
    {
      changed = true;
      argArray.expandBy(expansion);
      int p = nrArgs + expansion - 1;
      for (int i = nrArgs - 1; i >= 0; i--)
        {
          Assert(p >= i, "loop invariant broken");
          Term* t = argArray[i].term;
	  if (t->symbol() == s)
            {
	      Vector<Tuple>& argArray2 = safeCast(AU_Term*, t)->argArray;
	      for (int j = argArray2.length() - 1; j >= 0; j--)
		argArray[p--].term = argArray2[j].term;
              delete t;
	    }
	  else
	    argArray[p--].term = t;
	}
      nrArgs += expansion;
    }
  //
  //	Pass 3: remove identity elements.
  //
  Term* identity = s->getIdentity();
  if (identity != 0)
    {
      Term* savedId = 0;
      int p = 0;
      for (int i = 0; i < nrArgs; i++)
	{
	  Term* t = argArray[i].term;
	  if (identity->equal(t) && idPossible(i))
	    {
	      if (savedId == 0)
		savedId = t;
	      else
		t->deepSelfDestruct();
	      changed = true;
	      continue;
	    }
	  argArray[p].term = t;
	  ++p;
	}
      if (p == 0)
	{
	  //
	  //	All arguments were identity elements so we collapse to the saved one.
	  //
	  delete this;
	  return savedId;
	}
      if (savedId != 0)
	savedId->deepSelfDestruct();  // don't need it
      if (p == 1)
	{
	  //
	  //	Only one non-identity argument left so collapse to it.
	  //
	  Term* t = argArray[0].term;
	  delete this;
	  return t;
	}
      argArray.contractTo(p);
      nrArgs = p;
    }
  //
  //	Pass 4: compute hash value.
  //
  unsigned int hashValue = s->getHashValue();
  for (const Tuple& t : argArray)
    hashValue = hash(hashValue, t.term->getHashValue());
  setHashValue(hashValue);
  return this;
}

int
AU_Term::compareArguments(const Term* other) const
{
  const Vector<Tuple>& argArray2 = safeCast(const AU_Term*, other)->argArray;
  int r = argArray.length() - argArray2.length();
  if (r != 0)
    return r;
  Vector<Tuple>::const_iterator j = argArray2.begin();
  Vector<Tuple>::const_iterator i = argArray.begin();
  const Vector<Tuple>::const_iterator e = argArray.end();
  do
    {
      r = i->term->compare(j->term);
      if (r != 0)
	return r;
      ++j;
      ++i;
    }
  while (i != e);
  Assert(j == argArray2.end(), "iterator problem");
  return 0;
}

int
AU_Term::compareArguments(const DagNode* other) const
{
  int len = argArray.length();
  if (safeCast(const AU_BaseDagNode*, other)->isDeque())
    {
      const AU_DequeDagNode* d2 = safeCast(const AU_DequeDagNode*, other);
      int r = len - d2->nrArgs();
      if (r != 0)
	return r;
      AU_DequeIter j(d2->getDeque());
      Vector<Tuple>::const_iterator i = argArray.begin();
      const Vector<Tuple>::const_iterator e = argArray.end();
      do
	{
	  r = i->term->compare(j.getDagNode());
	  if (r != 0)
	    return r;
	  j.next();
	  ++i;
	}
      while (i != e);
      Assert(!j.valid(), "iterator problem");
    }
  else
    {
      const ArgVec<DagNode*>& argArray2 = safeCast(const AU_DagNode*, other)->argArray;
      int r = len - argArray2.length();
      if (r != 0)
	return r;
      ArgVec<DagNode*>::const_iterator j = argArray2.begin();
      Vector<Tuple>::const_iterator i = argArray.begin();
      const Vector<Tuple>::const_iterator e = argArray.end();
      do
	{
	  r = i->term->compare(*j);
	  if (r != 0)
	    return r;
	  ++j;
	  ++i;
	}
      while (i != e);
      Assert(j == argArray2.end(), "iterator problem");
    }
  return 0;
}

void
AU_Term::findEagerVariables(bool atTop, NatSet& eagerVariables) const
{
  BinarySymbol::PermuteStrategy strat = symbol()->getPermuteStrategy();
  if (strat == BinarySymbol::EAGER ||
      (strat == BinarySymbol::SEMI_EAGER && !atTop))
    {
      for (const Tuple& t : argArray)
	t.term->findEagerVariables(false, eagerVariables);
    }
}

void
AU_Term::markEagerArguments(int nrVariables,
			   const NatSet& eagerVariables,
			   Vector<int>& problemVariables)
{
  if (symbol()->getPermuteStrategy() == BinarySymbol::EAGER)
    {
      for (const Tuple& t : argArray)
	t.term->markEager(nrVariables, eagerVariables, problemVariables);
    }
}

DagNode*
AU_Term::dagify2()
{
  int nrArgs = argArray.length();
  AU_DagNode* d = new AU_DagNode(symbol(), nrArgs);
  ArgVec<DagNode*>& p = d->argArray;
  for (int i = 0; i < nrArgs; i++)    
    p[i] = argArray[i].term->dagify();
  return d;
}

void
AU_Term::analyseCollapses2()
{
  //
  //	(1) Analyse our subterms.
  //
  for (const Tuple& t : argArray)
    t.term->analyseCollapses();
  //
  //	(2) Does our top symbol have an identity?
  //
  uniqueCollapseSubtermIndex = NONE;
  AU_Symbol* s = symbol();
  if (s->getIdentity() == 0)
    return;  // if no identity element then we can't collapse
  //
  //	(3) Can we collapse?
  //
  int nrArgs = argArray.length();
  int firstNonIdArg = NONE;
  for (int i = 0; i < nrArgs; i++)    
    {
      Term* t = argArray[i].term;
      if (idPossible(i) && s->mightMatchOurIdentity(t))
	continue;
      if (firstNonIdArg != NONE)
	return;  // can't collapse
      firstNonIdArg = i;
    }
  //
  //	(4) If so, what to?
  //
  if (firstNonIdArg != NONE)
    {
      //
      //        Can only collapse to firstNonIdArg.
      //
      uniqueCollapseSubtermIndex = firstNonIdArg;
      addCollapseSymbol(argArray[firstNonIdArg].term->symbol());
      addCollapseSymbols(argArray[firstNonIdArg].term->collapseSymbols());
    }
  else
    {
      //
      //        Can collapse to any of our arguments.
      //
      for (const Tuple& i : argArray)
        {
	  Term* t = i.term;
	  addCollapseSymbol(t->symbol());
	  addCollapseSymbols(t->collapseSymbols());
	}
    }
}

void
AU_Term::insertAbstractionVariables(VariableInfo& variableInfo)
{
  AU_Symbol* s = symbol();
  bool honorsGroundOutMatch = true;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)    
    {
      Tuple& t = argArray[i];
      t.term->insertAbstractionVariables(variableInfo);
      if (!(t.term->honorsGroundOutMatch()))
        honorsGroundOutMatch = false;
      t.abstractionVariableIndex = NONE;
      t.collapseToOurSymbol = false;
      t.matchOurIdentity = false;
      if (dynamic_cast<VariableTerm*>(t.term) == 0)
        {
	  //
	  //	We don't bother checking wether we have a left identity
	  //	or a right identity because we may end up matching with
	  //	extension.
	  //
          t.matchOurIdentity = s->mightMatchOurIdentity(t.term);
          t.collapseToOurSymbol = s->mightCollapseToOurSymbol(t.term);
          if (t.matchOurIdentity || t.collapseToOurSymbol)
            {
              t.abstractionVariableIndex = variableInfo.makeProtectedVariable();
              honorsGroundOutMatch = false;
              DebugAdvisory("Introduced abstraction variable for " <<
			    t.term << " in " << this << '.');
            }
        }
    }
  setHonorsGroundOutMatch(honorsGroundOutMatch);
}

void
AU_Term::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (ground())
    return;
  if (!atTop)
    availableTerms.insertMatchedTerm(this, eagerContext);
  BinarySymbol::PermuteStrategy strat = symbol()->getPermuteStrategy();
  bool argEager = eagerContext && (strat == BinarySymbol::EAGER ||
				   (strat == BinarySymbol::SEMI_EAGER && !atTop));
  for (const Tuple& t : argArray)
    t.term->findAvailableTerms(availableTerms, argEager);
}

int
AU_Term::compileRhs2(RhsBuilder& rhsBuilder,
		     VariableInfo& variableInfo,
		     TermBag& availableTerms,
		     bool eagerContext)
{
  int nrArgs = argArray.length();
  //
  //	We want to minimize conflict between slots to avoid quadratic number of
  //	conflict arcs on giant right hand sides. The heuristic we use is crude:
  //	we sort in order of arguments by number of symbol occurences, and build
  //	largest first.
  //
  typedef pair<int, int> IntPair;
  typedef Vector<IntPair > PairVec;
  PairVec order(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      order[i].first = - argArray[i].term->computeSize();  // larger terms to the front
      order[i].second = i;
    }
  sort(order.begin(), order.end());
  //
  //	Compile each argument in largest first order.
  //
  bool argEager = eagerContext && symbol()->getPermuteStrategy() == BinarySymbol::EAGER;
  Vector<int> sources(nrArgs);
  for (const IntPair& p : order)
    {
      int j = p.second;
      sources[j] = argArray[j].term->compileRhs(rhsBuilder,
						variableInfo,
						availableTerms,
						argEager);
    }
  //
  //	Now add sources to automaton in original order, and flag last use
  //	of each source for conflict arc generation.
  //
  AU_RhsAutomaton* automaton = new AU_RhsAutomaton(symbol(), nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      int index = sources[i];
      automaton->addArgument(index);
      variableInfo.useIndex(index);
    }
  //
  //	Complete the automaton and add it  to the rhs builder.
  //
  int destination = variableInfo.makeConstructionIndex();
  automaton->close(destination);
  rhsBuilder.addRhsAutomaton(automaton);
  return destination;
}

#ifdef DUMP

void
AU_Term::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{AU_Term}\n";
  ++indentLevel;
  dumpCommon(s, variableInfo, indentLevel);
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  for (const Tuple& p : argArray)
    {
      s << Indent(indentLevel) << "collapseToOurSymbol = " <<
	bool(p.collapseToOurSymbol) <<
        "\tmatchOurIdentity = " << bool(p.matchOurIdentity);
      if (p.abstractionVariableIndex != NONE)
        s << "\tabstractionVariableIndex = " << p.abstractionVariableIndex;
      s << '\n';
      p.term->dump(s, variableInfo, indentLevel);
    }
  s << Indent(indentLevel - 2) << "End{AU_Term}\n";
}

#endif
