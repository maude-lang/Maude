/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to hold database of modules.
//
#ifndef _moduleDatabase_hh_
#define _moduleDatabase_hh_
#include <map>
#include <set>
#include "importModule.hh"

class ModuleDatabase
{
  NO_COPYING(ModuleDatabase);

public:
  typedef map<int, ImportModule::ImportMode> ImportMap;

  ModuleDatabase(){}

  bool insertModule(int name, PreModule* module);  // true if existing module displaced
  PreModule* getModule(int name) const;  // 0 if doesn't exist
  bool deleteModule(int name);  // true if module deleted
  void deleteNamedModules();
  void setAutoImport(ImportModule::ImportMode importMode, Token name, bool polarity);
  void setOoInclude(Token name, bool polarity);
  const ImportMap& getAutoImports() const;
  const ImportMap& getOoIncludes() const;
  void showNamedModules(ostream& s) const;

private:
  typedef map<int, PreModule*> ModuleMap;

  ModuleMap moduleMap;
  ImportMap autoImports;
  ImportMap defaultOoIncludes;
};

inline const ModuleDatabase::ImportMap&
ModuleDatabase::getAutoImports() const
{
  return autoImports;
}

inline const ModuleDatabase::ImportMap&
ModuleDatabase::getOoIncludes() const
{
  return defaultOoIncludes;
}

#endif
