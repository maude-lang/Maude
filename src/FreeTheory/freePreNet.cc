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
#include "pointerSet.hh"
#include "natSet.hh"
#include "indent.hh"
#include "graph.hh"

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
#include "argumentIterator.hh"
#include "rewritingContext.hh"
#include "equation.hh"

//	full compiler class definitions
#include "compilationContext.hh"
#include "variableName.hh"

//	variable class definitions
#include "variableTerm.hh"

//	free theory class definitions
#include "freePreNet.hh"
#include "freeSymbol.hh"
#include "freeDagNode.hh"
#include "freeOccurrence.hh"
#include "freeLhsAutomaton.hh"
#include "freeRhsAutomaton.hh"
#include "freeTerm.hh"
#include "freeRemainder.hh"

#include "freePreNetSubsumption.cc"
#include "freePreNetSemiCompiler.cc"

#ifdef COMPILER
#include "freePreNetFullCompiler.cc"
#endif

FreePreNet::FreePreNet(bool expandRemainderNodes)
  : expandRemainderNodes(expandRemainderNodes)
{
}

void
FreePreNet::buildNet(FreeSymbol* symbol)
{
  DebugEnter("for " << symbol);
  topSymbol = symbol;
  nrFailParents = 0;
  nrFailVisits = 0;
  const Vector<Equation*>& equations = symbol->getEquations();
  int nrEquations = equations.length();
  if (nrEquations == 0)
    return;
  DebugInfo("nrEquations = " << nrEquations);
  patterns.expandTo(nrEquations);
  LiveSet liveSet;
  LiveSet potentialSubsumers;
  for (int i = 0; i < nrEquations; i++)
    {
      int flags = 0;
      Equation* e = equations[i];
      Term* p = e->getLhs();
      patterns[i].term = p;
      for (int j : potentialSubsumers)
	{
	  if (patterns[j].term->subsumes(p, false))
	    {
	      DebugAdvisory(e << " subsumed");
	      goto subsumedAtTop;
	    }
	}
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(p))
	{
	  NatSet usedVariables(e->getRhs()->occursBelow());
	  if (e->hasCondition())
	    usedVariables.insert(e->getConditionVariables());
	  else
	    flags = SUBSUMER;
	  Vector<int> path;
	  NatSet boundVariables;
	  bool unfailing = f->scanFreeSkeleton(usedVariables,
					       path,
					       positions,
					       boundVariables,
					       patterns[i].subterms);
	  if (unfailing && (flags & SUBSUMER))
	    flags |= UNFAILING;
	}
      liveSet.insert(i);
      if ((flags & SUBSUMER) && !(p->ground()))
	potentialSubsumers.insert(i);  //  to avoid trivial quadratic blow-up we keep track of potential subsumers
    subsumedAtTop:
      patterns[i].flags = flags;
    }
  Vector<int> topPosition;
  topPositionIndex = positions.position2Index(topPosition);
  NatSet fringe;
  expandFringe(topPositionIndex, symbol, fringe);
  reduceFringe(liveSet, fringe);
  NatSet positionsTested;
  (void) makeNode(liveSet, fringe, positionsTested);
  // dump(cerr, 0);
  DebugExit("for " << symbol << " done; nodes = " << net.size());
}

