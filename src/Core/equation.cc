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
//	Implementation for class Equation.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
//#include "symbol.hh"
//#include "dagNode.hh"
#include "term.hh"
#include "rhsAutomaton.hh"
#include "instruction.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "termBag.hh"
#include "stackMachineRhsCompiler.hh"
#include "equation.hh"

Equation::Equation(int label,
		   Term* lhs,
		   Term* rhs,
		   bool owise,
		   const Vector<ConditionFragment*>& condition)
  : PreEquation(label, lhs, condition),
    rhs(rhs)
{
  if (owise)
    setFlags(OWISE);
  Assert(rhs != 0, "null rhs");
  instructionSequence = 0;
}

Equation::~Equation()
{
  rhs->deepSelfDestruct();
  delete instructionSequence;
}

void
Equation::check()
{
  NatSet boundVariables;
  PreEquation::check(boundVariables);
  rhs = rhs->normalize(false);
  rhs->indexVariables(*this);

  NatSet unboundVariables(rhs->occursBelow());
  unboundVariables.subtract(boundVariables);
  addUnboundVariables(unboundVariables);
  if (!isNonexec() && !getUnboundVariables().empty())
    {
      IssueWarning(*this << ": variable " <<
		   QUOTE(index2Variable(getUnboundVariables().min())) <<
		   " is used before it is bound in equation:\n" <<
		   this);
      //
      //	No legitimate use for such equations so mark it as bad.
      //
      markAsBad();
    }
}

void
Equation::preprocess()
{
  PreEquation::preprocess();
  rhs->symbol()->fillInSortInfo(rhs);
  Assert(getLhs()->getComponent() == rhs->getComponent(),
	 "connected component clash");
}

void
Equation::compile(bool compileLhs)
{
  if (isCompiled())
    return;
  setCompiled();
  TermBag availableTerms;  // terms available for reuse
  compileBuild(availableTerms, true);

  if (isVariant())
    {
      //
      //	If the equation has the variant attribute, we disallow left->right sharing so
      //	that the rhs can still be instantiated, even if the substitution was made by
      //	unification.
      //
      TermBag dummy;
      rhs->compileTopRhs(builder, *this, dummy);
      //
      //	For an equation with the variant attribute we always compile the lhs, even if the parent symbol
      //	doesn't make use of the compiled lhs (in the free theory because it uses a discrimination
      //	net for lhs matching).
      //
      compileLhs = true;
    }
  else
    rhs->compileTopRhs(builder, *this, availableTerms);  // normal case

  compileMatch(compileLhs, true);
  builder.remapIndices(*this);
  fast = hasCondition() ? DEFAULT : getNrProtectedVariables();  // HACK
}

void
Equation::stackMachineCompile()
{
  //
  //	We assume that the equation has already been compiled and thus its builder contains
  //	all the information on the rhs.
  //	We now generation a stack machine instruction sequence for that rhs.
  //
  StackMachineRhsCompiler compiler;
  if (builder.recordInfo(compiler) && !hasCondition())  // we don't handle conditions, and rhs might share subterms with condition
    {
      instructionSequence = compiler.compileInstructionSequence();
    }
}

int
Equation::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return context.traceBeginEqTrial(subject, this);
}

void
Equation::print(ostream& s) const
{
  s << this;
}
