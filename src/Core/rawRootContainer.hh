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
//      Abstract base class for objects that contain dag root pointers.
//
#ifndef _rawRootContainer_hh_
#define _rawRootContainer_hh_

class RawRootContainer
{
public:
  static void markPhase();

protected:
  void link();
  void unlink();
  virtual void markReachableNodes() = 0;

private:
  static RootContainer* listHead;

  RootContainer* next;
  RootContainer* prev;
};

inline
RootContainer::link()
{
  prev = 0;
  next = listHead;
  if (listHead != 0)
    listHead->prev = this;
  listHead = this;
}

inline
RootContainer::unlink()
{
  if (next != 0)
    next->prev = prev;
  if (prev != 0)
    prev->next = next;
  else
    listHead = next;
}

inline void
RootContainer::markPhase()
{
  for (RootContainer* p = listHead; p; p = p->next)
    p->markReachableNodes();
}

#endif
