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
//      Class for memoization tables.
//
//	In the current implmentation this is just a shell - the actually dag nodes are stored in
//	the module. Thus MemoTable needs to be derived from ModuleItem in order to find the module.
//
#ifndef _memoTable_hh_
#define _memoTable_hh_
#include "moduleItem.hh"

class MemoTable : public ModuleItem
{
public:
  typedef Vector<int> SourceSet;

  MemoTable(bool memoFlag);

  bool isMemoized() const;
  bool memoRewrite(SourceSet& sourceSet, DagNode* subject, RewritingContext& context);
  void memoEnter(SourceSet& sourceSet, DagNode* destination);

private:
  const bool memo;
};

inline
MemoTable::MemoTable(bool memoFlag)
  : memo(memoFlag)
{
}

inline bool
MemoTable::isMemoized() const
{
  return memo;
}

#endif
