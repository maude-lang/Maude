/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class PendingUnificationStack.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "unificationSubproblem.hh"

//	core class definitions
#include "unificationContext.hh"
#include "unificationSubproblemDisjunction.hh"
#include "compoundCycleSubproblem.hh"
#include "pendingUnificationStack.hh"

//	variable class definitions
#include "variableDagNode.hh"

PendingUnificationStack::PendingUnificationStack()
  : theoryTable(1)
{
  //
  //	The first theory is has a null controlling symbol and is used for
  //	keeping track of theory clashes where both symbols claim to
  //	be able to resolve theory clashes.
  //
  //	We want to solve these problems early.
  //
  theoryTable[0].controllingSymbol = 0;
  theoryTable[0].firstProblemInTheory = NONE;
}

PendingUnificationStack::~PendingUnificationStack()
{
  //
  //	Necessary to avoid memory leaks, and more importantly stale root pointers.
  //
  FOR_EACH_CONST(i, Vector<ActiveSubproblem>, subproblemStack)
    delete i->subproblem;
}

void
PendingUnificationStack::markReachableNodes()
{
  FOR_EACH_CONST(i, Vector<PendingUnification>, unificationStack)
    {
      i->lhs->mark();
      i->rhs->mark();
    }
}

void
PendingUnificationStack::push(Symbol* controllingSymbol, DagNode* lhs, DagNode* rhs, bool marked)
{
  DebugAdvisory("push " << controllingSymbol << ": " << lhs << " =? " << rhs << "  marked = " << marked);

  int e = unificationStack.size();
  unificationStack.resize(e + 1);
  PendingUnification& p = unificationStack[e];
  p.lhs = lhs;
  p.rhs = rhs;
  p.marked = marked;
  //
  //	We don't expect to handle too many distinct theories in a unification problem so
  //	we use a simple linear search.
  //
  int nrTheories = theoryTable.size();
  for (int i = 0; i < nrTheories; ++i)
    {
      if (theoryTable[i].controllingSymbol == controllingSymbol)
	{
	  p.theoryIndex = i;
	  p.nextProblemInTheory = theoryTable[i].firstProblemInTheory;
	  theoryTable[i].firstProblemInTheory = e;
	  return;
	}
    }
  //
  //	Haven't seen this theory before so make a new entry in the table.
  //
  theoryTable.resize(nrTheories + 1);
  p.theoryIndex = nrTheories;
  p.nextProblemInTheory = NONE;
  theoryTable[nrTheories].controllingSymbol = controllingSymbol;
  theoryTable[nrTheories].firstProblemInTheory = e;
}

bool
PendingUnificationStack::resolveTheoryClash(DagNode* lhs, DagNode* rhs)
{
  Symbol* controllingSymbol = lhs->symbol();
  if (controllingSymbol->canResolveTheoryClash())
    {
      if (rhs->symbol()->canResolveTheoryClash())
	{
	  //
	  //	Both symbols can might be able to resolve the theory clash
	  //	so we will need to try both.
	  //
	  controllingSymbol = 0;
	}
    }
  else
    {
      controllingSymbol = rhs->symbol();
      if (!(controllingSymbol->canResolveTheoryClash()))
	return false;  // unresolvable - fail
      swap(lhs, rhs);
    }
  DebugAdvisory("push to resolve theory clash");
  push(controllingSymbol, lhs, rhs, true);
  return true;
}

void
PendingUnificationStack::restore(Marker mark)
{
  //
  //	We need to blow away all unifications up to and including unificationStack[mark].
  //
  for (int i = unificationStack.size() - 1; i >= mark; --i)
    {
      PendingUnification& p = unificationStack[i];
      Assert(theoryTable[p.theoryIndex].firstProblemInTheory == i,
	     "trying to retract a unification which is not the first problem in its theory " <<
	     i << ' ' << p.lhs << " =? " <<  p.rhs << " first problem is " <<
	     theoryTable[p.theoryIndex].firstProblemInTheory);
      theoryTable[p.theoryIndex].firstProblemInTheory = p.nextProblemInTheory;
    }
  unificationStack.resize(mark);
}

