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
//      Class for cache of meta-operations.
//
#ifndef _metaOpCache_hh_
#define _metaOpCache_hh_

class MetaOpCache
{
  NO_COPYING(MetaOpCache);

public:
  MetaOpCache(int maxSize = 4);
  ~MetaOpCache();

  void insert(FreeDagNode* metaOp, SearchState* state, Int64 lastSolutionNr);
  bool remove(FreeDagNode* metaOp,
	      RewritingContext& parentContext,
	      SearchState*& state,
	      Int64& lastSolutionNr);

  void insert(FreeDagNode* metaOp, RewriteSequenceSearch* search, Int64 lastSolutionNr);
  bool remove(FreeDagNode* metaOp,
	      RewritingContext& parentContext,
	      RewriteSequenceSearch*& search,
	      Int64& lastSolutionNr);


private:
  bool sameProblem(FreeDagNode* m1, DagNode* m2);

  struct Item
  {
    void clear();

    DagRoot* metaOp;
    SearchState* state;
    RewriteSequenceSearch* search;
    Int64 lastSolutionNr;
  };

  const int maxSize;
  Vector<Item> cache;
};

#endif
