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
//	Abstract base class for round-robin based strategic execution.
//
#ifndef _strategicProcess_hh_
#define _strategicProcess_hh_
#include "strategicExecution.hh"

class StrategicProcess : public StrategicExecution
{
  NO_COPYING(StrategicProcess);

public:
  StrategicProcess(StrategicExecution* taskSibling, StrategicProcess* other);
  ~StrategicProcess();

  StrategicProcess* getNextProcess() const;
  virtual Survival run(StrategicSearch& searchObject) = 0;

private:
  StrategicProcess* prev;	// previous process in circular queue
  StrategicProcess* next;	// next process in circular queue
};

inline StrategicProcess*
StrategicProcess::getNextProcess() const
{
  return next;
}

#endif
