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
//      Class for premodules created at the metalevel.
//
#ifndef _metaPreModule_hh_
#define _metaPreModule_hh_
#include "preModule.hh"
#include "dagRoot.hh"
#include "moduleDatabase.hh"


class MetaPreModule : public PreModule
{
  NO_COPYING(MetaPreModule);

public:
  MetaPreModule(int name, DagNode* moduleDag, MetaLevel* metaLevel, MetaModule* module, Interpreter* owner);
  ~MetaPreModule();
  //
  //	Virtual functions we need to define.
  //
  int getNrParameters() const {return 0;}
  int getParameterName(int index) const {return 0;}
  const ModuleExpression* getParameter(int index) const {return 0;}

  const ModuleDatabase::ImportMap& getAutoImports() const {return *(new ModuleDatabase::ImportMap());}
  int getNrImports() const {return 0;}
  int getImportMode(int index) const {return 0;}
  const ModuleExpression* getImport(int index) const {return 0;}

  VisibleModule* getFlatSignature() {return getFlatModule();}
  VisibleModule* getFlatModule();

private:
  void regretToInform(Entity* doomedEntity);

  DagRoot moduleDag;
  MetaLevel* const metaLevel;
  MetaModule* flatModule;
};

#endif
