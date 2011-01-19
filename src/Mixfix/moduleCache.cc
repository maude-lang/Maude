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
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	front end class definitions
#include "renaming.hh"
#include "view.hh"
#include "moduleCache.hh"
#include "fileTable.hh"

ModuleCache::ModuleCache()
{
}

void
ModuleCache::regretToInform(Entity* doomedEntity)
{
  ImportModule* doomedModule = static_cast<ImportModule*>(doomedEntity);
  ModuleMap::iterator pos = moduleMap.find(doomedModule->id());
  Assert(pos != moduleMap.end(), "could find self-destructing module " << doomedModule);
  DebugAdvisory("removing module " << doomedModule << " from cache");
  moduleMap.erase(pos);
}

ImportModule*
ModuleCache::makeRenamedCopy(ImportModule* module, Renaming* renaming)
{
  //
  //	Step 1
  //	Build a canonical renaming based on modules signature.
  //	If empty, just return ourselves.
  //
  Renaming* canonical = renaming->makeCanonicalVersion(module);
  if (canonical == 0)
    return module;
  //
  //	Build the canonical name of the module we want.
  //
  crope name;
  if (module->getOrigin() == ImportModule::SUMMATION)
    name = '(';
  name += Token::name(module->id());
  if (module->getOrigin() == ImportModule::SUMMATION)
    name += ')';
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
  DebugAdvisory("making renamed copy " << name);
  ImportModule* copy = module->makeRenamedCopy(t, canonical, this);
  DebugAdvisory("finish renamed copy " << name);

  if (copy->isBad())
    {
      //
      //	Renaming  a good module can produce a bad module.
      //
      //	We never want to cache bad modules.
      //
      IssueAdvisory(*copy << ": unable to make renamed module " << QUOTE(name) <<
		    " due to earlier errors.");
      copy->removeUser(this);  // since we are not adding a bad module to the cache
      copy->deepSelfDestruct();
      return 0;
    }
  moduleMap[t] = copy;
  return copy;
}

ImportModule*
ModuleCache::makeParameterCopy(int parameterName, ImportModule* module)
{
  //
  //	Make the name of the module we want.
  //
  crope name(Token::name(parameterName));
  name += " :: ";
  name += Token::name(module->id());
  int t = Token::encode(name.c_str());
  //
  //	Check if we already have a parameter copy in cache.
  //	If so, just return it.
  //
  ModuleMap::const_iterator c = moduleMap.find(t);
  if (c != moduleMap.end())
    {
      DebugAdvisory("using existing copy of " << name);
      return c->second;
    }
  //
  //	Create new module; and insert it in cache.
  //
  DebugAdvisory("making parameter copy " << name);
  ImportModule* copy = module->makeParameterCopy(t, parameterName, this);
  DebugAdvisory("finished parameter copy " << name);

  Assert(!(copy->isBad()), "bad parameter copy");
  moduleMap[t] = copy;
  return copy;
}

ImportModule*
ModuleCache::makeInstatiation(ImportModule* module, const Vector<View*>& views, const Vector<int>& parameterArgs)
{
  //
  //	Make the name of the module we want.
  //
  crope name;
  if (module->getOrigin() == ImportModule::RENAMING)
    name = '(';
  name += Token::name(module->id());
  if (module->getOrigin() == ImportModule::RENAMING)
    name += ')';

  const char* sep = "{";
  int nrParameters = views.size();
  for (int i = 0; i < nrParameters; ++i)
    {
      name += sep;
      sep = ", ";
      View* v = views[i];
      if (v == 0)
	{
	  //
	  //	Place brackets around parameter arguments so that we don't confuse
	  //	them with views having the same name.
	  //
	  name += '[';
	  name += Token::name(parameterArgs[i]);
	  name += ']';
	}
      else
	name += Token::name(v->id());
    }
  name += "}";
  int t = Token::encode(name.c_str());
  //
  //	Check if we already have an instantiation in cache.
  //	If so, just return it.
  //
  ModuleMap::const_iterator c = moduleMap.find(t);
  if (c != moduleMap.end())
    {
      DebugAdvisory("using existing copy of " << name);
      return c->second;
    }
  //
  //	Create new module; and insert it in cache.
  //
  DebugAdvisory("making instantiation " << name);
  ImportModule* copy = module->makeInstantiation(t, views, parameterArgs, this);
  DebugAdvisory("finished instantiation " << name);

  if (copy->isBad())
    {
      //
      //	It is possible for the instantiation of a non-bad module to be bad;
      //	for example, by an attribute clash between an operator from the module
      //	and one from the to-module in the instantiation; or perhaps the
      //	instantiation of an import went bad.
      //
      //	We never want to cache bad modules.
      //
      IssueAdvisory(*copy << ": unable to make module instantiation " << QUOTE(name)
		    << " due to earlier errors.");
      copy->removeUser(this);  // since we are not adding a bad module to the cache
      copy->deepSelfDestruct();
      return 0;
    }
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
      if (!name.empty())
	name += " + ";
      name += Token::name((*i)->id());
    }
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
  DebugAdvisory("making summation " << name);

  Vector<ImportModule*>::const_iterator i = local.begin();
  MixfixModule::ModuleType moduleType = (*i)->getModuleType();
  for (++i; i != e; ++i)
    moduleType = MixfixModule::join(moduleType, (*i)->getModuleType());
  ImportModule* sum = new ImportModule(t, moduleType, ImportModule::SUMMATION, this);
  LineNumber lineNumber(FileTable::AUTOMATIC);
  for (Vector<ImportModule*>::const_iterator i = local.begin(); i != e; i++)
    sum->addImport(*i, ImportModule::INCLUDING, lineNumber);

  sum->importSorts();
  sum->closeSortSet();
  if (!(sum->isBad()))
    {
      sum->importOps();
      if (!(sum->isBad()))
	{
	  sum->closeSignature();
	  sum->fixUpImportedOps();
	  if (!(sum->isBad()))
	    { 
	      sum->closeFixUps();
	      //
	      //	We have no local statements.
	      //
	      sum->localStatementsComplete();
	    }
	}
    }
  //
  //	Reset phase counter in each imported module.
  //
  sum->resetImports();
  DebugAdvisory("finished summation " << name);

  if (sum->isBad())
    {
      //
      //	Summing good modules can produce a bad module.
      //
      //	We never want to cache bad modules.
      //
      IssueAdvisory(*sum << ": unable to make module summation " << QUOTE(name) <<
		    " due to earlier errors.");
      sum->removeUser(this);  // since we are not adding a bad module to the cache
      sum->deepSelfDestruct();
      return 0;
    }
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
  //	ImportModule to tell us when a module is down to 1 user (us!).
  //
 restart:
  {
    FOR_EACH_CONST(i, ModuleMap, moduleMap)
      {
	int nrUsers = i->second->getNrUsers();
	Assert(nrUsers >= 1, "no users");  // we are a user
	if (nrUsers == 1)
	  {
	    DebugAdvisory("module " << i->second << " has no other users");
	    i->second->deepSelfDestruct();  // invalidates i
	    goto restart;
	  }
      }
  }
}

void
ModuleCache::showCreatedModules(ostream& s) const
{
  FOR_EACH_CONST(i, ModuleMap, moduleMap)
    {
      MixfixModule* m = i->second;
      s << MixfixModule::moduleTypeString(m->getModuleType()) << ' ' << m << '\n';
    }
}
