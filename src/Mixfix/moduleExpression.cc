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
//      Implementation for class ModuleExpression.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "mixfix.hh"

//	front end class definitions
#include "token.hh"
#include "renaming.hh"
#include "moduleExpression.hh"

ModuleExpression::ModuleExpression(Token moduleName)
 : type(MODULE),
   moduleName(moduleName)
{
}

ModuleExpression::ModuleExpression(ModuleExpression* left, ModuleExpression* right)
 : type(SUMMATION)
{
  if (right->type == SUMMATION)
    {
      modules.swap(right->modules);
      delete right;
    }
  else
    modules.push_front(right);

  if (left->type == SUMMATION)
    {
      modules.splice(modules.begin(), left->modules);
      delete left;
    }
  else
    modules.push_front(left);
}

ModuleExpression::ModuleExpression(ModuleExpression* module, Renaming* renaming)
 : type(RENAMING),
   module(module),
   renaming(renaming)
{
}

ModuleExpression::ModuleExpression(ModuleExpression* module, const Vector<Token>& arguments)
 : type(INSTANTIATION),
   module(module),
   arguments(arguments)
{
}

void
ModuleExpression::deepSelfDestruct()
{
  switch (type)
    {
    case RENAMING:
      delete renaming;
      // fall thru
    case INSTANTIATION:
      module->deepSelfDestruct();
      break;
    case SUMMATION:
      {
	FOR_EACH_CONST(i, list<ModuleExpression*>, modules)
	  (*i)->deepSelfDestruct();
	break;
      }
    }
  delete this;
}

ostream&
operator<<(ostream& s, const ModuleExpression* expr)
{
  switch (expr->getType())
    {
    case ModuleExpression::MODULE:
      {
	s << expr->getModuleName();
	break;
      }
    case ModuleExpression::SUMMATION:
      {
	const list<ModuleExpression*>& modules = expr->getModules();
	const char* sep = "";
	FOR_EACH_CONST(i, list<ModuleExpression*>, modules)
	  {
	    s << sep << *i;
	    sep = " + ";
	  }
	break;
      }
    case ModuleExpression::RENAMING:
      {
	const ModuleExpression* module = expr->getModule();
	if (module->getType() == ModuleExpression::SUMMATION)
	  s << '(' << module << ')';
	else
	  s << module;
	s << " * " << expr->getRenaming();
	break;
      }
    case ModuleExpression::INSTANTIATION:
      {
	const ModuleExpression* module = expr->getModule();
	if (module->getType() == ModuleExpression::SUMMATION ||
	    module->getType() == ModuleExpression::RENAMING)
	  s << '(' << module << "){";
	else
	  s << module << '{';
	const Vector<Token>& arguments = expr->getArguments();
	const Vector<Token>::const_iterator e = arguments.end();
	for (Vector<Token>::const_iterator i = arguments.begin();;)
	  {
	    s << *i;
	    ++i;
	    if (i == e)
	      break;
	    s  << ", ";
	  }
	s << '}';
	break;
      }
    default:
      CantHappen("not implemented");
    }
  return s;
}
