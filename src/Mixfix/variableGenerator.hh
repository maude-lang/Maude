/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for generating SMT variables and handling CVC4 specifics.
//
#ifndef _variableGenerator_hh_
#define _variableGenerator_hh_
#include <map>
#include "SMT_Info.hh"
#include "SMT_EngineWrapper.hh"

#ifdef USE_CVC4
#include "cvc4/expr/expr_manager.h"
#include "cvc4/smt/smt_engine.h"

using namespace CVC4;
#endif

class VariableGenerator : public SMT_EngineWrapper
{
public:
  VariableGenerator(const SMT_Info& smtInfo);
  ~VariableGenerator();
  //
  //	Extra functionality for when we do full abstraction of SMT solving.
  //
  Result assertDag(DagNode* dag);
  Result checkDag(DagNode* dag);
  void clearAssertions();
  void push();
  void pop();

  VariableDagNode* makeFreshVariable(Term* baseVariable, const mpz_class& number);

private:
#ifdef USE_CVC4

  //
  //	We identify Maude variables that correspond to SMT variables by a pair
  //	where the first component in the variable's sort's index within its module
  //	and the second component is the variables name.
  //
  typedef pair<int, int> SortIndexVariableNamePair;
  //
  //	When we generate SMT variables on-the-fly we keep track of them in a map.
  //
  typedef map<SortIndexVariableNamePair, Expr> VariableMap;

  Expr makeVariable(VariableDagNode* v);
  Expr makeBooleanExpr(DagNode* dag);
  Expr makeRationalConstant(const mpq_class& rational);
  Expr dagToCVC4(DagNode* dag);
#endif

  const SMT_Info& smtInfo;

#ifdef USE_CVC4
  VariableMap variableMap;
  int pushCount;
  //
  //	CVC4 objects.
  //
  ExprManager* exprManager;
  SmtEngine* smtEngine;
#endif
};

inline void
VariableGenerator::push()
{
#ifdef USE_CVC4
  smtEngine->push();
  ++pushCount;
#endif
}

inline void
VariableGenerator::pop()
{
#ifdef USE_CVC4
  Assert(pushCount > 0, "bad pop");
  smtEngine->pop();
  --pushCount;
#endif
}

inline void
VariableGenerator::clearAssertions()
{
#ifdef USE_CVC4
  while (pushCount > 0)
    {
      smtEngine->pop();
      --pushCount;
    }
  smtEngine->pop();  // get back to original clean context
  smtEngine->push();  // make a new context so we don't pollute the context we want to pop back to
#endif
}

#endif
