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
#include "strategicProcess.hh"

StrategicProcess::StrategicProcess(StrategicExecution* taskSibling, StrategicProcess* other)
  : StrategicExecution(taskSibling)
{
  if (other == 0)
    {
      //
      //	Make degenerate circular list.
      //
      next = prev = this;
    }
  else
    {
      //
      //	Insert us into circular list just before other; i.e. we become
      //	the tail of the list w.r.t. other to ensure that all the remaining
      //	processes get to run before us.
      //
      prev = other->prev;
      prev->next = this;
      next = other;
      next->prev = this;
    }
}

StrategicProcess::~StrategicProcess()
{
  prev->next = next;
  next->prev = prev;
}
