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
//      Implementation for class S_LhsAutomaton.
//

//	utility stuff
#include "macros.hh"
#include "indent.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "S_Theory.hh"

//      core class definitions
#include "variableInfo.hh"
#include "substitution.hh"
#include "subproblemAccumulator.hh"
#include "variableAbstractionSubproblem.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"

//	S theory class definitions
#include "S_Symbol.hh"
#include "S_DagNode.hh"
#include "S_LhsAutomaton.hh"
#include "S_Subproblem.hh"
#include "S_ExtensionInfo.hh"

S_LhsAutomaton::S_LhsAutomaton(S_Symbol* symbol,
			       const mpz_class& number,
			       bool matchAtTop)
  : topSymbol(symbol),
    number(number),
    matchAtTop(matchAtTop)
{
}

S_LhsAutomaton::~S_LhsAutomaton()
{
  if (type == NON_GROUND_ALIEN || type == ABSTRACTED_NON_GROUND_ALIEN)
    delete automaton;
}

bool
S_LhsAutomaton::addSubpattern(Term* term,
			      int abstractionVariableIndex,
			      const VariableInfo& variableInfo,
			      NatSet& boundUniquely)
{
  if (term->ground())
    {
      type = GROUND_ALIEN;
      groundTerm = term;
      return false;
    }
  if (VariableTerm* v = dynamic_cast<VariableTerm*>(term))
    {
      type = VARIABLE;
      varIndex = v->getIndex();
      sort = v->getSort();
      boundUniquely.insert(varIndex);
      return false;
    }
  bool spl;
  automaton = term->compileLhs(false, variableInfo, boundUniquely, spl);
  if (abstractionVariableIndex == NONE)
    {
      type = NON_GROUND_ALIEN;
      return spl;
    }
  type = ABSTRACTED_NON_GROUND_ALIEN;
  varIndex = abstractionVariableIndex;
  sort = term->getComponent()->sort(Sort::ERROR_SORT);
  return true;
}

local_inline bool
S_LhsAutomaton::matchEq(DagNode* arg,
			Substitution& solution,
			Subproblem*& returnedSubproblem)
{
  switch (type)
    {
    case GROUND_ALIEN:
      {
	returnedSubproblem = 0;
	return groundTerm->equal(arg);
      }
    case VARIABLE:
      {
	if (DagNode* d = solution.value(varIndex))
	  {
	    returnedSubproblem = 0;
	    return d->equal(arg);
	  }
	if (arg->checkSort(sort, returnedSubproblem))
	  {
	    solution.bind(varIndex, arg);
	    return true;
	  }
	break;  // fail
      }
    case NON_GROUND_ALIEN:
    case ABSTRACTED_NON_GROUND_ALIEN:
      {
	return automaton->match(arg, solution, returnedSubproblem);
      }
    }
  return false;
}

local_inline bool
S_LhsAutomaton::nonExtMatchGt(DagNode* subject,
			      DagNode* arg,
			      const mpz_class& subjectNumber,
			      Substitution& solution,
			      Subproblem*& returnedSubproblem)
{
  if (type == GROUND_ALIEN || type == NON_GROUND_ALIEN)
    return false;  // nothing to absort extra iter operators
  mpz_class diff = subjectNumber - number;
  if (type == VARIABLE)
    {
      //
      //	Need to check for and handle the bound variable case.
      //
      if (DagNode* d = solution.value(varIndex))
	{
	  if (d->symbol() != topSymbol)
	    return false;
	  S_DagNode* value = safeCast(S_DagNode*, d);
	  returnedSubproblem = 0;
	  return value->getNumber() == diff &&
	    value->getArgument()->equal(arg);
	}
    }
  S_DagNode* d = new S_DagNode(topSymbol, diff, arg);
  solution.bind(varIndex, d);  // to protect new dag node
  if (type == ABSTRACTED_NON_GROUND_ALIEN)
    return automaton->match(d, solution, returnedSubproblem);
  if (d->checkSort(sort, returnedSubproblem))
    {
      if (subject->isReduced() && d->getSortIndex() != Sort::SORT_UNKNOWN)
	d->setReduced();  // optimization
      return true;
    }
  return false;
}

