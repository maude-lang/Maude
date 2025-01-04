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
//	Implementation for class FreeTerm.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "rawArgumentIterator.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//	core class definitions
#include "variableTerm.hh"
#include "rewritingContext.hh"
#include "equation.hh"
#include "symbolMap.hh"
#include "termBag.hh"
#include "rhsBuilder.hh"

//	free theory class definitions
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeOccurrence.hh"
#include "freeDagNode.hh"
#include "freeArgumentIterator.hh"
#include "freeLhsAutomaton.hh"
#include "freeRhsAutomaton.hh"
#include "freeOneInstructionRhsAutomaton.hh"
#include "freeTwoInstructionRhsAutomaton.hh"
#include "freeFast2RhsAutomaton.hh"
#include "freeFast3RhsAutomaton.hh"
#include "freeRemainder.hh"
#include "freeTerm.hh"

#include "freeLhsCompiler.cc"

//#ifdef COMPILER
#include "freeTermFullCompiler.cc"
//#endif

FreeTerm::FreeTerm(FreeSymbol* symbol, const Vector<Term*>& arguments)
  : Term(symbol), argArray(arguments.length())
{
  int nrArgs = arguments.length();
  Assert(symbol->arity() == nrArgs,
	 "number of arguments does not match symbol definition for " << symbol);
  for (int i = 0; i < nrArgs; i++)
    argArray[i] = arguments[i];
  slotIndex = -1;
  visitedFlag = false;
}

FreeTerm::FreeTerm(const FreeTerm& original, FreeSymbol* symbol, SymbolMap* translator)
  : Term(symbol),
    argArray(original.argArray.length())
{
  int nrArgs = original.argArray.length();
  for (int i = 0; i < nrArgs; ++i)
    argArray[i] = original.argArray[i]->deepCopy(translator);
  slotIndex = -1;
  visitedFlag = false;
}

FreeTerm::~FreeTerm()
{
}

RawArgumentIterator*
FreeTerm::arguments() const
{
  if (argArray.empty())
    return 0;
  else
    return new FreeArgumentIterator(&argArray);
}

void
FreeTerm::deepSelfDestruct()
{
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    argArray[i]->deepSelfDestruct();
  delete this;
}

Term*
FreeTerm::deepCopy2(SymbolMap* translator) const
{
  FreeSymbol* s = symbol();
  if (translator != 0)
    {
      Symbol* s2 = translator->translate(s);
      if (s2 == 0)
	return translator->translateTerm(this);
      s = dynamic_cast<FreeSymbol*>(s2);
      if (s == 0)
	{
	  int nrArgs = argArray.length();
	  Vector<Term*> args(nrArgs);
	  for (int i = 0; i < nrArgs; ++i)
	    args[i] = argArray[i]->deepCopy(translator);
	  return s2->makeTerm(args);
	}
    }
  return new FreeTerm(*this, s, translator);
}

Term*
FreeTerm::normalize(bool full, bool& changed)
{
  changed = false;
  unsigned int hashValue = symbol()->getHashValue();
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      bool subtermChanged;
      Term* t = argArray[i]->normalize(full, subtermChanged);
      argArray[i] = t;
      hashValue = hash(hashValue, t->getHashValue());
      if (subtermChanged)
	changed = true;
    }
  setHashValue(hashValue);
  return this;
}

int
FreeTerm::compareArguments(const Term* other) const
{
  Assert(symbol() == other->symbol(), "symbols differ");
  int nrArgs = argArray.length();
  const Vector<Term*>& ta = safeCastNonNull<const FreeTerm*>(other)->argArray;
  for (int i = 0; i < nrArgs; ++i)
    {
      int r = argArray[i]->compare(ta[i]);
      if (r != 0)
	return r;
    }
  return 0;
}

int
FreeTerm::compareArguments(const DagNode* other) const
{
  Assert(symbol() == other->symbol(), "symbols differ");
  int nrArgs = other->symbol()->arity();
  if (nrArgs != 0)
    {
      DagNode* const* q = static_cast<const FreeDagNode*>(other)->argArray();
      Vector<Term*>::const_iterator p = argArray.begin();
      for (;;)
	{
	  int r = (*p)->compare(*q);
	  if (r != 0)
	    return r;
	  if (--nrArgs == 0)
	    break;
	  ++p;
	  ++q;
	}
    }
  return 0;
}

