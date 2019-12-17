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
//	Class for vector memory management.
//
#ifndef _preVector_hh_
#define _preVector_hh_

//
//	This is a very low-level class that will only be use by the
//	Vector template. For efficiency we don't bother with constructors
//	and destructors but allow objects to temporarily be in a bad state
//	on the assumption that the user knows what he is doing.
//
class PreVector
{
public:
  //
  //	Initialization members; one must be called to put PreVector into
  //	a safe state.
  //
  void initEmpty();
  void initAllocate(size_t atLeastBytes);
  void initSteal(PreVector& victim);  // leaves victim in a bad state
  //
  //	Deinitialization function; leaves dangling pointer; can only be called if
  //	getBase() returns nonzero; can only be followed by
  //	initEmpty(), initAllocate() or initSteal().
  //	
  void freeMemory();  // leaves object in bad state
  //
  //	Remaining operators assume safe state and leave PreVector in safe state.
  //
  size_t getAllocatedBytes() const;
  size_t getLength() const;
  void setLength(size_t length);
  const void* getBase() const;
  void* getBase();
  bool empty() const;

private:
  enum MallocSpecificValues	// assume Doug Lea's malloc() - need to fix properly
  {
    MIN_SIZE = 3 * sizeof(int),		// minimum request to pass to malloc()
    TAG_SIZE = sizeof(size_t)		// assume preferred sizes are 2^n - TAG_SIZE
  };

  struct VectorHeader
  {
    size_t allocatedBytes;	// space allocated in bytes
    size_t length;		// number of objects that are properly initialized
  };

  VectorHeader* ptr;
};

inline void
PreVector::initEmpty()
{
  ptr = 0;
}

inline void
PreVector::initSteal(PreVector& victim)
{
  ptr = victim.ptr;
}

inline void
PreVector::freeMemory()
{
  operator delete[](ptr - 1);
}

inline size_t
PreVector::getAllocatedBytes() const
{
  return (ptr == 0) ? 0 : ptr[-1].allocatedBytes;
}

inline size_t
PreVector::getLength() const
{
  return (ptr == 0) ? 0 : ptr[-1].length;
}

inline bool
PreVector::empty() const
{
  if (ptr == 0)
    return true;
  return ptr[-1].length == 0;
  //
  //	For some reason gcc generates far better code for this when
  //	inlined on the x86 than the simpler
  //		return ptr == 0 || ptr[-1].length == 0
  //	Both versions suck on the Sparc - gcc constructs the 0 or 1
  //	result in a register and then tests it in the caller - even
  //	with inlining.
  //
}

inline void
PreVector::setLength(size_t length)
{
  Assert(ptr != 0, "ptr null");
  ptr[-1].length = length;
}

inline const void*
PreVector::getBase() const
{
  return static_cast<void*>(ptr);
}

inline void*
PreVector::getBase()
{
  return static_cast<void*>(ptr);
}

#endif
