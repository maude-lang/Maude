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
//      Implementation for greedy A/AU matcher flex part.
//

int
AU_LhsAutomaton::greedyMatch(AU_DagNode* subject,
			     Substitution& solution,
			     AU_ExtensionInfo* extensionInfo)
{
  local.copy(solution);
  int t = greedyMatchBlocks(subject, extensionInfo);
  if (t != true)
    return t;
  solution.copy(local);
  greedyBindVariables(subject, solution);
  if (extensionInfo != 0)
    {
      extensionInfo->setExtraIdentity(false);
      extensionInfo->setValidAfterMatch(true);
      Assert(extensionInfo->bigEnough(), "didn't match enough of subject");
    }
  return true;
}

void
AU_LhsAutomaton::greedyBindVariables(AU_DagNode* subject,
				     Substitution& solution)
{
  bool buildReducedNodes = subject->isReduced() && topSymbol->sortConstraintFree();
  int skip;
  for (int i = flexLeftPos; i <= flexRightPos; i += skip)
    {
      Subterm& f = flexPart[i];
      skip = f.blockLength;
      if (skip == NOT_FIXED)
	{
	  skip = 1;
	  TopVariable& tv = f.variable;
	  int index = tv.index;
	  DagNode* d = solution.value(index);
	  if (d == 0)
	    {
	      int nrSubjects = tv.lastSubject - tv.firstSubject + 1;
	      Assert(nrSubjects >= 0, "-ve number of subjects " << nrSubjects);
	      if (nrSubjects == 0)
		solution.bind(index, topSymbol->getIdentityDag());
	      else if (nrSubjects == 1)
		solution.bind(index, subject->argArray[tv.firstSubject]);
	      else
		{
		  AU_DagNode* n = new AU_DagNode(topSymbol, nrSubjects);
		  for (int j = 0; j < nrSubjects; j++)
		    n->argArray[j] = subject->argArray[tv.firstSubject + j];
		  n->setProducedByAssignment();
		  if (buildReducedNodes)
		    {
		      topSymbol->computeBaseSort(n);
		      n->setReduced();
		    }
		  solution.bind(index, n);
		}
	    }
	}
    }
}

int
AU_LhsAutomaton::greedyMatchRigidBlock(AU_DagNode* subject,
				       RigidBlock& block,
				       int minShift,
				       int maxShift,
				       int& matchShift)
{
  Assert(minShift <= maxShift, "bad shift range");
  Assert(block.start <= block.end, "bad block range");
  int shiftFactor;
  for (int i = minShift; i <= maxShift; i += shiftFactor)
    {
      scratch.copy(local);	// make a scratch copy for matching at shift i
      int t = greedyMatchRigidBlock2(subject, block, i, shiftFactor);
      if (t == UNDECIDED)
	return UNDECIDED;
      if (t == true)
	{
	  matchShift = i;
	  local.copy(scratch);
	  return true;
	}
    }
  return false;
}

int
AU_LhsAutomaton::greedyMatchRigidBlock2(AU_DagNode* subject,
					RigidBlock& block,
					int nextSubject,
					int& shiftFactor)
{
  ArgVec<DagNode*>& args = subject->argArray;
  shiftFactor = 1;  // default shift increment
  int skip;
  for (int i = block.start; i <= block.end; i += skip)
    {
      Subterm& f = flexPart[i];
      skip = f.blockLength;
      if (skip == NOT_FIXED)
	{
	  DagNode* d = scratch.value(f.variable.index);
	  Assert(d != 0, "unbound variable in rigid block");
	  if (!(subject->eliminateForward(d, nextSubject, rightPos)))  // better limit?
	    return false;
	  skip = 1;
	}
      else
	{
	  int sf;
	  int t = greedyMatchFixedLengthBlock(args, i, nextSubject, sf);
	  if (sf > shiftFactor)
	    shiftFactor = sf;
	  if (t != true)
	    return t;
	  nextSubject += skip;
	}
    }
  return true;
}