int
FreePreNet::makeNode(const LiveSet& liveSet,
		     const NatSet& reducedFringe,
		     const NatSet& positionsTested)
{
  DebugEnter("liveSet has " << liveSet.size() << " patterns");
  //
  //	First check to see if there is already a node with the same set
  //	of live patterns and the same reduced fringe that we can share.
  //
  NodeIndex key;
  key.liveSet = liveSet;
  key.reducedFringe = reducedFringe;
  {
    NodeMap::iterator i =  net.find(key);
    if (i != net.end())
      {
	//
	//	We can share this node but the positionsTested must be reduced
	//	to those positions tested on all routes to this node.
	//
	i->second.positionsTested.intersect(positionsTested);
	return i->second.nodeNr;
      }
  }
  //
  //	Need to make a brand new node.
  //
  NodeMap::iterator i = (net.insert(NodeMap::value_type(key, NodeBody()))).first;
  NodeBody& n = i->second;
  n.nodeNr = netVec.size();
  netVec.append(i);

  n.positionsTested = positionsTested;  // deep copy
  n.nrParents = 0;
  n.nrVisits = 0;
  if (reducedFringe.empty())
    {
      //
      //	Remainder node case; everything matched.
      //
      if (expandRemainderNodes)
	{
	  //
	  //	We expand remainder node by removing first pattern
	  //	and building a remainder node with the remaining
	  //	patterns. The idea is to share the tail ends of
	  //	the remainder lists. This is required for full compilation
	  //	since the code to handle the remainders is generated
	  //	inline and will become excessively bulky without sharing.
	  //	In the semi-compilation case, FreeNets have a layer
	  //	of indirection that gives us perfect sharing of
	  //	remainders and we don't use the expansion of remainder
	  //	nodes.
	  //
	  LiveSet::const_iterator start = liveSet.begin();
	  n.patternIndex = *start;
	  ++start;
	  LiveSet newLiveSet(start, liveSet.end());
	  int nextNode = newLiveSet.empty() ? NONE :
	    makeNode(newLiveSet, reducedFringe, positionsTested);
	  n.nextPattern = nextNode;
	}
    }
  else
    {
      //
      //	Match node case; choose a test position.
      //
      int positionIndex = findBestPosition(key, n);
      NatSet newPositionsTested(positionsTested);
      newPositionsTested.insert(positionIndex);
      n.testPositionIndex = positionIndex;
      //
      //	Partition live set. This isn't strictly necessary but it
      //	prevents a quadratic blow up on certain examples with
      //	huge pattern set.
      //
      std::map<int, LiveSet> liveSets;
      LiveSet defaultLiveSet;
      partitionLiveSet(liveSet,
		       positionIndex,
		       n.sons,	
		       liveSets,
		       defaultLiveSet);
      //
      //	Make an arc for each symbol we must check for.
      //
      int nrSymbols = n.sons.length();
      for (int i = 0; i < nrSymbols; i++)
	{
	  Symbol* symbol = n.sons[i].label;
	  //
	  //	Make a new fringe:
	  //	(1) Remove tested position from old fringe.
	  //	(2) Add in all positions directly beneath tested position assuming
	  //	    this position contains our current symbol.
	  //	(3) Compute the set of live patterns, taking subsumption into account.
	  //	(4) Reduce the fringe by discarding positions that are not stable
	  //	    in any live pattern.
	  //
	  NatSet newFringe(reducedFringe);
	  newFringe.subtract(positionIndex);
	  expandFringe(positionIndex, symbol, newFringe);
	  LiveSet newLiveSet;
	  findLiveSet(liveSets[symbol->getIndexWithinModule()],
		      positionIndex,
		      symbol, 
		      newFringe,
		      newLiveSet);
	  reduceFringe(newLiveSet, newFringe);
	  Assert(!(newLiveSet.empty()),
		 "empty live set on arc labeled with symbol");
	  int t = makeNode(newLiveSet, newFringe, newPositionsTested);
	  n.sons[i].target = t;
	}
      //
      //	Make an arc for the default case.
      //
      NatSet newFringe(reducedFringe);
      newFringe.subtract(positionIndex);
      LiveSet newLiveSet;
      findLiveSet(defaultLiveSet,
		  positionIndex,
		  0, 
		  newFringe,
		  newLiveSet);
      if (newLiveSet.empty())
	n.neqTarget = NONE;
      else
	{
	  reduceFringe(newLiveSet, newFringe);
	  int t = makeNode(newLiveSet, newFringe, newPositionsTested);
	  n.neqTarget = t;
	}
    }
  return n.nodeNr;
}

