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
//	Abstract class for table of sort constraints indexed under some symbol.
//
#ifndef _sortConstraintTable_hh_
#define _sortConstraintTable_hh_

class SortConstraintTable
{
public:
  SortConstraintTable();

  void offerSortConstraint(SortConstraint* sortConstraint);
  const Vector<SortConstraint*>& getSortConstraints() const;
  bool sortConstraintFree() const;
  void orderSortConstraints();
  virtual void compileSortConstraints();
  void constrainToSmallerSort(DagNode* subject, RewritingContext& context);
  void constrainToExactSort(DagNode* subject, RewritingContext& context);
  
protected:
  bool safeToInspectSortConstraints();
  virtual bool acceptSortConstraint(SortConstraint* sortConstraint) = 0;
  
private:
  static bool sortConstraintLt(SortConstraint* const& sc1, SortConstraint* const& sc2);
  void constrainToSmallerSort2(DagNode* subject, RewritingContext& context);

  bool tableComplete;
  Vector<SortConstraint*> sortConstraints;
};

inline void
SortConstraintTable::offerSortConstraint(SortConstraint* sortConstraint)
{
  if (acceptSortConstraint(sortConstraint))
    sortConstraints.append(sortConstraint);
}

inline const Vector<SortConstraint*>&
SortConstraintTable::getSortConstraints() const
{
  return sortConstraints;
}

inline bool
SortConstraintTable::sortConstraintFree() const
{
  return sortConstraints.isNull();
}

inline void
SortConstraintTable::constrainToSmallerSort(DagNode* subject, RewritingContext& context)
{
  if (!sortConstraintFree())
    constrainToSmallerSort2(subject, context);
}

inline void
SortConstraintTable::constrainToExactSort(DagNode* subject, RewritingContext& context)
{
  if (!sortConstraintFree())
    constrainToSmallerSort2(subject, context);
}

inline bool
SortConstraintTable::safeToInspectSortConstraints()
{
  return tableComplete;
}

#endif
