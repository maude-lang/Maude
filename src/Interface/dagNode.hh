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
//      Abstract base class for dag nodes.
//
#ifndef _dagNode_hh_
#define _dagNode_hh_
#include "redexPosition.hh"

class DagNode
{
  NO_COPYING(DagNode);

public:
  DagNode(Symbol* symbol);
  virtual ~DagNode() {}
  void setCallDtor();
  //
  //	Nasty cross casting stuff.
  //
  MemoryCell* getMemoryCell();
  const MemoryCell* getMemoryCell() const;
  //
  //	Static members (for memory management).
  //
  void* operator new(size_t size);
  void* operator new(size_t size, DagNode* old);
  //
  //	These member functions should not be overridden.
  //
  Symbol* symbol() const;
  int compare(const DagNode* other) const;
  bool equal(const DagNode* other) const;
  bool leq(const Sort* sort) const;

  bool isReduced() const;
  void reduce(RewritingContext& context);
  void mark();
  void setReduced();
  void setUnrewritable();
  bool isUnrewritable() const;
  void setUnstackable();
  bool isUnstackable() const;
  void copySetRewritingFlags(const DagNode* other);
  Byte getTheoryByte() const;
  void setTheoryByte(Byte value);

  DagNode* copyReducible();
  DagNode* copyEagerUptoReduced();
  void clearCopyPointers();
  DagNode* copyAndReduce(RewritingContext& context);
  void setSortIndex(int index);
  int getSortIndex() const;
  Sort* getSort() const;
  void computeTrueSort(RewritingContext& context);
  void repudiateSortInfo();
  bool checkSort(const Sort* boundSort, Subproblem*& returnedSubproblem);
  bool checkSort(const Sort* boundSort, RewritingContext& context);
  bool inErrorSort();
  bool matchVariable(int index,
		     const Sort* sort,
		     bool copyToAvoidOverwriting,
		     Substitution& solution,
		     Subproblem*& returnedSubproblem,
		     ExtensionInfo* extensionInfo);
  //
  //	These member functions must be defined for each derived class.
  //
  virtual RawDagArgumentIterator* arguments() = 0;
  virtual size_t getHashValue() = 0;
  virtual int compareArguments(const DagNode* other) const = 0;
  virtual void overwriteWithClone(DagNode* old) = 0;
  virtual DagNode* makeClone() = 0;
  virtual DagNode* copyWithReplacement(int argIndex, DagNode* replacement) = 0;
  virtual DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack,
				       int first,
				       int last) = 0;
  virtual void stackArguments(Vector<RedexPosition>& redexStack,
			      int parentIndex,
			      bool respectFrozen) = 0;
  //
  //	These member functions must be defined for each derived class in theories
  //	that need extension
  //
  virtual bool matchVariableWithExtension(int index,
					  const Sort* sort,
					  Substitution& solution,
					  Subproblem*& returnedSubproblem,
					  ExtensionInfo* extensionInfo);
  virtual void partialReplace(DagNode* replacement, ExtensionInfo* extensionInfo);
  virtual DagNode* partialConstruct(DagNode* replacement, ExtensionInfo* extensionInfo);
  virtual ExtensionInfo* makeExtensionInfo();

#ifdef DUMP
  //
  //	dump() routine is optional; Default will dump common stuff togther with args
  //	in naive way. Replacement routine should call dumpCommon() to dump stuff in
  //	base class.
  //
  virtual void dump(ostream& s, int indentLevel = 0);
  void dumpCommon(ostream& s, int indentLevel);
#endif

protected:
  enum Sizes
  {
    //
    //	We use 2 of the available words in a memory cell; one for our
    //	symbol/copy pointer and one for our virtual function table pointer.
    //	It would be nice to do this in a cleaner way, rather than assume
    //	the existence/size of our virtual function table pointer.
    //
    nrWords = 3 // HACK
  };

  static size_t hash(size_t v1, size_t v2);
  static size_t hash(size_t v1, size_t v2, size_t v3);
  //
  //	Functions to set and test the status of the hash valid flag.
  //
  void setHashValid();
  bool isHashValid() const;
  //
  //	These member functions must be defined for each derived class.
  //
  virtual DagNode* markArguments() = 0;
  virtual DagNode* copyEagerUptoReduced2() = 0;
  virtual void clearCopyPointers2() = 0;

