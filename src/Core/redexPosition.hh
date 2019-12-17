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
//      Class for holding position information of a potential redex
//
#ifndef _redexPosition_hh_
#define _redexPosition_hh_

class RedexPosition
{
public:
  RedexPosition() { CantHappen("called"); }  // needed so that Vector template can be instantiated
  //  RedexPosition(DagNode* node, int parentIndex, int argIndex);
  RedexPosition(DagNode* node, int parentIndex, int argIndex, bool eager);

  DagNode* node() const;
  int parentIndex() const;
  int argIndex() const;
  void replaceNode(DagNode* newNode);
  void setStale();
  void clearStale();
  bool isStale() const;
  bool isEager() const;

private:
  enum Flags
  {
    STALE = 1,
    EAGER = 2
  };

  DagNode* dagNode;
  int pIndex;
  int aIndex;
  int flags;
};

/*
inline
RedexPosition::RedexPosition(DagNode* node, int parentIndex, int argIndex)
  : dagNode(node),
    pIndex(parentIndex),
    aIndex(argIndex),
    flags(0)
{
}
*/

inline
RedexPosition::RedexPosition(DagNode* node, int parentIndex, int argIndex, bool eager)
  : dagNode(node),
    pIndex(parentIndex),
    aIndex(argIndex),
    flags(eager ? EAGER : 0)
{
}

inline DagNode*
RedexPosition::node() const
{
  return dagNode;
}

inline int
RedexPosition::parentIndex() const
{
  return pIndex;
}

inline int
RedexPosition::argIndex() const
{
  return aIndex;
}

inline void
RedexPosition::replaceNode(DagNode* newNode)
{
  dagNode = newNode;
}

inline void
RedexPosition::setStale()
{
  flags |= STALE;
}

inline void
RedexPosition::clearStale()
{
  flags &= ~STALE;
}

inline bool
RedexPosition::isStale() const
{
  return flags & STALE;
}

inline bool
RedexPosition::isEager() const
{
  return flags & EAGER;
}

#endif
