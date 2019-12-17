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
//      Implementation for class MetadataStore.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
//#include "variable.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
 
//      core class definitions
#include "preEquation.hh"

//	front end class definitions
#include "metadataStore.hh"

void
MetadataStore::insertMetadata(ItemType itemType, const ModuleItem* item, int metadata)
{
  if (metadata != NONE)
    metadataMap[itemType | item->getIndexWithinModule()] = metadata;
}

int
MetadataStore::getMetadata(ItemType itemType, const ModuleItem* item) const
{
  MetadataMap::const_iterator i = metadataMap.find(itemType | item->getIndexWithinModule());
  if (i == metadataMap.end())
    return NONE;
  return (*i).second;
}

void
MetadataStore::insertMetadata(ModuleItem* symbol, int opDeclIndex, int metadata)
{
  if (metadata != NONE)
    metadataMap[(static_cast<Int64>(opDeclIndex) << 32) | symbol->getIndexWithinModule()] = metadata; 
}

int
MetadataStore::getMetadata(ModuleItem* symbol, int opDeclIndex) const
{
  MetadataMap::const_iterator i = metadataMap.find((static_cast<Int64>(opDeclIndex) << 32) | symbol->getIndexWithinModule());
  if (i == metadataMap.end())
    return NONE;
  return (*i).second;
}

void
MetadataStore::insertPrintAttribute(ItemType itemType,
				    const PreEquation* statement,
				    const Vector<int>& names,
				    const Vector<Sort*>& sorts)
{
  printAttributeMap[itemType | statement->getIndexWithinModule()].fillOut(*statement, names, sorts);
}


const PrintAttribute*
MetadataStore::getPrintAttribute(ItemType itemType, const ModuleItem* item) const
{
  PrintAttributeMap::const_iterator i = printAttributeMap.find(itemType | item->getIndexWithinModule());
  if (i == printAttributeMap.end())
    return 0;
  return &(*i).second;
}
