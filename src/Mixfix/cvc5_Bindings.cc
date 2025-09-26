/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.
    Copyright 2025 Universidad Complutense de Madrid, 28040 Madrid, Spain.

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
//      Implementation for class VariableGenerator (cvc5 version)
//

//      utility stuff
#include "macros.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "mixfix.hh"
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "term.hh"

//	variable class definitions
#include "variableDagNode.hh"
#include "variableTerm.hh"

#include "freeDagNode.hh"

//	SMT class defintions
#include "SMT_Symbol.hh"
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberDagNode.hh"

//	front end class definitions
#include "token.hh"
#include "variableGenerator.hh"

using kind = cvc5::Kind;

VariableGenerator::VariableGenerator(const SMT_Info& smtInfo)
  : smtInfo(smtInfo), smtSolver(termManager)
{
  smtSolver.push();  // make a new context so we have a clean context to pop() back to
  pushCount = 0;
}

VariableGenerator::~VariableGenerator()
{
  variableMap.clear();  // need to get rid of cvc5::Term objects before the attribute termManager is destroyed
}

VariableGenerator::Result
VariableGenerator::assertDag(DagNode* dag)
{
  cvc5::Term e = makeBooleanExpr(dag);
  if (e.isNull())
    return BAD_DAG;

  try
    {
      smtSolver.assertFormula(e);
    }
  catch (const cvc5::CVC5ApiException&)
    {
      IssueWarning("Caught cvc5::CVC5ApiException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }
  //
  //	While assertFormula() returns a Result, it may be unknown since
  //	it doesn't do a full sat solve.
  //
  cvc5::Result result;
  try
    {
      result = smtSolver.checkSat();
      DebugAdvisory("cvc5 checkSat() returned " << result);
    }
  catch (const cvc5::CVC5ApiException&)
    {
      IssueWarning("Caught cvc5::CVC5ApiException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }

  if (result.isUnknown())
    {
      IssueWarning("CVC5 not able to determine satisfiability  - giving up.");
      return SAT_UNKNOWN;
    }
  return result.isUnsat() ? UNSAT : SAT;
}

VariableGenerator::Result
VariableGenerator::checkDag(DagNode* dag)
{
  cvc5::Term e = makeBooleanExpr(dag);
  if (e.isNull())
    return BAD_DAG;

  cvc5::Result result;
  try
    {
      result = smtSolver.checkSatAssuming(e);
      DebugAdvisory("cvc5 checkSat() returned " << result);
    }
  catch (const cvc5::CVC5ApiException& e)
    {
      cout << e.what() << endl;
      IssueWarning("Caught cvc5::CVC5ApiException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }

  if (result.isUnknown())
    {
      IssueWarning("cvc5 not able to determine satisfiability  - giving up.");
      return SAT_UNKNOWN;
    }
  return result.isUnsat() ? UNSAT : SAT;
}

inline void
VariableGenerator::clearAssertions()
{
  while (pushCount > 0)
    {
      smtSolver.pop();
      --pushCount;
    }
  smtSolver.pop();  // get back to original clean context
  smtSolver.push();  // make a new context so we don't pollute the context we want to pop back to
}

inline void
VariableGenerator::push()
{
  smtSolver.push();
  ++pushCount;
}

inline void
VariableGenerator::pop()
{
  Assert(pushCount > 0, "bad pop");
  smtSolver.pop();
  --pushCount;
}

cvc5::Term
VariableGenerator::makeVariable(VariableDagNode* v)
{
  //
  //	We first compute the <index of sort within module, name> pair that
  //	we use to index variables.
  //
  Symbol* s = v->symbol();
  Sort* sort = s->getRangeSort();
  int sortIndexWithinModule = sort->getIndexWithinModule();
  int id = v->id();
  SortIndexVariableNamePair p(sortIndexWithinModule, id);
  //
  //	If we have already converted this variable, we return the cached
  //	expression, to avoid cvc5 making a distinct variable of the same name
  //	and type.
  //
  VariableMap::const_iterator i = variableMap.find(p);
  if (i != variableMap.end())
    return i->second;
  //
  //	Haven't seen this variable before - need to corresponding cvc5 sort.
  //
  cvc5::Sort type;
  switch (smtInfo.getType(sort))
    {
    case SMT_Info::NOT_SMT:
      {
	IssueWarning("Variable " << QUOTE(static_cast<DagNode*>(v)) << " does not belong to an SMT sort.");
	return cvc5::Term();
      }
    case SMT_Info::BOOLEAN:
      {
	DebugAdvisory("made Boolean variable " << static_cast<DagNode*>(v));
	type = termManager.getBooleanSort();
	break;
      }
    case SMT_Info::INTEGER:
      {
	DebugAdvisory("made Integer variable " << static_cast<DagNode*>(v));
	type = termManager.getIntegerSort();
	break;
      }
    case SMT_Info::REAL:
      {
	type = termManager.getRealSort();
	DebugAdvisory("made Real variable " << static_cast<DagNode*>(v));
	break;
      }
    }
  //
  //	Get the name as a C string, make the new variable and cache it under the pair.
  //
  const char* name = Token::name(id);
  //
  // We do not use mkVar but mkConst because the former is for bound variables
  //
  cvc5::Term newVariable = termManager.mkConst(type, name);
  variableMap.insert(VariableMap::value_type(p, newVariable));
  return newVariable;
}

cvc5::Term
VariableGenerator::makeBooleanExpr(DagNode* dag)
{
  cvc5::Term e;

  if (SMT_Symbol* s = dynamic_cast<SMT_Symbol*>(dag->symbol()))
    {
      Sort* rangeSort = s->getRangeSort();
      if (smtInfo.getType(rangeSort) == SMT_Info::BOOLEAN)
	{
	  e = dagToCvc5(dag);
	  DebugAdvisory("Maude: " << dag << "  cvc5: " << e);
	}
    }
  else if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(dag))
    {
      Sort* rangeSort = v->symbol()->getRangeSort();
      if (smtInfo.getType(rangeSort) == SMT_Info::BOOLEAN)
	e = makeVariable(v);
    }

  WarningCheck(!e.isNull(), "Expecting an SMT Boolean expression but saw but saw " << dag);
  return e;
}

cvc5::Term
VariableGenerator::makeNumberConstant(const mpq_class& rational, bool asInteger)
{
  //
  //	We would like to do:
  //	  return em.mkReal(rational);
  //	but cvc5 makes the required constructor private thus we have this ugliness.
  //
  if (asInteger)
    return termManager.mkInteger(rational.get_str());
  else
    return termManager.mkReal(rational.get_str());
}

cvc5::Term
VariableGenerator::dagToCvc5(DagNode* dag)
{
  //
  //	Deal with number constants (cvc5 distinguishes between integers and reals)
  //
  if (SMT_NumberDagNode* n = dynamic_cast<SMT_NumberDagNode*>(dag))
    {
      Sort* sort = n->symbol()->getRangeSort();
      return makeNumberConstant(n->getValue(), smtInfo.getType(sort) == SMT_Info::INTEGER);
     }
  //
  //	Deal with variables.
  //
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(dag))
    return makeVariable(v);
  //
  //	Otherwise we expect an SMT_Symbol on top of 0 or more arguments.
  //
  if (SMT_Symbol* s = dynamic_cast<SMT_Symbol*>(dag->symbol()))
    {
      int nrArgs = s->arity();
      std::vector<cvc5::Term> exprs(nrArgs);
      FreeDagNode* f = safeCast(FreeDagNode*, dag);

      for (int i = 0; i < nrArgs; ++i)
	{
	  cvc5::Term expr = dagToCvc5(f->getArgument(i));
	  if (expr.isNull())
	    return expr;
	  exprs[i] = expr;
	}

      switch (s->getOperator())
	{
	  //
	  //	Boolean stuff.
	  //
	case SMT_Symbol::CONST_TRUE:
	  {
	    return termManager.mkTrue();
	  }
	case SMT_Symbol::CONST_FALSE:
	  {
	    return termManager.mkFalse();
	  }
	case SMT_Symbol::NOT:
	  {
	    return termManager.mkTerm(kind::NOT, exprs);
	  }
	case SMT_Symbol::AND:
	  {
	    return termManager.mkTerm(kind::AND, exprs);
	  }
	case SMT_Symbol::OR:
	  {
	    return termManager.mkTerm(kind::OR, exprs);
	  }
	case SMT_Symbol::XOR:
	  {
	    return termManager.mkTerm(kind::XOR, exprs);
	  }
	case SMT_Symbol::IMPLIES:
	  {
	    return termManager.mkTerm(kind::IMPLIES, exprs);
	  }
	  //
	  //	Polymorphic Boolean stuff.
	  //
	case SMT_Symbol::EQUALS:
	  {
	    return termManager.mkTerm(kind::EQUAL, exprs);
	  }
	case SMT_Symbol::NOT_EQUALS:
	  {
	    return termManager.mkTerm(kind::DISTINCT, exprs);
	  }
	case SMT_Symbol::ITE:
	  {
	    return termManager.mkTerm(kind::ITE, exprs);
	  }
	  //
	  //	Integer stuff.
	  //
	case SMT_Symbol::UNARY_MINUS:
	  {
	    return termManager.mkTerm(kind::NEG, exprs);
	  }
	case SMT_Symbol::MINUS:
	  {
	    return termManager.mkTerm(kind::SUB, exprs);
	  }
	case SMT_Symbol::PLUS:
	  {
	    return termManager.mkTerm(kind::ADD, exprs);
	  }
	case SMT_Symbol::MULT:
	  {
	    return termManager.mkTerm(kind::MULT, exprs);
	  }
	case SMT_Symbol::DIV:
	  {
	    return termManager.mkTerm(kind::INTS_DIVISION, exprs);
	  }
	case SMT_Symbol::MOD:
	  {
	    return termManager.mkTerm(kind::INTS_MODULUS, exprs);
	  }
	  //
	  //	Integer tests.
	  //
	case SMT_Symbol::LT:
	  {
	    return termManager.mkTerm(kind::LT, exprs);
	  }
	case SMT_Symbol::LEQ:
	  {
	    return termManager.mkTerm(kind::LEQ, exprs);
	  }
	case SMT_Symbol::GT:
	  {
	    return termManager.mkTerm(kind::GT, exprs);
	  }
	case SMT_Symbol::GEQ:
	  {
	    return termManager.mkTerm(kind::GEQ, exprs);
	  }
	case SMT_Symbol::DIVISIBLE:
	  {
	    //
	    //	Second argument must be a positive constant.
	    //	Typing ensures it is an integer.
	    //
	    DagNode* a = f->getArgument(1);
	    if (SMT_NumberDagNode* n = dynamic_cast<SMT_NumberDagNode*>(a))
	      {
		const mpq_class& rat = n->getValue();
		if (rat > 0)
		  {
		    cvc5::Op divOp = termManager.mkOp(kind::DIVISIBLE, rat.get_str());
		    exprs.resize(1);
		    return termManager.mkTerm(divOp, exprs);
		  }
	      }
	    IssueWarning("bad divisor in " << QUOTE(dag) << ".");
	    goto fail;
	  }
	  //
	  //	Stuff that is extra to reals.
	  //
	case SMT_Symbol::REAL_DIVISION:
	  {
	    return termManager.mkTerm(kind::DIVISION, exprs);
	  }
	case SMT_Symbol::TO_REAL:
	  {
	    return termManager.mkTerm(kind::TO_REAL, exprs);
	  }
	case SMT_Symbol::TO_INTEGER:
	  {
	    return termManager.mkTerm(kind::TO_INTEGER, exprs);
	  }
	case SMT_Symbol::IS_INTEGER:
	  {
	    return termManager.mkTerm(kind::IS_INTEGER, exprs);
	  }
	}
    }
  IssueWarning("term " << QUOTE(dag) << " is not a valid SMT term.");
 fail:
  return cvc5::Term();
}
