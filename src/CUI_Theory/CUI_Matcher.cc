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
//	CUI matcher.
//

bool
CUI_LhsAutomaton::match(DagNode* subject,
			Substitution& solution,
			Subproblem*& returnedSubproblem,
			ExtensionInfo* extensionInfo)
{
  DisjunctiveSubproblemAccumulator alternatives(solution);
  Flags f = flags;
  bool greedyMatchOK = f & GREEDY_MATCH_OK;
  if (subject->symbol() == topSymbol)
    {
      Assert(extensionInfo == 0, "shouldn't have extension info");
      CUI_DagNode* s = static_cast<CUI_DagNode*>(subject);
      DagNode* s0 = s->argArray[0];
      DagNode* s1 = s->argArray[1];
      if (f & FORWARD)
	{
	  //
	  //	Try forward non-collapse possibility.
	  //
	  local.copy(solution);
	  if (freeMatch(s0, s1, local, returnedSubproblem))
	    {
	      if ((f & UNIQUE_BRANCH) || (greedyMatchOK && returnedSubproblem == 0))
		{
		  solution.copy(local);
		  return true;
		}
	      alternatives.addOption(local, returnedSubproblem, 0);
	    }
	}
      if ((f & REVERSE) || ((f & CONDITIONAL_REVERSE) && !(s0->equal(s1))))
	{
	  //
	  //	Try reverse non-collapse possibility.
	  //
	  local.copy(solution);
	  if (freeMatch(s1, s0, local, returnedSubproblem))
	    {
	      if ((f & UNIQUE_BRANCH) || (greedyMatchOK && returnedSubproblem == 0))
		{
		  solution.copy(local);
		  return true;
		}
	      alternatives.addOption(local, returnedSubproblem, 0);
	    }
	}
    }
  else
    {
      if (!(f & COLLAPSE))
	return false;
    }
  //
  //	Three ways to collapse.
  //
  if (f & ID0_COLLAPSE)
    {
      local.copy(solution);
      if (id0CollapseMatch(subject, local, returnedSubproblem, extensionInfo))
	{
	  if (greedyMatchOK && returnedSubproblem == 0)
	    {
	      solution.copy(local);
	      return true;
	    }
	  alternatives.addOption(local, returnedSubproblem, extensionInfo);
	}
    }
  if (f & ID1_COLLAPSE)
    {
      local.copy(solution);
      if (id1CollapseMatch(subject, local, returnedSubproblem, extensionInfo))
	{
	  if (greedyMatchOK && returnedSubproblem == 0)
	    {
	      solution.copy(local);
	      return true;
	    }
	  alternatives.addOption(local, returnedSubproblem, extensionInfo);
	}
    }
  if (f & IDEM_COLLAPSE)
    {
      local.copy(solution);
      if ((extensionInfo == 0) ?
	  idemCollapseMatch(subject, local, returnedSubproblem) :
	  idemCollapseMatch(subject, local, returnedSubproblem, extensionInfo))
	{
	  if (greedyMatchOK && returnedSubproblem == 0)
	    {
	      solution.copy(local);
	      return true;
	    }
	  alternatives.addOption(local, returnedSubproblem, extensionInfo);
	}
    }
  return alternatives.extract(solution, returnedSubproblem, extensionInfo);
}

bool
CUI_LhsAutomaton::freeMatch(DagNode* subject0,
			    DagNode* subject1,
			    Substitution& solution,
			    Subproblem*& returnedSubproblem)
{
  Subproblem* sp0;
  Subproblem* sp1;
  if (subpattern0.match(subject0, solution, sp0))
    {
      if (subpattern1.match(subject1, solution, sp1))
	{
	  SubproblemAccumulator subproblems;
	  subproblems.add(sp0);
	  subproblems.add(sp1);
	  returnedSubproblem = subproblems.extractSubproblem();
	  return true;
	}
      else
	delete sp0;  // important!
    }
  return false;
}

bool
CUI_LhsAutomaton::id0CollapseMatch(DagNode* subject,
				   Substitution& solution,
				   Subproblem*& returnedSubproblem,
				   ExtensionInfo* extensionInfo)
{
  Subproblem* sp0;
  Subproblem* sp1;
  if (subpattern0.match(topSymbol->getIdentityDag(), solution, sp0))
    {
      if (subpattern1.match(subject, solution, sp1, matchAtTop, extensionInfo))
	{
	  SubproblemAccumulator subproblems;
	  subproblems.add(sp0);
	  subproblems.add(sp1);
	  returnedSubproblem = subproblems.extractSubproblem();
	  return true;
	}
      else
	delete sp0;
    }
  return false;
}

bool
CUI_LhsAutomaton::id1CollapseMatch(DagNode* subject,
				   Substitution& solution,
				   Subproblem*& returnedSubproblem,
				   ExtensionInfo* extensionInfo)
{
  Term* identity = topSymbol->getIdentity();
  if ((flags & ID0_COLLAPSE) && identity->equal(subject))
    return false;  // we will have already discovered solution on id0Collapse branch
  Subproblem* sp0;
  Subproblem* sp1;
  if (subpattern1.match(topSymbol->getIdentityDag(), solution, sp1))
    {
      if (subpattern0.match(subject, solution, sp0, matchAtTop, extensionInfo))
	{
	  SubproblemAccumulator subproblems;
	  subproblems.add(sp1);
	  subproblems.add(sp0);
	  if (extensionInfo != 0 && (flags & ID0_COLLAPSE))
	    subproblems.add(new EqualitySubproblem(identity, extensionInfo, false));
	  returnedSubproblem = subproblems.extractSubproblem();
	  return true;
	}
      else
	delete sp1;
    }
  return false;
}

bool
CUI_LhsAutomaton::idemCollapseMatch(DagNode* subject,
				    Substitution& solution,
				    Subproblem*& returnedSubproblem)
{
  Term* identity = topSymbol->getIdentity();
  if (identity != 0 && identity->equal(subject))
    return false; 
  Subproblem* sp0;
  Subproblem* sp1;
  if (subpattern0.match(subject, solution, sp0, matchAtTop))
    {
      if (subpattern1.match(subject, solution, sp1, matchAtTop))
	{
	  SubproblemAccumulator subproblems;
	  subproblems.add(sp1);
	  subproblems.add(sp0);
	  returnedSubproblem = subproblems.extractSubproblem();
	  return true;
	}
      else
	delete sp0;
    }
  return false;
}

bool
CUI_LhsAutomaton::idemCollapseMatch(DagNode* subject,
				    Substitution& solution,
				    Subproblem*& returnedSubproblem,
				    ExtensionInfo* extensionInfo)
{
  Subproblem* sp;
  if (subpattern0.topAutomaton->match(subject, solution, sp, extensionInfo))
    {
      SubproblemAccumulator subproblems;
      subproblems.add(sp);
      Term* identity = topSymbol->getIdentity();
      if (identity != 0)
	subproblems.add(new EqualitySubproblem(identity, extensionInfo, false));
      subproblems.add(new ExtensionMatchSubproblem(subpattern1.automaton,
						   extensionInfo,
						   solution.nrFragileBindings()));
      returnedSubproblem = subproblems.extractSubproblem();
      return true;
    }
  return false;
}
