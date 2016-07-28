/*

    This file is part of the Maude 2 interpreter.

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
//      Implementation for class SMT_Info.
//

//#include "cvc4/util/divisible.h"

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "connectedComponent.hh"

//	SMT stuff
#include "SMT_Info.hh"

void
SMT_Info::setEqualityOperator(Symbol* symbol)
{
  int kindIndex = symbol->domainComponent(0)->getIndexWithinModule();
  Assert(kindIndex == symbol->domainComponent(1)->getIndexWithinModule(),
	 "domain kind mismatch for SMT equality symbol");
  DebugAdvisory("added equality operator for kindIndex = " << kindIndex);
  equalityOperatorMap[kindIndex] = symbol;
}

Symbol*
SMT_Info::getEqualityOperator(DagNode* lhs, DagNode* rhs) const
{
  DebugAdvisory("number of equality operator =  " << equalityOperatorMap.size());
  int kindIndex = lhs->symbol()->rangeComponent()->getIndexWithinModule();
  Assert(kindIndex == rhs->symbol()->rangeComponent()->getIndexWithinModule(),
	 "kind mis-match for " << lhs << " and " << rhs);
  EqualityOperatorMap::const_iterator i = equalityOperatorMap.find(kindIndex);
  return (i == equalityOperatorMap.end()) ? 0 : i->second;
}
