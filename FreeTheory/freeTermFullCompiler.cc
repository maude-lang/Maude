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
//	Code to help with full compilation of free terms
//
#include "freePositionTable.hh"
#include "freeSubterm.hh"

bool
FreeTerm::scanFreeSkeleton(const NatSet& usedVariables,
			   Vector<int>& path,
			   FreePositionTable& positions,
			   NatSet& boundVariables,
			   Vector<FreeSubterm>& subterms)
{
  bool unfailing = true;
  int last = path.length();
  path.expandBy(1);
  int nrArgs = argArray.length();
  for (int i = 0; i < nrArgs; i++)
    {
      path[last] = i;
      Term* t = argArray[i];
      if (FreeTerm* f = dynamic_cast<FreeTerm*>(t))
	{
	  if (!(f->scanFreeSkeleton(usedVariables, path, positions, boundVariables, subterms)))
	    unfailing = false;
	}
      else
	{
	  int posIndex = positions.position2Index(path);
	  if (VariableTerm* v = dynamic_cast<VariableTerm*>(t))
	    {
	      int varIndex = v->getIndex();
	      if (boundVariables.contains(varIndex))
		{
		  unfailing = false;
		  subterms.append(FreeSubterm(FreeSubterm::BOUND_VARIABLE, posIndex));
		}
	      else
		{
		  boundVariables.insert(varIndex);
		  int efm = v->getSort()->errorFreeMaximal();
		  if (!efm)
		    unfailing = false;
		  if (!efm ||
		      usedVariables.contains(varIndex) ||
		      t->occursInContext().contains(varIndex))
		    subterms.append(FreeSubterm(FreeSubterm::FREE_VARIABLE, posIndex));
		}  
	    }
	  else
	    {
	      unfailing = false;
	      if (t->ground())
		subterms.append(FreeSubterm(FreeSubterm::GROUND_ALIEN, posIndex));
	      else
		subterms.append(FreeSubterm(FreeSubterm::NON_GROUND_ALIEN, posIndex));
	    }
	}
    }
  path.contractTo(last);
  return unfailing;
}
