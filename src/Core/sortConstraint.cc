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
//      Implementation for class SortConstraint.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "rewritingContext.hh"
#include "termBag.hh"
#include "sortConstraint.hh"

SortConstraint::SortConstraint(int label,
			       Term* lhs,
			       Sort* sort,
			       const Vector<ConditionFragment*>& condition)
  : PreEquation(label, lhs, condition),
    sort(sort)
{
  Assert(sort != 0, "null sort");
}

void
SortConstraint::check()
{
  NatSet boundVariables;
  PreEquation::check(boundVariables);
  if (!isNonexec() && !getUnboundVariables().empty())
    {
      IssueWarning(*this << ": variable " <<
                   QUOTE(index2Variable(getUnboundVariables().min())) <<
                   " is used before it is bound in membership axiom:\n" <<
                   this);
      markAsBad();
    }
}

void
SortConstraint::preprocess()
{
  PreEquation::preprocess();
  Assert(getLhs()->getComponent() == sort->component(), "component clash");
}

void
SortConstraint::compile(bool compileLhs)
{
  if (isCompiled())
    return;
  setCompiled();
  TermBag availableTerms;  // terms available for reuse
  compileBuild(availableTerms, false);
  compileMatch(compileLhs, false);
}

int
SortConstraint::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return context.traceBeginScTrial(subject, this);
}

void
SortConstraint::print(ostream& s) const
{
  s << this;
}
