/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class Sort
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions
#include "sort.hh"
#include "connectedComponent.hh"
#include "module.hh"
#include "sortBdds.hh"

SortBdds::SortBdds(Module* module)
{
  //
  //	For each connected component of sorts, we calculate how many BDD variables are
  //	need to encode such a sort.
  //
  const Vector<ConnectedComponent*>& components = module->getConnectedComponents();
  int nrComponents = components.size();
  componentInfo.resize(nrComponents);
  int maxNrSorts = 0;
  for (int i = 0; i < nrComponents; i++)
    {
      int nrSorts = components[i]->nrSorts();
      if (nrSorts > maxNrSorts)
	maxNrSorts = nrSorts;
      componentInfo[i].nrVariables = calculateNrBits(nrSorts);
    }
  maxNrVariables = calculateNrBits(maxNrSorts);
  setNrVariables(2 * maxNrVariables);
  //
  //	For each connected component c we build the BDD encoding the relation
  //	  valid(s1) /\ valid(s2) /\ s1 > s2
  //	for sorts s1 and s2 in c.
  //
  //	Here s1 is encoded by BDD variables:
  //	  0,...,maxNrVariables-1
  //	and s2 is encoded by BDD variables:
  //	  maxNrVariables,...,2*maxNrVariables-1
  //
  for (int i = 0; i < nrComponents; ++i)
    {
      int nrVariables = componentInfo[i].nrVariables;
      ConnectedComponent* c = components[i];
      int nrSorts = c->nrSorts();
      Bdd disjunct;  // initialized to false by default
      for (int s1 = 0; s1 < nrSorts; ++s1)
	{
	  Bdd lesserSorts;   // initialized to false by default
	  const NatSet& leqSorts = c->sort(s1)->getLeqSorts();
	  for (int s2 = s1 + 1; s2 < nrSorts; ++s2)  // starting at s1+1 rules out the equals case
	    {
	      if (leqSorts.contains(s2))
		lesserSorts = bdd_or(lesserSorts, makeIndexBdd(maxNrVariables, nrVariables, s2));
	    }
	  //
	  //	At this point lesserSorts holds the relation less_than_s1(s2) for current s1.
	  //
	  disjunct = bdd_or(disjunct, bdd_and(makeIndexBdd(0, nrVariables, s1), lesserSorts));
	}
      componentInfo[i].gtRelation = disjunct;
    }
  DebugAdvisory("After gtRelation computation: BDD nodes in use: " << bdd_getnodenum());
  //
  //	For each sort s we compute the BDD encoding the relation
  //	  valid(s1) /\ s1 <= s
  //
  //	Here s1 is encoded by BDD variables:
  //	  0,...,maxNrVariables-1
  //
  const Vector<Sort*>& sorts = module->getSorts();
  int nrSorts = sorts.size();
  leqRelations.resize(nrSorts);
  for (int i = 0; i < nrSorts; ++i)
    {
      Bdd disjunct;
      Sort* s = sorts[i];
      int nrVariables = getNrVariables(s->component()->getIndexWithinModule());
      const NatSet& leqSorts = s->getLeqSorts();
      FOR_EACH_CONST(j, NatSet, leqSorts)
	disjunct = bdd_or(disjunct, makeIndexBdd(0, nrVariables, *j));
      leqRelations[i] = disjunct;
      DebugAdvisory("leq BDD for sort " << s << " is " << disjunct << " using " << nrVariables << " variables");
    }
  DebugAdvisory("After leqRelation computation: BDD nodes in use: " << bdd_getnodenum());
}

const Vector<Bdd>&
SortBdds::getSortFunction(Symbol* symbol) const
{
  //
  //	We construct the sort function on demand. This is partly to save time and space
  //	on symbols that don't occur in unification problems, but mostly to handle
  //	late symbols.
  //
  int symbolIndex = symbol->getIndexWithinModule();
  int currentSize = sortFunctions.size();
  if (currentSize <= symbolIndex)
    {
      //
      //	Resizing a vector of vectors of BDDs is very expensive so make it as large
      //	as we currently expect.
      //
      sortFunctions.resize(symbol->getModule()->getSymbols().size());
    }
  Vector<Bdd>& f = sortFunctions[symbolIndex];
  if (f.isNull())
    symbol->computeSortFunctionBdds(*this, f);
  return f;
}

void
SortBdds::makeIndexVector(int nrBdds, int index, Vector<Bdd>& vec) const
{
  //
  //	Make a vector of nrBdds true/false BDDs that encodes index.
  //
  //	This is useful for returning the vector of BDDs encoding of a constant sort.
  //
  vec.resize(nrBdds);
  for (int i = 0; index != 0; ++i, (index >>= 1))
    {
      if (index & 1)
	vec[i] = bddtrue;
    }
}

void
SortBdds::makeVariableVector(int firstVariable, int nrVariables, Vector<Bdd>& vec) const
{
  //
  //	Make a vector of nrVariables BDDs that encodes the BDD variables 
  //	firstVariable,..., firstVariable + nrVariables - 1.
  //
  //	This is useful for returning the vector of BDDs encoding of the undetermined
  //	sort of a variable.
  //
  vec.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    vec[i] = bdd_ithvar(firstVariable + i);
}

