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
//      Implementation for matching where ACU top symbol collapses.
//

bool
ACU_LhsAutomaton::uniqueCollapseMatch(DagNode* subject,
				      Substitution& solution,
				      Subproblem*& returnedSubproblem,
				      ExtensionInfo* extensionInfo)
{
  //
  //	Because one subterm cannot take the identity element we can only
  //	collapse to that subterm (which has multiplicity 1).
  //	Every other subterm must be a variable that can take identity and
  //	it either must be already bound to identity or is now bound to identity.
  //
  SubproblemAccumulator subproblems;
  int nrTopVariables = topVariables.length();
  Assert(nrTopVariables >= 1, "not enough top variables to collapse");
  for (int i = 0; i < nrTopVariables; i++)
    {
      TopVariable& tv = topVariables[i];
      if (tv.takeIdentity)
	{
	  DagNode* d = solution.value(tv.index);
	  if (d == 0)
	    solution.bind(tv.index, topSymbol->getIdentityDag());
	  else
	    {
	      if(!(topSymbol->getIdentity()->equal(d)))
		return false;
	    }
	  if (tv.abstracted != 0)
	    {
	      Subproblem* subproblem;
	      if(!(tv.abstracted->match(solution.value(tv.index), solution, subproblem)))
		return false;
	      subproblems.add(subproblem);
	    }				      
	}
      else
	{
	  Assert(tv.multiplicity == 1,
		 "variable with multiplicity > 1 cannot take identity");
	}
    }
  Subproblem* subproblem;
  if (!(uniqueCollapseAutomaton->match(subject,solution, subproblem, extensionInfo)))
    return false;
  subproblems.add(subproblem);
  returnedSubproblem = subproblems.extractSubproblem();
  return true;
}

void
ACU_LhsAutomaton::bindUnboundVariablesToIdentity(Substitution& solution, int exception)
{
  int nrTopVariables = topVariables.length();
  for (int i = 0; i < nrTopVariables; i++)
    {
      if (i != exception)
	{
	  int vi = topVariables[i].index;
	  if (solution.value(vi) == 0)
	    solution.bind(vi, topSymbol->getIdentityDag());
	}
    }
}

bool
ACU_LhsAutomaton::multiwayCollapseMatch(DagNode* subject,
					Substitution& solution,
					Subproblem*& returnedSubproblem,
					ExtensionInfo* extensionInfo)
{
  Term* identity = topSymbol->getIdentity();
  int matchingVariable = NONE;
  int lastViableVariable = NONE;
  int nrViableVariables = 0;
  int nrTopVariables = topVariables.length();
  for (int i = 0; i < nrTopVariables; i++)
    {
      TopVariable& tv = topVariables[i];
      Assert(tv.takeIdentity, "variable can't take identity");
      DagNode* d = solution.value(tv.index);
      if (d != 0)
	{
	  if (!(identity->equal(d)))
	    {
	      if (matchingVariable != NONE || tv.multiplicity > 1)
		return false;
	      matchingVariable = i;
	    }
	}
      else
	{
	  if (tv.multiplicity == 1)
	    {
	      lastViableVariable = i;
	      ++nrViableVariables;
	    }
	  else
	    solution.bind(tv.index, topSymbol->getIdentityDag());
	}
    }
  if (matchingVariable != NONE)
    {
      //
      //	Exactly one variable was bound to something other than our identity.
      //
      TopVariable& mv = topVariables[matchingVariable];
      bool r = subject->matchVariable(mv.index,
				      mv.sort,
				      matchAtTop,
				      solution,
				      returnedSubproblem,
				      extensionInfo);
      if (r)
	bindUnboundVariablesToIdentity(solution, NONE);
      return r;
    }
  //
  //	At this point all variables are either (a) bound to our identity; or
  //	(b) unbound and have multiplicity 1 (viable).
  //
  if (identity->equal(subject))
    {
      //
      //	Subject is our identity; we must still deal with alien extension.
      //
      returnedSubproblem = 0;
      bindUnboundVariablesToIdentity(solution, NONE);
      if (extensionInfo != 0)
	{
	  extensionInfo->setValidAfterMatch(true);
	  extensionInfo->setMatchedWhole(true);
	}
      return true;
    }
  if (nrViableVariables == 0)
    {
      //
      //	All variables are bound to our identity and the subject is not
      //	our identity. We can succeed only if we have entension and the
      //	subject contains our identity.
      //
      if (extensionInfo == 0)
	return false;
      TopVariable& tv = topVariables[0];  // pick any since they are all bound to identity
      Assert(identity->equal(solution.value(tv.index)), "should be identity");
      return subject->matchVariable(tv.index,
				    tv.sort,
				    matchAtTop,
				    solution,
				    returnedSubproblem,
				    extensionInfo);
    }
  if (nrViableVariables == 1)
    {
      //
      //	All variables except one are bound to our identity and the
      //	subject is not our identity. The last viable variable must match
      //	the subject (with extension if present).
      //	This case need to make good on our constraint propagation
      //	guarantee when we have no extension.
      //
      TopVariable& vv = topVariables[lastViableVariable];
      return subject->matchVariable(vv.index,
				    vv.sort,
				    matchAtTop,
				    solution,
				    returnedSubproblem,
				    extensionInfo);
    }
  //
  //	General case: multiple unbound variables that can take identity and
  //	have multiplicity 1. Any bound variables are already bound to identity.
  //
  DisjunctiveSubproblemAccumulator alternatives(solution);
  for (int i = 0; i < nrTopVariables; i++)
    {
      TopVariable& tv = topVariables[i];
      if (solution.value(tv.index) == 0)
	{
	  Assert(tv.multiplicity == 1, "unbound variable of multiplicity > 1");
	  local.copy(solution);
	  bindUnboundVariablesToIdentity(local, i);
	  Subproblem* subproblem;
	  if (subject->matchVariable(tv.index, tv.sort, matchAtTop,
				     local, subproblem, extensionInfo))
	    {
	      if (!(alternatives.empty()) && extensionInfo != 0)
		{
		  //
		  //	Need to exclude the case where subject = id + ext and
		  //	we match our chosen variable against our identity
		  //	in all but the first branch to avoid duplicate solutions.
		  //
		  SubproblemAccumulator subproblems;
		  subproblems.add(subproblem);
		  subproblems.add(new EqualitySubproblem(identity, tv.index, false));
		  subproblem = subproblems.extractSubproblem();
		}
	      alternatives.addOption(local, subproblem, extensionInfo);
	      continue;
	    }
	}
    }
  return alternatives.extract(solution, returnedSubproblem, extensionInfo);
}

bool
ACU_LhsAutomaton::collapseMatch(DagNode* subject,
				Substitution& solution,
				Subproblem*& returnedSubproblem,
				ExtensionInfo* extensionInfo)
{
  if (uniqueCollapseAutomaton != 0)
    return uniqueCollapseMatch(subject, solution, returnedSubproblem, extensionInfo);
  Subproblem* subproblem;
  if (!multiwayCollapseMatch(subject, solution, subproblem, extensionInfo))
    return false;
  SubproblemAccumulator subproblems;
  subproblems.add(subproblem);
  int nrVariables = solution.nrFragileBindings();
  int nrTopVariables = topVariables.length();
  for (int i = 0; i < nrTopVariables; i++)
    {
      TopVariable& tv = topVariables[i];
      if (tv.abstracted != 0)
	{
	  subproblems.add(new VariableAbstractionSubproblem(tv.abstracted,
							    tv.index,
							    nrVariables));
	}
    }
  returnedSubproblem = subproblems.extractSubproblem();
  return true;
}
