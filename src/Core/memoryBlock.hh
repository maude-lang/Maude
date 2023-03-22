/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for the storage part of a MemoryCell.
//	This memory is only accessed by the classes where it is suppplied via
//	a call to new, with one except, where we know it was allocated as
//	a DagNode and we treat it as a DagNode to call the DagNode virtual
//	destructor.
//
#ifndef _memoryBlock_hh_
#define _memoryBlock_hh_

class MemoryBlock
{
  NO_COPYING(MemoryBlock);

public:
  //
  //	A machine word should be able to hold any pointer, int or size
  //	(but not necessarily an Int64).
  //
  union MachineWord
  {
    void* pointer;
    int integer;
    size_t size;
  };
  
protected:
  //
  //	A MemoryInfo object can only be created as part of MemoryCell.
  //
  MemoryBlock(){};

  void callDtor();

private:
  enum Sizes
  {
    //
    //	MemoryCells have this much extra memory allocated for
    //	derived classes to use. Some uses require at least room
    //	for 5 pointers so this is the minimum value.
    //
    NR_EXTRA_WORDS = 5  // minimum value seems best on average
  };

protected:
  //
  //	We never actually use this data member so if it were private we might
  //	get a compiler warning.
  //
  MachineWord filler[NR_EXTRA_WORDS];
};

inline void
MemoryBlock::callDtor()
{
  //
  //	First we get the void* pointer that was returned during allocation.
  //
  void* p = this;
  //
  //	Then because we know it was allocated as a DagNode, we convert
  //	it to a DagNode* pointer.
  //
  DagNode* d = static_cast<DagNode*>(p);
  //
  //	Finally we call the DagNode* virtual destructor to clean up so
  //	this memory block can be reused.
  //
  d->~DagNode();
}

#endif
