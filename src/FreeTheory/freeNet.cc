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
//	Implementation for class FreeNet.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "graph.hh"
#include "pointerSet.hh"
#include "natSet.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "fullCompiler.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "equation.hh"

//	full compiler class definitions
#include "compilationContext.hh"

//	variable class definitions
#include "variableTerm.hh"

//	free theory class definitions
#include "freeNet.hh"
#include "freeSymbol.hh"
#include "freeDagNode.hh"
#include "freeOccurrence.hh"
#include "freeLhsAutomaton.hh"
#include "freeRhsAutomaton.hh"
#include "freeTerm.hh"
#include "freeRemainder.hh"

#include "freeNetExec.hh"
#include "freeNetExec.cc"

FreeNet::FreeNet()
{
  fast = true;  // until we know otherwise
}

FreeNet::~FreeNet()
{
  int nrRemainders = remainders.length();
  for (int i = 0; i < nrRemainders; i++)
    delete remainders[i];
}

int
FreeNet::allocateNode(int nrMatchArcs)
{
  //
  //	We will need a real node for each exiting arc from the virtual node that is labeled with a symbol. 
  //
  int len = net.length();
  net.resize(len + nrMatchArcs);
  return len;
}

void
FreeNet::fillOutNode(int nodeNr,
		     int position,
		     int argIndex,
		     const Vector<Symbol*>& symbols,
		     const Vector<int>& targets,
		     const Vector<int>& slots,
		     int neqTarget)
{
  int nrSymbols = symbols.length();
  Vector<Triple> triples(nrSymbols);
  for (int i = 0; i < nrSymbols; i++)
    {
      Assert(symbols[i] != 0, "null symbol");
      triples[i].symbol = symbols[i];
      triples[i].slot = slots[i];
      triples[i].subtree = targets[i];
    }
  sort(triples.begin(), triples.end(), tripleLt);
  buildTernaryTree(nodeNr,
		   triples,
		   0,
		   nrSymbols - 1,
		   neqTarget,
		   position,
		   argIndex);
}

int
FreeNet::addRemainderList(const PatternSet& liveSet)
{
  int index = applicable.size();
  applicable.append(liveSet);
  return ~index;
}

void
FreeNet::translateSlots(int nrRealSlots, const Vector<int>& slotTranslation)
{
  stack.expandTo(nrRealSlots);
  int nrNodes = net.length();
  for (int i = 0; i < nrNodes; i++)
    {
      net[i].slot = (net[i].slot == NONE) ? NONE : slotTranslation[net[i].slot];
      net[i].position = (net[i].position == NONE) ? NONE : slotTranslation[net[i].position];
    }
}

void
FreeNet::buildRemainders(const Vector<Equation*>& equations,
			 const PatternSet& patternsUsed,
			 const Vector<int>& slotTranslation)
{
  int nrEquations = equations.length();
  remainders.expandTo(nrEquations);
  for (int i = 0; i < nrEquations; i++)
    remainders[i] = 0;
  for (int i : patternsUsed)
    {
      Equation* e = equations[i];
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(e->getLhs()))
	{
	  FreeRemainder* r = f->compileRemainder(e, slotTranslation);
	  remainders[i] = r;
	  //
	  //	If a remainder doesn't have fast handling, neither can the discrimination net.
	  //
	  if (!(r->fastHandling()))
	    fast = false;
	}
      else
	{
	  remainders[i] = new FreeRemainder(e);  // remainder for "foreign" equation
	  fast = false;  // a foreign equation always disables fast handling for the net
	}
    }
  //
  //	Build null terminated pointer version of applicable for added speed.
  //
  int nrApplicables = applicable.length();
  fastApplicable.resize(nrApplicables);
  for (int i = 0; i < nrApplicables; i++)
    {
      PatternSet& liveSet = applicable[i];
      Vector<FreeRemainder*>& rems = fastApplicable[i];
      rems.resize(liveSet.size() + 1);
      Vector<FreeRemainder*>::iterator r = rems.begin();
      for (int j : liveSet)
	*r++ = remainders[j];
      *r = 0;
    }
}

