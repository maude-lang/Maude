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
//      Implementation for class NA_DagNode.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      NA theory class definitions
#include "NA_Symbol.hh"
#include "NA_DagNode.hh"

RawDagArgumentIterator*
NA_DagNode::arguments()
{
  return 0;
}

DagNode*
NA_DagNode::markArguments()
{
  return 0;
}

DagNode*
NA_DagNode::copyEagerUptoReduced2()
{
  return makeClone();
}

void
NA_DagNode::clearCopyPointers2()
{
}

DagNode*
NA_DagNode::copyWithReplacement(int /* argIndex */, DagNode* /* replacement */)
{
  CantHappen("should never be called");
  return 0;
}

DagNode*
NA_DagNode::copyWithReplacement(Vector<RedexPosition>& /* redexStack  */,
				int /* first */,
				int /* last */)
{
  CantHappen("should never be called");
  return 0;
}

void
NA_DagNode::stackArguments(Vector<RedexPosition>& /* stack */,
			   int /* parentIndex */,
			   bool /* respectFrozen */)
{
}
