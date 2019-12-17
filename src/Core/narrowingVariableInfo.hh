/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to hold variable information from a dag that is about to be narrowed.
//
#ifndef _narrowingVariableInfo_hh_
#define _narrowingVariableInfo_hh_

class NarrowingVariableInfo
{
  NO_COPYING(NarrowingVariableInfo);

public:
  NarrowingVariableInfo() {}

  int getNrVariables() const;
  //
  //	These two functions map between indexes and variables.
  //
  int variable2Index(VariableDagNode* variableTerm);
  int variable2IndexNoAdd(VariableDagNode* variableTerm) const;
  VariableDagNode* index2Variable(int index) const;
  //
  //	Forgets all but the first variables seen.
  //	This hack is needed to deal with variables in blocker terms.
  //
  void forgetAllBut(int nrFirstVariables);

  void copy(const NarrowingVariableInfo& other);

private:
  //
  //	Should probably use a hash table for scalability.
  //
  Vector<VariableDagNode*> variables;
};

inline int
NarrowingVariableInfo::getNrVariables() const
{
  return variables.size();
}

inline VariableDagNode*
NarrowingVariableInfo::index2Variable(int index) const
{
  return variables[index];
}

inline void
NarrowingVariableInfo::forgetAllBut(int nrFirstVariables)
{
  variables.resize(nrFirstVariables);
}

inline void
NarrowingVariableInfo::copy(const NarrowingVariableInfo& other)
{
  variables = other.variables;
}

#endif
