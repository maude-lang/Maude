/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for expanding the assignments in a level.
//

bool
WordLevel::handleInitialOccursCheckFailure()
{
  //
  //	We never create assignments that look like
  //	  X |-> ... X ...
  //	other than X |-> X which indicates X is unbound.
  //
  //	However it is possible for a user to pass in such an
  //	assignment so we need to resolve those up front.
  //
  int nrAssignments = partialSolution.size();
  for (int i = 0; i < nrAssignments; ++i)
    {
      Word& word = partialSolution[i];
      if (word.size() > 1)
	{
	  for (int j : word)
	    {
	      if (i == j)
		{
		  if (resolveOccursCheckFailure(i, word))
		    break;
		  else
		    return false;
		}
	    }
	}
    }
  return true;  
}

bool
WordLevel::fullyExpandAssignments()
{
  if (levelType == PIGPUG)
    return expandAssignmentsToFixedPointNormalCase();
  return expandAssignmentsToFixedPointCollapseCase();
}

bool
WordLevel::append(Word& newWord, const Word& word, int var)
{
  bool occursCheckFail = false;
  for (int i : word)
    {
      newWord.append(i);
      if (i == var)
	occursCheckFail = true;
    }
  return occursCheckFail;
}
