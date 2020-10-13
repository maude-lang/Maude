/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2019 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ConfigSymbol.
//
//#include <list>

//	utility stuff
#include "macros.hh"
#include "vector.hh"
#include "indent.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "ACU_RedBlack.hh"
#include "ACU_Theory.hh"
#include "objectSystem.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "subproblem.hh"
#include "extensionInfo.hh"

//      core class definitions
//#include "rewritingContext.hh"
#include "lhsAutomaton.hh"
//#include "rhsAutomaton.hh"
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "rule.hh"

//      ACU theory class definitions
#include "ACU_Symbol.hh"
#include "ACU_DagNode.hh"
#include "ACU_Term.hh"
#include "ACU_ExtensionInfo.hh"

//	object system class definitions
#include "objectSystemRewritingContext.hh"
#include "configSymbol.hh"

//	our stuff
#include "objectMap.cc"
#include "remainder.cc"

ConfigSymbol::ConfigSymbol(int id,
			   const Vector<int>& strategy,
			   bool memoFlag,
			   Term* identity)
  : ACU_Symbol(id, strategy, memoFlag, identity, false)
{
  //cerr << "ConfigSymbol::ConfigSymbol()\n";
}

void
ConfigSymbol::addObjects(NatSet& objSymbols)
{
  objectSymbols.insert(objSymbols);
}

void
ConfigSymbol::addMessages(NatSet& msgSymbols)
{
  messageSymbols.insert(msgSymbols);
}

bool
ConfigSymbol::checkArgs(Term* pattern, Term*& object, Term*& message)
{
  //
  //	Make sure pattern actually has our symbol on top and didn't
  //	just get indexed under our symbol by collapse or being a variable.
  //
  if (pattern->symbol() != this)
    return false;
  //
  //	We're checking for an object-message pattern.
  //	We can't have fewer than two arguments, though they could be identical.
  //
  object = 0;
  message = 0;
  Term* name = 0;
  for (ArgumentIterator a(*pattern); a.valid(); a.next())
    {
      //
      //	Get argument; get its symbol and check whether symbol was declared
      //	to be an object or message constructor.
      //
      Term* arg = a.argument();
      int si = arg->symbol()->getIndexWithinModule();
      if (objectSymbols.contains(si))
	{
	  //
	  //	If we've already found an object, or the argument is unstable
	  //	we don't have an object-message pattern.
	  //
	  if (object != 0 || !(arg->stable()))
	    return false;
	  object = arg;
	}
      else if (messageSymbols.contains(si))
	{
	  //
	  //	If we've already found a message or the argument is unstable
	  //	we don't ahve an object-message pattern.
	  //
	  if (message != 0 || !(arg->stable()))
	    return false;
	  message = arg;
	}
      else
	{
	  //
	  //	Something that isn't an object or message constructor; hence
	  //	we don't have an object message pattern.
	  //
	  return false;
	}
      //
      //	Argument is an object or message constructor; check that it
      //	has a valid first argument which must be the name expression.
      //
      ArgumentIterator n(*arg);
      if (!(n.valid()))
	return false;
      if (name == 0)
	name = n.argument();
      else
	{
	  //
	  //	Object and message constructors have different name expressions;
	  //	hence we don't have an object-message pattern.
	  //
	  if (!(name->equal(n.argument())))
	    return false;
	}
    }
  //
  //	If we processed two arguments without returning false, we must
  //	have filled out a compatible object-message pair, though static
  //	analyzer can't know this.
  //
  Assert(object != 0 && message != 0, "object-message check error");
  return true;
}

void
ConfigSymbol::compileRules()
{
  //
  //	Call the member function in our base class to do the work.
  //
  ACU_Symbol::compileRules();
  //
  //	We're actually interested in identifying object-message rules that
  //	will get special object-message rewriting operational semantics.
  //
  const Vector<Rule*>& rules = getRules();
  int nrRules = rules.length();
  for (int i = 0; i < nrRules; i++)
    {
      Rule* rl = rules[i];
      if (!(rl->isNonexec()))
	{
	  Term* object;
	  Term* message;
	  if (checkArgs(rl->getLhs(), object, message))
	    {
	      ruleMap[message->symbol()].rules.append(rl);
	      DebugAdvisory("***obj-msg rule*** " << rl);
	    }
	  else
	    leftOver.rules.append(rl);
	}
    }
  //
  //	Just in case resetRules() is not called before first rewriting happens.
  //
  resetRules();
}

