/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class Rope.
//
#include "macros.hh"
#include "rope.hh"

//
//	Fibonacci numbers less than max rope length.
//
//	We omit the inital 0, 1. For a rope of height h to be balanced
//	it must have a length of at least fiboTable[h].
//
const Rope::size_type Rope::fiboTable[] =
#if SIZEOF_UNSIGNED_LONG >= 8
  {
    //
    //	For 64-bit machines.
    //
    1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
    121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169,
    63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073, 4807526976,
    7778742049, 12586269025, 20365011074, 32951280099, 53316291173, 86267571272, 139583862445, 225851433717, 365435296162,
    591286729879, 956722026041, 1548008755920, 2504730781961, 4052739537881, 6557470319842, 10610209857723, 17167680177565,
    27777890035288, 44945570212853, 72723460248141, 117669030460994, 190392490709135, 308061521170129, 498454011879264,
    806515533049393, 1304969544928657, 2111485077978050, 3416454622906707, 5527939700884757, 8944394323791464,
    14472334024676221, 23416728348467685, 37889062373143906, 61305790721611591, 99194853094755497, 160500643816367088,
    259695496911122585, 420196140727489673, 679891637638612258, 1100087778366101931, 1779979416004714189, 2880067194370816120,
    4660046610375530309, 7540113804746346429, 12200160415121876738UL
  };
#else
  {
    //
    //	For 32-bit machines.
    //
    1, 2, 3, 5, 8, 13, 21, 34, 55, 89, 144, 233, 377, 610, 987, 1597, 2584, 4181, 6765, 10946, 17711, 28657, 46368, 75025,
    121393, 196418, 317811, 514229, 832040, 1346269, 2178309, 3524578, 5702887, 9227465, 14930352, 24157817, 39088169,
    63245986, 102334155, 165580141, 267914296, 433494437, 701408733, 1134903170, 1836311903, 2971215073UL
  };
#endif

Rope::Rope(const char* cString)
{
  size_type n = strlen(cString);
  if (n == 0)
    {
      ptr = 0;
      return;
    }

  size_type nrLeaves = (n + (TEXT_SIZE - 1)) / TEXT_SIZE;
  ptr = makeTree(cString, n, nrLeaves);
}

Rope::Rope(const char* cString, size_type n)
{
  if (n == 0)
    {
      ptr = 0;
      return;
    }

  size_type nrLeaves = (n + (TEXT_SIZE - 1)) / TEXT_SIZE;
  ptr = makeTree(cString, n, nrLeaves);
}

Rope::~Rope()
{
  decCount(ptr);
}

void
Rope::deepDelete(Fragment* ptr)
{
  if (ptr->height > 0)
    {
      decCount(ptr->node.left);
      decCount(ptr->node.right);
    }
#if DEBUG_REF_COUNT
  ptr->deleted = true;
#else
  delete ptr;
#endif
}

Rope::Fragment*
Rope::makeTree(const char* cString, size_type n, size_type nrLeaves)
{
  Fragment* f = new Fragment(n);

  if (nrLeaves == 1)
    {
      f->height = 0;
      f->length = n;
      for (size_type i = 0; i < n; ++i)
	f->leaf[i] = cString[i];
    }
  else
    {
      size_type nrRightLeaves = nrLeaves / 2;
      size_type nrLeftLeaves = nrLeaves - nrRightLeaves;
      size_type nrLeftChars = nrLeftLeaves * TEXT_SIZE;
      size_type nrRightChars = n - nrLeftChars;

      f->node.left = makeTree(cString, nrLeftChars, nrLeftLeaves);
      f->height = 1 + f->node.left->height;
      f->node.right = makeTree(cString + nrLeftChars, nrRightChars, nrRightLeaves);
    }
  return f;
}

bool
Rope::operator==(const Rope& other) const
{
  size_type len = length();
  if (len != other.length())
    return false;
  if (ptr == other.ptr)
    return true;

  const_iterator p(*this);
  const_iterator q(other);
  for (int i = len; i > 0; --i, ++p, ++q)
    {
      if (*p != *q)
	return false;
    }
  return true;
}

