/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2013 SRI International, Menlo Park, CA 94025, USA.

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
//	Implementation for class FreeNullarySymbol.
//

//	utility stuff
#include "macros.hh"
#include "vector.hh"

//	forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "freeTheory.hh"

//	interface class definitions
#include "symbol.hh"

//      core class definitions
#include "rewritingContext.hh"

//	free theory class definitions
#include "freeNullarySymbol.hh"
#include "freeDagNode.hh"

FreeNullarySymbol::FreeNullarySymbol(int id)
  : FreeSymbol(id, 0)
{
}

bool
FreeNullarySymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  return DISC_NET.applyReplaceFast(subject, context);
}

void
FreeNullarySymbol::reset()
{
  setNode(0);
}
