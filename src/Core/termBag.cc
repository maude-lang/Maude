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
//      Implementation for class TermBag
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "term.hh"

//	core class definitions
#include "termBag.hh"

void
TermBag::insertMatchedTerm(Term* term, bool eagerContext)
{
  //
  //	New matched terms can never replace built terms (which are available at
  //	zero cost) nor existing matched terms (for which the cost of storing
  //	the extra pointer may already have been paid).
  //
  termsUsableInLazyContext.insert(term);
  if (eagerContext)
    termsUsableInEagerContext.insert(term);
}

void
TermBag::insertBuiltTerm(Term* term, bool eagerContext)
{
  //
  //	New built terms should not arise if there is an existing
  //	usable term in the appropriate context.
  //
  if (eagerContext)
    {
      DebugSave(p, termsUsableInEagerContext.insert(term));
      Assert(p.second, "re-insertion of " << term);
    }
  else
    {
      DebugSave(p, termsUsableInLazyContext.insert(term));
      Assert(p.second, "re-insertion of " << term);
    }
}

Term*
TermBag::findTerm(Term* term, bool eagerContext)
{
  if (eagerContext)
    {
      TermSet::iterator i = termsUsableInEagerContext.find(term);
      if (i != termsUsableInEagerContext.end())
	return *i;
    }
  else
    {
      TermSet::iterator i = termsUsableInLazyContext.find(term);
      if (i != termsUsableInLazyContext.end())
	return *i;
    }
  return 0;
}
