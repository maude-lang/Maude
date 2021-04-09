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
//      Implementation for class VariableGenerator.
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

#ifdef USE_CVC4
#include "cvc4_Bindings.cc"
#elif defined(USE_YICES2) 
#include "yices2_Bindings.cc"
#else

//
//	Code for no SMT support case.
//

VariableGenerator::VariableGenerator(const SMT_Info& smtInfo)
{
  IssueWarning("No SMT solver linked at compile time.");
}

VariableGenerator::~VariableGenerator()
{
}

VariableGenerator::Result
VariableGenerator::assertDag(DagNode* dag)
{
  return SAT_UNKNOWN;
}

VariableGenerator::Result
VariableGenerator::checkDag(DagNode* dag)
{
  return SAT_UNKNOWN;
}

void
VariableGenerator::push()
{
}

void
VariableGenerator::pop()
{
}

inline void
VariableGenerator::clearAssertions()
{
}

#endif

//
//	Common code
//

VariableDagNode*
VariableGenerator::makeFreshVariable(Term* baseVariable, const mpz_class& number)
{
  Symbol* s = baseVariable->symbol();
  VariableTerm* vt = safeCast(VariableTerm*, baseVariable);
  int id = vt->id();

  string newNameString = "#";
  char* name = mpz_get_str(0, 10, number.get_mpz_t());
  newNameString += name;
  free(name);
  newNameString += "-";
  newNameString +=  Token::name(id);
  int newId = Token::encode(newNameString.c_str());

  return new VariableDagNode(s, newId, NONE);
}
