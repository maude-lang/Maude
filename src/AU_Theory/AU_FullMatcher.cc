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
//      Implementation for full A/AU matcher flex part.
//

bool
AU_LhsAutomaton::fullMatch(AU_DagNode* subject,
			   Substitution& solution,
			   SubproblemAccumulator& subproblems,
			   AU_ExtensionInfo* extensionInfo)
{
  AU_Subproblem* subproblem = buildLeftmostPath(subject, solution, extensionInfo);
  if (subproblem == 0)
    return false;
  subproblems.add(subproblem);
  addVariableBlocks(solution, subproblem, subproblems);
  addRemainingPaths(subject, solution, subproblem);
  subproblem->complete();
  if (extensionInfo != 0)
    extensionInfo->setValidAfterMatch(false);
  return true;
}

void
AU_LhsAutomaton::addVariableBlocks(Substitution& solution,
				   AU_Subproblem* subproblem,
				   SubproblemAccumulator& subproblems)
{
  int nrVariables = solution.nrFragileBindings();
  int layerNr = 0;
  bool rigidEncountered = false;
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
	      if (rigidEncountered)
		{
		  rigidEncountered = false;
		  ++layerNr;
		}
	      subproblem->addTopVariable(layerNr,
					 index,
					 tv.takeIdentity || tv.awkward ? 0 : 1,
					 tv.upperBound,
					 tv.sort);
	      if (tv.abstracted != 0)
		{
		  subproblems.add
		    (new VariableAbstractionSubproblem(tv.abstracted,
						       index,
						       nrVariables));
		}
	      continue;
	    }
	  else if (tv.nastyBinding != NONE)
	    {
	      if (rigidEncountered)
		{
		  rigidEncountered = false;
		  ++layerNr;
		}
	      subproblem->addTopVariable(layerNr,
					 index,
					 tv.nastyBinding - 1,
					 tv.nastyBinding,
					 tv.sort);
	      continue;
	    }
	}
      rigidEncountered = true;
    }
}

bool
AU_LhsAutomaton::fullMatchRigidBlock(AU_DagNode* subject,
				     Substitution& solution,
				     RigidBlock& block,
				     int minShift,
				     int maxShift,
				     int& matchShift,
				     Subproblem*& subproblem)
{
  Assert(minShift <= maxShift, "bad shift range");
  Assert(block.start <= block.end, "bad block range");
  int shiftFactor;
  for (int i = minShift; i <= maxShift; i += shiftFactor)
    {
      local.copy(solution);	// make a local copy for matching at shift i
      if (fullMatchRigidBlock2(subject, block, i, subproblem, shiftFactor))
	{
	  matchShift = i;
	  return true;
	}
    }
  return false;
}

bool
AU_LhsAutomaton::fullMatchRigidBlock2(AU_DagNode* subject,
				      RigidBlock& block,
				      int nextSubject,
				      Subproblem*& subproblem,
				      int& shiftFactor)
{
  ArgVec<DagNode*>& args = subject->argArray;
  SubproblemAccumulator subproblems;
  shiftFactor = 1;  // default shift increment
  int skip;
  for (int i = block.start; i <= block.end; i += skip)
    {
      Subterm& f = flexPart[i];
      skip = f.blockLength;
      if (skip == NOT_FIXED)
	{
	  DagNode* d = local.value(f.variable.index);
	  Assert(d != 0, "unbound variable in rigid block");
	  if (!(subject->eliminateForward(d, nextSubject, rightPos)))  // better limit?
	    return false;
	  skip = 1;
	}
      else
	{
	  int sf = NONE;  // to avoid compiler warning
	  int t = fullMatchFixedLengthBlock(args, i, nextSubject, subproblems, sf);
	  if (sf > shiftFactor)
	    shiftFactor = sf;
	  if (!t)
	    return false;
	  nextSubject += skip;
	}
    }
  subproblem = subproblems.extractSubproblem();
  return true;
}