bool
Rope::operator<=(const Rope& other) const
{
  if (ptr == other.ptr)
    return true;

  const_iterator p(*this);
  const const_iterator pe(this);
  const_iterator q(other);
  const const_iterator qe(&other);

  for (; p != pe; ++p, ++q)
    {
      if (q == qe)
	return false;
      int pc = *p;
      int qc = *q;
      if (pc < qc)
	break;
      if (pc > qc)
	return false;
    }
  return true;
}

int
Rope::compare(const Rope& other) const
{
  const_iterator p(*this);
  const const_iterator pe(this);
  const_iterator q(other);
  const const_iterator qe(&other);

  for (; p != pe; ++p, ++q)
    {
      if (q == qe)
	return 1;  // other is smalller
      int d = *p - *q;
      if (d != 0)
	return d;
    }
  return (q == qe) ? 0 : -1;
}

Rope
Rope::operator+(const Rope& other) const
{
  CHECK_COUNT(ptr);
  CHECK_COUNT(other.ptr);

  if (ptr == 0)
    return Rope(other);
  if (other.ptr == 0)
    return Rope(*this);

  size_type ourLength = ptr->length;
  size_type otherLength = other.ptr->length;
  size_type newLength = ourLength + otherLength;

  if (newLength < ourLength)
    {
      //
      //	This happens iff we overflowed size_type.
      //
      CantHappen("overflow in rope summation " << ourLength << " + " << otherLength);
      return *this;  // safety
    }

  if (other.ptr->height == 0)
    {
      if (ptr->height == 0)
	{
	  //
	  //	Leaf + leaf case.
	  //
	  if (newLength <= TEXT_SIZE)
	    {
	      //
	      //	Make a new TextNode that is a simple string concat.
	      //
	      Fragment* n = new Fragment(newLength);
	      n->height = 0;
	      char* q = n->leaf;
	      for (size_type i = 0; i < ourLength; ++i, ++q)
		*q = ptr->leaf[i];
	      for (size_type i = 0; i < otherLength; ++i, ++q)
		*q = other.ptr->leaf[i];
	      return Rope(n);
	    }
	  //
	  //	Too big so fall into general case.
	  //
	}
      else
	{
	  Fragment* rightPtr = ptr->node.right;
	  CHECK_COUNT(rightPtr);
	  if (rightPtr->height == 0)
	    {
	      //
	      //	(... + leaf) + leaf case.
	      //
	      size_type rightLength = rightPtr->length;
	      size_type newRightLength = rightLength + otherLength;
	      if (newRightLength <= TEXT_SIZE)
		{
		  //
		  //	Make a new TextNode that is a simple string concat.
		  //
		  Fragment* n = new Fragment(newRightLength);
		  n->height = 0;
		  char* q = n->leaf;
		  for (size_type i = 0; i < rightLength; ++i, ++q)
		    *q = rightPtr->leaf[i];
		  for (size_type i = 0; i < otherLength; ++i, ++q)
		    *q = other.ptr->leaf[i];
		  //
		  //	Make a new summation node.
		  //
		  Fragment* n2 = new Fragment(newLength);
		  Fragment* leftPtr = ptr->node.left;
		  CHECK_COUNT(leftPtr);
		  n2->height = 1 + leftPtr->height;
		  n2->node.left = leftPtr;
		  incCount(leftPtr);
		  n2->node.right = n;
		  
		  return Rope(n2);
		}
	    }
	}
    }
  //
  //	General case.
  //
  Fragment* n = new Fragment(newLength);
  n->height = 1 + max(ptr->height, other.ptr->height);
  //
  //	If the new height is too big we need to do rebalancing.
  //
  n->node.left = ptr;
  incCount(ptr);
  n->node.right = other.ptr;
  incCount(other.ptr);

  size_type needed = (n->height) < RELAXATION ? 1 : fiboTable[n->height - RELAXATION];  // relaxed requirement for balance since we won't be returning strictly balanced ropes
  if (n->length < needed)
    {
      DebugSave(len, n->length);
#ifdef ROPE_DEBUG
      cout << "rebalancing because at height " << n->height << " length " << len <<
	" < " << needed << endl;
#endif
      Fragment* b = rebalance(n);
      decCount(n);  // should free old tree
      n = b;
#ifdef ROPE_DEBUG
      cout << "rebalanced rope has height " << n->height << " length " << n->length << endl;
#endif
      Assert(len == n->length, "length mismatch");
    }
  return Rope(n);
}

