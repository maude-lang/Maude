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
//      Implementation for class SMT_NumberDagNode.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
 
//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      built in class definitions
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberDagNode.hh"

SMT_NumberDagNode::SMT_NumberDagNode(SMT_NumberSymbol* symbol, const mpq_class& value)
  : NA_DagNode(symbol),
    value(new mpq_class(value))
{
  setCallDtor();  // need our dtor called when garbage collected to destruct number
}

SMT_NumberDagNode::~SMT_NumberDagNode()
{
  delete value;
}

size_t
SMT_NumberDagNode::getHashValue()
{
  return hash(symbol()->getHashValue(),
	      mpz_tdiv_ui(value->get_num_mpz_t(), INT_MAX),
	      mpz_tdiv_ui(value->get_den_mpz_t(), INT_MAX));
}

int
SMT_NumberDagNode::compareArguments(const DagNode* other) const
{
  const mpq_class* otherValue = static_cast<const SMT_NumberDagNode*>(other)->value;
  return (*value == *otherValue) ? 0 : ((*value > *otherValue) ? 1 : -1);
}

void
SMT_NumberDagNode::overwriteWithClone(DagNode* old)
{
  SMT_NumberDagNode* d = new(old) SMT_NumberDagNode(safeCast(SMT_NumberSymbol*, symbol()), *value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
}

DagNode*
SMT_NumberDagNode::makeClone()
{
  SMT_NumberDagNode* d = new SMT_NumberDagNode(safeCast(SMT_NumberSymbol*, symbol()), *value);
  d->copySetRewritingFlags(this);
  d->setSortIndex(getSortIndex());
  return d;
}
