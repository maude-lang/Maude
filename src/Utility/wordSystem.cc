/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2015 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class WordSystem.
//
#include "macros.hh"
#include "vector.hh"
#include "wordSystem.hh"

WordSystem::WordSystem(int nrVariables, int nrEquations)
{
  current = new WordLevel(nrVariables, nrEquations);
  incompletenessFlag = 0;
}

WordSystem::~WordSystem()
{
  delete current;
}

int
WordSystem::findNextSolution()
{
  for (;;)
    {
      //cout << "Solving level " << levelStack.size() << endl;
      WordLevel::ResultPair result = current->findNextPartialSolution();
      if (result.first & WordLevel::INCOMPLETE)
	incompletenessFlag = INCOMPLETE;
      if (result.first & WordLevel::SUCCESS)
	{
	  if (result.second == 0)
	    return SUCCESS | incompletenessFlag;  // current holds a complete solution
	  
	  //
	  //	Need to put current problem aside and solve residual problem.
	  //
	  levelStack.append(current);
	  current = result.second;
	}
      else
	{
	  if (levelStack.empty())
	    break;
	  //
	  //	Need to discard unsolvable current problem and look for another solution to the previous residual problem.
	  //
	  delete current;
	  int top = levelStack.size() - 1;
	  current = levelStack[top];
	  levelStack.resize(top);
	}
    }
  return FAILURE | incompletenessFlag;
}
