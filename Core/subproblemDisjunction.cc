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
//      Implementation for class SubproblemDisjunction
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "localBinding.hh"
#include "subproblemDisjunction.hh"

SubproblemDisjunction::SubproblemDisjunction()
{
  realExtensionInfo = 0;
}

SubproblemDisjunction::~SubproblemDisjunction()
{
  int nrOptions = options.length();
  for (int i = 0; i < nrOptions; i++)
    {
      Option& op = options[i];
      delete op.difference;
      delete op.subproblem;
      delete op.extensionInfo;
    }
}

void
SubproblemDisjunction::addOption(LocalBinding* difference,
				 Subproblem* subproblem,
				 ExtensionInfo* extensionInfo)
{
  int nrOptions = options.length();
  options.expandBy(1);
  Option& op = options[nrOptions];
  op.difference = difference;
  op.subproblem = subproblem;
  op.extensionInfo = extensionInfo;
  if (extensionInfo != 0)
    {
      Assert(realExtensionInfo == 0 || realExtensionInfo == extensionInfo,
	     "extension info clash");
      realExtensionInfo = extensionInfo;
    }
}

bool
SubproblemDisjunction::solve(bool findFirst, RewritingContext& solution)
{
  int nrOptions = options.length();
  if (findFirst)
    selectedOption = 0;
  for (; selectedOption < nrOptions; selectedOption++)
    {
      Option& so = options[selectedOption];
      if (findFirst)
	{
	  if (so.difference != 0 && !(so.difference->assert(solution)))
	    continue;
	  ExtensionInfo* extensionInfo = so.extensionInfo;
	  if (extensionInfo != 0)
	    realExtensionInfo->copy(extensionInfo);
	}
      Subproblem* subproblem = so.subproblem;
      if (subproblem == 0)
        {
          if (findFirst)
            return true;
        }
      else
        {
          if (subproblem->solve(findFirst, solution))
            return true;
        }
      if (so.difference != 0)
        so.difference->retract(solution);
      findFirst = true;
    }
  return false;
}
