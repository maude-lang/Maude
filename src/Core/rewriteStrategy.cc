/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class RewriteStrategy
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//		interface class definitions
#include "symbol.hh"
#include "term.hh"
#include "rawArgumentIterator.hh"

//      core class definitions
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

//	full compiler definitions
#include "compilationContext.hh"

RewriteStrategy::RewriteStrategy(int id,
				 const Vector<Sort*>& domain,
				 Sort* subjectSort, Symbol* auxSymbol)
  : NamedEntity(id),
    domain(domain),
    subjectSort(subjectSort),
    symbol(auxSymbol),
    simple(domain.empty())
{
  Assert(auxSymbol != 0, "null auxiliary symbol");

  // We do not assert anything about subjectSort because it has no use at the moment
}

Term*
RewriteStrategy::copyAuxiliaryTerm(Term* term, SymbolMap* translation) const
{
  Vector<Term*> args(symbol->arity());

  RawArgumentIterator* it = term->arguments();
  for (size_t i = 0; i < args.size(); i++)
    {
      Assert(it->valid(), "non-valid iterator");

      args[i] = it->argument()->deepCopy(translation);
      it->next();
    }
  delete it;

  return symbol->makeTerm(args);
}

void
RewriteStrategy::addDefinition(StrategyDefinition* sdef)
{
  if (simple && (definitions.length() > 0 || sdef->getCondition().length() > 0))
    simple = false;

  definitions.append(sdef);
}