void
FreePreNet::expandFringe(int positionIndex, Symbol* symbol, NatSet& fringe)
{
  if (dynamic_cast<FreeSymbol*>(symbol))
    {
      int nrArgs = symbol->arity();
      if (nrArgs > 0)
	{
	  Vector<int> newPath(positions.index2Position(positionIndex));
	  int length = newPath.length();
	  newPath.expandBy(1);
	  for (int i = 0; i < nrArgs; i++)
	    {
	      newPath[length] = i;
	      fringe.insert(positions.position2Index(newPath));
	    }
	}
    }
}

void
FreePreNet::reduceFringe(const LiveSet& liveSet, NatSet& fringe) const
{
  NatSet useless;
  //
  //	For each position in the fringe.
  //
  for (int i : fringe)
    {
      const Vector<int>& position = positions.index2Position(i);
      //
      //	See if at least one live pattern has a stable symbol at this position.
      //
      for (int j : liveSet)
	{
	  if (FreeTerm* f = dynamic_cast<FreeTerm*>(patterns[j].term))
	    {
	      if (Term* t = f->locateSubterm(position))
		{
		  if (t->stable())
		    goto survive;
		}
	    }
	}
      useless.insert(i);  // useless position
    survive:
      ;
    }
  fringe.subtract(useless);
}

void
FreePreNet::findLiveSet(const LiveSet& original,
			int positionIndex,
			Symbol* symbol,
			const NatSet& fringe,
			LiveSet& liveSet)
{
  const Vector<int> position(positions.index2Position(positionIndex));  // deep copy - because ref become stale

  for (int patternIndex : original)
    {
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(patterns[patternIndex].term))
	{
	  if (Term* t = f->locateSubterm(position))
	    {
	      if (t->stable())
		{
		  if (symbol == t->symbol())
		    liveSet.insert(patternIndex);
		  // else dead by not matching
		}
	      else  // not stable
		{
		  if (symbol == 0 || symbol->mightMatchPattern(t))
		    {
		      if (!partiallySubsumed(liveSet, patternIndex, fringe))
			liveSet.insert(patternIndex);
		      // else dead by subsumption
		    }
		  // else dead by not matching
		}
	    }
	  else // does not exist
	    {
	      if (!partiallySubsumed(liveSet, patternIndex, fringe))
		liveSet.insert(patternIndex);
	      // else dead by subsumption
	    }
	}
      else  // pattern not headed by a free function symbol
	liveSet.insert(patternIndex);
    }
}

void
FreePreNet::partitionLiveSet(const LiveSet& original,
			     int positionIndex,
			     const Vector<Arc>& arcs,
			     std::map<int, LiveSet>& liveSets,
			     LiveSet& defaultLiveSet)
{
  const Vector<int>& position = positions.index2Position(positionIndex);  // safe because we won't add new positions
  
  for (int patternIndex : original)
    {
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(patterns[patternIndex].term))
	{
	  if (Term* t = f->locateSubterm(position))
	    {
	      if (t->stable())
		liveSets[t->symbol()->getIndexWithinModule()].insert(patternIndex);
	      else
		{
		  //
		  //	Pattern has an unstable symbol at this position. Pattern goes into
		  //	live set for the default arc.
		  //
		  defaultLiveSet.insert(patternIndex);
		  //
		  //	Need to consider pattern for the live set of each active
		  //	symbol that could match our unstable subpattern.
		  //
		  for (const Arc& j : arcs)
		    {
		      Symbol* symbol = j.label;
		      if (symbol->mightMatchPattern(t))
			liveSets[symbol->getIndexWithinModule()].insert(patternIndex);
		    }
		}
	      continue;
	    }
	}
      //
      //	Pattern not headed by a free symbol or the tested position does not exist.
      //	Either way we add it to all live sets.
      //
      defaultLiveSet.insert(patternIndex);
      for (const Arc& j : arcs)
	liveSets[j.label->getIndexWithinModule()].insert(patternIndex);
    }
}

