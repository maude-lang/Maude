/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for handling object-oriented renamings.
//

void
Renaming::addClassMapping(const Token& fromClass, const Token& toClass)
{
  classMap.append({fromClass, toClass});
}

void
Renaming::addAttrMapping(const Token& token)
{
  attrMap.push_back({token, IdSet(), token});
}

void
Renaming::addAttrType(const Vector<Token>& tokens)
{
  IdSet& type = attrMap.back().type;
  for (const Token& t : tokens)
    type.insert(t.code());
}

void
Renaming::addAttrTarget(const Token& token)
{
  attrMap.back().toAttr = token;
}

void
Renaming::convertClassMappings(ImportModule* module, Renaming* canonical) const
{
  //
  //	Check and convert class mappings.
  //
  Sort* classIdSort = 0;
  bool lookedForClassIdSort = false;
  for (auto& m : classMap)
    {
      DebugAdvisory("looking for class sort " << m.fromClass << " in module to be renamed " << module);
      int className = m.fromClass.code();
      int toClassName = m.toClass.code();
      if (className == toClassName)
	continue;  // skip identity mapping

      if (Sort* classSort = module->findSort(className))
	{
	  if (module->parameterDeclared(classSort))
	    {
	      IssueAdvisory(m.fromClass.getLineNr() << ": ignoring a class mapping for " << QUOTE(classSort) <<
			    "because the class sort comes from a parameter.");
	      continue;
	    }
	  //
	  //	Check if there is a Cid sort in the module being renamed and if the classSort is below it.
	  //
	  if (!lookedForClassIdSort)
	    {
	      //
	      //	Only want to try this one time.
	      //
	      classIdSort = module->findClassIdSort();
	      lookedForClassIdSort = true;
	    }
	  if (classIdSort != 0)
	    {
	      //
	      //	Because we have a Cid we can check that the class sort is below it.
	      //
	      if (classIdSort->component() != classSort->component() || !leq(classSort, classIdSort))
		{
		  IssueAdvisory(m.fromClass.getLineNr() << ": class mapping for " << QUOTE(classSort) <<
				" maps a sort that is not a subsort of " << QUOTE(classIdSort) << ".");
		}
	    }
	  //
	  //	Now check for the class constant.
	  //
	  int classSortId = classSort->id();
	  const static Vector<ConnectedComponent*> emptyDomain;
	  if (Symbol* classSymbol = module->findSymbol(classSortId, emptyDomain, classSort->component()))
	    {
	      if (module->parameterDeclared(classSymbol))
		{
		  IssueAdvisory(m.fromClass.getLineNr() << ": ignoring a class mapping for " << QUOTE(classSort) <<
				" because the class constant comes from a parameter.");
		  continue;
		}
	      //
	      //	There's a valid class constant so we need to make a mapping for it.
	      //
	      OpMap::iterator classSymbolMapping =
		canonical->opMap.insert(OpMap::value_type(classSortId, OpMapping()));
	      //
	      //	Make mapping specific to range kind.
	      //
	      OpMapping& om = classSymbolMapping->second;
	      om.name = toClassName;
	      om.types.resize(1);
	      setType(om.types[0], classSymbol->rangeComponent());
	      om.index = canonical->opMapIndex.size();
	      canonical->opMapIndex.append(classSymbolMapping);
	    }
	  else
	    IssueAdvisory(m.fromClass.getLineNr() << ": missing class constant for renamed class " << QUOTE(classSort) << ".");
	  //
	  //	Add class sort mapping.
	  //
	  pair<IdMap::iterator, bool> p = canonical->sortMap.insert({className, toClassName});
	  Assert(p.second, "dup sort mapping");
	  canonical->sortMapIndex.append(p.first);
	}
    }
}

void
Renaming::convertAttrMappings(const ImportModule* module, Renaming* canonical) const
{
  Sort* attributeSort = 0;
  for (const auto& i : attrMap)
    {
      if (attributeSort == 0)
	{
	  //
	  //	This is our first attribute mapping so we didn't find the Attribute sort yet.
	  //
	  attributeSort = module->findAtttributeSort();
	  if (attributeSort == 0)
	    {
	      //
	      //	There's no way interpret attr mappings without an Attribute sort.
	      //
	      IssueAdvisory(i.fromAttr.getLineNr() << ": ignoring attr mappings in renaming.");
	      return;
	    }
	}
      if (i.fromAttr.code() == i.toAttr.code())
	continue;  // skip identity mapping

      string fromNameString(i.fromAttr.name());
      fromNameString += attributeSuffix;
      int fromName = Token::encode(fromNameString.c_str());
      
      string toNameString(i.toAttr.name());
      toNameString += attributeSuffix;
      int toName = Token::encode(toNameString.c_str());
      //
      //	We look at all the unary operators in module that have the right name and
      //	range component, excluding polymorphs.
      //
      const ConnectedComponent* attributeComponent = attributeSort->component();
      for (Symbol* s = module->findFirstUnarySymbol(fromName, attributeComponent); s != 0;
	   s = module->findNextUnarySymbol(s, attributeComponent))
	{
	  const ConnectedComponent* domainComponent = s->domainComponent(0);
	  if (i.type.empty() || typeMatch(i.type, domainComponent))
	    {
	      OpMap::iterator attrMapping = canonical->opMap.insert({fromName, OpMapping()});
	      OpMapping& am = attrMapping->second;
	      am.name = toName;
	      am.types.resize(2);
	      setType(am.types[0], domainComponent);
	      setType(am.types[1], attributeComponent);
	      am.index = canonical->opMapIndex.size();
	      canonical->opMapIndex.append(attrMapping);
	    }
	}
    }
}
