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
//	Code for processing object-oriented view mappings.
//

bool
View::getClassSortAndSymbol(Token className,
			    const char* direction,
			    Sort* classIdSort,
			    ImportModule* module,
			    Sort*& classSort,
			    Symbol*& classSymbol)
{
  classSort = module->findSort(className.code());
  if (classSort == 0)
    {
      IssueWarning(LineNumber(className.lineNumber()) << ": class mapping " << direction << QUOTE(className) <<
		   " fails because there is no such class sort in " << QUOTE(module) << ".");
      return false;
    }
  if (classIdSort->component() != classSort->component() || !leq(classSort, classIdSort))
    {
      IssueWarning(LineNumber(className.lineNumber()) << ": class mapping " << direction << " " <<
		   QUOTE(className) << " maps " << direction << " a sort in " << QUOTE(module) <<
		   " that is not a subsort of " << QUOTE(classIdSort) << ".");
      return false;
    }
  
  const static Vector<ConnectedComponent*> emptyDomain;
  classSymbol = module->findSymbol(className.code(), emptyDomain, classSort->component());
  if (classSymbol == 0)
    {
      IssueWarning(LineNumber(className.lineNumber()) << ": class mapping " << direction << " " << QUOTE(className) <<
		   " fails because there is no such class constant in " << QUOTE(module) << ".");
      return false;
    }
  return true;
}

bool
View::handleClassMappings()
{
  int nrClassMappings = getNrClassMappings();
  if (nrClassMappings == 0)
    return true;
  //
  //	Check if fromTheory and toModule appear to be object-oriented.
  //	If not, we don't waste time trying to interpret a class mapping.
  //
  Sort* fromClassIdSort = fromTheory->findClassIdSort();
  if (fromClassIdSort == 0)
    {
      Token fromClass = getFromClass(0);
      IssueWarning(LineNumber(fromClass.lineNumber()) << ": class mapping from theory " << QUOTE(fromTheory) <<
		   " which does not appear to have been generated from an oth.");
      return false;
    }
  Sort* toClassIdSort = toModule->findClassIdSort();
  if (toClassIdSort == 0)
    {
      Token fromClass = getFromClass(0);
      if (toModule->isTheory())
	{
	  IssueWarning(LineNumber(fromClass.lineNumber()) << ": class mapping to theory " << QUOTE(toModule) <<
		       " which does not appear to have been generated from an oth.");
	}
      else
	{
	  IssueWarning(LineNumber(fromClass.lineNumber()) << ": class mapping to module " << QUOTE(toModule) <<
		       " which does not appear to have been generated from an omod.");
	}
      return false;
    }
  //
  //	Check that each class mapping appears to go from a class to a class.
  //
  for (int i = 0; i < nrClassMappings; ++i)
    {
      //
      //	Check if from class makes sense.
      //
      Sort* fromClassSort;
      Symbol* fromClassSymbol;
      Token fromClass = getFromClass(i);
      if (!getClassSortAndSymbol(fromClass, "from", fromClassIdSort, fromTheory, fromClassSort, fromClassSymbol))
	return false;
      //
      //	Check if the view messes with the class sort or constant.
      //
      if (hasSortMapping(fromClassSort->id()))
	{
	  IssueWarning(LineNumber(fromClass.lineNumber()) << ": class mapping from " << QUOTE(fromClass) <<
		       " fails because class sort is already mapped by the view.");
	  return false;
	}
      if (renameOp(fromClassSymbol) != NONE)
	{
	  IssueWarning(LineNumber(fromClass.lineNumber()) << ": class mapping from " << QUOTE(fromClass) <<
		       " fails because class constant is already mapped by the view.");
	  return false;
	}
      //
      //	Check if to class makes sense.
      //
      Sort* toClassSort;
      Symbol* toClassSymbol;
      Token toClass = getToClass(i);
      if (!getClassSortAndSymbol(toClass, "to", toClassIdSort, toModule, toClassSort, toClassSymbol))
	return false;
      //
      //	Add mappings for class sort and class constant.
      //
      addSortMapping(fromClassSort->id(), toClassSort->id());
      addOpMapping(fromClassSymbol->id());
      addType(fromClassSort->component());
      addOpTarget(toClassSymbol->id());
    }
  return true;
}

bool
View::handleAttrMappings()
{

  Sort* fromAttributeSort = 0;
  Sort* toAttributeSort = 0;
  int nrAttrMappings = getNrAttrMappings();
  for (int i = 0; i < nrAttrMappings; ++i)
    {
      if (fromAttributeSort == 0)
	{
	  //
	  //	This is our first attribute mapping so we didn't find the Attribute sorts yet.
	  //
	  fromAttributeSort = fromTheory->findAtttributeSort();
	  if (fromAttributeSort == 0)
	    {
	      IssueWarning(*this << ": attr mapping from theory " << QUOTE(fromTheory) <<
		   " which does not appear to have been generated from an oth.");
	      return false;
	    }
	  toAttributeSort = toModule->findAtttributeSort();
	  if (toAttributeSort == 0)
	    {
	      if (toModule->isTheory())
		{
		  IssueWarning(*this << ": attr mapping to theory " << QUOTE(toModule) <<
			       " which does not appear to have been generated from an oth.");
		}
	      else
		{
		  IssueWarning(*this << ": attr mapping to module " << QUOTE(toModule) <<
			       " which does not appear to have been generated from an omod.");
		}
	      return false;
	    }
	}
      
      Token fromBaseName = getFromAttr(i);
      string fromNameString(fromBaseName.name());
      fromNameString += attributeSuffix;
      int fromName = Token::encode(fromNameString.c_str());
      
      Token toBaseName = getToAttr(i);
      string toNameString(toBaseName.name());
      toNameString += attributeSuffix;
      int toName = Token::encode(toNameString.c_str());
      //
      //	We look at all the unary operators in fromTheory that have the right name and
      //	range component, excluding polymorphs.
      //
      bool used = false;
      const ConnectedComponent* attributeComponent = fromAttributeSort->component();
      for (Symbol* s = fromTheory->findFirstUnarySymbol(fromName, attributeComponent); s != 0;
	   s = fromTheory->findNextUnarySymbol(s, attributeComponent))
	{
	  const ConnectedComponent* domainComponent = s->domainComponent(0);
	  const set<int>& attrTypeSorts = getAttrTypeSorts(i);
	  if (attrTypeSorts.empty() || Renaming::typeMatch(attrTypeSorts, domainComponent))
	    {
	      //
	      //	Symbol matches attr renaming. We add a specific op mapping.
	      //
	      addOpMapping(fromName);
	      addType(domainComponent);
	      addType(attributeComponent);
	      addOpTarget(toName);
	      used = true;
	    }
	}
      AdvisoryCheck(used, LineNumber(fromBaseName.lineNumber()) << ": attr mapping from " << QUOTE(fromBaseName) << " to " <<
		    QUOTE(toBaseName) << " does not match any attribute operator in " <<
		    QUOTE(fromTheory) << ".");
    }
  return true;
}
