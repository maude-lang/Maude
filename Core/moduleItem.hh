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
//	Class for objects that are numbered within a module.
//	This provides with
//	  (1) a way of getting back to the module containing an object; and
//	  (2) a number that is useful for indexing.
//
#ifndef _moduleItem_hh_
#define _moduleItem_hh_

class ModuleItem
{
public:
  void setModuleInfo(Module* module, int indexWithinModule);
  Module* getModule() const;
  int getIndexWithinModule() const;

private:
  Module* parentModule;
  int indexWithinParent;
};

inline void
ModuleItem::setModuleInfo(Module* module, int indexWithinModule)
{
  parentModule = module;
  indexWithinParent = indexWithinModule;
}

inline Module*
ModuleItem::getModule() const
{
  return parentModule;
}

inline int
ModuleItem::getIndexWithinModule() const
{
  return indexWithinParent;
}

#endif
