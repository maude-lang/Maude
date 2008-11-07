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
//      Implementation for class S_Subproblem.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"

//      interface class definitions
//#include "dagNode.hh"
//#include "subproblem.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "localBinding.hh"

//	S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_ExtensionInfo.hh"
#include "S_Subproblem.hh"

S_Subproblem::S_Subproblem(S_DagNode* subject,
			   const mpz_class& leftOver,
			   int varIndex,
			   const Sort* varSort,
			   S_ExtensionInfo* extensionInfo,
			   int mustMatchAtLeast)
  : subject(subject),
    leftOver(leftOver),
    extensionInfo(extensionInfo),
    varIndex(varIndex),
    varSort(varSort),
    mustMatchAtLeast(mustMatchAtLeast)
{
  Assert(leftOver > 0, "only makes sense with leftOver > 0");
  Assert(extensionInfo != 0, "only makes sense with extensionInfo");
  Assert(mustMatchAtLeast == 0 || mustMatchAtLeast == 1, "mustMatchAtLeast must be 0 or 1");
}

bool
S_Subproblem::solve(bool findFirst, RewritingContext& solution)
{
  if (findFirst)
    {
      S_DagNode* d = new S_DagNode(subject->symbol(), leftOver, subject->getArgument());
      solution.bind(varIndex, d);  // to protect new dagnode
      if (d->checkSort(varSort, solution))
	{
	  extensionInfo->setMatchedWhole(true);
	  extensionInfo->setUnmatched(0);
	  return true;
	}
    }
  for (;;)
    {
      mpz_class newUnmatched = extensionInfo->getUnmatched() + 1;
      mpz_class matched = leftOver - newUnmatched;
      if (matched < mustMatchAtLeast)
	break;  // fail

      extensionInfo->setUnmatched(newUnmatched);
      DagNode* d = subject->getArgument();
      if (matched > 0)
	d = new S_DagNode(subject->symbol(), matched, d);
      solution.bind(varIndex, d);  // to protect potentially new dagnode

      if (d->checkSort(varSort, solution))
	{
	  extensionInfo->setMatchedWhole(false);
	  return true;
	}
    }
  return false;
}

#ifdef DUMP
void
S_Subproblem::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{S_Subproblem}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "subject = " << subject << '\n';
  s << Indent(indentLevel) << "leftOver = " << leftOver << '\n';
  s << Indent(indentLevel) << "varIndex = " << varIndex <<
    " \"" << variableInfo.index2Variable(varIndex) << "\"\n";
  s << Indent(indentLevel) << "varSort = " << varSort << '\n';
  s << Indent(indentLevel) << "extensionInfo = " << extensionInfo << '\n';
  s << Indent(indentLevel - 1) << "End{S_Subproblem}\n";
}
#endif