int
FreeTerm::partialCompareArguments(const Substitution& partialSubstitution, DagNode* other) const
{
  Assert(symbol() == other->symbol(), "symbols differ");
  int nrArgs = argArray.size();
  if (nrArgs != 0)
    {
      DagNode* const* da = safeCast(FreeDagNode*, other)->argArray();
      for (Term* t : argArray)
	{
	  int r = t->partialCompare(partialSubstitution, *da);
	  if (r != EQUAL)
	    return r;
	  ++da;
	}
    }
  return EQUAL;
}

void
FreeTerm::findEagerVariables(bool atTop, NatSet& eagerVariables) const
{
  int nrArgs = argArray.length();
  FreeSymbol* sym = symbol();
  for (int i = 0; i < nrArgs; i++)
    {
      if (atTop ? sym->eagerArgument(i) : sym->evaluatedArgument(i))
	argArray[i]->findEagerVariables(false, eagerVariables);
    }
}

void
FreeTerm::markEagerArguments(int nrVariables,
			     const NatSet& eagerVariables,
			     Vector<int>& problemVariables)
{
  int nrArgs = argArray.length();
  FreeSymbol* sym = symbol();
  for (int i = 0; i < nrArgs; i++)
    {
      if (sym->eagerArgument(i))
	argArray[i]->markEager(nrVariables, eagerVariables, problemVariables);
    }
}

void
FreeTerm::computeMatchIndices() const
{
  //
  //	Make sure each stable symbol at the top of one of our arguments has
  //	a non-zero match index. This is done recursively.
  //
  for (Term* t : argArray)
    {
      Symbol* s = t->symbol();
      if (s->isStable() && s->getMatchIndex() == 0)
	s->setMatchIndex(s->rangeComponent()->getNewMatchIndex());
      t->computeMatchIndices();
    }
}

Term*
FreeTerm::locateSubterm(const Vector<int>& position, int backup)
{
  Term* t = this;
  int nrSteps = position.length() - backup;
  for (int i = 0; i < nrSteps; i++)
    {
      if (typeid(*t) != typeid(FreeTerm))
	return 0;
      int p = position[i];
      if (p >= t->symbol()->arity())
	return 0;
      t = (static_cast<FreeTerm*>(t)->argArray)[p];
    }
  return t;
}

Term*
FreeTerm::locateSubterm2(Vector<int>& position)
{
  Term* t = this;
  int nrSteps = position.length();
  for (int i = 0; i < nrSteps; i++)
    {
      FreeTerm* f = dynamic_cast<FreeTerm*>(t);
      if (f == 0)
	{
	  position.contractTo(i);
	  return t;
	}
      int p = position[i];
      if (p >= f->symbol()->arity())
	{
	  position.contractTo(i);
	  return t;
	}
      t = (f->argArray)[p];
    }
  return t;
}

void
FreeTerm::findActiveSlots(NatSet& slots)
{
  //
  //	A free term's slot is active if it has been visited and it has
  //	at least one subterm which has not been visited (or is alien) or
  //	which has a save index for left -> right sharing.
  //
  bool active = false;
  for (Term* t : argArray)
    {
      if (typeid(*t) == typeid(FreeTerm))
	{
	  FreeTerm* f = static_cast<FreeTerm*>(t);
	  if (f->visitedFlag)
	    {
	      f->findActiveSlots(slots);
	      if (f->getSaveIndex() == NONE)
		continue;
	    }
	}
      //
      //	We have a subterm which is not free or not visited or contains a save index.
      //
      active = true;
    }
  if (active)
    {
      Assert(slotIndex != NONE, "no slot index for active FreeTerm " << this);
      slots.insert(slotIndex);
    }
}

DagNode*
FreeTerm::dagify2()
{
  FreeDagNode* d = new FreeDagNode(symbol());
  int nrArgs = symbol()->arity();
  if (nrArgs != 0)
    {
      DagNode** p = d->argArray();
      for (int i = 0; i < nrArgs; i++, p++)
	(*p) = argArray[i]->dagify();
    }
  return d;
}

bool
FreeTerm::earlyMatchFailOnInstanceOf(const Term* other) const
{
  if (symbol() != other->symbol())
    return other->stable();  // terms headed by free symbols are always stable
  int nrArgs = argArray.length();
  const Vector<Term*>& argArray2 = safeCastNonNull<const FreeTerm*>(other)->argArray;
  for (int i = 0; i < nrArgs; ++i)
    {
      if (argArray[i]->earlyMatchFailOnInstanceOf(argArray2[i]))
	return true;
    }
  return false;
}

