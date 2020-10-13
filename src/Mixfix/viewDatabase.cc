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
//      Implementation for class ViewDatabase.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//	front end class definitions
#include "view.hh"
#include "viewDatabase.hh"

ViewDatabase::~ViewDatabase()
{
  //
  //	Cleaning up makes for a slow exit, but is essential now that
  //	interpreters can be created and destroyed on-the-fly.
  //
  const ViewMap::const_iterator e = viewMap.end();
  for (ViewMap::const_iterator i = viewMap.begin(); i != e; ++i)
    delete i->second;
}

bool
ViewDatabase::insertView(int name, View* view)
{
  pair<ViewMap::iterator, bool> p = viewMap.insert(ViewMap::value_type(name, view));
  if (p.second)
    return false;
  IssueAdvisory("redefining view " << QUOTE(static_cast<NamedEntity*>(view)) << '.');
  delete p.first->second;
  p.first->second = view;
  return true;
}

View*
ViewDatabase::getView(int name) const
{
  const ViewMap::const_iterator t = viewMap.find(name);
  return (t == viewMap.end()) ? 0 : t->second;
}

bool
ViewDatabase::deleteView(int name)
{
  const ViewMap::iterator t = viewMap.find(name);
  if (t == viewMap.end())
    return false;
  delete t->second;
  viewMap.erase(t);
  return true;
}

void
ViewDatabase::showNamedViews(ostream& s) const
{
  FOR_EACH_CONST(i, ViewMap, viewMap)
    s << "view " << static_cast<NamedEntity*>(i->second) << '\n';
}
