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
//      Implementation for class MetaModule.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "mixfix.hh"

//	front end class definitions
#include "metaModule.hh"


MetaModule::MetaModule(int name, ModuleType moduleType, Parent* parent)
  : ImportModule(name, moduleType, parent)
{
}

void
MetaModule::addComplexSymbol(int type, int index, DagNode* fixUpInfo)
{
  int nrComplexSymbols = complexSymbols.length();
  complexSymbols.expandBy(1);
  ComplexSymbol& cs = complexSymbols[nrComplexSymbols];
  cs.type = type;
  cs.index =  index;
  cs.fixUpInfo = fixUpInfo;
}

void
MetaModule::addComplexSymbol(int type,
			     int index,
			     DagNode* fixUpInfo,
			     const Vector<Sort*>& domainAndRange)
{
  int nrComplexSymbols = complexSymbols.length();
  complexSymbols.expandBy(1);
  ComplexSymbol& cs = complexSymbols[nrComplexSymbols];
  cs.type = type;
  cs.index = index;
  cs.fixUpInfo = fixUpInfo;
  cs.domainAndRange = domainAndRange;  // deep copy
}

bool
MetaModule::removeComplexSymbol(int& type,
				int& index,
				DagNode*& fixUpInfo,
				Vector<Sort*>& domainAndRange)
{
  int nrComplexSymbols = complexSymbols.length();
  if (nrComplexSymbols == 0)
    return false;
  --nrComplexSymbols;
  ComplexSymbol& cs = complexSymbols[nrComplexSymbols];
  type = cs.type;
  index = cs.index;
  fixUpInfo = cs.fixUpInfo;
  domainAndRange = cs.domainAndRange;  // deep copy
  complexSymbols.contractTo(nrComplexSymbols);
  return true;
}
