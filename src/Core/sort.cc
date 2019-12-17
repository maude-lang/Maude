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
//      Implementation for class Sort
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      core class definitions
#include "namedEntity.hh"
#include "sort.hh"
#include "connectedComponent.hh"

//	full compiler definitions
#include "compilationContext.hh"

Sort::Sort(int id)
  : NamedEntity(id)
{
  sortComponent = 0;
}

void
Sort::insertSubsort(Sort* subsort)
{
  subsorts.append(subsort);
  subsort->supersorts.append(this);
}

void
Sort::registerConnectedSorts(ConnectedComponent* component)
{
  sortComponent = component;
  component->registerSort(this);
  //
  //	explore subsorts
  //
  int nrSorts = subsorts.length();
  for (int i = 0; i < nrSorts; i++)
    {
      Sort* s = subsorts[i];
      if (s->sortComponent == 0)
	s->registerConnectedSorts(component);
    }
  //
  //	explore supersorts
  //
  nrSorts = supersorts.length();
  if (nrSorts == 0)
    sortIndex = component->appendSort(this);
  else
    {
      nrUnresolvedSupersorts = nrSorts;
      for (int i = 0; i < nrSorts; i++)
	{
	  Sort* s = supersorts[i];
	  if (s->sortComponent == 0)
	    s->registerConnectedSorts(component);
	}
    }
}

void
Sort::processSubsorts()
{
  int nrSorts = subsorts.length();
  for (int i = 0; i < nrSorts; i++)
    {
      Sort* s = subsorts[i];
      --(s->nrUnresolvedSupersorts);
      if (s->nrUnresolvedSupersorts == 0)
	s->sortIndex = sortComponent->appendSort(s);
    }
}

void
Sort::computeLeqSorts()
{
  leqSorts.insert(sortIndex);
  int nrSubSorts = subsorts.length();
  for (int i = 0; i < nrSubSorts; i++)
    leqSorts.insert(subsorts[i]->leqSorts);
  //
  //	fastTest is the smallest integer such that all sorts with
  //	index >= fastTest are less or equal us.
  //
  int nrSorts = sortComponent->nrSorts();
  fastTest = sortIndex;
  for (int i = nrSorts - 1; i > sortIndex; i--)
    {
      if (!(leqSorts.contains(i)))
	{
	  fastTest = i + 1;
	  break;
	}
    }
}

#ifdef COMPILER
void
Sort::generateSortVector(CompilationContext& context)
{
  context.head() << "char s" << getIndexWithinModule() << "[] = { ";
  int nrSorts = sortComponent->nrSorts();
  for (int i = 0; i < nrSorts ; i++)
    {
      context.head() << (leqSorts.contains(i) ? '1' : '0');
      if (i + 1 < nrSorts)
	context.head() << ", ";
    }
  context.head() << " };";
#ifdef ANNOTATE
  context.head() << "  // " << this;
#endif
  context.head() << '\n';
}
#endif

#ifdef DUMP
void
Sort::dump(ostream& s)
{
  s << "Sort name:\t" << this << "\tsortIndex = " << sortIndex <<
    "\tfastTest = " << fastTest << '\n';
}
#endif
