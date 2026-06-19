/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for module expressions.
//
#ifndef _moduleExpression_hh_
#define _moduleExpression_hh_
#include <list>
#include "token.hh"

class ModuleExpression
{
public:
  enum Type
  {
    MODULE,
    SUMMATION,
    RENAMING,
    INSTANTIATION,
    TRANSFORM
  };

  ModuleExpression(Token moduleName);
  ModuleExpression(ModuleExpression* left, ModuleExpression* right);
  ModuleExpression(ModuleExpression* module, Renaming* renaming);
  ModuleExpression(ModuleExpression* module, const Vector<ViewExpression*>& arguments);
  ModuleExpression(ModuleExpression* transformer,
		   const int opName,
		   const Vector<ModuleExpression*>& inputModules,
		   const Vector<Token>& arguments);

  Type getType() const;
  Token getModuleName() const;
  const list<ModuleExpression*>& getModules() const;
  ModuleExpression* getModule() const;
  Renaming* getRenaming() const;
  const Vector<ViewExpression*>& getArguments() const;

  int getOpName() const;
  const Vector<ModuleExpression*>& getInputModules() const;
  const Vector<Token>& getOptions() const;

  void deepSelfDestruct();

  void latexPrint(ostream& s, const Module* enclosingModule = nullptr) const;

private:
  const Type type;
  //
  //	For named module.
  //
  Token moduleName;
  //
  //	For summation.
  //
  list<ModuleExpression*> modules;
  //
  //	For renaming, instantiation and transform.
  //
  ModuleExpression* module;
  //
  //	For renaming.
  //
  Renaming* renaming;
  //
  //	For instantiation.
  //
  Vector<ViewExpression*> arguments;
  //
  //	For transform.
  //
  int opName;
  Vector<ModuleExpression*> inputModules;
  Vector<Token> options;
};

ostream& operator<<(ostream& s, const ModuleExpression* expr);

inline ModuleExpression::Type
ModuleExpression::getType() const
{
  return type;
}

inline Token
ModuleExpression::getModuleName() const
{
  Assert(type == MODULE, "not a named module");
  return moduleName;
}


inline const list<ModuleExpression*>&
ModuleExpression::getModules() const
{
  Assert(type == SUMMATION, "not a summation");
  return modules;
}

inline ModuleExpression*
ModuleExpression::getModule() const
{
  Assert(type == RENAMING || type == INSTANTIATION || type == TRANSFORM,
	 "not a renaming, instantiation or transform");
  return module;
}

inline Renaming*
ModuleExpression::getRenaming() const
{
  Assert(type == RENAMING, "not a renaming");
  return renaming;
}

inline const Vector<ViewExpression*>&
ModuleExpression::getArguments() const
{
  Assert(type == INSTANTIATION, "not instantiation");
  return arguments;
}

inline int
ModuleExpression::getOpName() const
{
  Assert(type == TRANSFORM, "not a transform");
  return opName;
}

inline const Vector<ModuleExpression*>&
ModuleExpression::getInputModules() const
{
  Assert(type == TRANSFORM, "not a transform");
  return inputModules;
}

inline const Vector<Token>&
ModuleExpression::getOptions() const
{
  Assert(type == TRANSFORM, "not a transform");
  return options;
}

#endif
