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
FreeNet::allocateNode(int /* nrMatchArcs */)
{
  //
  //	We will need a single real node for virtual node. 
  //
  int len = net.size();
  net.resize(len + 1);
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
  Index nrSymbols = symbols.size();
  Assert(nrSymbols > 0, "no symbols to test");
  Assert(symbols[0] != 0, "null symbol");
  int lastMatchIndex = symbols[0]->rangeComponent()->getLastAllocatedMatchIndex();
  TestNode& node = net[nodeNr];
  node.position = position;
  node.argIndex = argIndex;
  node.branches.resize(lastMatchIndex + 1);
  for (NextPair& p : node.branches)
    {
      //
      //	If a symbol isn't recognized, it gets the default arc and a pointer
      //	to its argument list isn't stored.
      //
      p.nodeIndex = neqTarget;
      p.slotIndex = NONE;
    }
  for (int i = 0; i < nrSymbols; ++i)
    {
      Assert(symbols[i] != 0, "null symbol");
      int matchIndex = symbols[i]->getMatchIndex();
      Assert(matchIndex != 0,
	     "symbol " << symbols[i] << " that we're matching against has matchIndex = 0");
      NextPair& p = node.branches[matchIndex];
      p.nodeIndex = targets[i];
      p.slotIndex = slots[i];
    }
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
  for (TestNode& n : net)
    {
      n.position = (n.position == NONE) ? NONE : slotTranslation[n.position];
      for (NextPair& p : n.branches)
	p.slotIndex = (p.slotIndex == NONE) ? NONE : slotTranslation[p.slotIndex];
    }
}

void
FreeNet::buildRemainders(const Vector<Equation*>& equations,
			 const PatternSet& patternsUsed,
			 const Vector<int>& slotTranslation)
{
  remainders.expandTo(equations.size());
  for (FreeRemainder*& r : remainders)
    r = nullptr;
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
  Index i = 0;
  for (const TestNode& n : net)
    {
      s << Indent(indentLevel) << "Node " << i++ <<
	": position " << n.position <<
	", argIndex " << n.argIndex << '\n';
      s << Indent(indentLevel + 1);
      for (const NextPair& p : n.branches)
	s << " (" << p.nodeIndex << ", " << p.slotIndex << ")";
      s << '\n';
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
