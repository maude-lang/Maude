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
//	Class for dag nodes containing floating point numbers.
//
#ifndef _floatDagNode_hh_
#define _floatDagNode_hh_
#include "NA_DagNode.hh"

class FloatDagNode : public NA_DagNode
{
public:
  FloatDagNode(FloatSymbol* symbol, double value);

  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();

  double getValue() const;

private:
  const union
  {
    double value;
    Int64 bitPattern;
  };

  friend class FloatTerm;		// for comparison
};

inline double
FloatDagNode::getValue() const
{
  return value;
}

#endif
