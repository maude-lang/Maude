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
//      Implementation for class AU_ExtensionInfo.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "AU_Theory.hh"
 
//      A theory class definitions
#include "AU_Symbol.hh"
#include "AU_DagNode.hh"
#include "AU_ExtensionInfo.hh"

DagNode*
AU_ExtensionInfo::buildMatchedPortion() const
{
  if (matchedWhole())
    return subject;
  int nrSubterms = last - first + 1;
  Assert(nrSubterms + extraIdentityFlag >= 2, "at least 2 subterms must be matched");
  return subject->makeFragment(first, nrSubterms, extraIdentityFlag);
}

ExtensionInfo*
AU_ExtensionInfo::makeClone() const
{
  AU_ExtensionInfo* e = new AU_ExtensionInfo(subject);
  e->setValidAfterMatch(validAfterMatch());
  bool whole = matchedWhole();
  e->setMatchedWhole(whole);
  if (!whole)
    {
      e->first = first;
      e->last = last;
      e->extraIdentityFlag = extraIdentityFlag;
    }
  return e;
}

void
AU_ExtensionInfo::copy(ExtensionInfo* extensionInfo)
{
  AU_ExtensionInfo* e = safeCast(AU_ExtensionInfo*, extensionInfo);
  setValidAfterMatch(e->validAfterMatch());
  bool whole = e->matchedWhole();
  setMatchedWhole(whole);
  subject = e->subject;
  if (!whole)
    {
      first = e->first;
      last = e->last;
      extraIdentityFlag = e->extraIdentityFlag;
    }
}
