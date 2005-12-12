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
//      Implementation for class ObjectSystemRewritingContext.
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
#include "term.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "externalObjectManagerSymbol.hh"

ObjectSystemRewritingContext::~ObjectSystemRewritingContext()
{
  //DebugAdvisory("~ObjectSystemRewritingContext() called; " << externalObjects.size() << " external objects");
  if (!(externalObjects.empty()))
    {
      FOR_EACH_CONST(i, ObjectMap, externalObjects)
	i->second->cleanUp(i->first);
    }
}

void
ObjectSystemRewritingContext::bufferMessage(DagNode* target, DagNode* message)
{
  incomingMessages[target].push_back(message);
}

void
ObjectSystemRewritingContext::addExternalObject(DagNode* name,
						ExternalObjectManagerSymbol* manager)
{
  externalObjects.insert(ObjectMap::value_type(name, manager));
}

void
ObjectSystemRewritingContext::deleteExternalObject(DagNode* name)
{
  externalObjects.erase(name);
}

bool
ObjectSystemRewritingContext::getExternalMessages(DagNode* target, list<DagNode*>& messages)
{
  MessageMap::iterator i = incomingMessages.find(target);
  if (i != incomingMessages.end())
    {
      messages.splice(messages.end(), i->second);
      incomingMessages.erase(i);
      return true;
    }
  return false;
}

bool
ObjectSystemRewritingContext::offerMessageExternally(DagNode* target, DagNode* message)
{
  //cerr << "offerMessageExternally(): saw " << message << endl;
  ObjectMap::iterator i = externalObjects.find(target);
  if (i != externalObjects.end())
    return i->second->handleMessage(message, *this);
  else if (ExternalObjectManagerSymbol* m = dynamic_cast<ExternalObjectManagerSymbol*>(target->symbol()))
    return m->handleManagerMessage(message, *this);
  return false;
}

void
ObjectSystemRewritingContext::markReachableNodes()
{
  {
    FOR_EACH_CONST(i, ObjectMap, externalObjects)
      i->first->mark();
  }
  { 
    FOR_EACH_CONST(i, MessageMap, incomingMessages)
      {
	FOR_EACH_CONST(j, list<DagNode*>, i->second)
	  (*j)->mark();
      }
  }
  RewritingContext::markReachableNodes();
}