private:  
  enum Flags
  {
    REDUCED = 1,	// reduced up to strategy by equations
    COPIED = 2,		// copied in current copy operation; copyPointer valid
    UNREWRITABLE = 4,	// reduced and not rewritable by rules
    UNSTACKABLE = 8,	// unrewritable and all subterms unstackable or frozen
    //CACHED = 16,	// node exists as part of a cache
    HASH_VALID = 32	// node has a valid hash value (storage is theory dependent)
  };

  bool isCopied() const;
  void setCopied();
  void clearCopied();

  union
  {
    //
    //	Since copy pointers are needed infrequently we will use the top symbol slot
    //	and restore the top symbol from the copy of the dag node when the entire
    //	copy operation in finished; this is slightly hairy but dag nodes are the
    //	fundemental runtime data structure and need to be highly optimized.
    //
    Symbol* topSymbol;
    DagNode* copyPointer;
  };
};

//
//	Output function for DagNode must be defined by library user.
//
ostream& operator<<(ostream& s, DagNode* dagNode);

#include "memoryCell.hh"

inline MemoryCell*
DagNode::getMemoryCell()
{
  return static_cast<MemoryCell*>(static_cast<void*>(this));
}

inline const MemoryCell*
DagNode::getMemoryCell() const
{
  return static_cast<const MemoryCell*>(static_cast<const void*>(this));
}

inline void
DagNode::copySetRewritingFlags(const DagNode* other)
{
  getMemoryCell()->copySetFlags(REDUCED | UNREWRITABLE | UNSTACKABLE,
				other->getMemoryCell());
}

inline void
DagNode::setCallDtor()
{
  getMemoryCell()->setCallDtor();
}

inline int
DagNode::getSortIndex() const
{
  return getMemoryCell()->getHalfWord();
}

inline void
DagNode::setSortIndex(int index)
{
  getMemoryCell()->setHalfWord(index);
}

inline void
DagNode::repudiateSortInfo()
{
  setSortIndex(Sort::SORT_UNKNOWN);
}

//#include "memoryCellNew.hh"

inline void*
DagNode::operator new(size_t size)
{
  //
  //	We rely on MemoryCell::allocateMemoryCell() setting the half word to
  //	Sort::SORT_UNKNOWN.
  //
  Assert(size <= sizeof(MemoryCell), "dag node too big");
  return MemoryCell::allocateMemoryCell();
}

inline void*
DagNode::operator new(size_t /* size */, DagNode* old)
{
  if (old->getMemoryCell()->needToCallDtor())
    old->~DagNode();	// explicitly call virtual destructor
  old->getMemoryCell()->clearAllExceptMarked();
  old->repudiateSortInfo();
  //DebugAdvisory("in place new called, old = " << (void*)(old));
  Assert(old->getSortIndex() == Sort::SORT_UNKNOWN, "bad sort init");
  return static_cast<void*>(old);
}

inline
DagNode::DagNode(Symbol* symbol)
{
  topSymbol = symbol;
  //DebugAdvisory("created dag node for " << symbol << " at " << (void*)(this));
  //DebugAdvisoryCheck(getSortIndex() == Sort::SORT_UNKNOWN,
  //	     "bad sort in dagnode");
}

inline Symbol*
DagNode::symbol() const
{
  return topSymbol;
}

inline int
DagNode::compare(const DagNode* other) const
{
  if (this == other)
    return 0;  // pointers to same shared node
  Symbol* s = other->topSymbol;
  return (topSymbol == s) ? compareArguments(other) : topSymbol->compare(s);
}

inline bool
DagNode::equal(const DagNode* other) const
{
  return this == other || (topSymbol == other->topSymbol && compareArguments(other) == 0);
}

inline bool
DagNode::isReduced() const
{
  return getMemoryCell()->getFlag(REDUCED);
}

inline void
DagNode::setReduced()
{
  getMemoryCell()->setFlag(REDUCED);
}

inline bool
DagNode::isCopied() const
{
  return getMemoryCell()->getFlag(COPIED);
}

inline void
DagNode::setCopied()
{
  getMemoryCell()->setFlag(COPIED);
}

