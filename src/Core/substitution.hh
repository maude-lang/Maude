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
//	Class for substitutions.
//
//	Substitutions are used to hold partly built dags as well as variable
//	variable bindings so that the same dag construction code can handle
//	variable and non-variable operator arguments without branching.
//
//	Default substitutions come in a standards size (allocateSize) allowing
//	them to be reused rather than reallocated. Within a default substitution
//	entries 0,..., copySize-1 are real entries (actual variables, shared dags)
//	while the remaining entries may be used as temporaries by any function
//	that cares to use them.
//
//	Special purpose substitutions of any size may be created, with no temporary
//	storage.
//
#ifndef _substitution_hh_
#define _substitution_hh_

class Substitution
{
  NO_COPYING(Substitution);

public:
  Substitution();
  Substitution(int size);  // for local substitutions in lhs automata

  static void notify(int size);

  void clear(int size);
  void finished();
  DagNode* value(int index) const;
  void bind(int index, DagNode* value);
  void copy(const Substitution& original);
  void clone(const Substitution& original);
  LocalBinding* operator-(const Substitution& original) const;

  int nrFragileBindings() const;

  //
  //	unificationBind() instantiate value to which variable will be bound,
  //	performs occurs check or sort check as neccessary and handles the case
  //	where a variable is bound to itself. Other bindings are updated to
  //	eliminate the bound variable.
  //
  bool unificationBind(int index, Sort* varSort, DagNode* value);
  LocalBinding* unificationDifference(const Substitution& original) const;

private:
  static int allocateSize;

  Vector<DagNode*> values;
  int copySize;
};

inline
Substitution::Substitution() : values(allocateSize)
{
  copySize = 0;
}

inline
Substitution::Substitution(int size) : values(size)
{
  copySize = size;
}

inline void
Substitution::notify(int size)
{
  if (size > allocateSize)
    {
      DebugAdvisory("allocateSize increased from " <<
		    allocateSize << " to " << size);
      allocateSize = size;
    }
}

inline void
Substitution::clear(int size)
{
  Assert(size >= 0, "-ve size");
  Assert(size <= allocateSize, "size > notified");
  //
  //	We alway clear at least 1 value in order to get a faster loop
  //	since the case size = 0 occurs very infrequently, and clearing
  //	1 value in this case does no harm.
  //
  Vector<DagNode*>::iterator i = values.begin();
  Vector<DagNode*>::iterator e = i + size;
  do
    *i = 0;
  while (++i < e);  // i > e possible if size = 0
  copySize = size;
}

inline DagNode*
Substitution::value(int index) const
{
  Assert(index >= 0, "-ve index");
  Assert(index < values.size(), "index too big");
  return values[index];
}

inline void
Substitution::bind(int index, DagNode* value)
{
  Assert(index >= 0, "-ve index");
  Assert(index < values.size(), "index too big");
  values[index] = value;
}

inline int
Substitution::nrFragileBindings() const
{
  return copySize;
}

inline void
Substitution::finished()
{
  copySize = 0;
}

inline void
Substitution::copy(const Substitution& original)
{
  Assert(copySize == original.copySize, "size mismatch (" << copySize <<
	 " vs " << original.copySize << ')');
  if (copySize > 0)
    {
      Vector<DagNode*>::iterator dest = values.begin();
      Vector<DagNode*>::const_iterator source = original.values.begin();
      Vector<DagNode*>::const_iterator end = source + copySize;
      do
	{
	  *dest = *source;
	  ++dest;
	  ++source;
	}
      while (source != end);
    }
}

inline void
Substitution::clone(const Substitution& original)
{
  //
  //	Unlike copy, we set the copy size from original instead of assuming them
  //	to be the same.
  //
  copySize = original.copySize;
  if (copySize > 0)
    {
      Vector<DagNode*>::iterator dest = values.begin();
      Vector<DagNode*>::const_iterator source = original.values.begin();
      Vector<DagNode*>::const_iterator end = source + copySize;
      do
	{
	  *dest = *source;
	  ++dest;
	  ++source;
	}
      while (source != end);
    }
}

#endif
