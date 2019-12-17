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
//	Class for argument iterator for AU dag nodes.
//
#ifndef _AU_DagArgumentIterator_hh_
#define _AU_DagArgumentIterator_hh_
#include "rawDagArgumentIterator.hh"

class AU_DagArgumentIterator : public RawDagArgumentIterator
{
public:
  AU_DagArgumentIterator(const ArgVec<DagNode*>& arguments);
 
  bool valid() const;
  DagNode* argument() const;
  void next();
 
private:
  const ArgVec<DagNode*>& argArray;
  int position;
};

inline
AU_DagArgumentIterator::AU_DagArgumentIterator(const ArgVec<DagNode*>& arguments)
  : argArray(arguments)
{
  position = 0;
}
 
#endif
