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
//      Class for the info part of a MemoryCell.
//	The reason for this class is that we need two bits for each cell to hold 
//	the marked and "need to call destructor" flags. In order not to
//	waste the rest of the word we allow access to the remaining bits.
//
#ifndef _memoryInfo_hh_
#define _memoryInfo_hh_

class MemoryInfo
{
  NO_COPYING(MemoryInfo);

public:
  //
  //	Flags 1, 2, 4, 8, 16 and 32 are available for derived classes.
  //	Flags 64 and 128 are reserved for garbage collector.
  //
  bool getFlag(int flag) const;
  void setFlag(int flag);
  void clearFlag(int flag);
  void copySetFlags(int flags, const MemoryInfo* other);
  //
  //	Access to garbage collector flags. We can't allow marked flag
  //	to be cleared and it only makes sense to clear the call dtor
  //	flag if we are clearing all flags other than marked.
  //
  bool isMarked() const;
  void setMarked();
  bool needToCallDtor() const;
  void setCallDtor();
  bool simpleReuse() const;
  //
  //	This is needed when a fresh cell is allocated. The reason
  //	for not doing this in the allocation code is to allow the compiler
  //	to combine clearing all flags with immediately setting one or
  //	more flags.
  //
  void clearAllFlags();
  //
  //	This is needed when we reallocate a node that is already in use
  //	(for in-place replacement of a subterm); the marked flag must
  //	be preserved.
  //
  void clearAllExceptMarked();
  //
  //	This is used when a fresh cell is allocated and we want to start
  //	with the flags in a state other than all clear.
  //
  void initFlags(int flagSet);
  //
  //	Access to the unused byte and half word.
  //
  int getHalfWord() const;
  void setHalfWord(int hw);
  int getByte() const;
  void setByte(int bt);

protected:
  enum Flags
    {
      MARKED = 64,	// marked in most recent mark phase
      CALL_DTOR = 128	// call DagNode::~DagNode() before reusing
    };
  //
  //	A MemoryInfo object can only be created as part of MemoryCell.
  //
  MemoryInfo(){};

  static size_t nrNodesInUse;

private:
  Ubyte flags;
  Byte byte;
  short halfWord;
};

inline bool
MemoryInfo::getFlag(int flag) const
{
  return flags & flag;
}

inline void
MemoryInfo::setFlag(int flag)
{
  flags |= flag;
}

inline void
MemoryInfo::clearFlag(int flag)
{
  flags &= ~flag;
}

inline void
MemoryInfo::copySetFlags(int flagSet, const MemoryInfo* other)
{
  flags |= flagSet & other->flags;
}

inline bool
MemoryInfo::isMarked() const
{
  return getFlag(MARKED);
}

inline void
MemoryInfo::setMarked()
{
  ++nrNodesInUse;
  setFlag(MARKED);
}

inline bool
MemoryInfo::needToCallDtor() const
{
  return getFlag(CALL_DTOR);
}

inline void
MemoryInfo::setCallDtor()
{
  setFlag(CALL_DTOR);
}

inline bool
MemoryInfo::simpleReuse() const
{
  //
  //	Neither marked as in-use nor requires the DagNode
  //	destructor to be called before reusing.
  //
  return (flags & (MARKED | CALL_DTOR)) == 0;
}

inline void
MemoryInfo::clearAllFlags()
{
  flags = 0;
}

inline void
MemoryInfo::clearAllExceptMarked()
{
  flags &= MARKED;
}

inline void
MemoryInfo::initFlags(int flagSet)
{
  flags = flagSet;
}

inline int
MemoryInfo::getHalfWord() const
{
  return halfWord;
}

inline void
MemoryInfo::setHalfWord(int hw)
{
  halfWord = hw;
}

inline int
MemoryInfo::getByte() const
{
  return byte;
}

inline void
MemoryInfo::setByte(int bt)
{
  byte = bt;
}

#endif
