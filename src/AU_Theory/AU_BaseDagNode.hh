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
//	Base class for DAG nodes in the A, AU, AUl and theories.
//
#ifndef _AU_BaseDagNode_hh_
#define _AU_BaseDagNode_hh_
#include "dagNode.hh"

class AU_BaseDagNode : public DagNode
{
  NO_COPYING(AU_BaseDagNode);

public:
  enum NormalizationStatus
  {
    //
    //	Default: no guarantees.
    //
    FRESH = 0,
    //
    //	Node was produced by an assignment in AU matcher:
    //	(a) all arguments are reduced up to strategy of our symbol
    //	   (this only holds if it was true of subject before matching);
    //	(b) all arguments have their true sort; and
    //	(c) argument list in theory normal form.
    //
    ASSIGNMENT = 1,
    //
    //	As above but arguments are stored in a deque (AU_DequeNode)
    //	rather than in an ArgVec (AU_DagNode).
    //
    DEQUE = 2
  };

  AU_BaseDagNode(AU_Symbol* symbol);

  AU_Symbol* symbol() const;
  NormalizationStatus getNormalizationStatus() const;
  void setNormalizationStatus(NormalizationStatus status);
  bool isDeque() const;
  bool isFresh() const;
  bool isProducedByAssignment() const;
};

inline
AU_BaseDagNode::AU_BaseDagNode(AU_Symbol* symbol)
  : DagNode(symbol)
{
}

inline AU_Symbol*
AU_BaseDagNode::symbol() const
{
  return safeCast(AU_Symbol*, DagNode::symbol());
}

inline AU_BaseDagNode::NormalizationStatus
AU_BaseDagNode::getNormalizationStatus() const
{
  return static_cast<NormalizationStatus>(getTheoryByte());
}

inline bool
AU_BaseDagNode::isDeque() const
{
  return getTheoryByte() == DEQUE;
}

inline bool
AU_BaseDagNode::isFresh() const
{
  return getTheoryByte() == FRESH;
}

inline bool
AU_BaseDagNode::isProducedByAssignment() const
{
  return getTheoryByte() == ASSIGNMENT;
}

inline void
AU_BaseDagNode::setNormalizationStatus(NormalizationStatus status)
{
  setTheoryByte(status);
}

#endif
