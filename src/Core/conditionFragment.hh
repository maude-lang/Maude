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
//	Abstract base class for condition fragments.
//
#ifndef _conditionFragment_hh_
#define _conditionFragment_hh_
#include <stack>
#include "rhsBuilder.hh"
#include "conditionState.hh"

class ConditionFragment
{
public:
  virtual ~ConditionFragment() {}

  virtual void check(VariableInfo& varInfo, NatSet& boundVariables) = 0;
  virtual void preprocess() = 0;
  virtual void compileBuild(VariableInfo& variableInfo, TermBag& availableTerms) = 0;
  virtual void compileMatch(VariableInfo& variableInfo, NatSet& boundUniquely) = 0;
  virtual bool solve(bool findFirst,
		     RewritingContext& solution,
		     stack<ConditionState*>& state) = 0;
};

//
//      Output function for ConditionFragment must be defined by library user.
//
ostream& operator<<(ostream& s, const ConditionFragment* c);

#endif

