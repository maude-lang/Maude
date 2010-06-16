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
//      Abstract base class for module descriptions that can produce actual modules.
//
#ifndef _preModule_hh_
#define _preModule_hh_
#include "namedEntity.hh"
#include "mixfixModule.hh"
#include "moduleDatabase.hh"

class PreModule
  : public NamedEntity,
    public Entity::User
{
  NO_COPYING(PreModule);

public:
  PreModule(int moduleName, Interpreter* owner);
  virtual ~PreModule();

  Interpreter* getOwner() const;
  void setModuleType(MixfixModule::ModuleType type);
  MixfixModule::ModuleType getModuleType() const;


  virtual int getNrParameters() const = 0;
  virtual int getParameterName(int index) const = 0;
  virtual const ModuleExpression* getParameter(int index) const = 0;

  virtual const ModuleDatabase::ImportMap& getAutoImports() const = 0;
  virtual int getNrImports() const = 0;
  virtual int getImportMode(int index) const = 0;
  virtual const ModuleExpression* getImport(int index) const = 0;

  virtual VisibleModule* getFlatSignature() = 0;
  virtual VisibleModule* getFlatModule() = 0;

private:
  Interpreter* const owner;
  MixfixModule::ModuleType moduleType;
};

inline Interpreter*
PreModule::getOwner() const
{
  return owner;
}

inline void
PreModule::setModuleType(MixfixModule::ModuleType type)
{
  moduleType = type;
}

inline MixfixModule::ModuleType
PreModule::getModuleType() const
{
  return moduleType;
}

#ifndef NO_ASSERT
inline ostream&
operator<<(ostream& s, const PreModule* p)
{
  //
  //	Needed to avoid overload ambiguity between NamedEntity and Entity::User.
  //
  s << static_cast<const NamedEntity*>(p);
  return s;
}
#endif

#endif
