/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class VariableGenerator (CVC4 version)
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

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

VariableGenerator::VariableGenerator(const SMT_Info& smtInfo)
  : smtInfo(smtInfo)
{
  exprManager = new ExprManager();
  //cout << "created manager " << exprManager << endl;
  smtEngine = new SmtEngine(exprManager);
  //cout << "created engine " << smtEngine << endl;

  smtEngine->setOption("rewrite-divk", SExpr(true));
  smtEngine->push();  // make a new context so we have a clean context to pop() back to
  pushCount = 0;
}

VariableGenerator::~VariableGenerator()
{
  variableMap.clear();  // need to get rid of Expr objects before we can safely delete exprManager
  //cout << "deleting engine " << smtEngine << endl;
  delete smtEngine;
  //cout << "deleting manager " << exprManager << endl;
  delete exprManager;
}

VariableGenerator::Result
VariableGenerator::assertDag(DagNode* dag)
{
  Expr e = makeBooleanExpr(dag);
  if (e.isNull())
    return BAD_DAG;

  try
    {
      smtEngine->assertFormula(e);
    }
  catch (const CVC4::LogicException&)
    {
      IssueWarning("Caught CVC4::LogicException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }
  //
  //	While assertFormula() returns a Result, it may be unknown since
  //	it doesn't do a full sat solve.
  //
  CVC4::Result result;
  try
    {
      result = smtEngine->checkSat();
      DebugAdvisory("CVC4 checkSat() returned " << result);
    }
  catch (const CVC4::LogicException&)
    {
      IssueWarning("Caught CVC4::LogicException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }

  CVC4::Result::Sat sat = result.isSat();
  if (sat == CVC4::Result::SAT_UNKNOWN)
    {
      IssueWarning("CVC4 not able to determine satisfiability  - giving up.");
      return SAT_UNKNOWN;
    }
  return (sat == false) ? UNSAT : SAT;
}

VariableGenerator::Result
VariableGenerator::checkDag(DagNode* dag)
{
  Expr e = makeBooleanExpr(dag);
  if (e.isNull())
    return BAD_DAG;

  CVC4::Result result;
  try
    {
      result = smtEngine->checkSat(e);
      DebugAdvisory("CVC4 checkSat() returned " << result);
    }
  catch (const CVC4::LogicException&)
    {
      IssueWarning("Caught CVC4::LogicException - giving up.");
      //
      //	An expression outside of the supported logic - e.g. nonlinear.
      //
      return SAT_UNKNOWN;
    }

  CVC4::Result::Sat sat = result.isSat();
  if (sat == CVC4::Result::SAT_UNKNOWN)
    {
      IssueWarning("CVC4 not able to determine satisfiability  - giving up.");
      return SAT_UNKNOWN;
    }
  return (sat == false) ? UNSAT : SAT;
}

inline void
VariableGenerator::clearAssertions()
{
  while (pushCount > 0)
    {
      smtEngine->pop();
      --pushCount;
    }
  smtEngine->pop();  // get back to original clean context
  smtEngine->push();  // make a new context so we don't pollute the context we want to pop back to
}

inline void
VariableGenerator::push()
{
  smtEngine->push();
  ++pushCount;
}

inline void
VariableGenerator::pop()
{
  Assert(pushCount > 0, "bad pop");
  smtEngine->pop();
  --pushCount;
}

Expr
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
  //	expression, to avoid CVC4 making a distinct variable of the same name
  //	and type.
  //
  VariableMap::const_iterator i = variableMap.find(p);
  if (i != variableMap.end())
    return i->second;
  //
  //	Haven't seen this variable before - need to corresponding CVC4 type.
  //
  Type type;
  switch (smtInfo.getType(sort))
    {
    case SMT_Info::NOT_SMT:
      {
	IssueWarning("Variable " << QUOTE(static_cast<DagNode*>(v)) << " does not belong to an SMT sort.");
	return Expr();
      }
    case SMT_Info::BOOLEAN:
      {
	DebugAdvisory("made Boolean variable " << static_cast<DagNode*>(v));
	type = exprManager->booleanType();
	break;
      }
    case SMT_Info::INTEGER:
      {
	DebugAdvisory("made Integer variable " << static_cast<DagNode*>(v));
	type = exprManager->integerType();
	break;
      }
    case SMT_Info::REAL:
      {
	type = exprManager->realType();
	DebugAdvisory("made Real variable " << static_cast<DagNode*>(v));
	break;
      }
    }
  //
  //	Get the name as a C string, make the new variable and cache it under the pair.
  //
  const char* name = Token::name(id);
  Expr newVariable = exprManager->mkVar(name, type);
  variableMap.insert(VariableMap::value_type(p, newVariable));
  return newVariable;
}

Expr
VariableGenerator::makeBooleanExpr(DagNode* dag)
{
  Expr e;

  if (SMT_Symbol* s = dynamic_cast<SMT_Symbol*>(dag->symbol()))
    {
      Sort* rangeSort = s->getRangeSort();
      if (smtInfo.getType(rangeSort) == SMT_Info::BOOLEAN)
	{
	  e = dagToCVC4(dag);
	  DebugAdvisory("Maude: " << dag << "  CVC4: " << e);
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

Expr
VariableGenerator::makeRationalConstant(const mpq_class& rational)
{
  //
  //	We would like to do:
  //	  return em.mkConst(Rational(rational));
  //	but CVC4 makes the required constructor private thus we have this ugliness.
  //
  string ratStr = rational.get_str();
  return exprManager->mkConst(Rational(ratStr.c_str()));
}

Expr
VariableGenerator::dagToCVC4(DagNode* dag)
{
  //
  //	Deal with number constants (Integer or Real - CVC4 doesn't make much distinction).
  //
  if (SMT_NumberDagNode* n = dynamic_cast<SMT_NumberDagNode*>(dag))
    return makeRationalConstant(n->getValue());
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
      Vector<Expr> exprs(nrArgs);
      FreeDagNode* f = safeCast(FreeDagNode*, dag);

      for (int i = 0; i < nrArgs; ++i)
	{
	  Expr expr = dagToCVC4(f->getArgument(i));
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
	    return exprManager->mkConst(true);
	  }
	case SMT_Symbol::CONST_FALSE:
	  {
	    return exprManager->mkConst(false);
	  }
	case SMT_Symbol::NOT:
	  {
	    return exprManager->mkExpr(kind::NOT, exprs[0]);
	  }
	case SMT_Symbol::AND:
	  {
	    return exprManager->mkExpr(kind::AND, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::OR:
	  {
	    return exprManager->mkExpr(kind::OR, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::XOR:
	  {
	    return exprManager->mkExpr(kind::XOR, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::IMPLIES:
	  {
	    return exprManager->mkExpr(kind::IMPLIES, exprs[0], exprs[1]);
	  }
	  //
	  //	Polymorphic Boolean stuff.
	  //
	case SMT_Symbol::EQUALS:
	  {
	    return exprManager->mkExpr(kind::EQUAL, exprs[0], exprs[1]);
	    /*
	    //
	    //	Bizarrely CVC4 requires the IFF be used for Boolean equality so we need to
	    //	check the SMT type associated with our first argument sort to catch this case.
	    //
	    Sort* domainSort = s->getOpDeclarations()[0].getDomainAndRange()[0];
	    SMT_Info::SMT_Type smtType = smtInfo.getType(domainSort);
	    if (smtType == SMT_Info::NOT_SMT)
	      {
		IssueWarning("term " << QUOTE(dag) << " does not belong to an SMT sort.");
		goto fail;
	      }
	    return exprManager->mkExpr(((smtType == SMT_Info::BOOLEAN) ? kind::EQUAL : kind::EQUAL), exprs[0], exprs[1]);
	    */
	  }
	case SMT_Symbol::NOT_EQUALS:
	  {
	    return exprManager->mkExpr(kind::DISTINCT, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::ITE:
	  {
	    return exprManager->mkExpr(kind::ITE, exprs[0], exprs[1], exprs[2]);
	  }
	  //
	  //	Integer stuff.
	  //
	case SMT_Symbol::UNARY_MINUS:
	  {
	    return exprManager->mkExpr(kind::UMINUS, exprs[0]);
	  }
	case SMT_Symbol::MINUS:
	  {
	    return exprManager->mkExpr(kind::MINUS, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::PLUS:
	  {
	    return exprManager->mkExpr(kind::PLUS, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::MULT:
	  {
	    return exprManager->mkExpr(kind::MULT, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::DIV:
	  {
	    return exprManager->mkExpr(kind::INTS_DIVISION, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::MOD:
	  {
	    return exprManager->mkExpr(kind::INTS_MODULUS, exprs[0], exprs[1]);
	  }
	  //
	  //	Integer tests.
	  //
	case SMT_Symbol::LT:
	  {
	    return exprManager->mkExpr(kind::LT, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::LEQ:
	  {
	    return exprManager->mkExpr(kind::LEQ, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::GT:
	  {
	    return exprManager->mkExpr(kind::GT, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::GEQ:
	  {
	    return exprManager->mkExpr(kind::GEQ, exprs[0], exprs[1]);
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
		    string ratStr = rat.get_str();
		    Integer k(ratStr.c_str());
		    Divisible div(k);
		    Expr divOp = exprManager->mkConst(div);
		    return exprManager->mkExpr(divOp, exprs[0]);
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
	    return exprManager->mkExpr(kind::DIVISION, exprs[0], exprs[1]);
	  }
	case SMT_Symbol::TO_REAL:
	  {
	    return exprManager->mkExpr(kind::TO_REAL, exprs[0]);
	  }
	case SMT_Symbol::TO_INTEGER:
	  {
	    return exprManager->mkExpr(kind::TO_INTEGER, exprs[0]);
	  }
	case SMT_Symbol::IS_INTEGER:
	  {
	    return exprManager->mkExpr(kind::IS_INTEGER, exprs[0]);
	  }
	}
    }
  IssueWarning("term " << QUOTE(dag) << " is not a valid SMT term.");
 fail:
  return Expr();
}