bool
AU_LhsAutomaton::fullMatchFixedLengthBlock(ArgVec<DagNode*>& args,
					   int blockStart,
					   int attemptedShift,
					   SubproblemAccumulator& subproblems,
					   int& shiftFactor)
{
  //
  //	could we merge this with greedyMatchFixedLengthBlock()?
  //	pass subproblems as a pointer and have it 0 for greedy
  //
  //
  for (int i = flexPart[blockStart].blockLength - 1; i >= 0; i--)
    {
      Subterm& f = flexPart[blockStart + i];
      DagNode* d = args[attemptedShift + i];
      Assert(d->getSortIndex() != Sort::SORT_UNKNOWN,
	     "unknown sort for AU argument " << d <<
	     " at index " << attemptedShift + i <<
	     " topSymbol = " << topSymbol <<
	     " blockStart = " << blockStart);

      shiftFactor = f.shiftFactor;
      switch (f.type)
        {
	case VARIABLE:
          {
            Assert(f.variable.upperBound == 1 && !f.variable.takeIdentity,
                   "non-unit variable in fixed length block");
            DagNode* v = local.value(f.variable.index);
            if (v == 0)
              {
                if (d->leq(f.variable.sort))
                  local.bind(f.variable.index, d);
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
            if (!(f.alienAutomaton->match(d, local, sp)))
              return false;
	    subproblems.add(sp);
            break;
          }
        }
    }
  return true;
}

AU_Subproblem*
AU_LhsAutomaton::buildLeftmostPath(AU_DagNode* subject,
				   Substitution& solution,
				   AU_ExtensionInfo* extensionInfo)
{
  int nrRigid = rigidBlocks.length();
  Assert(extensionInfo != 0 || nrRigid == 0 ||
	 (rigidBlocks[0].start > flexLeftPos &&
	  rigidBlocks[nrRigid - 1].end < flexRightPos),
	 "missing unbound variable(s)");
  int spare = rightPos - leftPos + 1 - nrSubjectsUsed;
  if (spare < 0)
    return 0;
  int nextSubject = leftPos;
  AU_Subproblem* subproblem =
    new AU_Subproblem(subject, leftPos, rightPos, nrRigid + 1, extensionInfo);
  
  for (int i = 0; i < nrRigid; i++)
    {
      RigidBlock& r = rigidBlocks[i];
      int min = nextSubject + r.nrSubjectsToLeave;
      int max = min + spare;
      int matchShift;
      Subproblem* sp;
      if (!fullMatchRigidBlock(subject, solution, r, min, max, matchShift, sp))
	{
	  delete subproblem;
	  return 0;
	}
      r.firstMatch = matchShift;
      nextSubject = matchShift + r.nrSubjectsForUs;
      spare -= matchShift - min;
      subproblem->addNode(i, local - solution, sp, matchShift, nextSubject - 1);
    }
  return subproblem;
}

void
AU_LhsAutomaton::addRemainingPaths(AU_DagNode* subject,
				  Substitution& solution,
				  AU_Subproblem* subproblem)
{
  int firstUnusable = rightPos + 1 - nrSubjectsForRightVars;
  for (int i = rigidBlocks.length() - 1; i >= 0; i--)
    {
      RigidBlock& r = rigidBlocks[i];
      int max = firstUnusable - r.nrSubjectsForUs;
      int min = r.firstMatch + 1;
      
      firstUnusable = r.firstMatch;
      while (max >= min)
	{
	  int matchShift;
	  Subproblem* sp;
	  if (fullMatchRigidBlock(subject, solution, r, min, max, matchShift, sp))
	    {      
	      subproblem->addNode(i, local - solution, sp, matchShift,
				  matchShift + r.nrSubjectsForUs - 1);
	      firstUnusable = matchShift;
	      min = matchShift + 1;
	    }
	  else
	    break;
	}
      firstUnusable -= r.nrSubjectsToLeave;
    }
}
