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
//      Implementation for class ExternalObjectManagerSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"

//      core class definitions
#include "rewritingContext.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "externalObjectManagerSymbol.hh"

ExternalObjectManagerSymbol::ExternalObjectManagerSymbol(int id)
  : FreeSymbol(id, 0)
{
}

void
ExternalObjectManagerSymbol::cleanUpManager(ObjectSystemRewritingContext& /* context */)
{
  //
  //	Most derived classes won't need this functionality so we provide
  //	a default function.
  //
  CantHappen("Shouldn't be called for this ExternalObjectManagerSymbol " << this);
}


void
ExternalObjectManagerSymbol::trivialReply(Symbol* replySymbol, 
					  FreeDagNode* originalMessage,
					  ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(2);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  reply[1] = originalMessage->getArgument(0);
  context.bufferMessage(target, replySymbol->makeDagNode(reply));
}
