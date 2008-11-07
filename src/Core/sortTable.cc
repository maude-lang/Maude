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
//      Implementation for abstract class SortTable.
//
#include <map>

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "sortBdds.hh"
#include "sortTable.hh"

#ifdef COMPILER
#include "compilationContext.hh"
#endif

#include "ctorDiagram.cc"
#include "sortErrorAnalysis.cc"

/*
void
SortTable::panic() const // HACK
{
  cerr << ((Symbol*)(this)) << endl;
  ((Symbol*)(this))->dump(cerr, 0);
  ((SortTable*)(this))->dumpSortDiagram(cerr, 0);
}
*/

SortTable::SortTable(int arity)
  : nrArgs(arity)
{
  singleNonErrorSort = 0;
  ctorStatus = 0;
}

bool
SortTable::domainSubsumes(int subsumer, int victim) const
{
  const Vector<Sort*>& s = opDeclarations[subsumer].getDomainAndRange();
  const Vector<Sort*>& v = opDeclarations[victim].getDomainAndRange();
  for (int i = 0; i < nrArgs; ++i)
    {
      if (!(leq(v[i], s[i])))
	return false;
    }
  return true;
}

void
SortTable::computeMaximalOpDeclSetTable()
{
  const ConnectedComponent* range = rangeComponent();
  int nrSorts = range->nrSorts();
  maximalOpDeclSetTable.resize(nrSorts);
  int nrDeclarations = opDeclarations.length();
  for (int i = 0; i < nrSorts; ++i)
    {
      NatSet& opDeclSet = maximalOpDeclSetTable[i];
      const Sort* target = range->sort(i);
      for (int j = 0; j < nrDeclarations; ++j)
	{
	  if (leq(opDeclarations[j].getDomainAndRange()[nrArgs], target))
	    {
	      for (int k = 0; k < j; ++k)
		{
		  if (opDeclSet.contains(k))
		    {
		      if(domainSubsumes(k, j))
			goto nextDecl;
		      else if (domainSubsumes(j, k))
			opDeclSet.subtract(k);
		    }
		}
	      opDeclSet.insert(j);
	    }
	nextDecl:
	  ;
	}
    }
}

void
SortTable::compileOpDeclarations()
{
#ifndef NO_ASSERT
  int nrDeclarations = opDeclarations.length();
  Assert(nrDeclarations > 0, "no operator declarations");
#endif

  componentVector.expandTo(nrArgs + 1);
  for (int i = 0; i <= nrArgs; i++)
    {
      ConnectedComponent* c = (opDeclarations[0].getDomainAndRange()[i])->component();
#ifndef NO_ASSERT
      //
      //	Check that components really do agree for subsort overloaded
      //	operator declarations.
      //
      for (int j = 1; j < nrDeclarations; j++)
	{
	  Assert(c == (opDeclarations[j].getDomainAndRange()[i])->component(),
		 "Sort declarations for operator " <<
		 static_cast<Symbol*>(this) <<  // hack
		 " disagree on the sort component for argument " << i + 1);
	}
#endif
      componentVector[i] = c;
    }
  buildSortDiagram();
  if (ctorStatus == IS_COMPLEX)
    buildCtorDiagram();
#ifdef DUMP
  // dumpSortDiagram(cout, 0);
#endif
}

bool
SortTable::kindLevelDeclarationsOnly() const
{
  FOR_EACH_CONST(i, Vector<OpDeclaration>, opDeclarations)
    {
      if (i->getDomainAndRange()[nrArgs]->index() != Sort::KIND)
	return false;
    }
  return true;
}

bool
SortTable::canProduceErrorSort() const
{
  if (specialSortHandling())
    return false;  // optimistic - derived class must overide if it wants to be bad!
  if (nrArgs == 0)
    return sortDiagram[0] == 0;
  NatSet currentStates;
  currentStates.insert(0);
  for (int i = 0; i < nrArgs; i++)
    {
      ConnectedComponent* component = componentVector[i];
      int index = component->errorFree() ? component->nrMaximalSorts() : 0;
      NatSet nextStates;
      const NatSet::const_iterator e = currentStates.end();
      for (NatSet::const_iterator j = currentStates.begin(); j != e; ++j)
	{
	  int state = *j;
	  int k = index;
	  do
	    nextStates.insert(traverse(state, k));
	  while (--k > 0);
	}
      currentStates.swap(nextStates);
    }
  return currentStates.contains(Sort::ERROR_SORT);
}