bool
FreeTerm::subsumes(const Term* other, bool sameVariableSet) const
{
  if (symbol() != other->symbol())
    return false;
  int nrArgs = argArray.length();
  const Vector<Term*>& argArray2 = safeCast(const FreeTerm*, other)->argArray;
  for (int i = 0; i < nrArgs; i++)
    {
      if (!argArray[i]->subsumes(argArray2[i], sameVariableSet))
	return false;
    }
  return true;
}

void
FreeTerm::findAvailableTerms(TermBag& availableTerms, bool eagerContext, bool atTop)
{
  if (ground())
    return;
  int nrArgs = argArray.length();
  FreeSymbol* s = symbol();
  if (atTop)
    {
      for (int i = 0; i < nrArgs; i++)
	argArray[i]->findAvailableTerms(availableTerms, eagerContext && s->eagerArgument(i));
    }
  else
    {
      availableTerms.insertMatchedTerm(this, eagerContext);
      for (int i = 0; i < nrArgs; i++)
	argArray[i]->findAvailableTerms(availableTerms, eagerContext && s->evaluatedArgument(i));
    }
}

//
//	Recursive function template to instantial the class template.
//
template<int n>
inline FreeRhsAutomaton* makeFreeOneInstructionRhsAutomaton(int arity, FreeRhsAutomaton* victim)
{
  return (arity == n) ? new FreeOneInstructionRhsAutomaton<n>(*victim) :
    makeFreeOneInstructionRhsAutomaton<n-1>(arity, victim);
}

template<>
inline FreeRhsAutomaton* makeFreeOneInstructionRhsAutomaton<0>(int /* arity */,
							       FreeRhsAutomaton* victim)
{
  return new FreeOneInstructionRhsAutomaton<0>(*victim);
}

//
//	We need partial specialization to handle the base cases of
//	of a double recursion so we wrap the functions in struct templates.
//
//	The second symbol in a two instruction automata will be the top
//	symbol for a free skeleton and thus must have at least 1 argument.
//
template<int n, int m>
struct HandleArgument2
{
  static FreeRhsAutomaton* handleArgument2(int arity2, FreeRhsAutomaton* victim)
  {
    return (arity2 == m) ? new FreeTwoInstructionRhsAutomaton<n, m>(*victim) :
      HandleArgument2<n, m-1>::handleArgument2(arity2, victim);
  }
};
  
template<int n>
struct HandleArgument2<n, 1>  // partial specialization
{
  static FreeRhsAutomaton* handleArgument2(int /* arity2 */, FreeRhsAutomaton* victim)
  {
    return new FreeTwoInstructionRhsAutomaton<n, 1>(*victim);
  }
};

template<int n, int m>
struct HandleArgument1
{
  static FreeRhsAutomaton* handleArgument1(int arity1, int arity2, FreeRhsAutomaton* victim)
  {
    return (arity1 == n) ? HandleArgument2<n,m>::handleArgument2(arity2, victim) :
      HandleArgument1<n-1,m>::handleArgument1(arity1, arity2, victim);
  }
};

template<int m>
struct HandleArgument1<0, m>  // partial specialization
{
  static FreeRhsAutomaton* handleArgument1(int /* arity1 */, int arity2, FreeRhsAutomaton* victim)
  {
    return HandleArgument2<0,m>::handleArgument2(arity2, victim);
  }
};

