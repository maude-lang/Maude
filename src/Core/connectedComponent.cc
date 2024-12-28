/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class ConnectedComponent
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions
#include "namedEntity.hh"

//      core class definitions
#include "sort.hh"
#include "connectedComponent.hh"
#include "module.hh"

ConnectedComponent::ConnectedComponent(Sort* firstSort)
{
  sortCount = 0;
  errorFreeFlag = true;
  Sort* errorSort = new Sort(firstSort->id());
  firstSort->getModule()->insertSort(errorSort);
  errorSort->registerConnectedSorts(this);
  firstSort->registerConnectedSorts(this);
  nrMaxSorts = sorts.length() - 1;
  if (nrMaxSorts == 0)
    {
      IssueWarning("the connected component in the sort graph that contains sort " <<
		   QUOTE(firstSort) << " has no maximal sorts due to a cycle.");
      firstSort->getModule()->markAsBad();
      return;
    }
  for (int i = 1; i <= nrMaxSorts; i++)
    errorSort->insertSubsort(sorts[i]);
  for (int i = 1; i < sorts.length(); i++)  // sorts.length() may increase
    sorts[i]->processSubsorts();
  if (sorts.length() != sortCount)
    {
      IssueWarning("the connected component in the sort graph that contains sort " <<
		   QUOTE(firstSort) << " could not be linearly ordered due to a cycle.");
      firstSort->getModule()->markAsBad();
      return;
    }
  for (int i = sortCount - 1; i >= 0; i--)
    sorts[i]->computeLeqSorts();

  lastAllocatedMatchIndex = 1;  // 1 is the default matchIndex; allocated values will be > 1
}

ConnectedComponent::~ConnectedComponent()
{
}

bool
ConnectedComponent::leq(int index1, int index2) const
{
  Assert(index1 != Sort::SORT_UNKNOWN, "bad index1");
  Assert(index2 != Sort::SORT_UNKNOWN, "bad index2");
  return sorts[index2]->getLeqSorts().contains(index1);
}

void
ConnectedComponent::findMaximalSorts(const NatSet& uSorts, Vector<Sort*>& maxSorts) const
{
  NatSet unionSoFar;
  for (int i = 0; !(unionSoFar.contains(uSorts)); i++)
    {
      Assert(i < sortCount, "index overrun");
      if (uSorts.contains(i) && !(unionSoFar.contains(i)))
	{
	  Sort* s = sorts[i];
	  maxSorts.append(s);
	  unionSoFar.insert(s->getLeqSorts());
	}
    }
}

int
ConnectedComponent::findIndex(const NatSet& leqSorts) const
{
  int i = leqSorts.min();
  for (; i > 0; i--)
    {
      if (sorts[i]->getLeqSorts().contains(leqSorts))
	break;
    }
  return i;
}

const NatSet&
ConnectedComponent::getLeqSorts(int index) const
{
  Assert(index != Sort::SORT_UNKNOWN, "bad sort index");
  return sorts[index]->getLeqSorts();
}

#ifdef DUMP
void
ConnectedComponent::dump(ostream& s)
{
  int nrSorts = sorts.length();
  for (int i = 0; i < nrSorts; i++)
    sorts[i]->dump(s);
}
#endif