void
SortTable::finalizeSortInfo()
{
  //	For derived classes that don't do any sort constraint dependent
  //	compile time sort calculations.
}

bool
SortTable::partiallySubsumes(int subsumer, int victim, int argNr)
{
  const Vector<Sort*>& s = opDeclarations[subsumer].getDomainAndRange();
  const Vector<Sort*>& v = opDeclarations[victim].getDomainAndRange();
  if (!(leq(s[nrArgs], v[nrArgs])))
    return false;
  for (int i = argNr; i < nrArgs; i++)
    {
      if (!(leq(v[i], s[i])))
	return false;
    }
  return true;
}

void
SortTable::minimize(NatSet& alive, int argNr)
{
  if (!(alive.empty()))
    {
      int min = alive.min();
      int max = alive.max();
      for (int i = min; i <= max; i++)
	{
	  if (alive.contains(i))
	    {
	      for (int j = min; j <= max; j++)
		{
		  if (j != i && alive.contains(j) && partiallySubsumes(i, j, argNr))
		    alive.subtract(j);
		}
	    }
	}
    }
}

void
SortTable::buildSortDiagram()
{
  Vector<NatSet> currentStates(1);
  NatSet& all = currentStates[0];
  int nrDeclarations = opDeclarations.length();
  for (int i = nrDeclarations - 1; i >= 0; i--)
    all.insert(i);  // insert in reverse order for efficiency
  if (nrArgs == 0)
    {
      sortDiagram.expandTo(1);
      bool unique;
      int sortIndex = findMinSortIndex(all, unique);
      WarningCheck(unique, "sort declarations for constant " << QUOTE(safeCast(Symbol*, this)) <<
		   " do not have an unique least sort.");
      sortDiagram[0] = sortIndex;
      singleNonErrorSort = componentVector[0]->sort(sortIndex);
      return;
    }
  enum SpecialSortIndices
  {
    UNINITIALIZED = 0,
    IMPOSSIBLE = -1
  };
  int singleNonErrorSortIndex = UNINITIALIZED;
  Vector<NatSet> nextStates;
  int currentBase = 0;
  set<int> badTerminals;
  for (int i = 0; i < nrArgs; i++)
    {
      const ConnectedComponent* component = componentVector[i];
      int nrSorts = component->nrSorts();
      int nrCurrentStates = currentStates.length();

      int nextBase = currentBase + nrSorts * nrCurrentStates;
      sortDiagram.expandTo(nextBase);
      int nrNextSorts = (i == nrArgs - 1) ? 0 : componentVector[i + 1]->nrSorts();
      
      for (int j = 0; j < nrSorts; j++)
	{
	  Sort* s = component->sort(j);
	  NatSet viable;
	  for (int k = 0; k < nrDeclarations; k++)
	    {
	      if (leq(s, opDeclarations[k].getDomainAndRange()[i]))
		viable.insert(k);
	    }
	  for (int k = 0; k < nrCurrentStates; k++)
	    {
	      NatSet nextState(viable);
	      nextState.intersect(currentStates[k]);
	      int index = currentBase + k * nrSorts + j;
	      if (nrNextSorts == 0)
		{
		  bool unique;
		  int sortIndex = findMinSortIndex(nextState, unique);
		  sortDiagram[index] = sortIndex;
		  if (!unique)
		    badTerminals.insert(index);
		  if (sortIndex > 0)
		    {
		      if (singleNonErrorSortIndex == UNINITIALIZED)
			singleNonErrorSortIndex = sortIndex;
		      else if (singleNonErrorSortIndex > 0 &&
			       singleNonErrorSortIndex != sortIndex)
			singleNonErrorSortIndex = IMPOSSIBLE;
		    }
		}
	      else
		{
		  minimize(nextState, i + 1);
		  sortDiagram[index] =
		    nextBase + nrNextSorts * findStateNumber(nextStates, nextState);
		}
	    }
	}
      currentStates.swap(nextStates);
      nextStates.contractTo(0);
      currentBase = nextBase;
    }
  if (singleNonErrorSortIndex > 0)
    singleNonErrorSort = componentVector[nrArgs]->sort(singleNonErrorSortIndex);
  if (!(badTerminals.empty()))
    sortErrorAnalysis(true, badTerminals);
}

