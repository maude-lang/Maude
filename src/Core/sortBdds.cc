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
	  for (int s2 = s1 + 1; s2 < nrSorts; ++s2)
	    {
	      if (leqSorts.contains(s2))
		lesserSorts = bdd_or(lesserSorts, makeIndexBdd(maxNrVariables, nrVariables, s2));
	    }
	  disjunct = bdd_or(disjunct, bdd_and(makeIndexBdd(0, nrVariables, s1), lesserSorts));
	}
      componentInfo[i].gtRelation = disjunct;
    }
  DebugAdvisory("After gtRelation computation: BDD nodes in use: " << bdd_getnodenum());
  //
  //	For each sort s we compute the BDD encoding the relation
  //	  valid(s1) /\ s1 < s
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
    }
  DebugAdvisory("After leqRelation computation: BDD nodes in use: " << bdd_getnodenum());
  //
  //	For each operator, we compute the vector of BDDs that encodes its sort
  //	function.
  //
  const Vector<Symbol*>& symbols = module->getSymbols();
  int nrSymbols = symbols.size();
  sortFunctions.resize(nrSymbols);
  for (int i = 0; i < nrSymbols; ++i)
    symbols[i]->computeSortFunctionBdds(*this, sortFunctions[i]);
  DebugAdvisory("After sort function computation: BDD nodes in use: " << bdd_getnodenum());
}

void
SortBdds::makeIndexVector(int nrBdds, int index, Vector<Bdd>& vec) const
{
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
  vec.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    vec[i] = ithvar(firstVariable + i);
}

Bdd
SortBdds::makeIndexBdd(int firstVariable, int nrVariables, int index) const
{
  Bdd result = bdd_true();
  int end = firstVariable + nrVariables;
  for (int i = firstVariable; i < end; ++i, index >>= 1)
    result = bdd_and(result, (index & 1) ? ithvar(i) : nithvar(i));  // HACK
  //    result = bdd_and(result, (index & 1) ? bdd_ithvar(i) : bdd_nithvar(i));
  return result;
}

Bdd
SortBdds::makeVariableBdd(int firstVariable, int nrVariables) const
{
   Bdd result = bdd_true();
  int end = firstVariable + nrVariables;
  for (int i = firstVariable; i < end; ++i)
    result = bdd_and(result, ithvar(i));  // HACK
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
