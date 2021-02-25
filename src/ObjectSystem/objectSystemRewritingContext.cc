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
#include <signal.h>

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
  for (auto& i : externalObjects)
    i.second->cleanUp(i.first);
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
  for (auto& i : externalObjects)
    i.first->mark();
  for (auto& i : incomingMessages)
    {
      for (DagNode* d : i.second)
	d->mark();
    }
  RewritingContext::markReachableNodes();
}

bool
ObjectSystemRewritingContext::interleave()
{
  //
  //	Interleave fair rewriting with handling of external events.
  //	We return false if rewriting needs to end because it hit limit
  //	or user aborted and true if we just ran out of rewrites to do.
  //
  for (;;)
    {
      DebugAdvisory("calling fairTraversal()");
      if (fairTraversal())
	return false;  // hit limit or abort
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
      (void) PseudoThread::eventLoop(false);
    }
  return true;
}

void
ObjectSystemRewritingContext::externalRewrite()
{
  //
  //	We assume caller has already set up limit and gas for fair rewriting.
  //	We interleave fair rewriting with calls to PseudoThread::eventLoop() to
  //	handle external events.
  //
  //
  //	Interleave fair rewriting and external events.
  //
  if (!interleave())
    return;

  for (;;)
    {
      //
      //	If we get here, we cannot do any more local rewrites in our
      //	current state, so if there are external events pending we
      //	block on them.
      //
      //	But first we need to deal with any signals that have
      //	been delivered and then block them so we don't miss them by their
      //	being delivered before we make our blocking system call.
      //
      sigset_t normalSet;
      if (!blockAndHandleInterrupts(&normalSet))
	{
	  //
	  //	Need to quit externalRewrite() because of an interrupt, but
	  //	first we need to restore normal signals.
	  //
	  sigprocmask(SIG_SETMASK, &normalSet, 0);
	  break;
	}
      //
      //	Now we can safely block on external events.
      //
      int r = PseudoThread::eventLoop(true, &normalSet);
      //
      //	Restore normal signals.
      //
      sigprocmask(SIG_SETMASK, &normalSet, 0);
      //
      //	eventLoop() will have restored signal mask to normalSet.
      //
      if (r & PseudoThread::NOTHING_PENDING)
	{
	  //
	  //	There were no external events pending and therefore no
	  //	callbacks were made. Since there are no local rewrites
	  //	available, we're done.
	  //
	  break;
	}
      if (r & PseudoThread::INTERRUPTED)
	{
	  //
	  //	Blocking call returned because of interrupt(s) which may
	  //	include one that our subclass knows how to handle.
	  //	PseudoThread::eventLoop() can't know which signals were delivered
	  //	other than any it is managing, so for safety, it exits.
	  //
	  if (!handleInterrupt())
	    {
	      //
	      //	We handled an interrupt that required us to abort.
	      //
	      break;
	    }
	  //
	  //	Didn't need to abort - may not have handled an interrupt.
	  //
	}
      if (r & PseudoThread::EVENT_HANDLED)
	{
	  //
	  //	We handled an event that may have changed our context
	  //	by enabling external rewrites; need to try rewriting again.
	  //
	  if (!interleave())
	    break;
	}
    }
}
