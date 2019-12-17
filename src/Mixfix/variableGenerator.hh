/*

    This file is part of the Maude 3 interpreter.

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
//      Class for generating SMT variables, with dummy version for no SMT support.
//
#ifndef _variableGenerator_hh_
#define _variableGenerator_hh_
#include "SMT_Info.hh"
#include "SMT_EngineWrapper.hh"

#ifdef USE_CVC4
#include "cvc4_Bindings.hh"
#elif defined(USE_YICES2) 
#include "yices2_Bindings.hh"
#else

//
//	Code for no SMT support case.
//

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
};

#endif
#endif