void
Rope::dumpForest(Fragment* forest[])
{
  int i = MAX_BALANCED_HEIGHT;
  while (i >= 0 && forest[i] == 0)
    --i;

  for (; i >= 0; --i)
    {
      Fragment* f = forest[i];
      if (f == 0)
	cout << " 0";
      else
	cout << " " << f->length << "," << f->height;
    }
  cout << endl;
}


Rope::Fragment*
Rope::rebalance(Fragment* fragment)
{
  CHECK_COUNT(fragment);
  //
  //	Create a tree that is at most 2 higher than allowed for by the balance condition.
  //
  Fragment* forest[MAX_BALANCED_HEIGHT + 1];
  for (size_type i = 0; i <= MAX_BALANCED_HEIGHT; ++i)
    forest[i] = 0;
  //
  //	Insert initial fragment into forest.
  //
  addFragment(fragment, forest);
#ifdef DEBUG_ROPE
  cout << "after fragment addition:" << endl;
  dumpForest(forest);
#endif
  //
  //	Construct a tree from the forest.
  //
  Fragment* result = 0;
  for (size_type i = 0; i <= MAX_BALANCED_HEIGHT; ++i)
    {
      if (forest[i] != 0)
	{
	  result = rawConcat(forest[i], result);
	  //incCount(forest[i]);
	}
    }
  return result;
}

void
Rope::addFragment(Fragment* fragment, Fragment* forest[])
{
  CHECK_COUNT(fragment);
  if (fragment->length >= fiboTable[fragment->height])
    {
      //
      //	Balanced case. Leaves are always trivially balanced since they need
      //	only contain one character.
      //
#if ROPE_DEBUG
      cout << "  balanced case: at height " << fragment->height << " length " <<
	fragment->length << " >= " << fiboTable[fragment->height] << endl;
#endif
      incCount(fragment);  // we will resuse fragment in the new tree
      insertFragment(fragment, forest);
#if ROPE_DEBUG
      dumpForest(forest);
#endif
    }
  else
    {
      //
      //	Unbalanced case - can't be a leaf.
      //
#if ROPE_DEBUG
      cout << "  unbalanced case: at height " << fragment->height << " length " <<
	fragment->length << " < " << fiboTable[fragment->height] << endl;
#endif
      addFragment(fragment->node.left, forest);
      addFragment(fragment->node.right, forest);
    }
}

void
Rope::insertFragment(Fragment* fragment, Fragment* forest[])
{
  //
  //	Need to find appropriate slot and check that all smaller slots are empty.
  //
  size_type length = fragment->length;
  Fragment* smallFry = 0;
  size_type i = 0;
  //
  //	If length >= fiboTable[i + 1] then it can't go in slot i; it must go in slot i+1 or later.
  //	Last slot is MAX_BALANCED_HEIGHT; there is no fiboTable[MAX_BALANCED_HEIGH + 1]
  //	- it would be larger than we can store in size_type.
  //	We exit when i = MAX_BALANCED_HEIGHT or we have length < fiboTable[i + 1].
  //	Any earlier entries have been rolled up in smallFry.
  //
  for (; i < MAX_BALANCED_HEIGHT && length >= fiboTable[i + 1]; ++i)
    {
      if (forest[i] != 0)
	{
	  //
	  //	Nonempty smaller slot concat it on to the front of small fry.
	  //
	  smallFry = rawConcat(forest[i], smallFry);
	  forest[i] = 0;
	}
    }
  Fragment* insertee = rawConcat(smallFry, fragment);
  
  for (;; ++i)
    {
      if (forest[i] != 0)
	{
	  insertee = rawConcat(forest[i], insertee);
	  forest[i] = 0;
	}
      if (i == MAX_BALANCED_HEIGHT || insertee->length < fiboTable[i + 1])
	break;
    }
  forest[i] = insertee;
}

Rope::Fragment*
Rope::rawConcat(Fragment* left, Fragment* right)
{
  if (right == 0)
    return left;
  if (left == 0)
    return right;
  Fragment* n = new Fragment(left->length + right->length);
  n->height = 1 + max(left->height, right->height);
  n->node.left = left;
  n->node.right = right;
  return n;
}

