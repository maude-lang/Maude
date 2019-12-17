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
//      Class to hold cache of views that are created for view expressions.
//
#ifndef _viewCache_hh_
#define _viewCache_hh_
#include <map>
#include "view.hh"

class ViewCache : public Entity::User
{
  NO_COPYING(ViewCache);

public:
  ViewCache();
  View* makeViewInstantiation(View* view, const Vector<Argument*>& arguments);

  void destructUnusedViews();
  void showCreatedViews(ostream& s) const;

private:
  typedef map<int, View*> ViewMap;

  static bool viewCompare(const View* v1, const View* v2);

  void regretToInform(Entity* doomedEntity);

  ViewMap viewMap;
};

#endif