inline void
DagNode::clearCopied()
{
  getMemoryCell()->clearFlag(COPIED);
}

inline void
DagNode::setUnrewritable()
{
  getMemoryCell()->setFlag(UNREWRITABLE);
}

inline bool
DagNode::isUnrewritable() const
{
  return getMemoryCell()->getFlag(UNREWRITABLE);
}

inline void
DagNode::setUnstackable()
{
  getMemoryCell()->setFlag(UNSTACKABLE);
}

inline bool
DagNode::isUnstackable() const
{
  return getMemoryCell()->getFlag(UNSTACKABLE);
}

inline void
DagNode::setHashValid()
{
  getMemoryCell()->setFlag(HASH_VALID);
}

inline bool
DagNode::isHashValid() const
{
  return getMemoryCell()->getFlag(HASH_VALID);
}

inline Byte
DagNode::getTheoryByte() const
{
  return getMemoryCell()->getByte();
}

inline void
DagNode::setTheoryByte(Byte value)
{
  getMemoryCell()->setByte(value);
}

//
//	We use Symbol::fastComputeTrueSort() inline function which in turn uses
//	our setSortIndex() inline function.
//
#include "symbol2.hh"

inline void
DagNode::reduce(RewritingContext& context)
{
  if (!isReduced())
    {
      while (topSymbol->eqRewrite(this, context))
	;
      setReduced();
      //      if (sortIndex == Sort::SORT_UNKNOWN)
      topSymbol->fastComputeTrueSort(this, context);
    }
}

inline void
DagNode::mark()
{
  Assert(this != 0, "bad dag node");
  Assert(topSymbol->arity() >= 0 &&
	 topSymbol->arity() <= 30,  // arbitrary - but helps catch bugs
	 "bad symbol at " << static_cast<void*>(topSymbol));
  DagNode* d = this;
  while (!(d->getMemoryCell()->isMarked()))
    {
      d->getMemoryCell()->setMarked();
      //
      //	markArguments() returns a pointer our the last argument
      //	rather than calling mark() on it. This allows us to
      //	do tail recursion elimination on the last argument
      //	which is important for cons style lists.
      //
      d = d->markArguments();
      if (d == 0)
	break;  // leaf
    }
}

inline bool
DagNode::inErrorSort()
{
  int t = getSortIndex();
  if (t == Sort::SORT_UNKNOWN)
    {
      topSymbol->computeBaseSort(this);
      t = getSortIndex();
      if (!(topSymbol->sortConstraintFree()))
	setSortIndex(Sort::SORT_UNKNOWN);
    }
  return t == Sort::ERROR_SORT;
}

inline Sort*
DagNode::getSort() const
{
  return (getSortIndex() == Sort::SORT_UNKNOWN) ? 0 :
    topSymbol->rangeComponent()->sort(getSortIndex());
}

inline void
DagNode::computeTrueSort(RewritingContext& context)
{
  if (getSortIndex() == Sort::SORT_UNKNOWN)
    topSymbol->normalizeAndComputeTrueSort(this, context);
}

inline DagNode*
DagNode::copyEagerUptoReduced()
{
  if (isReduced())
    return this;
  if (!isCopied())
    {
      copyPointer = copyEagerUptoReduced2();  // this destroys our top symbol
      setCopied();
    }
  return copyPointer;
}

inline void
DagNode::clearCopyPointers()
{
  if (isCopied())
    {
      clearCopied();
      topSymbol = copyPointer->topSymbol;  // restore our top symbol from copy
      clearCopyPointers2();  // can't safely call this until top symbol restored
    }
}

inline DagNode*
DagNode::copyReducible()
{
  DagNode* d = copyEagerUptoReduced();
  clearCopyPointers();
  return d;
}

inline DagNode*
DagNode::copyAndReduce(RewritingContext& context)
{
  DagNode* d = copyReducible();
  d->reduce(context);
  return d;
}

inline bool
DagNode::leq(const Sort* sort) const
{
  return ::leq(getSortIndex(), sort);
}

inline size_t
DagNode::hash(size_t v1, size_t v2)
{
  return 3 * v1 + v2;
}

inline size_t
DagNode::hash(size_t v1, size_t v2, size_t v3)
{
  return 3 * v1 + v2 * v3;
}

#endif