Rope
Rope::substr(size_type offset, size_type length) const
{
  if (ptr == 0 || length == 0)
    return Rope();  // empty rope or empty substring

  size_type ourLength = ptr->length;
  if (offset >= ourLength)
    return Rope();  // substring start is beyond our end so fail gracefully
  if (offset + length > ourLength)
    length = ourLength - offset;  // substring end is beyond our end so bring it back in range

  if (offset == 0 && length == ptr->length)
    {
      incCount(ptr);
      return Rope(ptr);  // whole thing
    }

  if (ptr->height == 0)
    {
      //
      //	Make a new text node.
      //
      Fragment* n = new Fragment(length);
      n->height = 0;
      char* p = ptr->leaf + offset;
      char* q = n->leaf;
      for (size_type i = 0; i < length; ++i, ++p, ++q)
	*q = *p;
      return Rope(n);
    }

  Fragment* left = ptr->node.left;
  size_type leftLength = left->length;
  if (offset + length < leftLength)
    {
      incCount(left);
      return Rope(left).substr(offset, length);  // substring contained within the left child
    }

  Fragment* right = ptr->node.right;
  if (offset >= leftLength)
    {
      incCount(right);
      return Rope(right).substr(offset - leftLength, length);  // substring contained withing the right child
    }

  size_type nrLeftChars = leftLength - offset;
  incCount(left);
  incCount(right);
  return Rope(left).substr(offset, nrLeftChars) + Rope(right).substr(0, length - nrLeftChars); // substring spans both children
}

Rope::size_type
Rope::copy(char* buffer) const
{
  const Rope::const_iterator e = end();
  for (Rope::const_iterator i = begin(); i != e; ++i, ++buffer)
    *buffer = *i;
  return length();
}

char*
Rope::makeZeroTerminatedString() const
{
  size_type len = length();
  char* zeroTerminatedString = new char[len + 1];
  copy(zeroTerminatedString);
  zeroTerminatedString[len] = 0;
  return zeroTerminatedString;
}

char
Rope::operator[](size_type n) const
{
  Assert(n < length(), "out of bounds " << n << " vs " << length());

  Fragment* p = ptr;
  while (p->height > 0)
    {
      size_type leftLength = p->node.left->length;
      if (n < leftLength)
	p = p->node.left;
      else
	{
	  n -= leftLength;
	  p = p->node.right;
	}
    }
  return p->leaf[n];
}

/*
 *	Friend function.
 */

ostream&
operator<<(ostream& s, const Rope& r)
{
  Rope::const_iterator e = r.end();
  for (Rope::const_iterator i = r.begin(); i != e; ++i)
    s << *i;
  return s;
}

/*
 *	Iterator class. Only const iterators are supported.
 */

Rope::const_iterator::const_iterator()
{
  stackPtr = 0;  // iterator into empty rope
  absolutePosition = 0;
  index = END_MARKER;  // indicates end
}

Rope::const_iterator::const_iterator(const const_iterator& other)
{
  absolutePosition = other.absolutePosition;
  index = other.index;
  size_t stackSize = other.stackPtr - other.ptrStack;
  for (size_t i = 0; i <= stackSize; ++i)
    ptrStack[i] = other.ptrStack[i];
  stackPtr = ptrStack + stackSize;
}

Rope::const_iterator::const_iterator(const Rope& rope)
{
  absolutePosition = 0;
  //
  //	iterator to first character in a rope.
  //
  Fragment* ptr = rope.ptr;
  stackPtr = ptrStack;
  stackPtr->right = false;
  stackPtr->ptr = ptr;

  if (ptr != 0)
    {
      //
      //	stack path to leftmost leaf.
      //
      while (ptr->height > 0)
	{
	  ptr = ptr->node.left;
	  ++stackPtr;
	  stackPtr->right = false;
	  stackPtr->ptr = ptr;
	}
      index = 0;
    }
  else
    {
      //
      //	begin() iterator to empty rope is also end() iterator.
      //
      index = END_MARKER;  // indicates end
    }
}

Rope::const_iterator::const_iterator(const Rope* rope)
{
  absolutePosition = rope->length();
  //
  //	Iterator to 1 beyond last character in a rope.
  //
  Fragment* ptr = rope->ptr;
  stackPtr = ptrStack;
  stackPtr->right = false;
  stackPtr->ptr = ptr;
  index = END_MARKER;
}

Rope::const_iterator&
Rope::const_iterator::operator=(const const_iterator& other)
{
  absolutePosition = other.absolutePosition;
  index = other.index;
  size_t stackSize = other.stackPtr - other.ptrStack;
  for (size_t i = 0; i <= stackSize; ++i)
    ptrStack[i] = other.ptrStack[i];
  stackPtr = ptrStack + stackSize;
  return *this;
}

