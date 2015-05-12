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
//      Implementation for base class DagNode
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"

//	interface class definitions
#include "symbol.hh"
#include "binarySymbol.hh"
#include "dagNode.hh"
#include "subproblem.hh"
#include "lhsAutomaton.hh"
#include "extensionInfo.hh"

//	core class definitions
#include "substitution.hh"
#include "rewritingContext.hh"
#include "sortCheckSubproblem.hh"
#include "dagArgumentIterator.hh"
#include "module.hh"
#include "rootContainer.hh"
#include "sortBdds.hh"
#include "unificationContext.hh"
#include "equation.hh"

//	variable class definitions
#include "variableDagNode.hh"

#if 0
//
//	Outlined hash code for easy experimentation without changing header.
//
#include <gmpxx.h>
size_t
DagNode::hash(size_t v1, size_t v2)
{

  size_t sq = v1 * v1;
  //  size_t result =  3 * v1 + v2;

  //size_t result =  v1 ^ (v1 >> 16) ^ v2;
  //size_t result =  sq  ^ (v1 >> 16) ^ v2;
  size_t result =  sq ^ (sq >> 16) ^ (v1 >> 16) ^ v2;
  /*
  DebugAdvisory("hash v1 = " << mpz_get_str(0, 2, mpz_class(v1).get_mpz_t()) <<
		" : v2 " << mpz_get_str(0, 2, mpz_class(v2).get_mpz_t()) <<
		" : sq " << mpz_get_str(0, 2, mpz_class(sq).get_mpz_t()) <<
		" : result " << mpz_get_str(0, 2, mpz_class(result).get_mpz_t()));
  */
  return result;


  //return (v1 * v2 + ((v1 >> 16))) ^ (v2 >> 16);
  //return (v1 >> 3) ^ (v1 << 29) ^ v2;
  //return 3 * v1 + v2;  // bad hash function!
  //return (v1 >> 1) + (v2 << 1);
}
#endif

bool
DagNode::checkSort(const Sort* boundSort, Subproblem*& returnedSubproblem)
{
  returnedSubproblem = 0;
  if (getSortIndex() != Sort::SORT_UNKNOWN)
    return leq(boundSort);
  topSymbol->computeBaseSort(this);
  if (leq(boundSort))
    {
      if (!(topSymbol->sortConstraintFree()))
	setSortIndex(Sort::SORT_UNKNOWN);
    }
  else
    { 
      if (topSymbol->sortConstraintFree())
	return false;
      setSortIndex(Sort::SORT_UNKNOWN);
      returnedSubproblem = new SortCheckSubproblem(this, boundSort);
    }
  return true;
}

bool
DagNode::checkSort(const Sort* boundSort, RewritingContext& context)
{
  if (getSortIndex() == Sort::SORT_UNKNOWN)
    {
      topSymbol->computeBaseSort(this);
      if (leq(boundSort))
	{
	  if (!(topSymbol->sortConstraintFree()))
	    setSortIndex(Sort::SORT_UNKNOWN);
	  return true;
	}
      else
	{
	  if (topSymbol->sortConstraintFree())
	    return false;
	  RewritingContext* local = context.makeSubcontext(this, RewritingContext::SORT_EVAL);
	  topSymbol->constrainToSmallerSort(this, *local);
	  context.addInCount(*local);
	  delete local;
	}
    }
  return leq(boundSort);
}

bool
DagNode::matchVariable(int index,
		       const Sort* sort,
		       bool copyToAvoidOverwriting,
		       Substitution& solution,
		       Subproblem*& returnedSubproblem,
		       ExtensionInfo* extensionInfo)
{
  if (extensionInfo != 0)
    return matchVariableWithExtension(index, sort, solution, returnedSubproblem, extensionInfo);
  DagNode* d = solution.value(index);
  if (d == 0)
    {
      if (checkSort(sort, returnedSubproblem))
        {
          solution.bind(index, copyToAvoidOverwriting ? makeClone() : this);
          return true;
        }
    }
  else
    {
      if (compare(d) == 0)
        {
          returnedSubproblem = 0;
          return true;
        }
    }
  return false;
}

ExtensionInfo*
DagNode::makeExtensionInfo()
{
  return 0;
}

//
//	Dummy functions to allow linking of theories which don't use extension.
//

bool
DagNode::matchVariableWithExtension(int /* index */,
				    const Sort* /* sort */,
				    Substitution& /* solution */,
				    Subproblem*& /* returnedSubproblem */,
				    ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
  return false;
}

void
DagNode::partialReplace(DagNode* /* replacement */, ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
}

DagNode*
DagNode::partialConstruct(DagNode* /* replacement */, ExtensionInfo* /* extensionInfo */)
{
  CantHappen("Called on subject " << this);
  return 0;
}

//
//	Unification code.
//