bool
FreePreNet::partiallySubsumed(const LiveSet& liveSet,
			      int victim,
			      const NatSet& fringe)
{
  if (liveSet.empty())
    return false;
  Term* vp = patterns[victim].term;
  for (int i : liveSet)
    {
      Assert(i < victim, "current liveSet contains pattern >= victim");
      if ((patterns[i].flags & SUBSUMER) &&
	  subsumesWrtReducedFringe(patterns[i].term, vp, topPositionIndex, fringe))
	return true;
    }
  return false;
}

int
FreePreNet::findBestPosition(const NodeIndex& ni, NodeBody& n) const
{
  //
  //	We chose a "best" position to test from node n's reduced fringe.
  //	We also fill out n.sons with the set of symbols we need to test
  //	at this position.
  //
  PointerSet bestSymbols;
  int bestPosition = NONE;
  int smallestScore = INT_MAX;
  //
  //	Consider each position in the reduced fringe.
  //
  const NatSet::const_iterator ei = ni.reducedFringe.end();
  for (NatSet::const_iterator i = ni.reducedFringe.begin(); i != ei; ++i)
    {
      const Vector<int>& path = positions.index2Position(*i);
      PointerSet symbols;
      int nrLive = 0;
      int nrStable = 0;
      int nrUnstableSubsumersAbove = 0;
      //
      //	Gather the set of symbols that occur at this position
      //	in a live pattern.
      //
      for (int j : ni.liveSet)
	{
	  if (FreeTerm* f = dynamic_cast<FreeTerm*>(patterns[j].term))
	    {
	      Term* t = f->locateSubterm(path);
	      if (t != 0 && t->stable())
		{
		  ++nrStable;
		  symbols.insert(t->symbol());
		}
	      else if (nrStable == 0 && patterns[j].flags & SUBSUMER)
		++nrUnstableSubsumersAbove;
	    }
	  ++nrLive;
	}
      int nrSymbols = symbols.cardinality();
      //
      //	We estimate how bad a position is by summing the number of live
      //	patterns on each branch. There are "nrSymbols" + 1 branches
      //	(including the default branch) and the "nrLive" - "nrStable"
      //	unstable patterns must be included on each branch while the
      //	"nrStable" stable patterns will each appear on just one branch.
      //	This gives a total of
      //		nrStable + (nrSymbols + 1) * (nrLive - nrStable)
      //		= nrLive + nrSymbols * (nrLive - nrStable)
      //
      //	However we now bias this result towards selecting positions
      //	that are stable in earlier subsumers by adding in the number
      //	of subsumer patterns before the first pattern that is stable
      //	at this position.
      //
      int badnessScore = nrUnstableSubsumersAbove +
	nrLive + nrSymbols * (nrLive - nrStable);
      if (badnessScore < smallestScore)
	{
	  smallestScore = badnessScore;
	  bestPosition = *i;
	  bestSymbols.swap(symbols);
	}
    }
  int nrSymbols = bestSymbols.cardinality();
  n.sons.expandTo(nrSymbols);
  for (int i = 0; i < nrSymbols; i++)
    n.sons[i].label = static_cast<Symbol*>(bestSymbols.index2Pointer(i));
  return bestPosition;
}