int
SortTable::findStateNumber(Vector<NatSet>& stateSet, const NatSet& state)
{
  int nrStates = stateSet.length();
  for (int i = 0; i < nrStates; i++)
    {
      if (stateSet[i] == state)
	return i;
    }
  stateSet.append(state);
  return nrStates;
}

int
SortTable::findMinSortIndex(const NatSet& state, bool& unique)
{
  Sort* minSort = componentVector[nrArgs]->sort(Sort::ERROR_SORT);  // start with error sort
  NatSet infSoFar(minSort->getLeqSorts());
  int nrDeclarations = opDeclarations.length();
  for (int i = 0; i < nrDeclarations; i++)
    {
      if (state.contains(i))
	{
	  Sort* result = opDeclarations[i].getDomainAndRange()[nrArgs];
	  const NatSet& resultLeqSorts = result->getLeqSorts();
	  infSoFar.intersect(resultLeqSorts);
	  if (infSoFar == resultLeqSorts)
	    minSort = result;
        }
    }
  unique = (infSoFar == minSort->getLeqSorts());
  return minSort->index();
}

void
SortTable::computeSortFunctionBdds(const SortBdds& sortBdds, Vector<Bdd>& sortFunctionBdds) const
{
  if (sortDiagram.isNull())
    return;  // operator doesn't use our mechanism
  //
  //	We take our sort decision diagram and produce a BDD version, encoding each sort index
  //	by a bit vector.
  //
  if (nrArgs == 0)
    {
      //
      //	Generate constant BDD vector for constant operator.
      //
      sortBdds.makeIndexVector(sortBdds.getNrVariables(componentVector[nrArgs]->getIndexWithinModule()),
			       singleNonErrorSort->index(),
			       sortFunctionBdds);
      return;
    }
  BddTable table(sortDiagram.size());  // only target entries actually used
  //  for (int i = 0; i < sortDiagram.size(); ++i)
  //    cerr << sortDiagram[i] << " ";
  //  cerr << endl;
  computeBddVector(sortBdds, 0, 0, table, 0);
  sortFunctionBdds.swap(table[0]);
}

void
SortTable::computeBddVector(const SortBdds& sortBdds,
			    int bddVarNr,
			    int argNr,
			    BddTable& table,
			    int nodeNr) const
{
  //  DebugAdvisory("starting computeBddVector(bddVarNr=" << bddVarNr << ",  argNr=" << argNr << ", nodeNr=" << nodeNr << ")");
  Assert(argNr < nrArgs, "bad argNr");
  BddVector& vec =  table[nodeNr];
  if (!vec.isNull())
    return;
  //
  //	We fill out the BDD vector vec to make it correspond to sortDiagram[nodeNr].
  //
  const ConnectedComponent* component = componentVector[argNr];
  int nrBddVariables = sortBdds.getNrVariables(component->getIndexWithinModule());
  //
  //	We first look at each value of our argument and OR together BDDs for those
  //	that go to the same place.
  //	
  //  DebugAdvisory("starting OR phase");
  int nrSorts = component->nrSorts();
  typedef map<int, Bdd> BddMap;
  BddMap disjuncts;
  for (int i = 0; i < nrSorts; ++i)
    {
      Bdd& disjunct = disjuncts[sortDiagram[nodeNr + i]];
      Bdd indexBdd = sortBdds.makeIndexBdd(bddVarNr, nrBddVariables, i);
      //DebugAdvisory("disjunct = " << disjunct.id() << " indexBdd = " << indexBdd.id());
      disjunct = bdd_or(disjunct, indexBdd /*sortBdds.makeIndexBdd(bddVarNr, nrBddVariables, i)*/);
      //DebugAdvisory("done disjunct = " << disjunct.id());
    }
  //
  //	Now we go through the disjunctions we collected,  ANDing them with the
  //	vectors at their targets and OR the products together to form our final
  //	vector.
  //
  //  DebugAdvisory("starting OR of ANDs phase");
  int nrBdds = sortBdds.getNrVariables(componentVector[nrArgs]->getIndexWithinModule());
  vec.resize(nrBdds);
  FOR_EACH_CONST(i, BddMap, disjuncts)
    {
      int target = i->first;
      if (argNr + 1 == nrArgs)
	{
	  //
	  //	Target is actually the index of a sort.
	  //
	  BddVector t;
	  sortBdds.makeIndexVector(nrBdds, target, t);
	  for (int j = 0; j < nrBdds; ++j)
	    {
	      //DebugAdvisory("vec[j] " << vec[j].id());
	      //DebugAdvisory("t[j] " << t[j].id());
	      vec[j] = bdd_or(vec[j], bdd_and(i->second, t[j]));
	    }
	}
      else
	{
	  computeBddVector(sortBdds, bddVarNr + nrBddVariables, argNr + 1, table, target);
	  BddVector& targetVec = table[target];
	  for (int j = 0; j < nrBdds; ++j)
	    {
	      //DebugAdvisory("vec[j] " << vec[j].id());
	      //DebugAdvisory("targetVec[j] " << targetVec[j].id());
	      vec[j] = bdd_or(vec[j], bdd_and(i->second, targetVec[j]));
	    }
	}
    }
  //DebugAdvisory("done computeBddVector(bddVarNr=" << bddVarNr << ",  argNr=" << argNr << ", nodeNr=" << nodeNr << ")");
}

