/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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

ViewExpression::ViewExpression(Token name)
 : name(name)
{
}

ViewExpression::ViewExpression(ViewExpression* view, const Vector<ViewExpression*>& arguments)
 : view(view),
   arguments(arguments)
{
}

void
ViewExpression::deepSelfDestruct()
{
  if (isInstantiation())
    {
      view->deepSelfDestruct();
      for (ViewExpression* v : arguments)
	v->deepSelfDestruct();
    }
  delete this;
}

ostream&
operator<<(ostream& s, const ViewExpression* expr)
{
  if (expr->isInstantiation())
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
    }
  else
    s << expr->getName();
  return s;
}
