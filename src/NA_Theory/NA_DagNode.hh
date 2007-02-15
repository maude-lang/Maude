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
//	Abstract base class for dag nodes in the NA theory.
//
#ifndef _NA_DagNode_hh_
#define _NA_DagNode_hh_
#include "dagNode.hh"

class NA_DagNode : public DagNode
{
public:
  NA_DagNode(NA_Symbol* symbol);
  //
  //    Functions required by theory interface.
  //
  //	The following functions are left to the derived class to handle:
  //
  //	int compareArguments(const DagNode* other) const
  //	void overwriteWithClone(DagNode* old);
  //	DagNode* makeClone();
  //
  RawDagArgumentIterator* arguments();
  DagNode* copyWithReplacement(int argIndex, DagNode* replacement);
  DagNode* copyWithReplacement(Vector<RedexPosition>& redexStack,
			       int first,
			       int last);
  void stackArguments(Vector<RedexPosition>& stack,
		      int parentIndex,
		      bool respectFrozen);
  //
  //	Stuff for unification.
  //
  bool unify(DagNode* rhs,
	     Substitution& solution,
	     Subproblem*& returnedSubproblem,
	     ExtensionInfo* extensionInfo);
  bool computeBaseSortForGroundSubterms();

private:
  DagNode* markArguments();
  DagNode* copyEagerUptoReduced2();
  void clearCopyPointers2();
};

inline
NA_DagNode::NA_DagNode(NA_Symbol* symbol) : DagNode(symbol)
{
}

#endif
