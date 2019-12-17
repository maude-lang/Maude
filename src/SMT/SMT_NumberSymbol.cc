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
//      Implementation for class SMT_NumberSymbol.
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
#include "term.hh"

//      core class definitions
#include "rewritingContext.hh"
#include "symbolMap.hh"

//      built in stuff
#include "bindingMacros.hh"

//	SMT stuff
#include "SMT_Info.hh"
#include "SMT_NumberSymbol.hh"
#include "SMT_NumberTerm.hh"
#include "SMT_NumberDagNode.hh"

SMT_NumberSymbol::SMT_NumberSymbol(int id)
  : NA_Symbol(id)
{
  numberSystem = NONE;
}

bool
SMT_NumberSymbol::attachData(const Vector<Sort*>& opDeclaration,
			     const char* purpose,
			     const Vector<const char*>& data)
{
  BIND_OP(purpose, SMT_NumberSymbol, numberSystem, data);
  return NA_Symbol::attachData(opDeclaration, purpose, data);
}

void
SMT_NumberSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  SMT_NumberSymbol* orig = safeCast(SMT_NumberSymbol*, original);
  numberSystem = orig->numberSystem;
  NA_Symbol::copyAttachments(original, map);
}

void
SMT_NumberSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				     Vector<const char*>& purposes,
				     Vector<Vector<const char*> >& data)
{
  if (numberSystem != NONE)
    {
      int nrDataAttachments = purposes.length();
      purposes.resize(nrDataAttachments + 1);
      purposes[nrDataAttachments] = "SMT_NumberSymbol";
      data.resize(nrDataAttachments + 1);
      data[nrDataAttachments].resize(1);
      const char*& d = data[nrDataAttachments][0];
  
      switch (numberSystem)
	{
	  CODE_CASE(d, 'i', 'n', "integers");
	  CODE_CASE(d, 'r', 'e', "reals");
	  default:
	    CantHappen("bad SMT number system");
	}
    }
  NA_Symbol::getDataAttachments(opDeclaration, purposes, data);
}

void
SMT_NumberSymbol::fillOutSMT_Info(SMT_Info& info)
{
  //
  //	If we are the construction operator for some SMT type, fill out that information.
  //
  switch (numberSystem)
    {
    case CODE('i', 'n'):
      {
	info.setType(getRangeSort(), SMT_Info::INTEGER);
	break;
      }
    case CODE('r', 'e'):
      {
	info.setType(getRangeSort(), SMT_Info::REAL);
	break;
      }
    default:
      break;
    }
}

Term*
SMT_NumberSymbol::termify(DagNode* dagNode)
{
  return new SMT_NumberTerm(this, safeCast(SMT_NumberDagNode*, dagNode)->getValue());
}
