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
//	Class for subproblems to check for equality or inequality of terms,
//	variable bindings and matched portions.
//
#ifndef _equalitySubproblem_hh_
#define _equalitySubproblem_hh_
#include "subproblem.hh"

class EqualitySubproblem : public Subproblem
{
public:
  EqualitySubproblem(int lhsVariable, int rhsVariable, bool checkEqual);
  EqualitySubproblem(const Term* lhsTerm, int rhsVariable, bool checkEqual);
  EqualitySubproblem(int lhsVariable,
		     const ExtensionInfo* rhsExtensionInfo,
		     bool checkEqual);
  EqualitySubproblem(const Term* lhsTerm,
		     const ExtensionInfo* rhsExtensionInfo,
		     bool checkEqual);

  bool solve(bool findFirst, RewritingContext& solution);

private:
  const Term* const lhsTerm;
  const int lhsVariable;
  const ExtensionInfo* const rhsExtensionInfo;
  const int rhsVariable;
  const bool checkEqual;
};

inline
EqualitySubproblem::EqualitySubproblem(int lhsVariable, int rhsVariable, bool checkEqual)
  : lhsTerm(0),
    lhsVariable(lhsVariable),
    rhsExtensionInfo(0),
    rhsVariable(rhsVariable),
    checkEqual(checkEqual)
{
}

inline
EqualitySubproblem::EqualitySubproblem(const Term* lhsTerm, int rhsVariable, bool checkEqual)
  : lhsTerm(lhsTerm),
    lhsVariable(NONE),
    rhsExtensionInfo(0),
    rhsVariable(rhsVariable),
    checkEqual(checkEqual)
{
}

inline
EqualitySubproblem::EqualitySubproblem(int lhsVariable,
				       const ExtensionInfo* rhsExtensionInfo,
				       bool checkEqual)
  : lhsTerm(0),
    lhsVariable(lhsVariable),
    rhsExtensionInfo(rhsExtensionInfo),
    rhsVariable(NONE),
    checkEqual(checkEqual)
{
}

inline
EqualitySubproblem::EqualitySubproblem(const Term* lhsTerm,
				       const ExtensionInfo* rhsExtensionInfo,
				       bool checkEqual)
  : lhsTerm(lhsTerm),
    lhsVariable(NONE),
    rhsExtensionInfo(rhsExtensionInfo),
    rhsVariable(NONE),
    checkEqual(checkEqual)
{
}

#endif
