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
//      Implementation for matching where AU top symbol collapses.
//

bool
AU_LhsAutomaton::uniqueCollapseMatch(DagNode* subject,
				     Substitution& solution,
				     Subproblem*& returnedSubproblem,
				     ExtensionInfo* extensionInfo)
{
  //
  //	Because one subterm cannot take the identity element we can only
  //	collapse to that subterm.
  //	Every other subterm must be a variable that can take identity and
  //	it either must be already bound to identity or is now bound to identity.
  //
  Term* identity = topSymbol->getIdentity();
  int rigidLength = rigidPart.length();
  for (int i = 0; i < rigidLength; i++)
    {
      Subterm& r = rigidPart[i];
      if (r.type == VARIABLE && r.variable.takeIdentity)
	{
	  DagNode* d = solution.value(r.variable.index);
	  Assert(d != 0, "unbound variable that can take identity in rigid part");
	  if (!(identity->equal(d)))
	    return false;
	}
    }      
  SubproblemAccumulator subproblems;
  int flexLength = flexPart.length();
  for (int i = 0; i < flexLength; i++)
    {
      Subterm& f = flexPart[i];
      if (f.type == VARIABLE && f.variable.takeIdentity)
	{
	  //
	  //	BUG: If f is an awkward variable or awkward abstraction variable
	  //	then it could be our uniqueCollapseSubterm even though
	  //	f.variable.takeIdentity is true. Need to find a clean way of
	  //	handling awkward variables.
	  //
	  int index = f.variable.index;
	  DagNode* d = solution.value(index);
	  if (d == 0)
	    solution.bind(index, topSymbol->getIdentityDag());
	  else
	    {
	      if(!(identity->equal(d)))
		return false;
	    }
	  if (f.variable.abstracted != 0)
	    {
	      Subproblem* subproblem;
	      if(!(f.variable.abstracted->match(solution.value(index), solution, subproblem)))
		return false;
	      subproblems.add(subproblem);
	    }				      
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
AU_LhsAutomaton::bindUnboundVariablesToIdentity(Substitution& solution, int exception)
{
  int nrFlexVariables = flexPart.length();
  for (int i = 0; i < nrFlexVariables; i++)
    {
      Assert(flexPart[i].type == VARIABLE && flexPart[i].variable.takeIdentity,
	     "non variable in multiway collapse");
      if (i != exception)
	{
	  int index = flexPart[i].variable.index;
	  if (solution.value(index) == 0)
	    solution.bind(index, topSymbol->getIdentityDag());
	}
    }
}

bool
AU_LhsAutomaton::multiwayCollapseMatch(DagNode* subject,
				       Substitution& solution,
				       Subproblem*& returnedSubproblem,
				       ExtensionInfo* extensionInfo)
{
  Term* identity = topSymbol->getIdentity();
  int matchingVariable = NONE;
  bool rigidMatchingVariable = false;
  int rigidLength = rigidPart.length();
  for (int i = 0; i < rigidLength; i++)
    {
      Assert(rigidPart[i].type == VARIABLE, "non variable in multiway collapse");
      TopVariable& tv = rigidPart[i].variable;
      Assert(tv.takeIdentity, "variable can't take identity");
      DagNode* d = solution.value(tv.index);
      Assert(d != 0, "unbound variable that can take identity in rigid part");
      if (!(identity->equal(d)))
	{
	  if (matchingVariable != NONE)  // repeated variable fails 2nd time around
	    return false;
	  matchingVariable = i;
	  rigidMatchingVariable = true;
	}
    }
  int lastViableVariable = NONE;
  int nrViableVariables = 0;
  int nrFlexVariables = flexPart.length();
  for (int i = 0; i < nrFlexVariables; i++)
    {
      Assert(flexPart[i].type == VARIABLE, "non variable in multiway collapse");
      TopVariable& tv = flexPart[i].variable;
      Assert(tv.takeIdentity, "variable can't take identity");
      DagNode* d = solution.value(tv.index);
      if (d != 0)
	{
	  if (!(identity->equal(d)))
	    {
	      if (matchingVariable != NONE)  // repeated variable fails 2nd time around
		return false;
	      matchingVariable = i;
	    }
	}
      else
	{
	  //
	  //	If variable is repeated under AU symbol then we will have
	  //	nrViableVariables >= 2 and we will get failure when we try to
	  //	match repeated variable against non-identity because it
	  //	will already be bound to identity by other instance. Somewhat
	  //	inefficient especially if alien extension is involved but
	  //	this is a rare case.
	  //
	  lastViableVariable = i;
	  ++nrViableVariables;
	}
    }
  if (matchingVariable != NONE)
    {
      //
      //	Exactly one variable was bound to something other than our identity.
      //
      TopVariable& mv = rigidMatchingVariable ?
	rigidPart[matchingVariable].variable : flexPart[matchingVariable].variable;
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
  //	At this point all variables are either:
  //	(a) bound to our identity; or
  //	(b) unbound.
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
      TopVariable& tv = flexPart[0].variable;  // pick any since they are all bound to identity
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
      TopVariable& vv = flexPart[lastViableVariable].variable;
      return subject->matchVariable(vv.index,
				    vv.sort,
				    matchAtTop,
				    solution,
				    returnedSubproblem,
				    extensionInfo);
    }
  //
  //	General case: multiple unbound variable instances that can take identity.
  //	Any bound variables are already bound to identity.
  //
  DisjunctiveSubproblemAccumulator alternatives(solution);
  for (int i = 0; i < nrFlexVariables; i++)
    {
      TopVariable& tv = flexPart[i].variable;
      if (solution.value(tv.index) == 0)
	{
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
AU_LhsAutomaton::collapseMatch(DagNode* subject,
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
  int nrFlexVariables = flexPart.length();
  for (int i = 0; i < nrFlexVariables; i++)
    {
      Assert(flexPart[i].type == VARIABLE, "non variable in multiway collapse");
      TopVariable& tv = flexPart[i].variable;
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