local_inline bool
S_LhsAutomaton::extMatchGt(S_DagNode* subject,
			   DagNode* arg,
			   const mpz_class& subjectNumber,
			   Substitution& solution,
			   Subproblem*& returnedSubproblem,
			   S_ExtensionInfo* extensionInfo)
{
  switch (type)
    {
    case GROUND_ALIEN:
      {
	if (groundTerm->equal(arg))
	  {
	    returnedSubproblem = 0;
	    extensionInfo->setMatchedWhole(false);
	    extensionInfo->setUnmatched(subjectNumber - number);
	    extensionInfo->setValidAfterMatch(true);
	    return true;
	  }
	break;  // fail
      }
    case VARIABLE:
      {
	if (DagNode* d = solution.value(varIndex))
	  {
	    if (d->symbol() == topSymbol)
	      {
		S_DagNode* value = safeCast(S_DagNode*, d);
		mpz_class leftOver = (subjectNumber - number) - value->getNumber();
		if (leftOver >= 0 && value->getArgument()->equal(arg))
		  {
		    returnedSubproblem = 0;
		    if (leftOver == 0)
		      extensionInfo->setMatchedWhole(true);
		    else
		      {
			extensionInfo->setMatchedWhole(false);
			extensionInfo->setUnmatched(leftOver);
		      }
		    extensionInfo->setValidAfterMatch(true);
		    return true;
		  }
	      }
	    else
	      {
		if (d->equal(arg))
		  {
		    returnedSubproblem = 0;
		    extensionInfo->setMatchedWhole(false);
		    extensionInfo->setUnmatched(subjectNumber - number);
		    extensionInfo->setValidAfterMatch(true);
		    return true;
		  }
	      }
	  }
	else
	  {
	    returnedSubproblem =
	      new S_Subproblem(subject, subjectNumber - number,
			       varIndex, sort, extensionInfo);
	    extensionInfo->setValidAfterMatch(false);
	    return true;
	  }
	break;  // fail
      }
    case NON_GROUND_ALIEN:
      {
	if (automaton->match(arg, solution, returnedSubproblem))
	  {
	    extensionInfo->setMatchedWhole(false);
	    extensionInfo->setUnmatched(subjectNumber - number); 
	    extensionInfo->setValidAfterMatch(true);
	    return true;
	  }
	break;  // fail
      }
    case ABSTRACTED_NON_GROUND_ALIEN:
      {
	SubproblemAccumulator subproblems;
	subproblems.add(new S_Subproblem(subject,
					 subjectNumber - number,
					 varIndex,
					 sort,
					 extensionInfo));
	subproblems.add(new VariableAbstractionSubproblem(automaton,
							  varIndex,
							  solution.nrFragileBindings()));
	returnedSubproblem = subproblems.extractSubproblem();
	extensionInfo->setValidAfterMatch(false);
	return true;
      }
    }
  return false;
}

bool
S_LhsAutomaton::match(DagNode* subject,
		      Substitution& solution,
		      Subproblem*& returnedSubproblem,
		      ExtensionInfo* extensionInfo)
{
  if (subject->symbol() != topSymbol)
    return false;
  Assert(matchAtTop == (extensionInfo != 0), "matchAtTop disagreement");
  S_DagNode* s = static_cast<S_DagNode*>(subject);
  const mpz_class& subjectNumber = s->getNumber();
  //
  //	The basic idea is that we can peel "number" iter operators off the subject or
  //	else we fail. If the subject is now out of theory we have a straight forward
  //	matching problem regardless of extension. If there are iter operators left on the
  //	subject they will need to be absorbed by the extension or a lhs (possibly
  //	abstraction) variable.
  //
  int r = cmp(subjectNumber, number);  // Purify goes wrong here with gmp 4.0.1 opt asm!
  if (r < 0)
    return false;

  DagNode* arg = s->getArgument();
  if (matchAtTop)
    {
      S_ExtensionInfo* e = safeCast(S_ExtensionInfo*, extensionInfo);
      if (r == 0)
	{
	  if (matchEq(arg, solution, returnedSubproblem))
	    {
	      e->setMatchedWhole(true);
	      e->setValidAfterMatch(true);
	      return true;
	    }
	}
      else
	return extMatchGt(s, arg, subjectNumber, solution, returnedSubproblem, e);
    }
  else
    {
      return (r == 0) ? matchEq(arg, solution, returnedSubproblem) :
	nonExtMatchGt(subject, arg, subjectNumber, solution, returnedSubproblem);
    }
  return false;
}

#ifdef DUMP
void
S_LhsAutomaton::dump(ostream& s, const VariableInfo& variableInfo, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{S_LhsAutomaton}\n";
  ++indentLevel;
  s << Indent(indentLevel) << "topSymbol = \"" << topSymbol <<
    "\"\tmatchAtTop = " << matchAtTop << '\n';
  s << Indent(indentLevel) << "number = " << number << "\ttype = ";
  switch(type)
    {
    case GROUND_ALIEN:
      {
	s << "GROUND_ALIEN\n" << Indent(indentLevel) << "groundTerm = \"" <<
	  groundTerm << "\"\n";
	break;
      }
    case VARIABLE:
      {
	s << "VARIABLE\n" << Indent(indentLevel) << "varIndex = " << varIndex << " \"" <<
	  variableInfo.index2Variable(varIndex) << "\"\tsort = \"" << sort << "\"\n";
	break;
      }
    case NON_GROUND_ALIEN:
      {
	s << "NON_GROUND_ALIEN\n" << Indent(indentLevel) << "automaton =\n";
	automaton->dump(s, variableInfo, indentLevel + 1);
	break;
      }
    case ABSTRACTED_NON_GROUND_ALIEN:
      {
	s << "ABSTRACTED_NON_GROUND_ALIEN\n" << Indent(indentLevel) <<
	  "varIndex = " << varIndex << "\tsort = \"" << sort <<
	  "\tautomaton =\n";
	automaton->dump(s, variableInfo, indentLevel + 1);
	break;
      }
    }
  s << Indent(indentLevel - 1) << "End{S_Automaton}\n";

}
#endif
