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
//      Implementation for class S_ExtensionInfo.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"
 
//      S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_ExtensionInfo.hh"

S_ExtensionInfo::S_ExtensionInfo(S_DagNode* subject)
  : subject(subject)
{
}

DagNode*
S_ExtensionInfo::buildMatchedPortion() const
{
  return new S_DagNode(subject->symbol(),
		       subject->getNumber() - unmatched,
		       subject->getArgument());
}

ExtensionInfo* 
S_ExtensionInfo::makeClone() const
{
  S_ExtensionInfo* n = new S_ExtensionInfo(subject);
  n->unmatched = unmatched;
  return n;
}

void
S_ExtensionInfo::copy(ExtensionInfo* extensionInfo)
{
  S_ExtensionInfo* e = safeCast(S_ExtensionInfo*, extensionInfo);
  subject = e->subject;
  unmatched = e->unmatched;
}
