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
#include "viewExpression.hh"
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

ModuleExpression::ModuleExpression(ModuleExpression* module, const Vector<ViewExpression*>& arguments)
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
      {
	module->deepSelfDestruct();
	delete renaming;
	break;
      }
    case INSTANTIATION:
      {
	module->deepSelfDestruct();
	for (ViewExpression* v : arguments)
	  v->deepSelfDestruct();
	break;
      }
    case SUMMATION:
      {
	for (ModuleExpression* m : modules)
	  m->deepSelfDestruct();
	break;
      }
    case MODULE:
      break;  // nothing to delete - avoid compiler warning
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
	for (ModuleExpression* m :  modules)
	  {
	    s << sep << m;
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
	const Vector<ViewExpression*>& arguments = expr->getArguments();
	const Vector<ViewExpression*>::const_iterator e = arguments.end();
	for (Vector<ViewExpression*>::const_iterator i = arguments.begin();;)
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

void
ModuleExpression::latexPrint(ostream& s, const Module* enclosingModule) const
{
  switch (type)
    {
    case MODULE:
      {
	s << "\\maudeModule{" << Token::latexName(moduleName.code()) << "}";
	break;
      }
    case SUMMATION:
      {
	const char* sep = "";
	for (ModuleExpression* m :  modules)
	  {
	    s << sep;
	    m->latexPrint(s);
	    sep = "\\maudeSummation";
	  }
	break;
      }
    case RENAMING:
      {
	bool parensNeeded =  (module->getType() == ModuleExpression::SUMMATION);
	if (parensNeeded)
	  s << "\\maudeLeftParen";
	module->latexPrint(s, enclosingModule);
	if (parensNeeded)
	  s << "\\maudeRightParen";
	s << "\\maudeRenaming" << renaming->latexRenaming("\\maudeLeftParen", "\\maudeComma", nullptr, enclosingModule, true) <<
	  "\\maudeRightParen";
	break;
      }
    case INSTANTIATION:
      {
	bool parensNeeded = (module->getType() == ModuleExpression::SUMMATION || module->getType() == ModuleExpression::RENAMING);
	if (parensNeeded)
	  s << "\\maudeLeftParen";
	module->latexPrint(s, enclosingModule);
	if (parensNeeded)
	    s << "\\maudeRightParen";
	s << "\\maudeLeftBrace";
	const char* sep = "";
	for (const ViewExpression* ve : arguments)
	  {
	    s << sep;
	    sep = "\\maudeComma";
	    ve->latexPrint(s, enclosingModule);
	  }
	s << "\\maudeRightBrace";
	break;
      }
    default:
      //
      //	We don't expect to encounter parameter copies.
      //
      CantHappen("not implemented");
    }
}
