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
//	Class for argument iterator for ACU tree dag nodes.
//
#ifndef _ACU_TreeDagArgumentIterator_hh_
#define _ACU_TreeDagArgumentIterator_hh_
#include "rawDagArgumentIterator.hh"
#include "ACU_FastIter.hh"

class ACU_TreeDagArgumentIterator : public RawDagArgumentIterator
{
  NO_COPYING(ACU_TreeDagArgumentIterator);

public:
  ACU_TreeDagArgumentIterator(const ACU_Tree& tree);
 
  bool valid() const;
  DagNode* argument() const;
  void next();
 
private:
  ACU_FastIter iter;
  int multiplicityRemaining;
};

inline
ACU_TreeDagArgumentIterator::ACU_TreeDagArgumentIterator(const ACU_Tree& tree)
  : iter(tree)
{
  multiplicityRemaining = iter.getMultiplicity();
}
 
#endif