void
ConfigSymbol::resetRules()
{
  Symbol::resetRules();
  const RuleMap::iterator e = ruleMap.end();
  for (RuleMap::iterator i = ruleMap.begin(); i != e; ++i)
    {
      RuleSet& rs = (*i).second;
      rs.next = rs.rules.begin();
    }
  leftOver.next = leftOver.rules.begin();
}

DagNode*
ConfigSymbol::ruleRewrite(DagNode* subject, RewritingContext& context)
{
  DebugAdvisory("ConfigSymbol::ruleRewrite() " << subject);
  ObjectSystemRewritingContext* rc = safeCastNonNull<ObjectSystemRewritingContext*>(&context);
  ObjectSystemRewritingContext::Mode mode = rc->getObjectMode();
  if (mode == ObjectSystemRewritingContext::STANDARD)
    return ACU_Symbol::ruleRewrite(subject, context);
  bool external = (mode == ObjectSystemRewritingContext::EXTERNAL);

  ACU_DagNode* s = safeCastNonNull<ACU_DagNode*>(subject);
  int nrArgs = s->nrArgs();
  ObjectMap objectMap;
  Remainder remainder;
  for (int i = 0; i < nrArgs; i++)
    {
      DagNode* d = s->getArgument(i);
      int m = s->getMultiplicity(i);
      int symbolNr = d->symbol()->getIndexWithinModule();
      if (objectSymbols.contains(symbolNr))
	{
	  DagArgumentIterator j(d);
	  Assert(j.valid(), "no args for object symbol");
	  DagNode* objectName = j.argument();
	  MessageQueue& mq = objectMap[objectName];
	  if (mq.object == 0)
	    {
	      mq.object = d;
	      if (m == 1)
		continue;
	      IssueWarning("saw duplicate object: " <<
			   BEGIN_QUOTE << d << END_QUOTE);
	      --m;
	    }
	  else
	    {
	      IssueWarning("saw two objects with the same name: " <<
			   BEGIN_QUOTE << d << END_QUOTE << " and " <<
			   BEGIN_QUOTE << mq.object << END_QUOTE);
	    }
	}
      else if (messageSymbols.contains(symbolNr))
	{
	  DagArgumentIterator j(d);
	  Assert(j.valid(), "no args for message symbol");
	  MessageQueue& mq = objectMap[j.argument()];
	  for (int i = 0; i < m; ++i)
	    mq.messages.push_back(d);
	  continue;
	}
      //
      //	Argument was not a valid object or message so it goes
      //	into the remainder.
      //
      remainder.dagNodes.append(d);
      remainder.multiplicities.append(m);
    }

  //objectMap.dump(cerr);
  //remainder.dump(cerr);


  if (objectMap.empty())
    {
      //
      //	No objects or messages in the configuration so
      //	do a plain ACU rewrite.
      //
      return ACU_Symbol::ruleRewrite(subject, context);
    }

  Vector<DagNode*> dagNodes(2);
  Vector<int> multiplicities(2);
  bool delivered = false;
  const ObjectMap::iterator e = objectMap.end();
  for (ObjectMap::iterator i = objectMap.begin(); i != e; ++i)
    {
      list<DagNode*>& messages = i->second.messages;
      //
      //	If we're doing erewriting, check for external messages
      //	aimed at our object.
      //
      if (external && rc->getExternalMessages(i->first, messages))
	delivered = true;  // make sure we do a rewrite

      FOR_EACH_CONST(j, list<DagNode*>, messages)
	{
	  DagNode* object = i->second.object;
	  if (object != 0)
	    {
	      dagNodes[0] = object;
	      dagNodes[1] = *j;
	      multiplicities[0] = 1;
	      multiplicities[1] = 1;
	      DagNode* r = makeDagNode(dagNodes, multiplicities);
	      RewritingContext* t = context.makeSubcontext(r);
	      t->reduce();  // need normal form
	      r = t->root();
	      if (r->symbol() == this)
		{
		  r = objMsgRewrite((*j)->symbol(), r, *t);
		  context.addInCount(*t);
		  delete t;
		  if (r != 0)
		    {
		      delivered = true;
		      //cerr << "delivered " << *j << endl;
		      t = context.makeSubcontext(r);
		      if (RewritingContext::getTraceStatus())
			t->tracePostRuleRewrite(r);
		      t->reduce();
		      i->second.object =
			retrieveObject(t->root(), i->first, remainder);
		      context.addInCount(*t);
		      delete t;
		      continue;  // next message
		    }
		}
	      else
		{
		  context.addInCount(*t);
		  delete t;
		}
	    }
	  else
	    {
	      DebugAdvisory("unresolved message " << *j <<  "  external = " << external);
	      if (external && rc->offerMessageExternally(i->first, *j))
		{
		  delivered = true;  // make sure we do a rewrite
		  continue;  // next message
		}
	    }
	  //
	  //	Message undelivered; put it into remainder.
	  //
	  //cerr << "failed to deliver " << *j << endl;
	  remainder.dagNodes.append(*j);
	  remainder.multiplicities.append(1);
	}
      //
      //	All messages tried; if object still remains, put it into remainder.
      //
      DagNode* object = i->second.object;
      if (object != 0)
	{
	  //cerr << "object still remains " << object << endl;
	  remainder.dagNodes.append(object);
	  remainder.multiplicities.append(1);
	}
    }
  if (!delivered)
    {
      if (leftOver.rules.empty())
	return 0;
      ACU_ExtensionInfo extensionInfo(safeCastNonNull<ACU_DagNode*>(subject));
      return leftOverRewrite(subject, context, &extensionInfo);
    }
  //
  //	Now deal with remainder.
  //
  //	Because we will be returning a new state, if we are tracing,
  //	caller will do the post part of the trace so we need
  //	to do the pre part of a trace, by doing non-object message
  //	rewrite on the remainder if possible or faking a built-in
  //	rewrite otherwise.
  //
  if (remainder.multiplicities.length() == 1 &&
      remainder.multiplicities[0] == 1)
    {
      //
      //	No left over rewrite, so if we're tracing we
      //	need to warn the trace system to ignore a fake rewrite.
      //
      if (RewritingContext::getTraceStatus())
	context.tracePreRuleRewrite(0, 0);
      return remainder.dagNodes[0];
    }

  DagNode* r = makeDagNode(remainder.dagNodes, remainder.multiplicities);
  //cerr << "remainder = " << r << endl;
  RewritingContext* t = context.makeSubcontext(r);
  t->reduce();
  r = t->root();
  if (r->symbol() == this && !(leftOver.rules.empty()))
    {
      ACU_ExtensionInfo extensionInfo(safeCastNonNull<ACU_DagNode*>(r));
      DagNode* d = leftOverRewrite(r, context, &extensionInfo);
      if (d == 0)
	{
	  //
	  //	No left over rewrite, so if we're tracing we
	  //	need to warn the trace system to ignore a fake rewrite.
	  //
	  if (RewritingContext::getTraceStatus())
	    context.tracePreRuleRewrite(0, 0);
	}
      else
	r = d;
    }
  else
    {
      //
      //	No left over rewrite, so if we're tracing we
      //	need to warn the trace system to ignore a fake rewrite.
      //
      if (RewritingContext::getTraceStatus())
	context.tracePreRuleRewrite(0, 0);
    }
  context.addInCount(*t);
  delete t;
  return r;
}