int
FreeTerm::compileRhs2(RhsBuilder& rhsBuilder,
		      VariableInfo& variableInfo,
		      TermBag& availableTerms,
		      bool eagerContext)
{
  int maxArity = 0;
  compileRhsAliens(rhsBuilder, variableInfo, availableTerms, eagerContext, maxArity);
  FreeRhsAutomaton* automaton;
  if (maxArity > 3)
    automaton = new FreeRhsAutomaton();  // general case
  else
    {
      //
      //	We have faster rhs automata for low arity cases.
      //
      if (maxArity == 3)
	automaton = new FreeFast3RhsAutomaton();  // all dag nodes padded to 3 args
      else
	automaton = new FreeFast2RhsAutomaton();  // all dag nodes padded to 2 args
    }
  //
  //	Now compile the free skeleton into the automaton.
  //
  int index = compileRhs3(automaton, rhsBuilder, variableInfo, availableTerms, eagerContext);
  if (maxArity <= 3)
    {
      //
      //	After compiling we know exactly how many instructions we have,
      //	taking into account common subexpression sharing and left->right
      //	sharing.
      //
      Index nrInstructions = automaton->getNrInstructions();
      if (nrInstructions == 1)
	{
	  //
	  //	Cannibalize automaton to make a faster one.
	  //	4 cases.
	  //
	  DebugInfo("using 1 instruction optimization for free skeleton " << this);
	  FreeRhsAutomaton* newAutomaton =
	    makeFreeOneInstructionRhsAutomaton<3>(automaton->getArity(0),
						 automaton);
	  delete automaton;
	  automaton = newAutomaton;
	}
      else if (nrInstructions == 2)
	{
	  //
	  //	Cannibalize automaton to make a faster one.
	  //	12 cases (2nd symbol can't be nullary).
	  //
	  DebugInfo("using 2 instruction optimization for free skeleton " << this);
	  Assert(automaton->getArity(1) != 0, "nullary 2nd symbol " << this);
	  FreeRhsAutomaton* newAutomaton =
	    HandleArgument1<3,3>::handleArgument1(automaton->getArity(0),
						  automaton->getArity(1),
						  automaton);
	  delete automaton;
	  automaton = newAutomaton;
	}
      else
	DebugInfo("only low arity optimization for " << this);
    }
  rhsBuilder.addRhsAutomaton(automaton);
  return index;
}

void
FreeTerm::compileRhsAliens(RhsBuilder& rhsBuilder,
			   VariableInfo& variableInfo,
			   TermBag& availableTerms,
			   bool eagerContext,
			   int& maxArity)
{
  //
  //	Traverse the free skeleton, calling compileRhs() on all non-free subterms.
  //
  int nrArgs = argArray.length();
  if (nrArgs > maxArity)
    maxArity = nrArgs;
  FreeSymbol* s = symbol();
  for (int i = 0; i < nrArgs; i++)
    {
      bool argEager = eagerContext && s->eagerArgument(i);
      Term* t = argArray[i];
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(t))
	{
	  if (!(availableTerms.findTerm(f, argEager)))
	    f->compileRhsAliens(rhsBuilder, variableInfo, availableTerms, argEager, maxArity);
	}
      else
	(void) t->compileRhs(rhsBuilder, variableInfo, availableTerms, argEager);
    }
}

int
FreeTerm::compileRhs3(FreeRhsAutomaton* automaton,
		      RhsBuilder& rhsBuilder,
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
  typedef Vector<pair<int, int> > PairVec;
  PairVec order(nrArgs);
  for (int i = 0; i < nrArgs; i++)
    {
      order[i].first = - argArray[i]->computeSize();  // larger terms to the front
      order[i].second = i;
    }
  sort(order.begin(), order.end());
  //
  //	Consider each argument in largest first order.
  //
  FreeSymbol* s = symbol();
  Vector<int> sources(nrArgs);
  for (const auto& p : order)
    {
      int argNr = p.second;
      bool argEager = eagerContext && s->eagerArgument(argNr);
      Term* t = argArray[argNr];
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(t))
	{
	  //
	  //	Argument is free - see if we need to compile it into current automaton.
	  //
	  if (!(availableTerms.findTerm(f, argEager)))
	    {
	      int source = f->compileRhs3(automaton,
					  rhsBuilder,
					  variableInfo,
					  availableTerms,
					  argEager);
	      sources[argNr] = source;
	      f->setSaveIndex(source);
	      availableTerms.insertBuiltTerm(f, argEager);
	      continue;
	    } 
	}
      //
      //	Alien, variable or available term so use standard mechanism.
      //
      sources[argNr] = t->compileRhs(rhsBuilder, variableInfo, availableTerms, argEager);
    }
  //
  //	Need to flag last use of each source.
  //
  for (int i : sources)
    variableInfo.useIndex(i);
  //
  //	Add to free step to automaton.
  //
  int destination = variableInfo.makeConstructionIndex();
  automaton->addFree(s, destination, sources);
  return destination;
}

void
FreeTerm::resetSlotIndices()
{
  slotIndex = -1;
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(argArray[i]))
	f->resetSlotIndices();
    }
}
