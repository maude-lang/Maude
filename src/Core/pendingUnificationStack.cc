/*

    This file is part of the Maude 2 interpreter.

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

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "unificationSubproblem.hh"

//	core class definitions
//#include "freshVariableGenerator.hh"
//#include "connectedComponent.hh"
//#include "unificationContext.hh"
#include "pendingUnificationStack.hh"

PendingUnificationStack::PendingUnificationStack()
{
}

PendingUnificationStack::~PendingUnificationStack()
{
}

void
PendingUnificationStack::push(Symbol* controllingSymbol, DagNode* lhs, DagNode* rhs)
{
  DebugAdvisory("push " << controllingSymbol << ": " << lhs << " =? " << rhs);

  int e = unificationStack.size();
  unificationStack.resize(e + 1);
  PendingUnification& p = unificationStack[e];
  p.lhs = lhs;
  p.rhs = rhs;
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

void
PendingUnificationStack::restore(Marker mark)
{
  //
  //	We need to blow away all unifications upto and including unificationStack[mark].
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
  if (findFirst ? makeNewSubproblem() : !(subproblemStack.empty()))
    {
      for (;;)
	{
	  findFirst = subproblemStack[subproblemStack.size() - 1].subproblem->solve(findFirst, solution, *this);
	  if (findFirst)
	    {
	      if (!makeNewSubproblem())
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

bool
PendingUnificationStack::makeNewSubproblem()
{
  DebugAdvisory("makeNewSubproblem()");
#ifndef NO_ASSERT
  //  dump(cerr);
#endif
#if 1
  //
  //    Find a theory with unsolved unifications and put all of its unsolved unifications
  //    into a new active subproblem.
  //
  int nrTheories = theoryTable.size();
  for (int i = 0; i < nrTheories; ++i)
    {
      int j = theoryTable[i].firstProblemInTheory;
      if (j != NONE)
        {
          UnificationSubproblem* sp = theoryTable[i].controllingSymbol->makeUnificationSubproblem();
          do
            {
              PendingUnification& p = unificationStack[j];
              sp->addUnification(p.lhs, p.rhs);
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
    }
  return false;
#else
  //
  //	Find the theory with the most unsolved unifications.
  //	This doesn't seem beneficial so far so we use the old code in the release build.
  //	
  int chosenTheory = NONE;
  int maxUnsolved = 0;
  int nrTheories = theoryTable.size();
  for (int i = 0; i < nrTheories; ++i)
    {
      int nrUnsolved = 0;
      for (int j = theoryTable[i].firstProblemInTheory; j != NONE; j = unificationStack[j].nextProblemInTheory)
	++nrUnsolved;
      DebugAdvisory("theory " << theoryTable[i].controllingSymbol << " has " << nrUnsolved << " unsolved unifications");
      nrUnsolved *= theoryTable[i].controllingSymbol->unificationPriority();
      if (nrUnsolved > maxUnsolved)
	{
	  if (maxUnsolved > 0)
	    { cerr << "choice situation\n";  dump(cerr); }
	  chosenTheory = i;
	  maxUnsolved = nrUnsolved;
	}
      
    }
  if (maxUnsolved == 0)
    return false;
  //
  //	Put all of its unsolved unifications into a new active subproblem.
  //
  Theory& t = theoryTable[chosenTheory];
  UnificationSubproblem* sp = t.controllingSymbol->makeUnificationSubproblem();
  for (int j = t.firstProblemInTheory; j != NONE;)
    {
      PendingUnification& p = unificationStack[j];
      sp->addUnification(p.lhs, p.rhs);
      j = p.nextProblemInTheory;
    }
  int nrSubproblems = subproblemStack.size();
  subproblemStack.resize(nrSubproblems + 1);
  ActiveSubproblem& a = subproblemStack[nrSubproblems];
  a.theoryIndex = chosenTheory;
  a.savedFirstProblem = t.firstProblemInTheory;
  a.subproblem = sp;
  t.firstProblemInTheory = NONE;
  return true;
#endif
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
  Assert(theoryTable[a.theoryIndex].firstProblemInTheory == NONE,
	 "newer unification problems have not been retracted");
  theoryTable[a.theoryIndex].firstProblemInTheory = a.savedFirstProblem;
  subproblemStack.resize(i);
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