DagNode*
ConfigSymbol::objMsgRewrite(Symbol* messageSymbol,
			    DagNode* subject,
			    RewritingContext& context)
{
  RuleSet& rs = ruleMap[messageSymbol];
  const Vector<Rule*>::const_iterator e = rs.rules.end();
  for (int tries = rs.rules.length(); tries > 0; --tries)
    {
      Rule* rl = *(rs.next);
      if (++(rs.next) == e)
	rs.next = rs.rules.begin();

      int nrVariables = rl->getNrProtectedVariables();
      context.clear(nrVariables);
      Subproblem* sp;
      if (rl->getNonExtLhsAutomaton()->match(subject, context, sp))
	{
	  if ((sp == 0 || sp->solve(true, context)) &&
	      (!(rl->hasCondition()) || rl->checkCondition(subject, context, sp)))
	    {
	      if (RewritingContext::getTraceStatus())
		{
		  context.tracePreRuleRewrite(subject, rl);
		  if (context.traceAbort())
		    {
		      delete sp;
		      context.finished();
		      return 0;
		    }
		}
	      DagNode* r =  rl->getRhsBuilder().construct(context);
	      context.incrementRlCount();
	      delete sp;
	      context.finished();
	      return r;
	    }
	  delete sp;
	}
      context.finished();
    }
  return 0;
}

