/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2006 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SharedRewriteSearchState.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"

//	interface class definitions
#include "term.hh"

//	strategy language class definitions
#include "sharedRewriteSearchState.hh"

SharedRewriteSearchState::SharedRewriteSearchState(RewritingContext* parentContext, DagNode* start, int label, int maxDepth)
  : RewriteSearchState(parentContext->makeSubcontext(start),
		       label,
		       GC_CONTEXT | ALLOW_NONEXEC | IGNORE_CONDITION,
		       0,
		       maxDepth)
{
  refCount = 1;
}

SharedRewriteSearchState::Ptr::Ptr(RewritingContext* parentContext, DagNode* start, int label, int maxDepth)
{
  ptr = new SharedRewriteSearchState(parentContext, start, label, maxDepth);
}

SharedRewriteSearchState::Ptr::Ptr(const Ptr& other)
{
  ptr = other.ptr;
  ++(ptr->refCount);
}

SharedRewriteSearchState::Ptr::~Ptr()
{
  if (--(ptr->refCount) == 0)
    delete ptr;
}
