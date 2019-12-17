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
//      Implementation for class NA_Symbol.
//
 
//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      NA theory class definitions
#include "NA_Symbol.hh"

NA_Symbol::NA_Symbol(int id)
  : Symbol(id, 0)
{
}

Term*
NA_Symbol::makeTerm(const Vector<Term*>& /* args */)
{
  CantHappen("makeTerm() not useable on non-algebraic symbol " << this);
  return 0;
}

DagNode* 
NA_Symbol::makeDagNode(const Vector<DagNode*>& /* args */)
{
  CantHappen("makeDagNode() not useable on non-algebraic symbol " << this);
  return 0;
}

bool
NA_Symbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  return applyReplace(subject, context, 0);
}

void
NA_Symbol::computeBaseSort(DagNode* subject)
{
  subject->setSortIndex(traverse(0, 0));
}

void
NA_Symbol::normalizeAndComputeTrueSort(DagNode* subject, RewritingContext& context)
{
  fastComputeTrueSort(subject, context);
}

bool
NA_Symbol::isStable() const
{
  return true;
}

//
//	Hash cons code.
//

DagNode*
NA_Symbol::makeCanonical(DagNode* original, HashConsSet* /* hcs */)
{
  //
  //	No arguments that could be non-canonical so we can make the original
  //	instance into the canonical instance.
  //
  return original;
}

DagNode*
NA_Symbol::makeCanonicalCopy(DagNode* original, HashConsSet* /* hcs */)
{
  //
  //	Copy forced.
  //
  return original->makeClone();
}
