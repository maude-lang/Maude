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
//      Implementation for class EqualitySymbol.
//
 
//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
 
//      core class definitions
#include "argumentIterator.hh"
#include "rewritingContext.hh"

//      free theory class definitions
#include "freeDagNode.hh"
 
//	built in class definitions
#include "bindingMacros.hh"
#include "equalitySymbol.hh"

//	full compiler classes
#include "compilationContext.hh"
#include "variableName.hh"

EqualitySymbol::EqualitySymbol(int id, const Vector<int>& strategy)
  : FreeSymbol(id, 2, strategy)
{
}

bool
EqualitySymbol::attachData(const Vector<Sort*>& opDeclaration,
			   const char* purpose,
			   const Vector<const char*>& data)
{
  NULL_DATA(purpose, EqualitySymbol, data);
  return FreeSymbol::attachData(opDeclaration, purpose, data);
}

bool
EqualitySymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, equalTerm);
  BIND_TERM(purpose, term, notEqualTerm);
  return FreeSymbol::attachTerm(purpose, term);
}

void
EqualitySymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  EqualitySymbol* orig = safeCast(EqualitySymbol*, original);
  COPY_TERM(orig, equalTerm, map);
  COPY_TERM(orig, notEqualTerm, map);
  FreeSymbol::copyAttachments(original, map);
}

void
EqualitySymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				   Vector<const char*>& purposes,
				   Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, EqualitySymbol);
  FreeSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
EqualitySymbol::getTermAttachments(Vector<const char*>& purposes,
				   Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, equalTerm);
  APPEND_TERM(purposes, terms, notEqualTerm);
  FreeSymbol::getTermAttachments(purposes, terms);
}

void
EqualitySymbol::postInterSymbolPass()
{
  (void) equalTerm.normalize();
  equalTerm.prepare();
  (void) notEqualTerm.normalize();
  notEqualTerm.prepare();
}

void
EqualitySymbol::reset()
{
  equalTerm.reset();  // so equal dag can be garbage collected
  notEqualTerm.reset();  // so notEqualDag dag can be garbage collected
  FreeSymbol::reset();  // parents reset() tasks
}

bool
EqualitySymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  FreeDagNode* f = static_cast<FreeDagNode*>(subject);
  DagNode* l = f->getArgument(0);
  DagNode* r = f->getArgument(1);
  if (standardStrategy())
    {
      l->reduce(context);
      r->reduce(context);
    }
  else
    {
      const Vector<int>& userStrategy = getStrategy();
      for(int i = 0;; i++)
	{
	  int a = userStrategy[i];
	  if (a == 0)
	    break;
	  f->getArgument(a - 1)->reduce(context);
	}
      l->computeTrueSort(context);  // we don't need the sort but we do need to normalize
      r->computeTrueSort(context);
    }
  return context.builtInReplace(subject, l->equal(r) ?
				equalTerm.getDag() : notEqualTerm.getDag());
}

bool
EqualitySymbol::domainSortAlwaysLeqThan(Sort* /* sort */, int /* argNr */)
{
  return false;
}

bool
EqualitySymbol::acceptEquation(Equation* /* equation */)
{
  return false;
}

void
EqualitySymbol::compileEquations()
{
}

#ifdef COMPILER

void
EqualitySymbol::generateCode(CompilationContext& context) const
{
  Vector<VariableName> varNames;  // dummy
  context.body() << Indent(1) << "if (compare(a0, a1) == 0)\n";
  context.body() << Indent(2) << "{\n";
  equalTerm.getTerm()->generateRhs(context, 3, varNames, 0);
  context.body() << Indent(2) << "}\n";
  context.body() << Indent(1) << "else\n";
  context.body() << Indent(2) << "{\n";
  notEqualTerm.getTerm()->generateRhs(context, 3, varNames, 0);
  context.body() << Indent(2) << "}\n";
}

#endif
