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
//	Class for sorts
//
#ifndef _sort_hh_
#define _sort_hh_
#include "namedEntity.hh"
#include "lineNumber.hh"
#include "moduleItem.hh"
#include "natSet.hh"
#include "connectedComponent.hh"
#include "fullCompiler.hh"

class Sort : public NamedEntity, public LineNumber, public ModuleItem
{
  NO_COPYING(Sort);

public:
  enum SpecialSorts
    {
      KIND = 0,
      ERROR_SORT = 0,
      SORT_UNKNOWN = -1
    };

  Sort(int id);

  void insertSubsort(Sort* subsort);
  int index() const;
  ConnectedComponent* component() const;
  const Vector<Sort*>& getSubsorts() const;
  const Vector<Sort*>& getSupersorts() const;
  const NatSet& Sort::getLeqSorts() const;
  bool errorFreeMaximal() const;

#ifdef COMPILER
  void generateSortVector(CompilationContext& context);
#endif

#ifdef DUMP
  void dump(ostream& s);
#endif

  //
  //	for use by ConnectedComponent only
  //
  void registerConnectedSorts(ConnectedComponent* component);
  void processSubsorts();
  void computeLeqSorts();

  friend bool leq(const Sort* sort1, const Sort* sort2);
  friend bool leq(int index1, const Sort* sort2);
  friend bool leq(const Sort* sort1, int index2);

private:
  union
  {
    int sortIndex;
    int nrUnresolvedSupersorts;		// only used in calculating sortIndex
  };
  int fastTest;
  ConnectedComponent* sortComponent;
  Vector<Sort*> subsorts;
  Vector<Sort*> supersorts;
  NatSet leqSorts;
};

inline int
Sort::index() const
{
  return sortIndex;
}

inline ConnectedComponent*
Sort::component() const
{
  return sortComponent;
}

inline const Vector<Sort*>&
Sort::getSubsorts() const
{
  return subsorts;
}

inline const Vector<Sort*>&
Sort::getSupersorts() const
{
  return supersorts;
}

inline const NatSet&
Sort::getLeqSorts() const
{
  return leqSorts;
}

inline bool
Sort::errorFreeMaximal() const
{
  return sortIndex == 1 && sortComponent->nrMaximalSorts() == 1 && sortComponent->errorFree();
}

inline bool
leq(const Sort* sort1, const Sort* sort2)
{
  return sort2->leqSorts.contains(sort1->sortIndex);
}

inline bool
leq(int index1, const Sort* sort2)
{
  Assert(index1 != Sort::SORT_UNKNOWN, "unknown sort");
  if (index1 >= sort2->fastTest)
    return true;
  return sort2->leqSorts.contains(index1);
}

inline bool
leq(const Sort* sort1, int index2)
{
  return leq(sort1->sortIndex, sort1->sortComponent->sort(index2));
}

//
//      Output function for Sort must be defined by library user.
//
ostream& operator<<(ostream& s, const Sort* sort);

#endif