DagNode::ReturnResult
DagNode::computeBaseSortForGroundSubterms()
{
  //
  //	This is the backstop version for an unimplemented theory. If
  //	all our subterms are ground we compute our sort and return GROUND
  //	other we return UNIMPLEMENTED.
  //
  for (DagArgumentIterator a(*this); a.valid(); a.next())
    {
      switch (a.argument()->computeBaseSortForGroundSubterms())
	{
	case NONGROUND:
	  IssueWarning("Term " << QUOTE(this) <<
		       " is non-ground and unification for its top symbol is not currently supported.");
	  // fall thru
	case UNIMPLEMENTED:
	  return UNIMPLEMENTED;
	default:
	  ;  // to avoid compiler warning
	}
    }
  topSymbol->computeBaseSort(this);
  setGround();
  return GROUND;
}

bool
DagNode::computeSolvedForm(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  DebugAdvisory("computeSolvedForm() lhs = " << this << " rhs = " << rhs);
  //
  //	If we are nonground we dispatch the theory specific algorithm.
  //
  if (!isGround())
    return computeSolvedForm2(rhs, solution, pending);
  //
  //	Ground. If the other unificand is nonground, call its algorithm.
  //
  if (!(rhs->isGround()))
    return rhs->computeSolvedForm2(this, solution, pending);
  //
  //
  //	We have two ground terms so we can just compare them without the
  //	need for an unification algorithm.
  //
  return equal(rhs);
}

bool
DagNode::computeSolvedForm2(DagNode* rhs, UnificationContext& solution, PendingUnificationStack& pending)
{
  DebugAdvisory("DagNode::computeSolvedForm2() " << this << " vs " << rhs);

  if (isGround())
    {
      //
      //	We handle the case
      //	  <ground term> =? X
      //	for unimplmented theories now that variable code no longer binds variables to nonvariables.
      //
      if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(rhs))
	{
	  VariableDagNode* repVar = v->lastVariableInChain(solution);
	  if (DagNode* value = solution.value(repVar->getIndex()))
	    return computeSolvedForm(value, solution, pending);
	  else
	    {
	      solution.unificationBind(repVar, this);  // bind variable to unpurified ground term
	      return true;
	    }
	}
    }
  IssueWarning("Unification modulo the theory of operator " << QUOTE(this->topSymbol) << " is not currently supported.");
  return false;
}

void
DagNode::computeGeneralizedSort(const SortBdds& sortBdds,
				const Vector<int>& realToBdd,
				Vector<Bdd>& generalizedSort)
{
  if (isGround())
    {
      //
      //	We assume that any code setting the ground flag will also ensure a sort index is set.
      //	FIXME: this may not be true if the node is unreduced.
      //
      Assert(getSortIndex() != Sort::SORT_UNKNOWN, "unknown sort in node flagged as ground");
      int nrBdds = sortBdds.getNrVariables(symbol()->rangeComponent()->getIndexWithinModule());
      sortBdds.makeIndexVector(nrBdds, getSortIndex(), generalizedSort);
    }
  else
    symbol()->computeGeneralizedSort(sortBdds, realToBdd, this, generalizedSort);
}

//
//	Narrowing code.
//

bool
DagNode::indexVariables2(NarrowingVariableInfo& indices, int baseIndex)
{
  //
  //	This is the backstop version for an unimplemented theory. It does the right
  //	thing but is rather inefficient for runtime code.
  //
  bool ground = true;
  for (DagArgumentIterator a(*this); a.valid(); a.next())
    {
      if (!(a.argument()->indexVariables(indices, baseIndex)))
	ground = false;
    }
  return ground;
}

//
//	Variant narrowing code.
//

bool
DagNode::reducibleByVariantEquation(RewritingContext& context)
{
  //
  //	If it is already reduced wrt all equations it clearly can't be reduced by a variant equation.
  //
  if (isReduced() || isIrreducibleByVariantEquations())
    return false;
  //
  //	Naive and inefficient approach.
  //
  for (DagArgumentIterator a(this); a.valid(); a.next())
    {
      if (a.argument()->reducibleByVariantEquation(context))
	return true;
    }

  Subproblem* sp;
  ExtensionInfo* extensionInfo = makeExtensionInfo();

  const Vector<Equation*>& equations = symbol()->getEquations();
  FOR_EACH_CONST(i, Vector<Equation*>, equations)
    {
      Equation* eq = *i;
      if (eq->isVariant())
	{
	  int nrVariables = eq->getNrProtectedVariables();
	  context.clear(nrVariables);
	  if (eq->getLhsAutomaton()->match(this, context, sp, extensionInfo))
	    {
	      if (sp == 0 || sp->solve(true, context))
		{
		  delete extensionInfo;
		  delete sp;
		  return true;
		}
	      delete sp;
	    }
	}
    }
  setIrreducibleByVariantEquations();
  delete extensionInfo;
  return false;
}

#ifdef DUMP
void
DagNode::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "Begin{DagNode}\n";
  ++indentLevel;
  dumpCommon(s, indentLevel);
  s << Indent(indentLevel) << "arguments:\n";
  ++indentLevel;
  for (DagArgumentIterator a(*this); a.valid(); a.next())
    a.argument()->dump(s, indentLevel);
  s << Indent(indentLevel - 2) << "End{DagNode}\n";
}

void
DagNode::dumpCommon(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "topSymbol = " << topSymbol << '\n';
}
#endif
