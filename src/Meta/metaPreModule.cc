/*

    This file is part of the Maude 3 interpreter.

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
  setModuleType(module->getModuleType());
  module->addUser(this);
}

MetaPreModule::~MetaPreModule()
{
  if (flatModule != 0)
    flatModule->deepSelfDestruct();
}

const ModuleDatabase::ImportMap*
MetaPreModule::getAutoImports() const
{
  return 0;
}

VisibleModule*
MetaPreModule::getFlatModule()
{
  DebugAdvisory("MetaPreModule::getFlatModule() called on " << this);

  VisibleModule* m = getFlatSignature();
  //
  //	getFlatSignature() returns null pointer rather than calling
  //	markAsBad() if anything went wrong.
  //
  if (m != 0 && m->getStatus() < Module::THEORY_CLOSED)
    {
      //
      //	We didn't import statements or compile module yet.
      //
      m->importStatements();
      Assert(!(m->isBad()), "importStatements() unexpectedly set bad flag in " << *m);
      m->resetImports();
      //
      //	Compile  module.
      //
      m->closeTheory();
      //
      //	We don't allow reserved fresh variable names in variant
      //	equations or narrowing rules. We can't do this until statements
      //	have been compiled since it relied on VariableInfo being filled out.
      //
      m->checkFreshVariableNames();
    }
  return m;  // could be null pointer
}

VisibleModule*
MetaPreModule::getFlatSignature()
{
  DebugAdvisory("MetaPreModule::getFlatSignature() called on " << this);

  if (flatModule == 0)
    {
      IssueAdvisory("reparsing meta-module " << QUOTE(this) << " due to changes in imported modules.");
      flatModule = metaLevel->downSignature(moduleDag.getNode(), getOwner());
      if (flatModule != 0)
	flatModule->addUser(this);
    }
  return flatModule;
}

void
MetaPreModule::regretToInform(Entity* doomedEntity)
{
  DebugAdvisory("MetaPreModule::regretToInform() called on " << this);
  Assert(doomedEntity == flatModule, "module pointer error");
  flatModule = 0;
}
