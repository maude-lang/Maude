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
#include "pseudoThread.hh"
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
  DebugAdvisory("offerMessageExternally(): saw " << message);
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

void
ObjectSystemRewritingContext::externalRewrite()
{
  // HACK for experiments
  const char* extBiasString = getenv("EXT_BIAS");
  int extBias = extBiasString ? atoi(extBiasString) : 1;
  //
  //	We assume caller has already set up limit and gas for fair rewriting.
  //	We interleave fair rewriting with calls to PseudoThread::eventLoop() to
  //	handle external events.
  //
  for (;;)
    {
      //
      //	Fair rewrite until we can make no further progress.
      //	We now interleave nonblocking calls to handle external events.
      //
      for (;;)
	{
	  DebugAdvisory("calling fairTraversal()");
	  if (fairTraversal())
	    return;  // hit limit or abort
	  if (!getProgress())
	    break;  // no progress made on last traversal
	  //
	  //	Check for external events. We made progress with local rewrites so
	  //	we can't block on pending external events.
	  //
	  //	We don't bother checking whether we had an interrupt or handled
	  //	external events - we're going to try more rewriting anyway
	  //	and we prefer to handle interrupts mid-rewrite.
	  //
	  for (int i = 0; i < extBias; ++i) // HACK for experiments
	    (void) PseudoThread::eventLoop(false);
	}
      //
      //	If we get here, we cannot do any more local rewrites in our
      //	current state, so if there are external events pending we
      //	block on them.
      //
      int r = PseudoThread::eventLoop(true);
      if (r & PseudoThread::NOTHING_PENDING)
	break;  // nothing to wait for
      if (r & PseudoThread::INTERRUPTED)
	{
	  //cerr << "eventLoop() interrupted" << endl;
	  //
	  //	Blocking call returned because of interrupt.
	  //
	  if (!handleInterrupt())
	    break;  // assume we abort
	  //cerr << "appears to have been handled" << endl;
	}
    }
}
