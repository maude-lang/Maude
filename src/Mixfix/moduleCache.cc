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
//      Implementation for class MetaModuleCache.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "mixfix.hh"

//	front end class definitions
#include "renaming.hh"
#include "moduleCache.hh"

ModuleCache::ModuleCache()
{
}

void
ModuleCache::regretToInform(ImportModule* doomedModule)
{
  ModuleMap::iterator pos = moduleMap.find(doomedModule->id());
  Assert(pos != moduleMap.end(), "could find self-destructing module");
  DebugAdvisory("removing module " << doomedModule << " from cache");
  moduleMap.erase(pos);
}

ImportModule*
ModuleCache::makeRenamedCopy(ImportModule* module, Renaming* renaming)
{
  //
  //	Step 1
  //	Build a  canonical renaming based on modules signature.
  //	If empty, just return ourselves.
  //
  Renaming* canonical = renaming->makeCanonicalVersion(module);
  if (canonical == 0)
    return module;
  //
  //	Build the canonical name of the module we want.
  //
  crope name(Token::name(module->id()));
  name += " * (";
  name += canonical->makeCanonicalName() + ")";
  int t = Token::encode(name.c_str());
  //
  //	Check if we already have a renamed copy in cache.
  //	If so, just return it.
  //
  ModuleMap::const_iterator c = moduleMap.find(t);
  if (c != moduleMap.end())
    {
      DebugAdvisory("using existing copy of " << name);
      delete canonical;
      return c->second;
    }
  //
  //	Create new module; and insert it in cache.
  //
  DebugAdvisory("making " << name);
  ImportModule* copy = module->makeRenamedCopy(t, canonical, this);
  moduleMap[t] = copy;
  return copy;
}

bool
ModuleCache::moduleCompare(const ImportModule* m1, const ImportModule* m2)
{
  return m1->id() < m2->id();
}

ImportModule*
ModuleCache::makeSummation(const Vector<ImportModule*>& modules)
{
  //
  //	Sort and delete duplicates.
  //
  Vector<ImportModule*> local(modules);
  sort(local.begin(), local.end(), moduleCompare);
  const Vector<ImportModule*>::const_iterator e = unique(local.begin(), local.end());
  if (e - local.begin() == 1)
    return local[0];
  //
  //	Make name.
  //
  crope name;
  for (Vector<ImportModule*>::const_iterator i = local.begin(); i != e; ++i)
    {
      name += name.empty() ? "(" : " + ";
      name += Token::name((*i)->id());
    }
  name += ')';
  int t = Token::encode(name.c_str());
  //
  //	Check if it is already in cache.
  //
  ModuleMap::const_iterator c = moduleMap.find(t);
  if (c != moduleMap.end())
    {
      DebugAdvisory("using existing copy of " << name);
      return c->second;
    }
  //
  //	Otherwise build it.
  //
  DebugAdvisory("making " << name);

  Vector<ImportModule*>::const_iterator i = local.begin();
  MixfixModule::ModuleType moduleType = (*i)->getModuleType();
  for (++i; i != e; ++i)
    moduleType = MixfixModule::join(moduleType, (*i)->getModuleType());
  ImportModule* sum = new ImportModule(t, moduleType, this);
  for (Vector<ImportModule*>::const_iterator i = local.begin(); i != e; i++)
    sum->addImport(*i);

  sum->importSorts();
  sum->closeSortSet();
  
  sum->importOps();
  sum->closeSignature();

  sum->fixUpImportedOps();
  sum->closeFixUps();
  //
  //	We have no local statements.
  //
  sum->localStatementsComplete();
  //
  //	Reset phase counter in each imported module and return sum.
  //
  sum->resetImports();
  moduleMap[t] = sum;
  return sum;
}

void
ModuleCache::destructUnusedModules()
{
  //
  //	This O(n^2) solution to finding unused cached modules is slow but
  //	simple. If the number of cached modules grows beyond a few hundred
  //	a more complex O(n) solution based on keeping a linked list of
  //	candidates would be appropriate. We would need a call back from
  //	ImportModule to tell us when a module loses its last dependent.
  //
 restart:
  {
    FOR_EACH_CONST(i, ModuleMap, moduleMap)
      {
	if (!(i->second->hasDependents()))
	  {
	    DebugAdvisory("module " << i->second << " has no dependents");
	    i->second->deepSelfDestruct();  // invalidates i
	    goto restart;
	  }    
      }
  }
}

void
ModuleCache::showCreatedModules() const
{
  FOR_EACH_CONST(i, ModuleMap, moduleMap)
    {
      MixfixModule* m = i->second;
      cout << ((m->getModuleType() == MixfixModule::SYSTEM_MODULE) ? "mod " : "fmod ") <<
	m << '\n';
    }
}
