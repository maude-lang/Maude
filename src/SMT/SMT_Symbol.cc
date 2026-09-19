/*

    This file is part of the Maude 3 interpreter.

    Copyright 2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SMT_Symbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "variable.hh"
#include "SMT.hh"

//      interface class definitions
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"

//      variable class definitions
#include "variableSymbol.hh"
#include "variableDagNode.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//	SMT stuff
#include "SMT_Info.hh"
#include "SMT_Symbol.hh"
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberDagNode.hh"

const char* SMT_Symbol::operatorNames[] =
  {
    //
    //	Boolean stuff.
    //
    "true",
    "false",
    "not",
    "and",
    "or",
    "xor",
    "implies",
    //
    //	Polymorphic Boolean stuff.
    //
    "===",
    "=/==",
    "ite",
    //
    //	Integer stuff.
    //
    "-",
    "-",
    "+",
    "*",
    "div",
    "mod",
    //
    //	Integer tests.
    //
    "<",
    "<=",
    ">",
    ">=",
    "divisible",
    //
    //	Stuff that is extra to reals.
    //
    "/",
    "toReal",
    "toInteger",
    "isInteger",
    //
    //	Set stuff.
    //
    "setMembership",
    "setUnion",
    "setIntersection",
    "setDifference",
    "setSubset",
    "setEmpty",
    "setCardinality",
    "setSingleton",
    "setInsert",
    "setRemove",
    "setComplement",
    "setUniverse",
    0
  };

SMT_Symbol::SMT_Symbol(int id, int arity)
  : FreeSymbol(id, arity)
{
  op = NONE;
}

bool
SMT_Symbol::attachData(const Vector<Sort*>& opDeclaration,
		       const char* purpose,
		       const Vector<const char*>& data)
{
  const char* opName = data[0];
  if (strcmp(opName, "-") == 0)
    {
      op = (arity() == 1) ? UNARY_MINUS : MINUS;
      return true;
    }
  for (const char** p =  operatorNames; *p; ++p)
    {
      if (strcmp(*p, opName) == 0)
	{
	  op = p - operatorNames;
	  return true;
	}
    }
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

void
SMT_Symbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  SMT_Symbol* orig = safeCast(SMT_Symbol*, original);
  op = orig->op;
  FreeSymbol::copyAttachments(original, map);
}

void
SMT_Symbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
			       Vector<const char*>& purposes,
			       Vector<Vector<const char*> >& data)
{
  if (op != NONE)
    {
      int nrDataAttachments = purposes.length();
      purposes.resize(nrDataAttachments + 1);
      purposes[nrDataAttachments] = "SMT_Symbol";
      data.resize(nrDataAttachments + 1);
      data[nrDataAttachments].resize(1);
      data[nrDataAttachments][0] = operatorNames[op];
    }
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
SMT_Symbol::fillOutSMT_Info(SMT_Info& info)
{
  //
  //	If we are the construction operator for some SMT type, fill out that information.
  //
  switch (op)
    {
    case CONST_TRUE:
      {
	info.setTrueSymbol(this);
	// fall thru
      }
    case CONST_FALSE:
      {
	info.setType(getRangeSort(), SMT_Info::BOOLEAN);
	break;
      }
    case AND:
      {
	info.setConjunctionOperator(this);
	break;
      }
    case EQUALS:
      {
	info.setEqualityOperator(this);
	break;
      }
      //
      //	Set stuff :)
      //
      //	We NEED to be careful here to mark the set sort and
      //	not the range sort as setMembership / setSubset return Boolean
      //	but setCardinality returns Integer
      //
    case SET_EMPTY:
    case SET_UNIVERSE:
    case SET_UNION:
    case SET_INTERSECTION:
    case SET_DIFFERENCE:
    case SET_COMPLEMENT:
      {
	//
	//	range is the set sort and there is no element sort info
	//	here (setUnion et al. are recorded by their element-taking
	//	sibs in the same mod)
	//
	info.setType(getRangeSort(), SMT_Info::SET);
	break;
      }
    case SET_SINGLETON:
    case SET_INSERT:
    case SET_REMOVE:
      {
	//
	//	elt -> set or elt set -> set
  //  arg 0 is the element
	//
	Sort* setSort = getRangeSort();
	info.setType(setSort, SMT_Info::SET);
	if (getOpDeclarations().length() > 0)
	  info.setSetElementSort(setSort, getOpDeclarations()[0].getDomainAndRange()[0]);
	break;
      }
    case SET_MEMBERSHIP:
      {
	//
	//	elt set -> Boolean .  
  //  arg 0 is the element, arg 1 the set
	//
	if (getOpDeclarations().length() > 0)
	  {
	    const Vector<Sort*>& domainAndRange = getOpDeclarations()[0].getDomainAndRange();
	    info.setType(domainAndRange[1], SMT_Info::SET);
	    info.setSetElementSort(domainAndRange[1], domainAndRange[0]);
	  }
	break;
      }
    case SET_SUBSET:
    case SET_CARDINALITY:
      {
	//
	//	set ... -> Boolean/Integer
  /// arg 0 is a set here
	//
	if (getOpDeclarations().length() > 0)
	  info.setType(getOpDeclarations()[0].getDomainAndRange()[0], SMT_Info::SET);
	break;
      }
    default:
      break;
    }
}