int
AU_LhsAutomaton::greedyMatchFixedLengthBlock(ArgVec<DagNode*>& args,
					     int blockStart,
					     int attemptedShift,
					     int& shiftFactor)
{
  for (int i = flexPart[blockStart].blockLength - 1; i >= 0; i--)
    {
      Subterm& f = flexPart[blockStart + i];
      DagNode* d = args[attemptedShift + i];
      shiftFactor = f.shiftFactor;
      switch (f.type)
        {
	case VARIABLE:
          {
            Assert(f.variable.upperBound == 1 && !f.variable.takeIdentity,
                   "non-unit variable in fixed length block");
            DagNode* v = scratch.value(f.variable.index);
            if (v == 0)
              {
                if (d->leq(f.variable.sort))
                  scratch.bind(f.variable.index, d);
                else
                  return false;
              }
            else
              {
                Assert(v->symbol() != topSymbol,
                       "illegal binding to unit variable");
                if (!(v->equal(d)))
                  return false;
              }
            break;
          }
        case GROUND_ALIEN:
          {
            if (!(f.groundAlien->equal(d)))
              return false;
            break;
          }
        case NON_GROUND_ALIEN:
          {
            Subproblem* sp;
            if (!(f.alienAutomaton->match(d, scratch, sp)))
              return false;
	    if (sp != 0)
	      {
		delete sp;
		return UNDECIDED;
	      }
            break;
          }
        }
    }
  return true;
}

int
AU_LhsAutomaton::greedyMatchBlocks(AU_DagNode* subject, AU_ExtensionInfo* extensionInfo)
{
  int nrRigid = rigidBlocks.length();
  Assert(extensionInfo != 0 || nrRigid == 0 ||
	 (rigidBlocks[0].start > flexLeftPos &&
	  rigidBlocks[nrRigid - 1].end < flexRightPos),
	 "missing unbound variable(s)");
  int spare = rightPos - leftPos + 1 - nrSubjectsUsed;
  if (spare < 0)
    return false;
  int varStart = flexLeftPos;
  int nextSubject = leftPos;
  
  for (int i = 0; i < nrRigid; i++)
    {
      //
      //	First match rigid block.
      //
      RigidBlock& r = rigidBlocks[i];
      int min = nextSubject + r.nrSubjectsToLeave;
      int max = min + spare;
      int matchShift;
      int t = greedyMatchRigidBlock(subject, r, min, max, matchShift);
      if (t != true)
	return t;
      int surplus = matchShift - min;
      //
      //	Now match preceeding variable block.
      //
      bool b = greedyMatchVariableBlock(subject->argArray,
					varStart,
					r.start - varStart,
					nextSubject,
					matchShift - nextSubject,
					surplus,
					i == 0 && extensionInfo != 0,
					false,
					extensionInfo);
      if (!b)
	return UNDECIDED;
      r.firstMatch = matchShift;
      spare -= surplus;
      varStart = r.end + 1;
      nextSubject = matchShift + r.nrSubjectsForUs;
    }
  //
  //	Match last variable block.
  //
  int nrSubjectsRemaining = rightPos - nextSubject + 1;
  bool b = greedyMatchVariableBlock(subject->argArray,
				    varStart,
				    flexRightPos - varStart + 1,
				    nextSubject,
				    nrSubjectsRemaining,
				    nrSubjectsRemaining - nrSubjectsForRightVars,
				    extensionInfo != 0 && nrRigid == 0,
				    extensionInfo != 0,
				    extensionInfo);
  return b ? int(true) : UNDECIDED;
}

