/*

    This file is part of the Maude 2 interpreter.

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
//	Base class for an item of strategic execution that is owned by a task.
//
#ifndef _strategicExecution_hh_
#define _strategicExecution_hh_

class StrategicExecution
{
  NO_COPYING(StrategicExecution);

public:
  //
  //	Return values to indicate whether the StrategicExecution object
  //	called should be deleted, possibly after some book-keeping.
  //
  enum Survival
    {
      DIE,
      SURVIVE
    };

  StrategicExecution(StrategicTask* owner);
  StrategicExecution(StrategicExecution* other);
  //
  //	Needed so that things can be deleted through a StrategicExecution*.
  //
  virtual ~StrategicExecution();

  StrategicExecution* getNextSlave() const;

  void finished(StrategicProcess* insertionPoint);
  void succeeded(int resultIndex, StrategicProcess* insertionPoint);

  StrategicTask* getOwner() const;

private:
  StrategicTask* const owner;	// our owner
  StrategicExecution* prev;	// previous execution belonging to owner
  StrategicExecution* next;	// next exectution belonging to owner
};

inline StrategicExecution*
StrategicExecution::getNextSlave() const
{
  return next;
}

inline StrategicTask*
StrategicExecution::getOwner() const
{
  return owner;
}

#endif