void
SortBdds::appendVariableVector(int firstBddVariable, int nrBddVariables, Vector<Bdd>& vec) const
{
  //
  //	Make a vector of nrBddVariables BDDs that encodes the BDD variables 
  //	firstBddVariable,..., firstBddVVariable + nrBddVVariables - 1 and append it to vec.
  //
  //	sort of a Maude variable.
  //
  for (int i = 0; i < nrBddVariables; ++i)
    vec.append(bdd_ithvar(firstBddVariable + i));
}

Bdd
SortBdds::makeIndexBdd(int firstVariable, int nrVariables, int index) const
{
  //
  //	Return a BDD in (firstVariable,..., firstVariable + nrVariables - 1) which is true exactly if
  //	for all k in 0,..., nrVariables - 1, BDD variable firstVariable + k corresponds to the kth
  //	bit in the binary representation of index.
  //
  Bdd result = bdd_true();
  int end = firstVariable + nrVariables;
  for (int i = firstVariable; i < end; ++i, index >>= 1)
    result = bdd_and(result, (index & 1) ? bdd_ithvar(i) : bdd_nithvar(i));
  return result;
}

Bdd
SortBdds::makeVariableBdd(int firstVariable, int nrVariables) const
{
  //
  //	Make a BDD that represents the set of BDD variables
  //	  {firstVariable,  firstVariable + 1 firstVariable + nrVariables - 1}
  //	as a conjunction.
  //
  //	This is useful for quantification over a set of BDD variables.
  //
  Bdd result = bdd_true();
  int end = firstVariable + nrVariables;
  for (int i = firstVariable; i < end; ++i)
    result = bdd_and(result, bdd_ithvar(i));
  return result;
}

int
SortBdds::calculateNrBits(int nrIndicies)
{
  //
  //	Calculate the number of bits needed to represent 0,..., nrIndicies-1.
  //
  int nrBits = 1;
  for (int representable = 2; representable < nrIndicies; ++nrBits, representable <<= 1)
    ;
  return nrBits;
}

Bdd
SortBdds::getRemappedLeqRelation(Sort* sort, int firstVariable) const
{
  //
  //	We return a BDD for the sort relation valid(s) /\ s <= sort
  //	with the BDD variables representing s remapped to start from
  //	firstVariable.
  //
  int nrVariables = getNrVariables(sort->component()->getIndexWithinModule());
  //bddPair* varMap = bdd_newpair();
  bddPair* varMap = getCachedPairing();
  for (int i = 0; i < nrVariables; ++i)
    bdd_setpair(varMap, i, firstVariable + i);
  Bdd leqRelation = bdd_replace(getLeqRelation(sort->getIndexWithinModule()), varMap);
  //bdd_freepair(varMap);
  for (int i = 0; i < nrVariables; ++i)
    bdd_setbddpair(varMap, i, bdd_false());
  return leqRelation;
}

Bdd
SortBdds::applyLeqRelation(Sort* sort, const Vector<Bdd>& argument) const
{
  //
  //	We return a Bdd for the sort relation valid(s) /\ s <= sort
  //	applied to an s given by a vector of BDDs.
  //
  int nrBdds = argument.size();
  Assert(nrBdds == getNrVariables(sort->component()->getIndexWithinModule()),
	 "wrong number of BDD arguments: expecting " << getNrVariables(sort->component()->getIndexWithinModule()) <<
	 " passed " << nrBdds <<
	 " sort " << sort);

  //bddPair* argMap = bdd_newpair();
  bddPair* argMap = getCachedPairing();
  for (int i = 0; i < nrBdds; ++i)
    bdd_setbddpair(argMap, i, argument[i]);
  Bdd result = bdd_veccompose(getLeqRelation(sort->getIndexWithinModule()), argMap);
  //bdd_freepair(argMap);
  for (int i = 0; i < nrBdds; ++i)
    bdd_setbddpair(argMap, i, bdd_false());
  return result;
}

// experimental code for faster sort computations

void
SortBdds::appendIndexVector(int nrBdds, int index, Vector<Bdd>& vec) const
{
  //
  //	Make a vector of nrBdds true/false BDDs that encodes index.
  //
  //	This is useful for returning the vector of BDDs encoding of a constant sort.
  //
  int oldSize = vec.size();
  vec.resize(nrBdds + oldSize);
  for (int i = oldSize; index != 0; ++i, (index >>= 1))
    {
      if (index & 1)
	vec[i] = bddtrue;
    }
}

void
SortBdds::operatorCompose(Symbol* op, const Vector<Bdd>& inputBdds, Vector<Bdd>& outputBdds) const
{
  //
  //	Calling this could increase the number of BDD variables so need to
  //	get it out of the way before we get the caching pairing.
  //
  const Vector<Bdd>& sortFunction = getSortFunction(op);
  //
  //	Now fill out the cached pairing with our input BDDs.
  //
  bddPair* cachedPairing = getCachedPairing();
  int nrInputBdds = inputBdds.size();
  for (int i = 0; i < nrInputBdds; ++i)
    bdd_setbddpair(cachedPairing, i, inputBdds[i]);
  //
  //	Apply compose with each of BDDs representing the sort function
  //	and append the result to the output BDDs.
  //
  FOR_EACH_CONST(i, Vector<Bdd>, sortFunction)
    outputBdds.append(bdd_veccompose(*i, cachedPairing));
  //
  //	Clean up the cached pairing by setting all the used slots
  //	to bdd_false(). This avoids the cached pairing containing pointers
  //	to unused junk and messing with garbage collection.
  //
  for (int i = 0; i < nrInputBdds; ++i)
    bdd_setbddpair(cachedPairing, i, bdd_false());
}
