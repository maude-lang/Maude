/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class VariableGenerator (Yices2 version)
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

int VariableGenerator::nrUsers = 0;

VariableGenerator::VariableGenerator(const SMT_Info& smtInfo)
  : smtInfo(smtInfo)
{
  if (nrUsers == 0)
    yices_init();
  ++nrUsers;
  smtContext = yices_new_context(NULL);
}

VariableGenerator::~VariableGenerator()
{
  yices_free_context(smtContext);
   --nrUsers;
  if (nrUsers == 0)
    yices_exit();
}

VariableGenerator::Result
VariableGenerator::assertDag(DagNode* dag)
{
  term_t t = makeBooleanExpr(dag);
  if (t == NULL_TERM)
    return BAD_DAG;

  yices_assert_formula(smtContext, t);
  int code = yices_assert_formula(smtContext, t);
  if (code < 0)
    {
      IssueWarning("Yices2 reported an error - giving up:");
      yices_print_error(stderr);
      return SAT_UNKNOWN;
    }
  //
  //	While yices_assert_formula() returns a Result, it may be unknown since
  //	it doesn't do a full sat solve.
  //
  smt_status_t result = yices_check_context(smtContext, NULL);
  DebugAdvisory("yices_check_context() returned " << result);

  if (result == STATUS_SAT)
    return SAT;
  if (result == STATUS_UNSAT)
    return UNSAT;
  IssueWarning("Yices2 not able to determine satisfiability  - giving up.");
  return SAT_UNKNOWN;
}

VariableGenerator::Result
VariableGenerator::checkDag(DagNode* dag)
{
  term_t t = makeBooleanExpr(dag);
  if (t == NULL_TERM)
    return BAD_DAG;

  yices_push(smtContext);
  int code = yices_assert_formula(smtContext, t);
  if (code < 0)
    {
      IssueWarning("Yices2 reported an error - giving up:");
      yices_print_error(stderr);
      yices_pop(smtContext);
      return SAT_UNKNOWN;
    }

  smt_status_t result = yices_check_context(smtContext, NULL);
  DebugAdvisory("yices_check_context() returned " << result);
  yices_pop(smtContext);

  if (result == STATUS_SAT)
    return SAT;
  if (result == STATUS_UNSAT)
    return UNSAT;
  IssueWarning("Yices2 not able to determine satisfiability  - giving up.");
  return SAT_UNKNOWN;
}

inline void
VariableGenerator::clearAssertions()
{
  yices_reset_context(smtContext);
}

inline void
VariableGenerator::push()
{
  yices_push(smtContext);
}

inline void
VariableGenerator::pop()
{
  DebugSave(r, yices_pop(smtContext));
  Assert(r == 0, "bad pop");
}

term_t
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
  //	expression, to avoid Yices2 making a distinct variable of the same name
  //	and type.
  //
  VariableMap::const_iterator i = variableMap.find(p);
  if (i != variableMap.end())
    return i->second;
  //
  //	Haven't seen this variable before - need to corresponding Yices2 type.
  //
  type_t type;
  switch (smtInfo.getType(sort))
    {
    case SMT_Info::NOT_SMT:
      {
	IssueWarning("Variable " << QUOTE(static_cast<DagNode*>(v)) << " does not belong to an SMT sort.");
	return NULL_TERM;
      }
    case SMT_Info::BOOLEAN:
      {
	DebugAdvisory("made Boolean variable " << static_cast<DagNode*>(v));
	type = yices_bool_type();
	break;
      }
    case SMT_Info::INTEGER:
      {
	DebugAdvisory("made Integer variable " << static_cast<DagNode*>(v));
	type = yices_int_type();
	break;
      }
    case SMT_Info::REAL:
    default:  // to avoid uninitialized variable warning
      {
	DebugAdvisory("made Real variable " << static_cast<DagNode*>(v));
	type = yices_real_type();
	break;
      }
    }
  //
  //	Get the name as a C string, make the new variable and cache it under the pair.
  //
  term_t newVariable = yices_new_uninterpreted_term(type);  // uninterpreted terms represent global variables
  const char* name = Token::name(id);
  yices_set_term_name(newVariable, name);
  variableMap.insert(VariableMap::value_type(p, newVariable));
  return newVariable;
}

term_t
VariableGenerator::makeBooleanExpr(DagNode* dag)
{
  //
  //	Check that dag actually represents a Boolean expression
  //	and convert it to Yices representation.
  //
  term_t t = NULL_TERM;

  if (SMT_Symbol* s = dynamic_cast<SMT_Symbol*>(dag->symbol()))
    {
      Sort* rangeSort = s->getRangeSort();
      if (smtInfo.getType(rangeSort) == SMT_Info::BOOLEAN)
	t = dagToYices2(dag);

    }
  else if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(dag))
    {
      Sort* rangeSort = v->symbol()->getRangeSort();
      if (smtInfo.getType(rangeSort) == SMT_Info::BOOLEAN)
	t = makeVariable(v);
    }

  WarningCheck(t != NULL_TERM, "Expecting an SMT Boolean expression but saw but saw " << dag);
  return t;
}

