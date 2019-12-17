/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class UnifierFilter.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
//#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "lhsAutomaton.hh"
#include "subproblem.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "subproblemAccumulator.hh"
#include "variableInfo.hh"

//	higher class definitions
#include "unifierFilter.hh"

UnifierFilter::UnifierFilter(int firstInterestingVariable, int nrInterestingVariables)
  : firstInterestingVariable(firstInterestingVariable),
    nrInterestingVariables(nrInterestingVariables)
{
  startedExtractingUnifiers = false;
}

UnifierFilter::~UnifierFilter()
{
  FOR_EACH_CONST(i, RetainedUnifierList, mostGeneralSoFar)
    delete *i;
}

void
UnifierFilter::markReachableNodes()
{
  FOR_EACH_CONST(i, RetainedUnifierList, mostGeneralSoFar)
    {
      const Substitution& unifier = (*i)->unifier;
      int nrFragile = unifier.nrFragileBindings();
      for (int j = 0; j < nrFragile; ++j)
	{
	  DagNode* d = unifier.value(j);
	  if (d != 0)
	    d->mark();
	}
    }
}

void
UnifierFilter::insertUnifier(const Substitution& unifier, int positionIndex, int equationIndex)
{
  //
  //	First we check if it is subsumed by one of the existing unifiers.
  //
  FOR_EACH_CONST(i, RetainedUnifierList, mostGeneralSoFar)
    {
      if (subsumes(*i, unifier))
	{
	  DebugAdvisory("new unifier subsumed");
	  return;
	}
    }
  DebugAdvisory("new unifier added");
  //
  //	Compile a set of matching automata for this unifier.
  //
  RetainedUnifier* newUnifier = new RetainedUnifier(unifier, firstInterestingVariable, nrInterestingVariables);
  //
  //	See if this unifier can evict an existing unifier.
  //
  RetainedUnifierList::iterator i = mostGeneralSoFar.begin();
  while (i != mostGeneralSoFar.end())
    {
      RetainedUnifierList::iterator next = i;
      ++next;

      RetainedUnifier* potentialVictim = *i;
      if (subsumes(newUnifier, potentialVictim->unifier))
	{
	  DebugAdvisory("new unifier evicted an older unifier");
	  delete potentialVictim;
	  mostGeneralSoFar.erase(i);
	}

      i = next;
    }
  //
  //	Add to the mostGeneralSoFar collection of unifiers.
  //
  newUnifier->positionIndex = positionIndex;
  newUnifier->equationIndex = equationIndex;
  mostGeneralSoFar.push_back(newUnifier);
  //cout << "nr unifiers = " << mostGeneralSoFar.size() << ' ';
}

bool
UnifierFilter::getNextSurvivingUnifier(Substitution*& unifier, int& positionIndex, int& equationIndex)
{
  if (startedExtractingUnifiers)
    ++nextUnifier;
  else
    {
      startedExtractingUnifiers = true;
      nextUnifier = mostGeneralSoFar.begin();
    }

  if (nextUnifier == mostGeneralSoFar.end())
    return false;

  RetainedUnifier* r = *nextUnifier;
  unifier = &(r->unifier);
  positionIndex = r->positionIndex;
  equationIndex = r->equationIndex;
  return true;
}

bool
UnifierFilter::subsumes(const RetainedUnifier* retainedUnifier, const Substitution& unifier)
{
  MemoryCell::okToCollectGarbage();  // otherwise we have huge accumulation of junk from matching
  //
  //	We check if retained unifier is at least as general as unifier on variables of interest.
  //
  int nrVariablesToUse = retainedUnifier->nrVariablesInBindings;
  int nrSlotsToAllocate = nrVariablesToUse;
  if (nrSlotsToAllocate == 0)
    nrSlotsToAllocate = 1;  // substitutions subject to clear() must always have at least one slot
  RewritingContext matcher(nrSlotsToAllocate);
  SubproblemAccumulator subproblems;

  matcher.clear(nrVariablesToUse);
  for (int i = 0; i < nrInterestingVariables; ++i)
    {
      Subproblem* subproblem;

      DebugAdvisory("Considering interesting variable " << i);
      DebugAdvisory("Pattern has " << retainedUnifier->unifier.value(firstInterestingVariable + i));
      DebugAdvisory("Pattern term is " << retainedUnifier->interestingBindings[i]);
      DebugAdvisory("Subject has " << unifier.value(firstInterestingVariable + i));

      if (retainedUnifier->matchingAutomata[i]->match(unifier.value(firstInterestingVariable + i), matcher, subproblem))
	subproblems.add(subproblem);
      else
	{
	  DebugAdvisory("non subsumption declared on variable " << i);
	  return false;
	}
    }
  Subproblem* final = subproblems.extractSubproblem();
  if (final == 0)
    {
      DebugAdvisory("no subproblem so declaring subsumption");
      return true;
    }
  bool result = final->solve(true, matcher);
  DebugAdvisory("solve result = " << result);
  delete final;
  return result;
}

UnifierFilter::RetainedUnifier::RetainedUnifier(const Substitution& original,
						int firstInterestingVariable,
						int nrInterestingVariables)
  : unifier(original.nrFragileBindings()),
    interestingBindings(nrInterestingVariables),
    matchingAutomata(nrInterestingVariables)
{
  unifier.clone(original);
  //
  //	Convert bindings of interesting variables to terms, index their variables, fill in sort information and look for
  //	potential collapses.
  //
  VariableInfo variableInfo;  // does this need to be retained?
  for (int i = 0; i < nrInterestingVariables; ++i)
    {
      DagNode* d = unifier.value(firstInterestingVariable + i);
      Term* t = d->symbol()->termify(d);
      t = t->normalize(true);  // needed even though we're in normal form, in order to set hash value

      t->indexVariables(variableInfo); 
      t->symbol()->fillInSortInfo(t);
      t->analyseCollapses();
      interestingBindings[i] = t;
    }
  //
  //	Now go through them again, inserting context variable information, inserting abstraction variables where needed to
  //	handle potential collapses and compiling.
  //
  NatSet boundUniquely;
  bool subproblemLikely;
  for (int i = 0; i < nrInterestingVariables; ++i)
    {
      Term* t = interestingBindings[i];
      for (int j = 0; j < nrInterestingVariables; ++j)
	{
	  if (j != i)
	    t->addContextVariables(interestingBindings[j]->occursBelow());  // variables from other bindings are in our context
	}
      t->determineContextVariables();
      t->insertAbstractionVariables(variableInfo);
      
      DebugAdvisory("Compiling " << t);
      matchingAutomata[i] = t->compileLhs(false, variableInfo, boundUniquely, subproblemLikely);
      //matchingAutomata[i]->dump(cerr, variableInfo);
    }

  nrVariablesInBindings = variableInfo.getNrProtectedVariables();  // may also have some abstraction variables
  DebugAdvisory("compiled retained unifier has  " << variableInfo.getNrRealVariables() << " real variables");
  DebugAdvisory("compiled retained unifier has  " << variableInfo.getNrProtectedVariables() << " protected variables");
}

UnifierFilter::RetainedUnifier::~RetainedUnifier()
{
  int nrInterestingVariables = interestingBindings.size();
  for (int i = 0; i < nrInterestingVariables; ++i)
    {
      delete matchingAutomata[i];
      interestingBindings[i]->deepSelfDestruct();
    }
}
