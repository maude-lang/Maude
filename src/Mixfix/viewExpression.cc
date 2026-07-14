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
#include "importModule.hh"
#include "moduleExpression.hh"
#include "viewExpression.hh"

ViewExpression::ViewExpression(Token name)
  : type(SIMPLE_NAME),
    name(name)
{
}

ViewExpression::ViewExpression(ViewExpression* view, const Vector<ViewExpression*>& arguments)
  : type(INSTANTIATION),
    view(view),
    arguments(arguments)
{
}

ViewExpression::ViewExpression(Token name,
			       const Vector<ModuleExpression*>& inputModules,
			       const Vector<int>& options)
  : type(GENERATION),
    name(name),
    inputModules(inputModules),
    options(options)
{
}

void
ViewExpression::deepSelfDestruct()
{
  switch (type)
    {
    case INSTANTIATION:
      {
	view->deepSelfDestruct();
	for (ViewExpression* v : arguments)
	  v->deepSelfDestruct();
	break;
      }
    case GENERATION:
      {
	for (ModuleExpression* m : inputModules)
	  m->deepSelfDestruct();
	break;
      }
    case SIMPLE_NAME:
      break;  // nothing to delete - avoid compiler warning
    }
  delete this;
}

ostream&
operator<<(ostream& s, const ViewExpression* expr)
{
  switch (expr->getType())
    {
    case ViewExpression::SIMPLE_NAME:
      {
	s << expr->getName();
	break;
      }
    case ViewExpression::INSTANTIATION:
      {
	s << expr->getView() << '{';
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
    case ViewExpression::GENERATION:
      {
	const Vector<ModuleExpression*>& inputModules = expr->getInputModules();
	const Vector<int>& options = expr->getOptions();
	//
	//	Generator specification.
	//
	s << expr->getName();
	//
	//	Input modules.
	//
	if (!inputModules.empty())
	  {
	    const char* sep = "[";
	    for (ModuleExpression* m :  inputModules)
	      {
		s << sep << m;
		sep = ", ";
	      }
	    s << ']';
	  }
	//
	//	Options.
	//
	if (!options.empty() || inputModules.empty())
	  {
	    s << '(';
	    const char* sep = "";
	    for (int v : options)
	      {
		s << sep << Token::name(v);
		sep = " ";
	      }
	    s << ')';
	  }
	break;
      }
    }
  return s;
}

void
ViewExpression::latexPrint(ostream& s, const Module* enclosingModule) const
{
    switch (type)
    {
    case ViewExpression::SIMPLE_NAME:
      {
	//
	//	If we don't have an enclosing module, a name cannot be a parameter.
	//
	int code = name.code();
	if (enclosingModule == nullptr ||
	    (safeCastNonNull<const ImportModule*>(enclosingModule)->findParameterIndex(code) == NONE))
	  s << "\\maudeView{";
	else
	  s << "\\maudeParameter{";
	s << Token::latexName(code);
	s << "}";
	break;
      }
    case ViewExpression::INSTANTIATION:
      {
	view->latexPrint(s,  enclosingModule);
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
    case ViewExpression::GENERATION:
      {
	//
	//	Transformer specification.
	//
	s << "\\maudeModule{" << Token::latexName(name.code()) << "}";
	//
	//	Input modules.
	//
	if (!inputModules.empty())
	  {
	    const char* sep = "\\maudeLeftBracket ";
	    for (ModuleExpression* m : inputModules)
	      {
		s << sep;
		m->latexPrint(s, enclosingModule);
		sep = "\\maudeComma ";
	      }
	    s << "\\maudeRightBracket";
	  }
	//
	//	Options.
	//
	if (!options.empty() || inputModules.empty())
	  {
	    s << "\\maudeLeftParen";
	    const char* sep = "";
	    for (int v : options)
	      {
		s << sep << "\\maudeQid{" << Token::latexName(v) << "}";
		sep = "\\maudeSpace";
	      }
	    s << "\\maudeRightParen";
	  }
	break;
      }
    }
}
