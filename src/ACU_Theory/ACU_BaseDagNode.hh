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
//	Base class for DAG nodes in the AC and ACU theories.
//
#ifndef _ACU_BaseDagNode_hh_
#define _ACU_BaseDagNode_hh_
#include "dagNode.hh"

class ACU_BaseDagNode : public DagNode
{
public:
  enum NormalizationStatus
  {
    //
    //	Default: no guarantees.
    //
    FRESH,
    //
    //	Node was produced by an assignment in ACU matcher:
    //	(a) all arguments are reduced up to strategy of our symbol
    //	   (this only holds if it was true of subject before matching);
    //	(b) all arguments have the correct sort;
    //	(c) argument list in theory normal form; and
    //
    ASSIGNMENT,
    //
    //	As above but arguments are stored in a red-black (ACU_TreeDagNode)
    //	rather than in an ArgVec (ACU_DagNode).
    //
    TREE
  };

  ACU_BaseDagNode(ACU_Symbol* symbol);

  ExtensionInfo* makeExtensionInfo();
  ACU_Symbol* symbol() const;
  NormalizationStatus getNormalizationStatus() const;
  void setNormalizationStatus(NormalizationStatus status);
  bool isTree() const;
  bool isFresh() const;
  int getSize() const;
};

inline
ACU_BaseDagNode::ACU_BaseDagNode(ACU_Symbol* symbol)
  : DagNode(symbol)
{
}

inline ACU_Symbol*
ACU_BaseDagNode::symbol() const
{
  return safeCast(ACU_Symbol*, DagNode::symbol());
}

inline ACU_BaseDagNode::NormalizationStatus
ACU_BaseDagNode::getNormalizationStatus() const
{
  return static_cast<NormalizationStatus>(getTheoryByte());
}

inline bool
ACU_BaseDagNode::isTree() const
{
  return getTheoryByte() == TREE;
}

inline bool
ACU_BaseDagNode::isFresh() const
{
  return getTheoryByte() == FRESH;
}

inline void
ACU_BaseDagNode::setNormalizationStatus(NormalizationStatus status)
{
  setTheoryByte(status);
}

#endif
