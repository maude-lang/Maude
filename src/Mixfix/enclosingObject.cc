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
//      Implementation for class EnclosingObject.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
//#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
//#include "term.hh"
//#include "binarySymbol.hh"

//      core class definitions
//#include "equation.hh"
//#include "rule.hh"
//#include "sortConstraint.hh"
//#include "conditionFragment.hh"
#include "namedEntity.hh"

//     higher class definitions
//#include "equalityConditionFragment.hh"
//#include "sortTestConditionFragment.hh"
//#include "assignmentConditionFragment.hh"
//#include "rewriteConditionFragment.hh"

//	front end class definitions
//#include "renaming.hh"
//#include "view.hh"
//#include "importModule.hh"
//#include "moduleCache.hh"
//#include "importTranslation.hh"
//#include "quotedIdentifierSymbol.hh"
#include "token.hh"

#include "enclosingObject.hh"

int
EnclosingObject::instantiateSortName(int sortId,
				     const ParameterMap& parameterMap,
				     const ParameterSet& extraParameterSet)
{
  //
  //	We are passed the id of a sort whose name may contain parameters, a map
  //	which gives the instantiation of each parameter and a set of parameters
  //	that are instantiated by theory-views and thus don't disappear, but rather
  //	are tacked on the end of the sort name as extra instantiation.
  //
  if (Token::auxProperty(sortId) == Token::AUX_STRUCTURED_SORT)
    {
      bool mapped = false;
      //
      //	We have a sort that looks like:
      //	  header{p1,p2,...,pn}
      //	where each pi could be a parameter, simple view or a structured
      //	view which may contain parameters that also need instantiation.
      //
      int header;
      Vector<int> parameters;
      Token::splitParameterList(sortId, header, parameters);
      //
      //	We accumulate parameters that have been instantiated by theory-views.
      //
      Vector<int> extraParameters;
      //
      //	We now check each parameter to see if we can map it to something else.
      //
      int nrParameters = parameters.size();
      for (int i = 0; i < nrParameters; ++i)
	{
	  int original = parameters[i];
	  ParameterMap::const_iterator m = parameterMap.find(original);
	  if (m != parameterMap.end())
	    {
	      mapped = true;
	      //
	      //	original is an instantiated parameter; replace it by its
	      //	instantiation and check to see if survives as an extra
	      //	parameter.
	      //
	      parameters[i] = m->second;
	    }
	  else
	    {
	      //
	      //	original wasn't instantiated. However it could be
	      //	a structured view that contains parameters so we
	      //	make a recursive call to check for this.
	      //
	      int instantiatedParameter = instantiateSortName(original, parameterMap, extraParameterSet);
	      if (instantiatedParameter != original)
		{
		  mapped = true;
		  parameters[i] = instantiatedParameter;
		  continue;  // original can't be an extra parameter
		}
	    }
	  //
	  //	If the original parameter was instantiated by a theory-view
	  //	it survives as an extra parameter.
	  //
	  if (extraParameterSet.find(original) != extraParameterSet.end())
	    extraParameters.append(original);
	}
      if (mapped)
	sortId = Token::joinParameterList(header, parameters);
      if (!extraParameters.empty())
	sortId = Token::joinParameterList(sortId, extraParameters);
    }
  return sortId;
}

void
EnclosingObject::addConflict(int parameterName1, int parameterName2)
{
  if (parameterName2 < parameterName1)
    swap(parameterName1, parameterName2);
  conflicts.insert(Conflict(parameterName1, parameterName2));
  DebugNew("For " << getObjectType() << " " << Token::name(getObjectName()->id()) <<
	    " adding conflict between " << Token::name(parameterName1) <<
 	    " and " << Token::name(parameterName2));
}

void
EnclosingObject::addInAllConflicts(const EnclosingObject* innerObject)
{
  conflicts.insert(innerObject->conflicts.begin(), innerObject->conflicts.end());
  DebugNew("For " << getObjectType() << " " << Token::name(getObjectName()->id()) <<
	    " adding in conflicts from " << innerObject->getObjectType() << " " <<
	    Token::name(innerObject->getObjectName()->id()));
}

void
EnclosingObject::addConflictsWithBoundParameters(const EnclosingObject* innerObject, int bareParameterName)
{
  for (int i : innerObject->boundParameters)
    addConflict(i, bareParameterName);
}

bool
EnclosingObject::hasConflict(int parameterName1, int parameterName2) const
{
  if (parameterName2 < parameterName1)
    swap(parameterName1, parameterName2);
  return conflicts.find(Conflict(parameterName1, parameterName2)) != conflicts.end();
}

int
EnclosingObject::findFirstClash(const ParameterSet& chosen, int candidate) const
{
  for (int i : chosen)
    {
      if (hasConflict(i, candidate))
	return i;
    }
  return NONE;
}