local_inline bool
FreeNet::tripleLt(const Triple& p1, const Triple& p2)
{
  return p1.symbol->getIndexWithinModule() < p2.symbol->getIndexWithinModule();
}

void
FreeNet::buildTernaryTree(int& nodeIndex,
			  Vector<Triple>& triples,
			  int first,
			  int last,
			  int defaultSubtree,
			  int position,
			  int argIndex)
{
  //
  //	Pick a middle element as the test symbol.
  //	If the sum of the first and last eligible indices is odd we have a choice
  //	of middle elements and we try to break the tie in a smart way.
  //
  int sum = first + last;
  int testSymbol = sum / 2;
  if ((sum & 1) && moreImportant(triples[testSymbol + 1].symbol, triples[testSymbol].symbol))
    ++testSymbol;
  //
  //	Fill out a new node.
  //
  int i = nodeIndex++;
  net[i].position = position;
  net[i].argIndex = argIndex;
  net[i].symbolIndex = triples[testSymbol].symbol->getIndexWithinModule();
  net[i].slot = triples[testSymbol].slot;
  net[i].equal = triples[testSymbol].subtree;
  //
  //	If the are any symbols remaining to the left of the test symbol, build a subtree for them.
  //
  if (first < testSymbol)
    {
      net[i].notEqual[LESS] = nodeIndex;
      buildTernaryTree(nodeIndex, triples, first, testSymbol - 1, defaultSubtree, NONE, NONE);
    }
  else
    net[i].notEqual[LESS] = defaultSubtree;
  //
  //	If the are any symbols remaining to the right of the test symbol, build a subtree for them.
  //
  if (last > testSymbol)
    {
      net[i].notEqual[GREATER] = nodeIndex;
      buildTernaryTree(nodeIndex, triples, testSymbol + 1, last, defaultSubtree, NONE, NONE);
    }
  else
    net[i].notEqual[GREATER] = defaultSubtree;
}

bool
FreeNet::moreImportant(Symbol* first, Symbol* second)
{
  //
  //	Heuristic to decide which symbol is more important and thus
  //	should have the fastest matching.
  //	The current heuristic favors free symbols over non-free symbols and
  //	high arity symbols over low arity symbols.
  //
  //	Returns true if first symbol is considered more important.
  //
  FreeSymbol* f = dynamic_cast<FreeSymbol*>(first);
  FreeSymbol* s = dynamic_cast<FreeSymbol*>(second);
  if (f != 0 && s == 0)
    return true;
  if (f == 0 && s != 0)
    return false;
  return first->arity() > second->arity();
}

#ifdef DUMP
void
FreeNet::dump(ostream& s, int indentLevel)
{
  if (applicable.empty())
    {
      s << Indent(indentLevel) << "Empty FreeNet\n";
      return;
    }
  s << Indent(indentLevel) << "Begin{FreeNet}\n";
  ++indentLevel;

  s << Indent(indentLevel) << "testNodes:\n";
  ++indentLevel;
  for (int i = 0; i < net.length(); i++)
    {
      s << Indent(indentLevel) << "Node " << i <<
	": position " << net[i].position <<
	", argIndex " << net[i].argIndex <<
	", symbolIndex " << net[i].symbolIndex <<
	", slot " << net[i].slot <<
	", equal " << net[i].equal <<
	", notEqual[LESS] " << net[i].notEqual[LESS] <<
	", notEqual[GREATER] " << net[i].notEqual[GREATER] << '\n';
    }

  s << Indent(indentLevel - 1) << "applicable:\n";
  for (int i = 0; i < applicable.length(); i++)
    {
      s << Indent(indentLevel) << "Applicable sequence " << -1 - i << ':';
      for (int j : applicable[i])
	s << ' ' << j;
      s << '\n';
    }

  s << Indent(indentLevel - 1) << "remainders:\n";
  for (int i = 0; i < remainders.length(); i++)
    {
      s << Indent(indentLevel) << "Remainder " << i << '\n';
      if (remainders[i] != 0)
	remainders[i]->dump(s, indentLevel);
      else
	s << Indent(indentLevel) << "(not generated due to subsumption)\n";
    }

  s << Indent(indentLevel - 2) << "End{FreeNet}\n";
}
#endif
