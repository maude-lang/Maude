/*

    This file is part of the Maude 3 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for dag nodes containing SMT numbers.
//
#ifndef _SMT_NumberDagNode_hh_
#define _SMT_NumberDagNode_hh_
#include <gmpxx.h>
#include "NA_DagNode.hh"

class SMT_NumberDagNode : public NA_DagNode
{
public:
  SMT_NumberDagNode(SMT_NumberSymbol* symbol, const mpq_class& value);
  ~SMT_NumberDagNode();

  size_t getHashValue();
  int compareArguments(const DagNode* other) const;
  void overwriteWithClone(DagNode* old);
  DagNode* makeClone();

  const mpq_class& getValue() const;

private:
  mpq_class *value;

  friend class SMT_NumberTerm;		// for comparison
};

inline const mpq_class&
SMT_NumberDagNode::getValue() const
{
  return *value;
}

#endif
