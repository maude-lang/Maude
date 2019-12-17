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
//      Implementation for class QuotedIdentifierSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "NA_Theory.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"

//	mixfix class definitions
#include "token.hh"
#include "importTranslation.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"

QuotedIdentifierSymbol::QuotedIdentifierSymbol(int id)
  : NA_Symbol(id)
{
  baseSort = 0;
  constantSort = 0;
  variableSort = 0;
  sortSort = 0;
  kindSort = 0;
}

bool
QuotedIdentifierSymbol::attachData(const Vector<Sort*>& opDeclaration,
				   const char* purpose,
				   const Vector<const char*>& data)
{
  if (strcmp(purpose, "QuotedIdentifierSymbol") == 0)
    {
      if (data.length() == 0)
	return true;
      if (data.length() == 1)
	{
	  const char* subsortName = data[0];
	  Sort* sort = opDeclaration[0];
	  if (strcmp(subsortName, "constantQid") == 0)
	    constantSort = sort;
	  else if (strcmp(subsortName, "variableQid") == 0)
	    variableSort = sort;
	  else if (strcmp(subsortName, "sortQid") == 0)
	    sortSort = sort;
	  else if (strcmp(subsortName, "kindQid") == 0)
	    kindSort = sort;
	  else
	    return false;
	  return true;
	}
      return false;
    }
  return NA_Symbol::attachData(opDeclaration, purpose, data);
}

#define COPY_SORT(original, name, mapping) \
  if (name == 0) \
    { \
      if (Sort* s = original->name) \
	name = (mapping == 0) ? s : mapping->translate(s); \
    }

void
QuotedIdentifierSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  QuotedIdentifierSymbol* orig = safeCast(QuotedIdentifierSymbol*, original);
  ImportTranslation* mapping = safeCast(ImportTranslation*, map);
  COPY_SORT(orig, constantSort, mapping);
  COPY_SORT(orig, variableSort, mapping);
  COPY_SORT(orig, sortSort, mapping);
  COPY_SORT(orig, kindSort, mapping);
  NA_Symbol::copyAttachments(original, map);
}

void
QuotedIdentifierSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
					   Vector<const char*>& purposes,
					   Vector<Vector<const char*> >& data)
{
  int nrDataAttachments = purposes.length();
  purposes.resize(nrDataAttachments + 1);
  purposes[nrDataAttachments] = "QuotedIdentifierSymbol";
  data.resize(nrDataAttachments + 1);
  Vector<const char*>& d = data[nrDataAttachments];
  if (opDeclaration[0] == constantSort)
    d.append("constantQid");
  else if(opDeclaration[0] == variableSort)
    d.append("variableQid");
  else if(opDeclaration[0] == sortSort)
    d.append("sortQid");
  else if(opDeclaration[0] == kindSort)
    d.append("kindQid");
  NA_Symbol::getDataAttachments(opDeclaration, purposes, data);
}

Sort*
QuotedIdentifierSymbol::determineSort(int idIndex)
{
  switch (Token::auxProperty(idIndex))
    {
    case Token::AUX_CONSTANT:
      {
	if (constantSort != 0)
	  return constantSort;
	break;
      }
    case Token::AUX_VARIABLE:
      {
	if (variableSort != 0)
	  return variableSort;
	break;
      }
    case Token::AUX_KIND:
      {
	if (kindSort != 0)
	  return kindSort;
	break;
      }
    case Token::AUX_SORT:
    case Token::AUX_STRUCTURED_SORT:
      {
	if (sortSort != 0)
	  return sortSort;
	break;
      }
    }
  return baseSort;
}

void
QuotedIdentifierSymbol::fillInSortInfo(Term* subject)
{
  Sort* s = determineSort(static_cast<QuotedIdentifierTerm*>(subject)->getIdIndex());
  subject->setSortInfo(s->component(), s->index());
}

void
QuotedIdentifierSymbol::computeBaseSort(DagNode* subject)
{
  Sort* s = determineSort(static_cast<QuotedIdentifierDagNode*>(subject)->getIdIndex());
  subject->setSortIndex(s->index());
}

bool
QuotedIdentifierSymbol::isConstructor(DagNode* /* subject */)
{
  return true;
}

void
QuotedIdentifierSymbol::compileOpDeclarations()
{
  const Vector<OpDeclaration>& opDecls = getOpDeclarations();
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      Sort* s = opDecls[i].getDomainAndRange()[0];
      if (baseSort == 0 || s->index() < baseSort->index())
	baseSort = s;  // set baseSort to largest (smallest index) declared sort
    }
}

Term*
QuotedIdentifierSymbol::termify(DagNode* dagNode)
{
  return new QuotedIdentifierTerm(this, safeCast(QuotedIdentifierDagNode*, dagNode)->getIdIndex());
}
