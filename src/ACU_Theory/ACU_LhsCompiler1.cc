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
//	Implementation of match compilation for AC and ACU theories.
//	Part 1: constraint propagation analysis, easy cases and
//	weak constraint propagation.
//

struct ACU_Term::CP_Sequence
{
  Vector<int> sequence;
  int nrIndependent;
  NatSet bound;
  int cardinality;
};

void
ACU_Term::analyseConstraintPropagation(NatSet& boundUniquely) const
{
  int nrArgs = argArray.length();
  Vector<Pair> nonGroundAliens;
  int lastUnboundVariable = UNDEFINED;
  for (int i = 0; i < nrArgs; i++)
    {
      const Pair& p = argArray[i];
      if (p.abstractionVariableIndex != NONE)
	return;  // abstraction variable kills constraint propagation analysis
      Term* t = p.term;
      if (t->ground())
	continue;  // ignore ground terms
      if (t->willGroundOutMatch(boundUniquely))
	continue;  // ignore terms that will "ground out match"
      VariableTerm* v = dynamic_cast<VariableTerm*>(t);
      if (v != 0)
	{
	  //
	  //	A variable.
	  //
	  //	Two unbound variables or an unbound variable plus an NGA kills
	  //	constraint propagation analysis.
	  //
	  if (lastUnboundVariable != UNDEFINED || nonGroundAliens.size() != 0)
	    return;
	  lastUnboundVariable = v->getIndex();
	}
      else
	{
	  //
	  //	Must be a non-ground alien.
	  //
	  //	An NGA plus a unbound variable kills constraint propagation analysis.
	  //
	  if (lastUnboundVariable != UNDEFINED)
	    return;
	  nonGroundAliens.append(p);
	}
    }
  if (lastUnboundVariable != UNDEFINED)
    {
      Assert(nonGroundAliens.empty(), "ACU_Term::analyseConstraintPropagation() : shouldn't have NGAs");
      //
      //	If the only thing that doesn't ground out is a variable we can
      //	bind it uniquely.
      //
      boundUniquely.insert(lastUnboundVariable);
    }
  else if (!nonGroundAliens.empty())
    {
      //
      //	If the only things that don't ground out are NGAs we can choose a
      //	sequence to propagate constraints on shared variables among them.
      //
      CP_Sequence bestSequence;
      findConstraintPropagationSequence(nonGroundAliens, boundUniquely, bestSequence);
      boundUniquely = bestSequence.bound;  // deep copy
    }
}

