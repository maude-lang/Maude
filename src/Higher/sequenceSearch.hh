/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Base class for rewrite and narrowing sequence searches.
//
#ifndef _sequenceSearch_hh_
#define _sequenceSearch_hh_
#include "cacheableState.hh"

class SequenceSearch : public CacheableState
{
public:
  enum SearchType
  {
   ONE_STEP,		// state reached after exactly one step
   AT_LEAST_ONE_STEP,	// state reached after 1 or more steps
   ANY_STEPS,		// state reached after 0 or more steps
   NORMAL_FORM,		// state has no successor states
   BRANCH		// state has two or more distinct successor states
  };
};

#endif
