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
//      Class to hold database of modules.
//
#ifndef _moduleDatabase_hh_
#define _moduleDatabase_hh_
#include <map>
#include "intSet.hh"

class ModuleDatabase
{
  NO_COPYING(ModuleDatabase);

public:
  ModuleDatabase(){}
  ~ModuleDatabase();

  bool insertModule(int name, PreModule* module);  // true if existing module displaced
  PreModule* getModule(int name) const;  // 0 if doesn't exist
  bool deleteModule(int name);  // true if module deleted

  void setInclude(Token name, bool polarity);
  void setOmodInclude(Token name, bool polarity);
  const IntSet& getIncludes() const;
  const IntSet& getOmodIncludes() const;
  void showNamedModules() const;

private:
  typedef map<int, PreModule*> ModuleMap;

  ModuleMap moduleMap;
  IntSet defaultIncludes;
  IntSet defaultOmodIncludes;
};

inline const IntSet&
ModuleDatabase::getIncludes() const
{
  return defaultIncludes;
}

inline const IntSet&
ModuleDatabase::getOmodIncludes() const
{
  return defaultOmodIncludes;
}

#endif
