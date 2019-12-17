/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for getLesserSorts()/getMaximalSorts()/getMinimalSorts()/
//	compareTypes()/getKind()/getKinds()/getGlbTypes() messages.
//

bool
InterpreterManagerSymbol::getLesserSorts(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Sort* s;
      if (metaLevel->downType(message->getArgument(3), m, s))
	{
	  ConnectedComponent* component = s->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> lesserSorts;
	  for (int i = s->index() + 1; i < nrSorts; i++)
	    {
	      Sort* s2 = component->sort(i);
	      if (leq(s2, s))
		lesserSorts.append(s2);
	    }

	  Vector<DagNode*> reply(3);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upSortSet(lesserSorts);
	  context.bufferMessage(target, gotLesserSortsMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getMaximalSorts(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Sort* k;
      if (metaLevel->downType(message->getArgument(3), m, k) && k->index() == Sort::KIND)
	{
	  ConnectedComponent* component = k->component();
	  int nrMaximalSorts = component->nrMaximalSorts();
	  Vector<Sort*> maximalSorts(nrMaximalSorts);
	  for (int i = 1; i <= nrMaximalSorts; i++)
	    maximalSorts[i - 1] = component->sort(i);

	  Vector<DagNode*> reply(3);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upSortSet(maximalSorts);
	  context.bufferMessage(target, gotMaximalSortsMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getMinimalSorts(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Sort* k;
      if (metaLevel->downType(message->getArgument(3), m, k) && k->index() == Sort::KIND)
	{
	  ConnectedComponent* component = k->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> minimalSorts;
	  for (int i = 1; i < nrSorts; i++)
	    {
	      Sort* s = component->sort(i);
	      if (s->getSubsorts().length() == 0)
		minimalSorts.append(s);
	    }

	  Vector<DagNode*> reply(3);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upSortSet(minimalSorts);
	  context.bufferMessage(target, gotMinimalSortsMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::compareTypes(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Sort* t1;
      Sort* t2;
      if (metaLevel->downType(message->getArgument(3), m, t1) &&
	  metaLevel->downType(message->getArgument(4), m, t2))
	{
	  bool sameKind = t1->component() == t2->component();
	  Vector<DagNode*> reply(5);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upBool(sameKind);
	  reply[3] = metaLevel->upBool(sameKind && leq(t1, t2));
	  reply[4] = metaLevel->upBool(sameKind && leq(t2, t1));
	  context.bufferMessage(target, comparedTypesMsg->makeDagNode(reply));
	  return true;

	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getKind(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Sort* s;
      if (metaLevel->downType(message->getArgument(3), m, s))
	{
	  Sort* kind = s->component()->sort(Sort::ERROR_SORT);
	  Vector<DagNode*> reply(3);
	  PointerMap qidMap;
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upType(kind, qidMap);
	  context.bufferMessage(target, gotKindMsg->makeDagNode(reply));
	  return true;
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getKinds(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Vector<DagNode*> reply(3);
      DagNode* target = message->getArgument(1);
      reply[0] = target;
      reply[1] = message->getArgument(0);
      reply[2] = metaLevel->upKindSet(m->getConnectedComponents(), m->getNrUserComponents());
      context.bufferMessage(target, gotKindsMsg->makeDagNode(reply));
      return true;
    }
  return false;
}

bool
InterpreterManagerSymbol::getGlbTypes(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      Vector<Sort*> typeSet;
      if (metaLevel->downTypeSet(message->getArgument(3), m, typeSet))
	{
	  int nrTypes = typeSet.size();
	  if (nrTypes > 1)  // empty and singleton type sets are their own glb set
	    {
	      Sort* firstType = typeSet[0];
	      ConnectedComponent* component = firstType->component();
	      NatSet leqSorts(firstType->getLeqSorts());

	      for (int i = 1; i < nrTypes; ++i)
		{
		  Sort* nextType = typeSet[i];
		  if (nextType->component() != component)  // different kinds implies empty glb set
		    {
		      typeSet.clear();
		      goto done;
		    }
		  leqSorts.intersect(nextType->getLeqSorts());
		}
	      typeSet.clear();
	      component->findMaximalSorts(leqSorts, typeSet);
	    }
	done:
	  Vector<DagNode*> reply(3);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  reply[2] = metaLevel->upSortSet(typeSet);
	  context.bufferMessage(target, gotGlbTypesMsg->makeDagNode(reply));
	  return true;

	}
    }
  return false;
}

bool
InterpreterManagerSymbol::getMaximalAritySet(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  ImportModule* m;
  if (getInterpreterAndModule(message, interpreter, m))
    {
      int id;
      Vector<Sort*> arity;
      Sort *targetSort;
      if (metaLevel->downOpName(message->getArgument(3), id) &&
	  metaLevel->downTypeList(message->getArgument(4), m, arity) &&
	  metaLevel->downSimpleSort(message->getArgument(5), m, targetSort))
	{
	  //
	  //	First we find the operator from its name and arity.
	  //
	  int nrArgs = arity.size();
	  Vector<ConnectedComponent*> domain(nrArgs);
	  for (int i = 0; i < nrArgs; i++)
	    domain[i] = arity[i]->component();
	  if (Symbol* s = m->findSymbol(id, domain, targetSort->component()))
	    {
	      //
	      //	The answer is a subset of the set of declarations
	      //	for the operator.
	      //
	      Vector<DagNode*> reply(3);
	      PointerMap qidMap;
	      DagNode* target = message->getArgument(1);
	      reply[0] = target;
	      reply[1] = message->getArgument(0);
	      reply[2] = metaLevel->upTypeListSet(s->getOpDeclarations(),
						  s->getMaximalOpDeclSet(targetSort),  // subset
						  qidMap);
	      context.bufferMessage(target, gotMaximalAritySetMsg->makeDagNode(reply));
	      return true;
	    }
	}
    }
  return false;
}

bool
InterpreterManagerSymbol::normalizeTerm(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter;
  if (getInterpreter(message->getArgument(0), interpreter))
    {
      int id;
      if (metaLevel->downQid(message->getArgument(2), id))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      if (ImportModule* m = pm->getFlatModule())
		{
		   if (Term* t = metaLevel->downTerm(message->getArgument(3), m))
		     {
		       t = t->normalize(true);
		       t->symbol()->fillInSortInfo(t);

		       Vector<DagNode*> reply(4);
		       DagNode* target = message->getArgument(1);
		       reply[0] = target;
		       reply[1] = message->getArgument(0);

		       PointerMap qidMap;
		       reply[2] = metaLevel->upTerm(t, m, qidMap);
		       reply[3] = metaLevel->upType(t->getSort(), qidMap);
		       t->deepSelfDestruct();

		       context.bufferMessage(target, normalizedTermMsg->makeDagNode(reply));
		       return true;
		     }
		}
	    }
	}
    }
  return false;
}
