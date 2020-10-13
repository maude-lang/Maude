/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
//	Substitutions are used to hold partly built dags as well as
//	variable bindings so that the same dag construction code can handle
//	variable and non-variable operator arguments without branching.
//
//	For normal rewriting purposes a substitution is divided in to a fragile
//	part which holds variables bindings and construction subdags that are shared
//	between conditions fragments and/or the rhs, and local construction
//	temporaries. The former need to be protected from the garbage collector
//	and are copied when making a copy of the substitution. The latter are
//	assumed to valid only during the construction of a single dag, and can
//	be trashed afterwards.
//
#ifndef _substitution_hh_
#define _substitution_hh_

class Substitution
{
  NO_COPYING(Substitution);

public:
  Substitution(int size);  // for local substitutions in lhs automata
  Substitution(int size, int cSize);  // rewriting contexts
  static void notify(int size);

  void clear(long size);
  void finished();
  DagNode* value(int index) const;
  void bind(int index, DagNode* value);
  void copy(const Substitution& original);
  void clone(const Substitution& original);
  LocalBinding* operator-(const Substitution& original) const;
  int nrFragileBindings() const;

protected:
  int addNewVariable();

private:
  Vector<DagNode*> values;  // only allow even sizes
  int copySize;
};

inline
Substitution::Substitution(int size) : values(size)
{
  Assert(size >= 0, "-ve substitution size" << size);
  DebugAdvisoryCheck(size != 0, "made a zero length substitution");
  copySize = size;
}

inline
Substitution::Substitution(int size, int cSize) : values(size)
{
  Assert(size >= 0, "-ve substitution size " << size);
  Assert(cSize >= 0, "-ve substitution cSize " << cSize);
  Assert(cSize <= size, "cSize > size " << cSize << ' ' << size);
  DebugAdvisoryCheck(size != 0, "made a zero length substitution");
  copySize = cSize;
}

inline void
Substitution::clear(long size)
{
  //cout << "Substitution::clear() values.size() = " << values.size() <<
  //  "  copySize " << copySize << endl;
  //
  //	Take a long argument to avoid the need for an explicit extension instruction on x86-64.
  //
  Assert(size >= 0, "-ve size");
  Assert(size <= values.length(), "size > length");
  Assert(values.length() != 0, "clearing of zero length substitutions is not supported");
  //
  //	Save size early so we don't tie up a register.
  //
  copySize = size;
  //
  //	We alway clear at least 1 value in order to get a faster loop
  //	since the case size = 0 occurs very infrequently, and clearing
  //	1 value in this case does no harm.
  //
  Vector<DagNode*>::iterator i = values.begin();
  Vector<DagNode*>::iterator e = i + size;
  do
    {
      *i = 0;
    }
  while (++i < e);  // i > e possible if size = 0
}

inline DagNode*
Substitution::value(int index) const
{
  Assert(index >= 0, "-ve index " << index);
  Assert(index < static_cast<int>(values.size()), "index too big " << index << " vs " << values.size());
  return values[index];
}

inline void
Substitution::bind(int index, DagNode* value)
{
  Assert(index >= 0, "-ve index " << index);
  Assert(index < static_cast<int>(values.size()), "index too big " << index << " vs " << values.size());
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
  //cout << "Substitution::copy() values.size() = " << values.size() <<
  //  "  copySize " << copySize << "  original.copySize = " << original.copySize << endl;
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

inline int
Substitution::addNewVariable()
{
  int index = copySize;
  ++copySize;
  //
  //	We make the underlying substitution larger if necessary. We never make it smaller
  //	since some slots may be reserved for construction purposes (not part of the copy size)
  //	and we must not lose them.
  //
  if (copySize > values.length())
    values.expandTo(copySize);
  values[index] = 0;
  return index;
}

#endif