term_t
VariableGenerator::dagToYices2(DagNode* dag)
{
  //
  //	Deal with number constants (Do we need to make a distinction between Integer and Real?)
  //
  if (SMT_NumberDagNode* n = dynamic_cast<SMT_NumberDagNode*>(dag))
    return yices_mpq(n->getValue().get_mpq_t());
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
      Vector<term_t> terms(nrArgs);
      FreeDagNode* f = safeCast(FreeDagNode*, dag);

      for (int i = 0; i < nrArgs; ++i)
	{
	  term_t t = dagToYices2(f->getArgument(i));
	  if (t == NULL_TERM)
	    return t;
	  terms[i] = t;
	}

      switch (s->getOperator())
	{
	  //
	  //	Boolean stuff.
	  //
	case SMT_Symbol::CONST_TRUE:
	  {
	    return yices_true();
	  }
	case SMT_Symbol::CONST_FALSE:
	  {
	    return yices_false();
	  }
	case SMT_Symbol::NOT:
	  {
	    return yices_not(terms[0]);
	  }
	case SMT_Symbol::AND:
	  {
	    return yices_and2(terms[0], terms[1]);
	  }
	case SMT_Symbol::OR:
	  {
	    return yices_or2(terms[0], terms[1]);
	  }
	case SMT_Symbol::XOR:
	  {
	    return yices_xor2(terms[0], terms[1]);
	  }
	case SMT_Symbol::IMPLIES:
	  {
	    return yices_implies(terms[0], terms[1]);
	  }
	  //
	  //	Polymorphic Boolean stuff.
	  //
	case SMT_Symbol::EQUALS:
	  {
	    return yices_eq(terms[0], terms[1]);
	  }
	case SMT_Symbol::NOT_EQUALS:
	  {
	    return yices_neq(terms[0], terms[1]);
	  }
	case SMT_Symbol::ITE:
	  {
	    return yices_ite(terms[0], terms[1], terms[2]);
	  }
	  //
	  //	Integer stuff.
	  //
	case SMT_Symbol::UNARY_MINUS:
	  {
	    return yices_neg(terms[0]);
	  }
	case SMT_Symbol::MINUS:
	  {
	    return yices_sub(terms[0], terms[1]);
	  }
	case SMT_Symbol::PLUS:
	  {
	    return yices_add(terms[0], terms[1]);
	  }
	case SMT_Symbol::MULT:
	  {
	    return yices_mul(terms[0], terms[1]);
	  }
	case SMT_Symbol::DIV:
	  {
	    return yices_idiv(terms[0], terms[1]);
	  }
	case SMT_Symbol::MOD:
	  {
	    return yices_imod(terms[0], terms[1]);
	  }
	  //
	  //	Integer tests.
	  //
	case SMT_Symbol::LT:
	  {
	    return yices_arith_lt_atom(terms[0], terms[1]);
	  }
	case SMT_Symbol::LEQ:
	  {
	    return yices_arith_leq_atom(terms[0], terms[1]);
	  }
	case SMT_Symbol::GT:
	  {
	    return yices_arith_gt_atom(terms[0], terms[1]);
	  }
	case SMT_Symbol::GEQ:
	  {
	    return yices_arith_geq_atom(terms[0], terms[1]);
	  }
	case SMT_Symbol::DIVISIBLE:
	  {
	    DagNode* a = f->getArgument(1);
	    if (SMT_NumberDagNode* n = dynamic_cast<SMT_NumberDagNode*>(a))
	      {
		const mpq_class& rat = n->getValue();
		if (rat > 0)
		  return yices_divides_atom(terms[1], terms[0]);
	      }
	    IssueWarning("bad divisor in " << QUOTE(dag) << ".");
	    goto fail;
	  }
	  //
	  //	Stuff that is extra to reals.
	  //
	case SMT_Symbol::REAL_DIVISION:
	  {
	    return yices_division(terms[0], terms[1]);
	  }
	case SMT_Symbol::TO_REAL:
	  {
	    //
	    //	Yices2 treats integers as a subset of the reals.
	    //
	    return terms[0];
	  }
	case SMT_Symbol::TO_INTEGER:
	  {
	    return yices_floor(terms[0]);
	  }
	case SMT_Symbol::IS_INTEGER:
	  {
	    return yices_is_int_atom(terms[0]);

	  }
	}
    }
  IssueWarning("term " << QUOTE(dag) << " is not a valid SMT term.");
 fail:
  return NULL_TERM;
}