bool
PendingUnificationStack::solve(bool findFirst, UnificationContext& solution)
{
  DebugAdvisory("PendingUnificationStack::solve() findFirst = " << findFirst);
#ifndef NO_ASSERT
  //  dump(cerr);
#endif
  if (findFirst ? makeNewSubproblem(solution) : !(subproblemStack.empty()))
    {
      for (;;)
	{
	  findFirst = subproblemStack[subproblemStack.size() - 1].subproblem->solve(findFirst, solution, *this);
	  if (findFirst)
	    {
	      if (!makeNewSubproblem(solution))
		break;  // all done
	    }
	  else
	    {
	      killTopSubproblem();
	      if (subproblemStack.empty())
		break;  // out of alternatives
	    }
	}
    }
  return findFirst;
}

int
PendingUnificationStack::chooseTheoryToSolve()
{
  //
  //	Chose a theory with unsolved unifications and return its index in the theory table.
  //
  int priority = INT_MAX;
  int theoryIndex = NONE;

  int nrTheories = theoryTable.size();
  for (int i = 0; i < nrTheories; ++i)
    {
      if (theoryTable[i].firstProblemInTheory != NONE)
	{
	  Symbol* s = theoryTable[i].controllingSymbol;
	  if (s == 0)
	    return i;  // prioritize disjunctions
	  //
	  //	Otherwise we want the theory with the lowest unification priority number.
	  //
	  int p = s->unificationPriority();
	  if (p < priority)
	    {
	      theoryIndex = i;
	      priority = p;
	    }
	}
    }
  return theoryIndex;
}

bool
PendingUnificationStack::makeNewSubproblem(UnificationContext& solution)
{
  //
  //	Try to make a new unification subproblem.
  //
  //	Returns true if a new unification subproblem made and false
  //	if there are no unresolved unification problems in the stack.
  //
  DebugAdvisory("makeNewSubproblem()");
#ifndef NO_ASSERT
  //  dump(cerr);
#endif
  //
  //    Find a theory with unsolved unifications and put all of its unsolved unifications
  //    into a new active subproblem.
  //
  int i = chooseTheoryToSolve();
  if (i != NONE)
    {
      int j = theoryTable[i].firstProblemInTheory;
      Symbol* controllingSymbol = theoryTable[i].controllingSymbol;
      DebugAdvisory("makeNewSubproblem() making subproblem for controlling symbol " <<
		    controllingSymbol);
      UnificationSubproblem* sp = (controllingSymbol == 0) ? new UnificationSubproblemDisjunction() :
	controllingSymbol->makeUnificationSubproblem();
      do
	{
	  PendingUnification& p = unificationStack[j];
	  sp->addUnification(p.lhs, p.rhs, p.marked, solution);
	  j = p.nextProblemInTheory;
	}
      while (j != NONE);
      int nrSubproblems = subproblemStack.size();
      subproblemStack.resize(nrSubproblems + 1);
      ActiveSubproblem& a = subproblemStack[nrSubproblems];
      a.theoryIndex = i;
      a.savedFirstProblem = theoryTable[i].firstProblemInTheory;
      a.subproblem = sp;
      theoryTable[i].firstProblemInTheory = NONE;
      return true;
    }
  //
  //	All unification problems solved - now check for compound cycles.
  //
  int cycleStart = findCycle(solution);
  if (cycleStart == NONE)
    {
      //
      //	We're done so instantatiate bound variables.
      //
      FOR_EACH_CONST(i, Vector<int>, variableOrder)
	{
	  if (DagNode* d = solution.value(*i)->instantiate(solution))
	    solution.bind(*i, d);
	}
      return false;  // no more subproblems to be solved
    }
  //
  //	Create a new compound cycle subproblem.
  //
  DebugAdvisory("Cycle found");
  CompoundCycleSubproblem* sp = new CompoundCycleSubproblem();
  for (int i = cycleStart;;)
    {
      DagNode* value = solution.value(i);
      if (dynamic_cast<VariableDagNode*>(value) == 0)
	sp->addComponent(i);
      else
	DebugAdvisory("ignoring a variable->variable binding:");
      DebugAdvisory(static_cast<DagNode*>(solution.getVariableDagNode(i)) << " <- " << value);
      i = variableStatus[i];
      if (i == cycleStart)
	break;
    }
  //
  //	Push the compound cycle subproblem on the stack.
  //
  int nrSubproblems = subproblemStack.size();
  subproblemStack.resize(nrSubproblems + 1);
  ActiveSubproblem& a = subproblemStack[nrSubproblems];
  a.theoryIndex = COMPOUND_CYCLE;
  a.savedFirstProblem = NONE;
  a.subproblem = sp;
  return true;
}

