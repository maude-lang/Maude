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
//      Class for generating SMT variables, version for CVC5 support.
//
#ifndef _cvc5_Bindings_hh_
#define _cvc5_Bindings_hh_

#include <map>
#include "cvc5/cvc5.h"

class VariableGenerator : public SMT_EngineWrapper
{
public:
  VariableGenerator(const SMT_Info& smtInfo);
  ~VariableGenerator();
  //
  //	Virtual functions for SMT solving.
  //
  Result assertDag(DagNode* dag);
  Result checkDag(DagNode* dag);
  void clearAssertions();
  void push();
  void pop();

  VariableDagNode* makeFreshVariable(Term* baseVariable, const mpz_class& number);

private:
  //
  //	We identify Maude variables that correspond to SMT variables by a pair
  //	where the first component in the variable's sort's index within its module
  //	and the second component is the variables name.
  //
  typedef pair<int, int> SortIndexVariableNamePair;
  //
  //	When we generate SMT variables on-the-fly we keep track of them in a map.
  //
  typedef map<SortIndexVariableNamePair, cvc5::Term> VariableMap;

  cvc5::Term makeVariable(VariableDagNode* v);
  cvc5::Term makeBooleanExpr(DagNode* dag);
  cvc5::Term makeNumberConstant(const mpq_class& rational, bool isInteger);
  cvc5::Term dagToCvc5(DagNode* dag);

  const SMT_Info& smtInfo;

  VariableMap variableMap;
  int pushCount;
  //
  //	CVC5 objects.
  //
  cvc5::TermManager termManager;
  cvc5::Solver smtSolver;
};

#endif