bool
AU_LhsAutomaton::greedyMatchVariableBlock(ArgVec<DagNode*>& args,
					  int blockStart,
					  int blockLength,
					  int firstSubject,
					  int nrSubjects,
					  int spare,
					  bool leftExtend,
					  bool rightExtend,
					  AU_ExtensionInfo* extensionInfo)
{
  //
  //	Handle pathological case where we have to match no subjects.
  //
  if (nrSubjects == 0)
    {
      for (int i = 0; i < blockLength; i++)
	{
	  TopVariable& tv = flexPart[blockStart + i].variable;
	  if (tv.takeIdentity)
	    {
	      tv.firstSubject = firstSubject;  // take nothing
	      tv.lastSubject = firstSubject - 1;
	      DebugAdvisory("1 tv.firstSubject " << tv.firstSubject);
	    }
	  else
	    return false;
	}
      if (leftExtend)
	extensionInfo->setFirstMatched(0);
      else if (rightExtend)
	extensionInfo->setLastMatched(args.length() - 1);
      return true;
    }
  //
  //	Normal case.
  //
  int pos = firstSubject;
  for (int i = 0; i < blockLength; i++)
    {
      TopVariable& tv = flexPart[blockStart + i].variable;
      if (tv.takeIdentity)
	{
	  if (spare == 0 || !(args[pos]->leq(tv.sort)))
	    {
	      tv.firstSubject = pos;  // take nothing
	      tv.lastSubject = pos - 1;
	      DebugAdvisory("2 tv.firstSubject " << tv.firstSubject);
	    }
	  else
	    {
	      if (leftExtend)
		{
		  //
		  //	Now we have committed to giving a subterm (rather than
		  //	an identity) to a variable we can no longer give subterms
		  //	to the left extension.
		  //
		  extensionInfo->setFirstMatched(pos);
		  leftExtend = false;
		}
	      tv.firstSubject = tv.lastSubject = pos++;  // take first available
	      --spare;
	      DebugAdvisory("3 tv.firstSubject " << tv.firstSubject);
	    }
	}
      else
	{
	  for (; !(args[pos]->leq(tv.sort)); pos++, spare--)
	    {
	      //
	      //	Can't assign current subterm to current variable so
	      //	we must find a way to dispose of it.
	      //
	      if (spare == 0)
		return false;  //  can't afford to dispose of it so fail
	      if (!leftExtend)
		{
		  if (i == 0)
		    return false;
		  TopVariable& pv = flexPart[blockStart + i - 1].variable;
		  if (args[pos]->leq(pv.sort))
		    pv.lastSubject = pos;
		  else
		    return false;  // previous variable cannot take it so fail
		}
	    }
	  if (leftExtend)
	    {
	      //
	      //	Now we have committed to giving a subterm (rather than
	      //	an identity) to a variable we can no longer give subterms
	      //	to the left extension.
	      //
	      extensionInfo->setFirstMatched(pos);
	      leftExtend = false;
	    }
	  tv.firstSubject = tv.lastSubject = pos++;
	  DebugAdvisory("4 tv.firstSubject " << tv.firstSubject);
	}
    }
  if (leftExtend)
    {
      //
      //	Left extension gets the whole thing.
      //
      Assert(!rightExtend,
	     "whole subject in extension should not happen in greedy match");
      DebugAdvisoryCheck(blockLength == 0,
			 "worst case scenario for AU greedy match");
      extensionInfo->setFirstMatched(firstSubject + nrSubjects);
      return true;
    }
  if (blockLength > 0)
    {
      //
      //	Try to assign spare subterms to last variable
      //
      TopVariable& lv = flexPart[blockStart + blockLength - 1].variable;
      for (; spare > 0; spare--, pos++)
	{
	  Assert(pos < firstSubject + nrSubjects, "pos overrun");
	  if (!(args[pos]->leq(lv.sort)))
	    {
	      if (rightExtend)
		break;
	      else
		return false;
	    }
	}
      lv.lastSubject = pos - 1;
    }
  if (rightExtend)
    extensionInfo->setLastMatched(pos - 1);
  return true;
}
