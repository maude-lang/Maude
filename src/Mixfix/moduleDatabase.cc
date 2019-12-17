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
//      Implementation for class ModuleDatabase.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	front end class definitions
#include "preModule.hh"
#include "moduleDatabase.hh"

void
ModuleDatabase::deleteNamedModules()
{
  //
  //	We can't do this in our dtor because we don't know what
  //	data structures may have already be destroyed.
  //
  const ModuleMap::const_iterator e = moduleMap.end();
  for (ModuleMap::const_iterator i = moduleMap.begin(); i != e; ++i)
    delete i->second;
}

bool
ModuleDatabase::insertModule(int name, PreModule* module)
{
  pair<ModuleMap::iterator, bool> p = moduleMap.insert(ModuleMap::value_type(name, module));
  if (p.second)
    return false;
  IssueAdvisory("redefining module " << QUOTE(module) << '.');
  delete p.first->second;
  p.first->second = module;
  return true;
}

PreModule*
ModuleDatabase::getModule(int name) const
{
  const ModuleMap::const_iterator t = moduleMap.find(name);
  return (t == moduleMap.end()) ? 0 : t->second;
}

bool
ModuleDatabase::deleteModule(int name)
{
  const ModuleMap::iterator t = moduleMap.find(name);
  if (t == moduleMap.end())
    return false;
  delete t->second;
  moduleMap.erase(t);
  return true;
}

void
ModuleDatabase::setAutoImport(ImportModule::ImportMode importMode,
			      Token name,
			      bool polarity)
{
  int code = name.code();
  if (polarity)
    autoImports[code] = importMode;
  else
    autoImports.erase(code);
}

void
ModuleDatabase::setOmodInclude(Token name, bool polarity)
{
  int code = name.code();
  if (polarity)
    defaultOmodIncludes.insert(code);
  else
    defaultOmodIncludes.erase(code);
}

void
ModuleDatabase::showNamedModules(ostream& s) const
{
  FOR_EACH_CONST(i, ModuleMap, moduleMap)
    {
      PreModule* m = i->second;
      s << MixfixModule::moduleTypeString(m->getModuleType()) << ' ' << m << '\n';
    }
}