#ifdef COMPILER
void
SortTable::generateSortDiagram(CompilationContext& context) const
{
  Vector<int> minimum(0, nrArgs);
  Vector<int> maximum(0, nrArgs);
  {
    int min = 0;
    int max = 0;
    minimum.append(0);
    maximum.append(0);
    for (int i = 0; i < nrArgs - 1; i++)
      {
	int nrSorts = componentVector[i]->nrSorts();
	int newMin = UNBOUNDED;
	int newMax = 0;
	for (int j = min; j < max + nrSorts; j++)
	  {
	    int t = sortDiagram[j];
	    if (t < newMin)
	      newMin = t;
	    if (t > newMax)
	      newMax = t;
	  }
	min = newMin;
	max = newMax;
        minimum.append(min);
	maximum.append(max);
      }
  }
  int index = static_cast<const Symbol*>(this)->getIndexWithinModule();
  for (int i = nrArgs - 1; i >= 0; i--)
    {
      int nrSorts = componentVector[i]->nrSorts();
      int min = minimum[i];
      int max = maximum[i];

      for (int j = min; j <= max; j += nrSorts)
	{
	  context.head() << "Flags";
	  for (int k = i; k < nrArgs - 1; k++)
	    context.head() << '*';
	  context.head() << " f" << index << '_' << j << "[] = { ";
	  for (int k = 0; k < nrSorts; k++)
	    {
	      if (i == nrArgs - 1)
		context.head() << '{' << sortDiagram[j + k] << '}';
	      else
		context.head() << 'f' << index << '_' << sortDiagram[j + k];
	      if (k + 1 < nrSorts)
		context.head() << ", ";
	    }
	  context.head() << " };\n";
	}
    }
}
#endif

#ifdef DUMP
void
SortTable::dumpSortDiagram(ostream& s, int indentLevel)
{
  if (specialSortHandling())
    return;  // HACK
  s << Indent(indentLevel) << "Begin{SortDiagram}";
  indentLevel += 2;
  set<int> nodes;
  nodes.insert(0);
  ConnectedComponent* range = componentVector[nrArgs];
  if (nrArgs == 0)
    {
      int target = sortDiagram[0];
      s << '\n' << Indent(indentLevel - 1) << "node 0 -> sort " <<
	target << " (" << range->sort(target) << ")\n";
    }
  for (int i = 0; i < nrArgs; i++)
    {
      ConnectedComponent* component = componentVector[i];
      int nrSorts = component->nrSorts();
      set<int>  nextNodes;
      FOR_EACH_CONST(j, set<int>, nodes)
	{
	  int n = *j;
	  s << '\n' << Indent(indentLevel - 1) << "Node " << n <<
	    " (testing argument " << i << ")\n";
	  for (int k = 0; k < nrSorts; k++)
	    {
	      int target = sortDiagram[n + k];
	      s << Indent(indentLevel) << "sort " << k << " (" <<
		component->sort(k) << ") -> ";
	      if (i == nrArgs - 1)
		{
		  s << "sort " << target << " (" <<
		    range->sort(target) << ")\n";
		}
	      else
		{
		  s << "node " << target << '\n';
		  nextNodes.insert(target);
		}
	    }
	}
      nodes.swap(nextNodes);
    }
  s << Indent(indentLevel - 2) << "End{SortDiagram}\n";
}

#endif
