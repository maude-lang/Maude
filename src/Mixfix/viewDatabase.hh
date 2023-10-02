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
//      Class to hold database of views.
//
#ifndef _viewDatabase_hh_
#define _viewDatabase_hh_
#include <map>

class ViewDatabase
{
  NO_COPYING(ViewDatabase);

public:
  ViewDatabase(){}
  ~ViewDatabase();

  bool insertView(int name, View* view);  // true if existing view displaced
  View* getView(int name) const;  // 0 if doesn't exist
  bool deleteView(int name);  // true if view deleted

  void showNamedViews(ostream& s) const;
  void latexShowNamedViews(ostream& s) const;

private:
  typedef map<int, View*> ViewMap;

  ViewMap viewMap;
};

#endif
