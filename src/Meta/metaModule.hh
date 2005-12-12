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
//      Class for modules created on the fly at the meta level.
//
#ifndef _metaModule_hh_
#define _metaModule_hh_
#include "importModule.hh"
#include "metaOpCache.hh"

class MetaModule : public ImportModule, public MetaOpCache
{
  NO_COPYING(MetaModule);

public:
  MetaModule(int name, ModuleType moduleType, Entity::User* parent);

  void addComplexSymbol(int type, int index, DagNode* identity, DagNode* fixUpInfo);
  void addComplexSymbol(int type,
			int index,
			DagNode* identity,
			DagNode* fixUpInfo,
			const Vector<Sort*>& domainAndRange);
  bool removeComplexSymbol(int& type,
			   int& index,
			   DagNode*& identity,
			   DagNode*& fixUpInfo,
			   Vector<Sort*>& domainAndRange);

private:
  struct ComplexSymbol
  {
    int type;
    int index;
    DagNode* identity;
    DagNode* fixUpInfo;
    Vector<Sort*> domainAndRange;
  };

  Vector<ComplexSymbol> complexSymbols;
};

#endif