DagNode*
ConfigSymbol::leftOverRewrite(DagNode* subject,
			      RewritingContext& context,
			      ExtensionInfo* extensionInfo)
{
  Assert(subject->getSortIndex() != Sort::SORT_UNKNOWN, "sort unknown");

  const Vector<Rule*>::const_iterator e = leftOver.rules.end();
  for (int tries = leftOver.rules.length(); tries > 0; --tries)
    {
      Rule* rl = *(leftOver.next);
      if (++(leftOver.next) == e)
	leftOver.next = leftOver.rules.begin();

      int nrVariables = rl->getNrProtectedVariables();
      context.clear(nrVariables);
      Subproblem* sp;
      if (rl->getLhsAutomaton()->match(subject, context, sp, extensionInfo))
	{
	  if ((sp == 0 || sp->solve(true, context)) &&
	      (!(rl->hasCondition()) || rl->checkCondition(subject, context, sp)))
	    {
	      if (RewritingContext::getTraceStatus())
		{
		  context.tracePreRuleRewrite(subject, rl);
		  if (context.traceAbort())
		    {
		      delete sp;
		      context.finished();
		      return subject;
		    }
		}
	      DagNode* r = extensionInfo->matchedWhole() ?
		rl->getRhsBuilder().construct(context) :
		subject->partialConstruct(rl->getRhsBuilder().construct(context),
					  extensionInfo);
	      context.incrementRlCount();
	      delete sp;
	      context.finished();
	      return r;
	    }
	  delete sp;
	}
      context.finished();
    }
  return 0;
}

DagNode*
ConfigSymbol::retrieveObject(DagNode* rhs, DagNode* name, Remainder& remainder)
{
  Symbol* s = rhs->symbol();
  if (s == this)
    {
      ACU_DagNode* r = safeCastNonNull<ACU_DagNode*>(rhs);
      int nrArgs = r->nrArgs();
      for (int i = 0; i < nrArgs; ++i)
	{
	  DagNode* d = r->getArgument(i);
	  int m = r->getMultiplicity(i);
	  if (objectSymbols.contains(d->symbol()->getIndexWithinModule()))
	    {
	      if (m == 1)
		{
		  DagArgumentIterator j(d);
		  if (j.valid() && j.argument()->equal(name))
		    {
		      for (++i; i < nrArgs; ++i)
			{
			  remainder.dagNodes.append(r->getArgument(i));
			  remainder.multiplicities.append(r->getMultiplicity(i));
			}
		      return d;
		    }
		}
	      else
		{
		  IssueWarning("saw duplicate object: " <<
			       BEGIN_QUOTE << d << END_QUOTE);
		}
	    }
	  remainder.dagNodes.append(d);
	  remainder.multiplicities.append(m);
	}
      return 0;
    }
  else
    {
      //
      //	Check to see if rhs is the object we are looking for.
      //
      if (objectSymbols.contains(s->getIndexWithinModule()))
	{
	  DagArgumentIterator j(rhs);
	  if (j.valid() && j.argument()->equal(name))
	    return rhs;
	}
    }
  remainder.dagNodes.append(rhs);
  remainder.multiplicities.append(1);
  return 0;
}