bool
Rope::const_iterator::operator==(const const_iterator& other) const
{
  return absolutePosition == other.absolutePosition;
}

bool
Rope::const_iterator::operator!=(const const_iterator& other) const
{
  return absolutePosition != other.absolutePosition;
}

Rope::const_iterator::const_reference
Rope::const_iterator::operator*() const
{
  Assert(index < END_MARKER, "trying to dereference an end iterator, index = " << index);
  return stackPtr->ptr->leaf[index];
}

Rope::const_iterator&
Rope::const_iterator::operator++()
{
  Assert(index < END_MARKER, "trying to increment an end iterator");
  //
  //	Pointer on top of stack should point to a leaf node.
  //
  Fragment* f = stackPtr->ptr;
  ++absolutePosition;
  ++index;
  if (index >= f->length)
    {
      //
      //	We exhausted this leaf node; now need to find the next leaf in sequence, by stacking until we find a node
      //	that is a left child. Such a node must have a corresponding right child and the left most descendant of the right child
      //	is the leaf we seek.
      //
      for (;;)
	{
	  if (stackPtr == ptrStack)
	    {
	      //
	      //	The end iterator is a pointer to the root of the DAG and in index of END_MARKER
	      //
	      index = END_MARKER;
	      return *this;
	    }
	  if (!(stackPtr->right))
	    break;  // found a left child; now need to go to the right child
	  --stackPtr;
	}

      Fragment* parent = (stackPtr - 1)->ptr;
      Fragment* ptr = parent->node.right;
      stackPtr->right = true;
      stackPtr->ptr = ptr;
      while (ptr->height > 0)
	{
	  ptr = ptr->node.left;
	  ++stackPtr;
	  stackPtr->right = false;
	  stackPtr->ptr = ptr;
	}
      index = 0;
    }
  return *this;
}

Rope::const_iterator&
Rope::const_iterator::operator--()
{
  Fragment* ptr;

  --absolutePosition;
  if (index == END_MARKER)
    {
      //
      //	Need to point iterator to last character in a rope.
      //
      ptr = stackPtr->ptr;
      Assert(ptr != 0, "trying to decrement iterator into an empty rope");
    }
  else
    {
      if (index > 0)
	{
	  --index;
	  return *this;
	}
      //
      //	Pop stack to find a node where we can take a left child.
      //
      for (; !(stackPtr->right); --stackPtr)
	Assert(stackPtr != ptrStack, "decremented past begin() iterator");

      Fragment* parent = (stackPtr - 1)->ptr;
      ptr = parent->node.left;
      stackPtr->right = false;
      stackPtr->ptr = ptr;
    }
  //
  //	Find right most path to leaf and point at last character in leaf.
  //
  while (ptr->height > 0)
    {
      ptr = ptr->node.right;
      ++stackPtr;
      stackPtr->right = true;
      stackPtr->ptr = ptr;
    }
  Assert(ptr->length > 0, "empty leaf node");
  index = ptr->length - 1;
  return *this;
}

void
Rope::const_iterator::rebuildStack()
{
  //
  //	We assume absolutePosition is correct and the bottom pointer in the stack points to the top-most fragment
  //	in the rope. We fill out the stack and the index.
  //
  stackPtr = ptrStack;  // bottom of stack
  Fragment* ptr = stackPtr->ptr;  // root fragment of rope
  Assert(ptr != 0, "trying to add or subtract to an iterator into an empty rope");
  Assert(absolutePosition <= ptr->length, "add or subtract makes iterator out of range");

  if (absolutePosition == ptr->length)
    {
      //
      //	Absolute position at the end of rope.
      //
      index = END_MARKER;
      return;
    }

  size_type relPosition = absolutePosition;
  while (ptr->height > 0)
    {
      ++stackPtr;
      size_type leftLen = ptr->node.left->length;
      if (relPosition >= leftLen)
	{
	  stackPtr->right = true;
	  relPosition -= leftLen;
	  ptr = ptr->node.right;
	}
      else
	{
	  stackPtr->right = false;
	  ptr = ptr->node.left;
	}
      stackPtr->ptr = ptr;
    }
  index = relPosition;
}
