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
//      Implementation for class StringSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	built in class definitions
#include "stringSymbol.hh"
#include "stringTerm.hh"
#include "stringDagNode.hh"

StringSymbol::StringSymbol(int id)
  : NA_Symbol(id)
{
  sort = 0;
  charSort = 0;
}

void
StringSymbol::fillInSortInfo(Term* subject)
{
  Sort* s = (static_cast<StringTerm*>(subject)->getValue().length() == 1) ? charSort : sort;
  subject->setSortInfo(s->component(), s->index());
}

void
StringSymbol::computeBaseSort(DagNode* subject)
{
  Sort* s = (static_cast<StringDagNode*>(subject)->getValue().length() == 1) ? charSort : sort;
  subject->setSortIndex(s->index());
}

bool
StringSymbol::isConstructor(DagNode* /* subject */)
{
  return true;
}

void
StringSymbol::compileOpDeclarations()
{
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      Sort* s = opDecls[i].getDomainAndRange()[0];
      if (sort == 0 || s->index() < sort->index())
	sort = s;  // set sort to largest (smallest index) declared sort
      if (charSort == 0 || s->index() > charSort->index())
	charSort = s;  // set charSort to smallest (largest index) declared sort
    }
}

Term*
StringSymbol::termify(DagNode* dagNode)
{
  return new StringTerm(this, safeCast(StringDagNode*, dagNode)->getValue());
}
