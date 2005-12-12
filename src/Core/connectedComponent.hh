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
//	Class for connected components in the sort hierarchy
//
#ifndef _connectedComponent_hh_
#define _connectedComponent_hh_
#include "moduleItem.hh"
#include "natSet.hh"

class ConnectedComponent : public ModuleItem
{
  NO_COPYING(ConnectedComponent);

public:
  ConnectedComponent(Sort* firstSort);
  ~ConnectedComponent();

  int nrSorts() const;
  int nrMaximalSorts() const;
  Sort* sort(int index) const;
  bool leq(int index1, int index2) const;
  void findMaximalSorts(const NatSet& uSorts, Vector<Sort*>& maxSorts) const;
  int findIndex(const NatSet& leqSorts) const;
  const NatSet& getLeqSorts(int index) const;
  bool errorFree() const;
  void errorSortSeen();
  //
  //	for use by Sort only
  //
  void registerSort(Sort* sort);
  int appendSort(Sort* sort);

#ifdef DUMP
  void dump(ostream& s);
#endif

private:
  int sortCount;
  short nrMaxSorts;
  Bool errorFreeFlag;
  Vector<Sort*> sorts;
};

inline void
ConnectedComponent::errorSortSeen()
{
  errorFreeFlag = false;
}

inline bool
ConnectedComponent::errorFree() const
{
  return errorFreeFlag;
}

inline int
ConnectedComponent::nrSorts() const
{
  return sortCount;
}

inline int
ConnectedComponent::nrMaximalSorts() const
{
  return nrMaxSorts;
}

inline Sort*
ConnectedComponent::sort(int index) const
{
  return sorts[index];
}

inline void
ConnectedComponent::registerSort(Sort* /* sort */)
{
  ++sortCount;
}

inline int
ConnectedComponent::appendSort(Sort* sort)
{
  int i = sorts.length();
  sorts.append(sort);
  return i;
}

#endif
