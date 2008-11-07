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
//	Class for keeping track of metadata attached to ModuleItems.
//
#ifndef _metadataStore_hh_
#define _metadataStore_hh_
#include <map>
#include "printAttribute.hh"

class MetadataStore
{
public:
  enum ItemType
  {
    MEMB_AX = 0x10000000,
    EQUATION = 0x20000000,
    RULE = 0x30000000
  };

  void insertMetadata(ItemType itemType, const ModuleItem* item, int metadata);
  int getMetadata(ItemType itemType, const ModuleItem* item) const;

  void insertMetadata(ModuleItem* symbol, int opDeclIndex, int metadata);
  int getMetadata(ModuleItem* symbol, int opDeclIndex) const;

  void insertPrintAttribute(ItemType itemType,
			    const PreEquation* statement,
			    const Vector<int>& names,
			    const Vector<Sort*>& sorts);
  const PrintAttribute* getPrintAttribute(ItemType itemType, const ModuleItem* item) const;

private:
  typedef map<Int64, int> MetadataMap;
  typedef map<Int64, PrintAttribute> PrintAttributeMap;

  MetadataMap metadataMap;
  PrintAttributeMap printAttributeMap;
};

#endif