//#include "rule.hh" // HACK
void
ACU_Term::compileLhs3(bool matchAtTop,
		      const VariableInfo& variableInfo,
		      NatSet& boundUniquely,
		      bool& subproblemLikely,
		      ACU_LhsAutomaton* automaton)
{
  if (uniqueCollapseSubtermIndex != NONE)
    {
      //
      //	Maybe we should add the variable(s) which will be bound to identity
      //	to this local bound uniquely.
      //
      NatSet local(boundUniquely);
      bool spl;
      automaton->
	addUniqueCollapseAutomaton(argArray[uniqueCollapseSubtermIndex].term->
				   compileLhs(matchAtTop, variableInfo, local, spl));
    }
  //
  //	Insert variables, ground aliens and grounded out aliens
  //	in to automaton and collect non-ground aliens.
  //
  Vector<Pair> nonGroundAliens;
  bool greedy = true;
  int lastUnboundVariable = UNDEFINED;
  int nrUnboundVariables = 0;
  int nrAbstractionVariables = 0;
  int nrArgs = argArray.length();
  //int V_COUNT = 0; // HACK
  for (int i = 0; i < nrArgs; i++)
    {
      Pair& p = argArray[i];
      Term* t = p.term;
      int m = p.multiplicity;
      VariableTerm* v = dynamic_cast<VariableTerm*>(t);
      if (v != 0)
	{
	  int index = v->getIndex();
	  bool bound = boundUniquely.contains(index);
	  automaton->addTopVariable(v, m, bound);
	  if (!bound)
	    {
	      lastUnboundVariable = index;
	      ++nrUnboundVariables;
	      if (t->occursInContext().contains(index) ||
		  variableInfo.getConditionVariables().contains(index))
		greedy = false;
	      //++V_COUNT; // HACK
	    }
	}
      else if (p.abstractionVariableIndex != NONE)
	{
	  NatSet local(boundUniquely);
	  bool spl;
	  automaton->addAbstractionVariable(p.abstractionVariableIndex,
					    t->getComponent()->sort(Sort::ERROR_SORT),
					    p.collapseToOurSymbol ? UNBOUNDED : 1,
					    p.matchOurIdentity,
					    t->compileLhs(false, variableInfo, local, spl),
					    m);
	  ++nrAbstractionVariables;
	  greedy = false;
	}
      else if (t->ground())
	automaton->addGroundAlien(t, m);
      else if (t->willGroundOutMatch(boundUniquely))
	{
	  bool spl;
	  LhsAutomaton* subAutomaton =
	    t->compileLhs(false, variableInfo, boundUniquely, spl);
	  Assert(!spl, "grounded out alien should not return subproblem");
	  automaton->addGroundedOutAlien(t, subAutomaton, m);
	}
      else
	{
	  greedy = greedy && t->greedySafe(variableInfo, boundUniquely);
	  nonGroundAliens.append(argArray[i]);
	} 
    }
  /*
  if (V_COUNT > 2 || matchAtTop && V_COUNT > 1) // HACK
    {
      cout << Tty(Tty::RED) << "Nasty : " << this << Tty(Tty::RESET) << endl;
      const PreEquation* pe = static_cast<const PreEquation*>(&variableInfo);
      if (const Rule* rl = dynamic_cast<const Rule*>(pe))
	cout << rl;
    }
  */
  //
  //	Now decide on a matching strategy
  //
  if (nonGroundAliens.empty())
    {
      //
      //	No aliens case
      //
      subproblemLikely = !greedy;
      ACU_LhsAutomaton::MatchStrategy strategy =
	greedy ? ACU_LhsAutomaton::GREEDY : ACU_LhsAutomaton::FULL;
      if (!matchAtTop)
	{
	  switch (nrUnboundVariables + nrAbstractionVariables)
	    {
	    case 0:
	      strategy = ACU_LhsAutomaton::GROUND_OUT;
	      break;
	    case 1:
	      if (nrUnboundVariables == 1)
		{
		  boundUniquely.insert(lastUnboundVariable);
		  subproblemLikely = false;  // variable forced so red-black ok
		}
	      strategy = ACU_LhsAutomaton::LONE_VARIABLE;
	      break;
	    }
	}
      automaton->complete(strategy, 0);
    }
  else if (!matchAtTop && nrUnboundVariables == 0 && nrAbstractionVariables == 0)
    {
      //
      //	Aliens only case
      //
      compileAliensOnlyCase(automaton, nonGroundAliens, variableInfo, boundUniquely,
			    subproblemLikely);
    }
  else
    {
      //
      //	General case
      //
      subproblemLikely = !greedy;
      compileGreedyAndFullCases(automaton, nonGroundAliens, variableInfo, boundUniquely,
				subproblemLikely);
    }
}

void
ACU_Term::weakConstraintPropagation(const Vector<Pair>& aliens,
				    const NatSet& boundUniquely,
				    int step,
				    Vector<int>& sequence)
{
  int nrAliens = aliens.length();
  //
  //	Reorder sequence[step],...,sequence[nrAliens - 1] to optimize weak constraint
  //	propagation at solve-time.
  //
  //	First we compute (a conservative approximation to) the set of variables
  //	that will be already bound when we select a match of aliens[sequence[step]].term.
  //
  NatSet alreadyBound(boundUniquely);
  for (int i = 0; i < step; i++)
    alreadyBound.insert(aliens[sequence[i]].term->occursBelow());
  //
  //	Determine the set of variables which occur in more than one of
  //	aliens[sequence[step]].term,...,aliens[sequence[nrAliens - 1]].term.
  //
  NatSet shared;
  {
    NatSet seen(aliens[sequence[step]].term->occursBelow());
    for (int i = step + 1; i < nrAliens; i++)
      {
	NatSet result(seen);
	const NatSet& ourVariables = aliens[sequence[i]].term->occursBelow();
	result.intersect(ourVariables);
	shared.insert(result);
	seen.insert(ourVariables);
      }
  }
  if (shared.empty())
    return;  // if no shared variables, no point in reordering sequence
  //
  //	Now sequence aliens to bind unbound shared variables as quickly
  //	as possible in order to minimize search space.
  //
  for (int i = step; i < nrAliens - 1; i++)
    {
      int best = UNDEFINED;
      int bestCardinality = 0;
      for (int j = i; j < nrAliens; j++)
	{
	  NatSet willBindShared(aliens[sequence[j]].term->occursBelow());
	  willBindShared.intersect(shared);
	  willBindShared.subtract(alreadyBound);
	  int cardinality = willBindShared.cardinality();
	  if (cardinality > bestCardinality)
	    {
	      best = j;
	      bestCardinality = cardinality;
	    }
	}
      if (best == UNDEFINED)
	break;
      swap(sequence[i], sequence[best]);
      alreadyBound.insert(aliens[sequence[i]].term->occursBelow());
    }
}