void
PendingUnificationStack::killTopSubproblem()
{
  DebugAdvisory("killTopSubproblem()");
#ifndef NO_ASSERT
  //  dump(cerr);
#endif
  //
  //	Kill the top element of the active subproblem stack, and link the unifications it was
  //	solving back into the theory table as unsolved.
  //
  int i = subproblemStack.size() - 1;
  ActiveSubproblem& a = subproblemStack[i];
  delete a.subproblem;
  if (a.theoryIndex != COMPOUND_CYCLE)
    {
      Assert(theoryTable[a.theoryIndex].firstProblemInTheory == NONE,
	     "newer unification problems have not been retracted");
      theoryTable[a.theoryIndex].firstProblemInTheory = a.savedFirstProblem;
    }
  subproblemStack.resize(i);
}

int
PendingUnificationStack::findCycle(UnificationContext& solution)
{
  //
  //	This includes all the original variables together with
  //	fresh variables we added during unification.
  //
  int nrVariables = solution.nrFragileBindings();
  variableStatus.resize(nrVariables);
  for (int i = 0; i < nrVariables; ++i)
    variableStatus[i] = UNEXPLORED;
  //
  //	We look for a dependency cycle reachable from original variables.
  //	If we fail, to find a cycle we generate an instantiation ordering.
  //
  variableOrder.clear();
  int nrOriginalVariables = solution.getNrOriginalVariables();
  for (int i = 0; i < nrOriginalVariables; ++i)
    {
      int cycleStart = findCycleFrom(i, solution);
      if (cycleStart != NONE)
	return cycleStart;
    }
  return NONE;
}

int
PendingUnificationStack::findCycleFrom(int index, UnificationContext& solution)
{
  int status = variableStatus[index];
  if (status == UNEXPLORED)
    {
      DagNode* d = solution.value(index);
      if (d == 0)
	{
	  variableStatus[index] = EXPLORED;
	  return NONE;
	}
      NatSet occurs;
      d->insertVariables(occurs);
      FOR_EACH_CONST(i, NatSet, occurs)
	{
	  variableStatus[index] = *i;
	  int cycleStart = findCycleFrom(*i, solution);
	  if (cycleStart != NONE)
	    return cycleStart;
	}
      variableStatus[index] = EXPLORED;
      variableOrder.append(index);
    }
  else if (status != EXPLORED)
    {
      //
      //	We hit index while we were exploring it - we must have a cycle.
      //
      return index;
    }
  return NONE;
}

void
PendingUnificationStack::dump(ostream& s)
{
  s << "\n---theoryTable: index, controllingSymbol, firstProblemInTheory---\n";
  int nrTheories = theoryTable.size();
  for (int i = 0; i < nrTheories; ++i)
    s << i << '\t' << theoryTable[i].controllingSymbol << '\t' << theoryTable[i].firstProblemInTheory << '\n';

  s << "---unificationStack: index, theoryIndex, nextProblemInTheory, unification---\n";
  int nrUnifications = unificationStack.size();
  for (int i = 0; i < nrUnifications; ++i)
    {
      s << i << '\t' << unificationStack[i].theoryIndex << '\t' << unificationStack[i].nextProblemInTheory <<
	'\t' << unificationStack[i].lhs << " =? " << unificationStack[i].rhs << '\n';
    }

  s << "---subproblemStack: index, theoryIndex, savedFirstProblem, subproblem ptr---\n";
  int nrSubproblems = subproblemStack.size();
  for (int i = 0; i < nrSubproblems; ++i)
    {
      s << i << '\t' << subproblemStack[i].theoryIndex << '\t' << subproblemStack[i].savedFirstProblem <<
	'\t' << ((void*) subproblemStack[i].subproblem) << '\n';
    }
  s << endl;
}
