/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class CompoundCycleSubproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//      interface class definitions
#include "dagNode.hh"
#include "binarySymbol.hh"

//      core class definitions
#include "unificationContext.hh"
#include "compoundCycleSubproblem.hh"

//      variable class definitions
#include "variableDagNode.hh"

void
CompoundCycleSubproblem::markReachableNodes()
{
  int nrFragile = preBreakSubstitution.nrFragileBindings();
  for (int i = 0; i < nrFragile; i++)
    {
      DagNode* d = preBreakSubstitution.value(i);
      if (d != 0)
	  d->mark();
    }
}

bool
CompoundCycleSubproblem::solve(bool findFirst,
			       UnificationContext& solution,
			       PendingUnificationStack& pending)
{
  //DebugAlways("findFirst = " << findFirst);
  if (findFirst)
    {
      preBreakSubstitution.clone(solution);
      preBreakPendingState = pending.checkPoint();
      currentEdgeIndex = 0;
    }
  else
    {
      solution.clone(preBreakSubstitution);
      pending.restore(preBreakPendingState);
    }
  //
  //	Find an edge whose top symbol can resolve to a proper subterm and
  //	create a restricted unification problem.
  //	With nonregular theories it is easy to break a cycle by resolving
  //	a single edge. We don't currently support nonregular theories but
  //	we do support arbitrary ground terms as identities so we can break
  //	a cycle by collapsing a single edge in a situation such as this:
  //
  //	fmod FOO is
  //	  sort Foo .
  //	  op f : Foo Foo -> Foo [id: g(a)] .
  //	  op g : Foo -> Foo .
  //	  op a : -> Foo .
  //	  vars W X Y Z : Foo .
  //	endfm
  //
  //	unify X =? f(Y, Z) /\ Z =? g(X) .
  //
  int nrEdges = cycle.size();
  while (currentEdgeIndex < nrEdges)
    {
      int variableIndex = cycle[currentEdgeIndex];
      DagNode* variable = solution.getVariableDagNode(variableIndex);
      DagNode* assignment = solution.value(variableIndex);
      //DebugAlways("trying to collapse " << variable << " <- " << assignment);
      ++currentEdgeIndex;
      Symbol* controllingSymbol = assignment->symbol();
      if (controllingSymbol->canResolveTheoryClash())
	{
	  pending.push(controllingSymbol, assignment, variable, true);
	  //
	  //	Because we just put the binding back as a unification problem
	  //	it is a safe optimization to remove the binding from our current
	  //	solution.
	  //
	  solution.bind(variableIndex, 0);
	  return true;
	}
    }
  //
  //	Now we look for an edge whose variable can be eliminated by unifying it
  //	against a "cyclic" identity.
  //	We do this to catch a pathological corner case when we have multiple symbols
  //	with identities in other symbol's theories. For example:
  //
  //	fmod FOO is
  //	  sort Foo .
  //	  op f : Foo Foo -> Foo [assoc comm id: g(a, b)] .
  //	  op g : Foo Foo -> Foo [assoc comm id: f(c, d)] .
  //	  ops a b c d : -> Foo .
  //	  vars W X Y Z : Foo .
  //	endfm
  //
  //	unify X =? f(c, d, Y) /\ Y =? g(a, b, X) .
  //
  //	Here, neither rhs can collapse to an alien subterm (the case handled above)
  //	but a solution is still possible by solving Y =? g(a, b)
  //	Even more subtle cases can be constructed using an otherwise inert symbol;
  //	for example:
  //
  //	fmod FOO is
  //	  sort Foo .
  //	  op f : Foo Foo -> Foo [assoc comm id: h(g(a, b))] .
  //	  op g : Foo Foo -> Foo [assoc comm id: f(c, d)] .
  //	  op h : Foo -> Foo .
  //	  ops a b c d : -> Foo .
  //	  vars W X Y Z : Foo .
  //	endfm
  //
  //	unify X =? f(c, d, Y)  /\ Y =? h(Z) /\ Z =? g(a, b, X).
  //
  //	Here again there are no collapses to an alien subterm and h(Z) is inert.
  //	But solving X =? f(c, d) yields a solution.
  //	It is for this reason that the cyclic identity finding code looks for
  //	cycles through subterms and well as direct cycles.
  //	These cases are so artificial case that we don't care about efficiency at all.
  //
  while (currentEdgeIndex < 2 * nrEdges)
    {
      int variableIndex = cycle[currentEdgeIndex % nrEdges];
      DagNode* assignment = solution.value(variableIndex);
      ++currentEdgeIndex;
      int nextVariableIndex = cycle[currentEdgeIndex % nrEdges];  // wrap around caused by %
      DagNode* nextVariable = solution.getVariableDagNode(nextVariableIndex);

      Symbol* controllingSymbol = assignment->symbol();
      if (BinarySymbol* bs = dynamic_cast<BinarySymbol*>(controllingSymbol))
	{
	  DebugAdvisory("checking " << bs << " for cyclic identity");
	  if (bs->hasCyclicIdentity())
	    {
	      DagNode* d = bs->getIdentityDag();
	      //
	      //	If this is the first time we use the identity element it is possible
	      //	that it will not have its sort computed or ground flag set.
	      //
	      if (!(d->isGround()))
		d->computeBaseSortForGroundSubterms(false);
	      DebugAdvisory("trying " << d << " vs " << nextVariable);
	      if (d->computeSolvedForm(nextVariable, solution, pending))
		return true;
	    }
	}
    }
#if 0
  //
  //	Now we look for an edge whose variable can be eliminated using Boudet's variable
  //	elimination technique.
  //	Because we don't currently support nonregular theories this isn't needed.
  //	We don't support variable elimination either. But we include the code just
  //	in case we support these things in the future.
  //
  while (currentEdgeIndex < 3 * nrEdges)
    {
      int variableIndex = cycle[currentEdgeIndex % nrEdges];
      DagNode* subject = solution.value(variableIndex);
      ++currentEdgeIndex;
      int nextVariableIndex = cycle[currentEdgeIndex % nrEdges];
      DagNode* nextVariable = solution.getVariableDagNode(nextVariableIndex);
      if (subject->symbol()->eliminateVariable(subject, nextVariable, solution, pending))
	return true;
    }
#endif
  return false;
}
