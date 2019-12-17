/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for abstract class StrategicTask.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"

//	strategy language class definitions
#include "strategicExecution.hh"
#include "strategicTask.hh"

StrategicExecution::StrategicExecution(StrategicTask* owner)
  : owner(owner)
{
  //
  //	This ctor is only used for initializing the dummy list element.
  //
  prev = next = this;  // needed for insertions to work correctly
}

StrategicExecution::StrategicExecution(StrategicExecution* other)
  : owner(other->owner)
{
  //
  //	This ctor inserts us into "other"s owners list of slaves before "other".
  //
  prev = other->prev;
  prev->next = this;
  next = other;
  next->prev = this;
}

StrategicExecution::~StrategicExecution()
{
  //
  //	We don't unlink from owners slave list here. Either we were deleted by
  //	owner, in which case the owner has taken care of that, or we
  //	requested deletion (running process) or we requested deletion by the return
  //	value of executionSucceeded() or executionsExhausted() (task) and
  //	a succeeded() or finished() call would have unlinked us.
  //
  //	The upshot of this is that a StrategicProcess must always call succeeded() or
  //	finished() before requesing deletion, otherwise there will be a dangling pointer
  //	in its owners slave list.
  //
}

void
StrategicExecution::finished(StrategicProcess* insertionPoint)
{
  //
  //	First we remove ourselves from our owners list of slaves.
  //
  prev->next = next;
  next->prev = prev;
  //
  //	Then if we were the only slave on the list we report that.
  //
  if (next == prev)
    {
      if (owner->executionsExhausted(insertionPoint) == DIE)
	{
	  owner->finished(insertionPoint);
	  delete owner;
	}
    }
}

void
StrategicExecution::succeeded(int resultIndex, StrategicProcess* insertionPoint)
{
  //
  //	Can an execution succceed multiple times?
  //	- might be necessary for condition fragments.
  //
  //	First we remove ourselves from our owners list of slaves.
  //
  prev->next = next;
  next->prev = prev;
  //
  //	Then we report our success to our owner.
  //
  if (owner->executionSucceeded(resultIndex, insertionPoint) == DIE)
    {
      owner->finished(insertionPoint);
      delete owner;
      return;
    }
  //
  //	Then if we were the only slave on the list we report that.
  //
  if (next == prev)
    {
      if (owner->executionsExhausted(insertionPoint) == DIE)
	{
	  owner->finished(insertionPoint);
	  delete owner;
	}
    }
}
