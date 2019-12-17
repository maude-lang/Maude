/*

    This file is part of the Maude 3 interpreter.

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
//      Implementation for class CUI_LhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "CUI_Theory.hh"

//      interface class definitions
#include "argVec.hh"
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "lhsAutomaton.hh"
#include "rhsAutomaton.hh"
#include "extensionInfo.hh"

//      core class definitions
#include "variableInfo.hh"
#include "subproblemAccumulator.hh"
#include "disjunctiveSubproblemAccumulator.hh"
#include "equalitySubproblem.hh"
#include "extensionMatchSubproblem.hh"
#include "substitution.hh"
#include "localBinding.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	CUI theory class definitions
#include "CUI_Symbol.hh"
#include "CUI_DagNode.hh"
#include "CUI_LhsAutomaton.hh"

#include "CUI_Matcher.cc"

CUI_LhsAutomaton::CUI_LhsAutomaton(CUI_Symbol* topSymbol,
				   bool matchAtTop,
				   Flags flags,
				   int nrVariables)
  :  topSymbol(topSymbol),
     matchAtTop(matchAtTop),
     flags(flags),
     local(nrVariables)
{
  subpattern0.type = UNDEFINED;
  subpattern0.automaton = 0;
  subpattern0.topAutomaton = 0;
  subpattern1.automaton = 0;
  subpattern1.topAutomaton = 0;
}

CUI_LhsAutomaton::~CUI_LhsAutomaton()
{
  delete subpattern0.automaton;
  delete subpattern0.topAutomaton;
  delete subpattern1.automaton;
  delete subpattern1.topAutomaton;
}

bool
CUI_LhsAutomaton::addSubpattern(Term* term,
				const VariableInfo& variableInfo,
				NatSet& boundUniquely)
{
  bool first = (subpattern0.type == UNDEFINED);
  Subpattern& sp = first ? subpattern0 : subpattern1;
  if (matchAtTop && (flags & (first ?
			      (ID1_COLLAPSE | IDEM_COLLAPSE) : 
			      (ID0_COLLAPSE | IDEM_COLLAPSE))))
    {
      //
      //	Need to compile a top automaton for this subpattern.
      //
      VariableInfo localVariableInfo(variableInfo);
      //
      //	Ulgy hack to prevent greedy matching because extension
      //	is shared in idem match.
      //
      if (first && (flags & IDEM_COLLAPSE))
	localVariableInfo.addConditionVariables(term->occursBelow());
      NatSet local(boundUniquely);
      bool spl;
      sp.topAutomaton = term->compileLhs(true, localVariableInfo, local, spl);
    }
  if (term->ground())
    {
      sp.type = GROUND_ALIEN;
      sp.term = term;
      return false;
    }
  VariableTerm* v = dynamic_cast<VariableTerm*>(term);
  if (v != 0)
    {
      sp.type = VARIABLE;
      sp.varIndex = v->getIndex();
      sp.sort = v->getSort();
      if (flags & UNIQUE_BRANCH)
	{
	  boundUniquely.insert(v->getIndex());
	  return false;
	}
      if (first || !matchAtTop || !(flags & IDEM_COLLAPSE))
	return false;
    }
  else
    {
      sp.type = NON_GROUND_ALIEN;
      if (flags & UNIQUE_BRANCH)
	{
	  bool spl;
	  sp.automaton = term->compileLhs(false, variableInfo, boundUniquely, spl);
	  return spl;
	}
    }
  NatSet local(boundUniquely);
  bool spl;
  sp.automaton = term->compileLhs(false, variableInfo, local, spl);
  return spl;
}

#ifdef DUMP
void
CUI_LhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{CUI_LhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "topSymbol = \"" << topSymbol <<
    "\"\tmatchAtTop = " << matchAtTop << '\n';
  s << Indent(indentLevel) << "flags = " << flags << '\n';
  s << Indent(indentLevel) << "subpattern0:\n";
  subpattern0.dump(s, variableInfo, indentLevel + 1);
  s << Indent(indentLevel) << "subpattern1:\n";
  subpattern1.dump(s, variableInfo, indentLevel + 1);
  s << Indent(indentLevel - 1) << "End{CUI_Automaton}\n";
}

void
CUI_LhsAutomaton::Subpattern::dump(ostream& s,
				   const VariableInfo& variableInfo,
				   int indentLevel)
{
  s << Indent(indentLevel) << "type = " << type << '\n';
  switch (type)
    {
    case UNDEFINED:
      return;
    case GROUND_ALIEN:
      s << Indent(indentLevel) << "term = \"" << term << "\"\n";
      break;
    case VARIABLE:
      s << Indent(indentLevel) << "varIndex = " << varIndex << " \"" <<
	variableInfo.index2Variable(varIndex) << "\"\tsort = \"" << sort << "\"\n";
      if (automaton == 0)
	break;
      // fall thru
    case NON_GROUND_ALIEN:
      s << Indent(indentLevel) << "automaton =\n";
      automaton->dump(s, variableInfo, indentLevel + 1);
      break;
    }
  if (topAutomaton != 0)
    {
      s << Indent(indentLevel) << "topAutomaton =\n";
      topAutomaton->dump(s, variableInfo, indentLevel + 1);
    }
}

ostream&
operator<<(ostream& s, CUI_LhsAutomaton::Flags flags)
{
  static const char* const names[] = {"GREEDY_MATCH_OK",
				      "UNIQUE_BRANCH",
				      "FORWARD",
				      "REVERSE",
				      "CONDITIONAL_REVERSE",
				      "ID0_COLLAPSE",
				      "ID1_COLLAPSE",
				      "IDEM_COLLAPSE"};
  s << '{';
  int m = 1;
  bool already = false;
  for (int i = 0; i < 8; i++)
    {
      if (flags & m)
	{
	  if (already)
	    s << ", ";
	  s << names[i];
	  already = true;
	}
      m <<= 1;
    }
  s << '}';
  return s;
}

ostream&
operator<<(ostream& s, CUI_LhsAutomaton::PatternType type)
{
  static const char* const names[] = {"UNDEFINED",
				      "GROUND_ALIEN",
				      "VARIABLE",
				      "NON_GROUND_ALIEN"};
  s << names[type];
  return s;
}
#endif
