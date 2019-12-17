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
//      Abstract base class for parameterized objects (modules/views) that can
//	enclose module expressions. Currently we also use it for enclosed objects.
//

#ifndef _enclosingObject_hh_
#define _enclosingObject_hh_
#include <set>
#include <map>

class EnclosingObject
{
public:
  bool hasBoundParameters() const;
  //
  //	This is needed for renaming modules - parameters both free and
  //	bound are unchangedby a renaming.
  //
  void copyBoundParameters(const EnclosingObject* other);
  //
  //	To deal with the parameters of an enclosing object we need
  //	functions to look up the index of a parameter associated with
  //	a name, and to get the theory of the parameter at a given index.
  //
  virtual int findParameterIndex(int name) const = 0;
  virtual ImportModule* getParameterTheory(int index) const = 0;
  //
  //	To generate error messages we need to be able to get a string
  //	with the kind of object we have, and get the objects name.
  //
  virtual const char* getObjectType() const = 0;
  virtual const NamedEntity* getObjectName() const = 0;

protected:
  //
  //	Both modules and views need to deal with instantiating
  //	parameters within sort names. For want of a better place we
  //	put this code here.
  //
  typedef map<int,int> ParameterMap;
  typedef set<int> ParameterSet;
  

  static int instantiateSortName(int sortId,
				 const ParameterMap& parameterMap,
				 const ParameterSet& extraParameterSet);

  void addBoundParameter(int parameterName);

  //
  //	We track conflicts between parameters; this is a symmetric relation.
  //
  void addConflict(int parameterName1, int parameterName2);
  bool hasConflict(int parameterName1, int parameterName2) const;
  //
  //	If candidate conflicts with any in chosen, we return the first such one.
  //	If there are no conflicts then we return NONE.
  //
  int findFirstClash(const ParameterSet& chosen, int candidate) const;
  //
  //	Accumulate all the conflicts on parameter names from another enclosed object.
  //
  void addInAllConflicts(const EnclosingObject* innerObject);
  void addConflictsWithBoundParameters(const EnclosingObject* innerObject, int bareParameterName);

private:
  typedef pair<int, int> Conflict;
  typedef set<Conflict> ConflictSet;
  //
  //	We keep track of the bound parameters in an enclosed object.
  //	These parameters are a subset of those in the enclosing object
  //	and cannot be explicity instantiated but get instantiated when the
  //	enclosing object is instantiated.
  //
  ParameterSet boundParameters;
  //
  //	We keep track of which parameter names conflict with other parameter names.
  //
  ConflictSet conflicts;
};

inline void
EnclosingObject::addBoundParameter(int parameterName)
{
  boundParameters.insert(parameterName);
}

inline void
EnclosingObject::copyBoundParameters(const EnclosingObject* other)
{
  boundParameters = other->boundParameters;  // deep copy
}

inline bool
EnclosingObject::hasBoundParameters() const
{
  return !(boundParameters.empty());
}

#endif