#ifdef DUMP
void
FreePreNet::dump(ostream& s, int indentLevel) const
{
  s << Indent(indentLevel) << "begin{FreePreNet}\n";
  ++indentLevel;
  s << Indent(indentLevel) <<"Patterns:\n";
  ++indentLevel;
  int nrPatterns = patterns.length();
  for (int i = 0; i < nrPatterns; i++)
    {
      s << Indent(indentLevel) << i << '\t' << patterns[i].term;
      if (patterns[i].flags & SUBSUMER)
	s << "  (subsumer)";
      if (patterns[i].flags & UNFAILING)
	s << "  (unfailing)";
      s << '\n';
    }
  int nrNodes = netVec.length();
  for (int i = 0; i < nrNodes; i++)
    {
      NodeMap::iterator j = netVec[i];
      const NodeIndex& ni = j->first;
      const NodeBody& n = j->second;
      Assert(i == n.nodeNr, "node index clash");

      s << Indent(indentLevel - 1) << "Node " << i << '\n';
      s << Indent(indentLevel) << "Live set: ";
      dumpLiveSet(s, ni.liveSet);
      s << '\n' << Indent(indentLevel) << "Reduced fringe: ";
      dumpPositionSet(s, ni.reducedFringe);
      s << '\n' << Indent(indentLevel) << "Positions tested: ";
      dumpPositionSet(s, n.positionsTested);
      s << '\n' << Indent(indentLevel) << "Slot map: ";
      dumpSlotMap(s, n.slotMap);
      s << '\n';
      if (ni.reducedFringe.empty())
	{
	  if (expandRemainderNodes)
	    {
	      //
	      //	These fields are set in the !expandRemainderNodes case.
	      //
	      s << Indent(indentLevel) << "Pattern index: " << n.patternIndex <<  '\n';
	      s << Indent(indentLevel) << "Next pattern: " << n.nextPattern << '\n';
	    }
	}
      else
	{
	  s << Indent(indentLevel) << "Test position: ";
	  if (n.testPositionIndex == NONE)
	    s << "NONE";
	  else
	    dumpPath(s, positions.index2Position(n.testPositionIndex));
	  s << '\n' << Indent(indentLevel) << "Sons:\n";
	  int nrSons = n.sons.length();
	  for (int j = 0; j < nrSons; j++)
	    {
	      const Arc& a = n.sons[j];
	      s << Indent(indentLevel + 1) << a.label << " -> " << a.target << '\n';
	    }
	  s << Indent(indentLevel) << "=/= arc: " << n.neqTarget << '\n';
	}
    }
  s << Indent(indentLevel - 2) << "end{FreePreNet}\n";
}

void
FreePreNet::dumpSlotMap(ostream& s, const Vector<Pair>& slotMap) const
{
  int nrPositions = slotMap.length();
  if (nrPositions == 0)
    s << "*empty*";
  else
    {
      for (int i = 0; i < nrPositions; i++)
	{
	  if (i != 0)
	    s << ", ";
	  s << '(';
	  dumpPath(s, positions.index2Position(slotMap[i].positionIndex));
	  s << " -> " << slots.findRep(slotMap[i].slot) << ')';
	}
    }
}

void
FreePreNet::dumpPath(ostream& s, const Vector<int>& path)
{
  int length = path.length();
  if (length == 0)
    s << '^';
  else
    {
      for (int i = 0; i < length; i++)
	{
	  if (i != 0)
	    s << '.';
	  s << path[i];
	}
    }
}

void
FreePreNet::dumpPositionSet(ostream& s, const NatSet& positionSet) const
{
  if (positionSet.empty())
    s << "*empty*";
  else
    {
      bool first = true;
      int max = positionSet.max();
      for (int j = 0; j <= max; j++)
	{
	  if (positionSet.contains(j))
	    {
	      if (first)
		first = false;
	      else
		s << ", ";
	      dumpPath(s, positions.index2Position(j));
	    }
	}
    }
}

void
FreePreNet::dumpNatSet(ostream& s, const NatSet& natSet)
{
  if (natSet.empty())
    s << "*empty*";
  else
    {
      bool first = true;
      int max = natSet.max();
      for (int j = 0; j <= max; j++)
	{
	  if (natSet.contains(j))
	    {
	      if (first)
		first = false;
	      else
		s << ", ";
	      s << j;
	    }
	}
    }
}

void
FreePreNet::dumpLiveSet(ostream& s, const LiveSet& liveSet)
{
  if (liveSet.empty())
    s << "*empty*";
  else
    {
      bool first = true;
      for (int j : liveSet)
	{
	  if (first)
	    first = false;
	  else
	    s << ", ";
	  s << j;
	}
    }
}
#endif
