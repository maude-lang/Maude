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
//      Implementation for class FloatSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "mathStuff.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"

//	built in class definitions
#include "floatSymbol.hh"
#include "floatTerm.hh"
#include "floatDagNode.hh"

FloatSymbol::FloatSymbol(int id)
  : NA_Symbol(id)
{
  sort = 0;
  finiteSort = 0;
}

void
FloatSymbol::fillInSortInfo(Term* subject)
{
  Sort* s = finite(static_cast<FloatTerm*>(subject)->getValue()) ? finiteSort : sort;
  subject->setSortInfo(s->component(), s->index());
}

void
FloatSymbol::computeBaseSort(DagNode* subject)
{
  Sort* s = finite(static_cast<FloatDagNode*>(subject)->getValue()) ? finiteSort : sort;
  subject->setSortIndex(s->index());
}

bool
FloatSymbol::isConstructor(DagNode* /* subject */)
{
  return true;
}

void
FloatSymbol::compileOpDeclarations()
{
  // NA_Symbol::compileOpDeclarations();  // default processing
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      Sort* s = opDecls[i].getDomainAndRange()[0];
      if (sort == 0 || s->index() < sort->index())
	sort = s;  // set sort to largest (smallest index) declared sort
      if (finiteSort == 0 || s->index() > finiteSort->index())
	finiteSort = s;  // set finiteSort to smallest (largest index) declared sort
    }
}

bool
FloatSymbol::rewriteToFloat(DagNode* subject,
			    RewritingContext& context,
			    double result)
{
  bool trace = RewritingContext::getTraceStatus();
  if (trace)
    {
      context.tracePreEqRewrite(subject, 0, RewritingContext::BUILTIN);
      if (context.traceAbort())
	return false;
    }
  (void) new(subject) FloatDagNode(this, result);
  context.incrementEqCount();
  if (trace)
    context.tracePostEqRewrite(subject);
  return true;  
}

Term*
FloatSymbol::termify(DagNode* dagNode)
{
  return new FloatTerm(this, safeCast(FloatDagNode*, dagNode)->getValue());
}
