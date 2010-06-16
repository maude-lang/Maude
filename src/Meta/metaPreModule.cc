/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2009 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class MetaPreModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"
#include "meta.hh"

//	interface class definitions
#include "term.hh"

//	metalevel class definitions
#include "metaLevel.hh"
#include "metaModule.hh"
#include "metaPreModule.hh"

MetaPreModule::MetaPreModule(int name, DagNode* moduleDag, MetaLevel* metaLevel, MetaModule* module, Interpreter* owner)
  : PreModule(name, owner),
    moduleDag(moduleDag),
    metaLevel(metaLevel),
    flatModule(module)
{
}

MetaPreModule::~MetaPreModule()
{
  if (flatModule != 0)
    flatModule->deepSelfDestruct();
}

VisibleModule*
MetaPreModule::getFlatModule()
{
  DebugAdvisory("MetaPreModule::getFlatModule() called on " << this);
  if (flatModule == 0)
    {
      IssueAdvisory("reparsing meta-module " << QUOTE(this) <<
		    " due to changes in imported modules.");
      flatModule = metaLevel->downModule(moduleDag.getNode(), false, getOwner());
      if (flatModule != 0)
	flatModule->addUser(this);
    }
  return flatModule;
}

void
MetaPreModule::regretToInform(Entity* doomedEntity)
{
  DebugAdvisory("MetaPreModule::regretToInform() called");
  Assert(doomedEntity == flatModule, "module pointer error");
  flatModule = 0;
}
