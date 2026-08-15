/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for view expressions.
//
//	Parameters from an enclosing module or view are also stored
//	using this class since they have the same syntactic appearance
//	as unparameterized named views, and can only be distinguished from
//	them by context.
//
#ifndef _viewExpression_hh_
#define _viewExpression_hh_
#include "token.hh"

class ViewExpression
{
public:
  enum Type
  {
    SIMPLE_NAME,
    INSTANTIATION,
    TRANSFORMATION
  };
  //
  //	A view expression is the name of a view or parameter:
  //
  ViewExpression(Token name);
  //
  //	OR a view expression with an instantiation which is
  //	a vector of view expressions:
  //
  ViewExpression(ViewExpression* view, const Vector<ViewExpression*>& arguments);
  //
  //	OR a call to a user-defined view transformer with a
  //	a vector of input modules and a vector of options:
  //
  ViewExpression(Token name,
		 const Vector<ModuleExpression*>& inputModules,
		 const Vector<int>& options);

  Type getType() const;
  //
  //	For the simple view and transformer cases only.
  //
  Token getName() const;
  //
  //	For the instantiation case only.
  //
  ViewExpression* getView() const;
  const Vector<ViewExpression*>& getArguments() const;
  //
  //	For the transformer case only.
  //
  const Vector<ModuleExpression*>& getInputModules() const;
  const Vector<int>& getOptions() const;

  void deepSelfDestruct();
  void latexPrint(ostream& s, const Module* enclosingModule = nullptr) const;

private:
  const Type type;
  //
  //	This is a view name, a parameter name or the name of the module
  //	defining a view transformer.
  //
  Token name;
  //
  //	For the instantiation case only.
  //
  ViewExpression* view;
  Vector<ViewExpression*> arguments;
  //
  //	For the transformer case only.
  //
  Vector<ModuleExpression*> inputModules;
  Vector<int> options;
};

inline ViewExpression::Type
ViewExpression::getType() const
{
  return type;
}

ostream& operator<<(ostream& s, const ViewExpression* expr);

inline Token
ViewExpression::getName() const
{
  Assert(type == SIMPLE_NAME || type == TRANSFORMATION, "instantiation " << this);
  return name;
}

inline ViewExpression*
ViewExpression::getView() const
{
  Assert(type == INSTANTIATION, "not an instantiation " << this);
  return view;
}

inline const Vector<ViewExpression*>&
ViewExpression::getArguments() const
{
  Assert(type == INSTANTIATION, "not an instantiation " << this);
  return arguments;
}

inline const Vector<ModuleExpression*>&
ViewExpression::getInputModules() const
{
  Assert(type == TRANSFORMATION, "not a transformation");
  return inputModules;
}

inline const Vector<int>&
ViewExpression::getOptions() const
{
  Assert(type == TRANSFORMATION, "not a transformation");
  return options;
}

#endif
