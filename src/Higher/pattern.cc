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
//	Implementation for class Pattern.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"

//	interface class definitions
#include "term.hh"
#include "lhsAutomaton.hh"

//	core class definitions
#include "termBag.hh"
#include "substitution.hh"
#include "pattern.hh"

Pattern::Pattern(Term* patternTerm,
		 bool withExtension,
		 const Vector<ConditionFragment*>& condition)
  : PreEquation(NONE, patternTerm, condition)
{
  //
  //	Patterns belong to commands and descent function invocations; they
  //	don't belong to modules - we set null information here so we don't
  //	have uninitialized data and other things can recognize we don't belong
  //	to a module.
  //
  setModuleInfo(0, NONE);

  NatSet boundVariables;  // variables bound by matching
  check(boundVariables);
  if (!(isBad()))
    {
      preprocess();
      addConditionVariables(getLhs()->occursBelow());
      TermBag availableTerms;  // terms available for reuse
      compileBuild(availableTerms, false);
      compileMatch(true, withExtension);
    }
}

int
Pattern::traceBeginTrial(DagNode* subject, RewritingContext& context) const
{
  return UNDEFINED;
}

void
Pattern::print(ostream& s) const
{
  s << this;
}
