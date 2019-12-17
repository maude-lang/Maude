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
//      Abstract base class for objects that receive syntax from surface parser.
//
#ifndef _syntaxContainer_hh_
#define _syntaxContainer_hh_
#include "token.hh"

class SyntaxContainer
{
public:
  //
  //	Subclasses SyntacticPreModule and SyntacticView recieve parameters and
  //	variable declarations.
  //
  virtual void addParameter2(Token name, ModuleExpression* theory) = 0;
  virtual void addVarDecl(Token varName) = 0;
  //
  //	Subclasses SyntacticPreModule, SyntacticView and Renaming receive
  //	types.
  //
  virtual void addType(bool kind, const Vector<Token>& tokens) = 0;

protected:
  struct Type
  {
    bool kind;
    Vector<Token> tokens;
  };

  friend ostream& operator<<(ostream& s, const Type& type);
};

#endif
