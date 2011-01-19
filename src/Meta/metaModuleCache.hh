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
//      Class for cache of MetaModules.
//
#ifndef _metaModuleCache_hh_
#define _metaModuleCache_hh_
#include "importModule.hh"

class MetaModuleCache : public Entity::User
{
  NO_COPYING(MetaModuleCache);

public:
  MetaModuleCache();
  ~MetaModuleCache();

  MetaModule* find(DagNode* dag);
  void insert(DagNode* dag, MetaModule* module);
  void flush();

private:
  enum Sizes
    {
      DEFAULT_MAX_SIZE = 4, //  this is the normal maximum size of the cache
      MIN_MAX_SIZE = 1,  // this is the smallest custom maximum size allowed
      MAX_MAX_SIZE = 1024  // this is the largest custom maximum size allowed
    };

  struct Pair
  {
    void clear();

    DagRoot* dag;
    MetaModule* module;
  };

  void regretToInform(Entity* doomedEntity);
  MetaModule* moveToFront(int chosen);

  static int maxSize;
  Vector<Pair> cache;
};

#endif
