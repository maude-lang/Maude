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
//      Class for subproblems in the S theory.
//
#ifndef _S_Subproblem_hh_
#define _S_Subproblem_hh_
#include <gmpxx.h>
#include "subproblem.hh"

class S_Subproblem : public Subproblem
{
  NO_COPYING(S_Subproblem);

public:
  S_Subproblem(S_DagNode* subject,
	       const mpz_class& leftOver,
	       int varIndex,
	       const Sort* varSort,
	       S_ExtensionInfo* extension);
  //
  //	Member functions required by theory interface.
  //
  bool solve(bool findFirst, RewritingContext& solution);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel);
#endif

private:
  S_DagNode* const subject;
  const mpz_class leftOver;
  S_ExtensionInfo* const extensionInfo;
  const int varIndex;
  const Sort* const varSort;
};

#endif
