/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2023 SRI International, Menlo Park, CA 94025, USA.

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
  //
  //	A view expression is the name of a view or parameter:
  //
  ViewExpression(Token name);
  //
  //	OR a view expression with an instantiation which is
  //	a vector of view expressions:
  //
  ViewExpression(ViewExpression* view, const Vector<ViewExpression*>& arguments);

  bool isInstantiation() const;
  //
  //	For the non-instantiation case only.
  //
  Token getName() const;
  //
  //	For the instantiation case only.
  //
  ViewExpression* getView() const;
  const Vector<ViewExpression*>& getArguments() const;
  void deepSelfDestruct();
  void latexPrint(ostream& s, const Module* enclosingModule = nullptr) const;

private:
  //
  //	For the non-instantiation case only.
  //
  Token name;
  //
  //	For the instantiation case only.
  //
  ViewExpression* view;
  Vector<ViewExpression*> arguments;
};

ostream& operator<<(ostream& s, const ViewExpression* expr);

inline bool
ViewExpression::isInstantiation() const
{
  return !(arguments.isNull());
}

inline Token
ViewExpression::getName() const
{
  Assert(!isInstantiation(), "instantiation, not named view/parameter " << this);
  return name;
}

inline ViewExpression*
ViewExpression::getView() const
{
  Assert(isInstantiation(), "named view/parameter not instantiation " << this);
  return view;
}

inline const Vector<ViewExpression*>&
ViewExpression::getArguments() const
{
  Assert(isInstantiation(), "named view/parameter not instantiation " << this);
  return arguments;
}

#endif
