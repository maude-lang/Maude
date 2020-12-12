/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//      MetaInterpreters: module/view messages.
//

MetaModule*
InterpreterManagerSymbol::getMetaModule(FreeDagNode* message,
					int metaModuleArgIndex,
					Interpreter* interpreter,
					DagNode*& errorMessage)
{
  //
  //	Returns metaModule on success.
  //	Returns 0 on failure with errorMessage filled out.
  //
  int id;
  const char* errorString;
  if (metaLevel->downQid(message->getArgument(metaModuleArgIndex), id))
    {
      if (PreModule* pm = interpreter->getModule(id))
	{
	  if (ImportModule* m = pm->getFlatModule())
	    {
	      //
	      //	We need a MetaModule and its safe to do the case
	      //	because a metaInterpreter can only store MetaModules.
	      //
	      return safeCast(MetaModule*, m);
	    }
	  errorString = "Bad module.";
	}
      else
	errorString = "Nonexistent module.";
    }
  else
    errorString = "Bad module name.";
  errorMessage = makeErrorReply(errorString, message);
  return 0;
}

DagNode*
InterpreterManagerSymbol::insertModule(FreeDagNode* message,
				       ObjectSystemRewritingContext& context,
				       Interpreter* interpreter)
{
  DagNode* metaModule = message->getArgument(2);
  //
  //	We create the flattened module first. Only if it is
  //	successful do we create the MetaPreModule and insert it in
  //	the interpreters module database.
  //
  if (MetaModule* m = metaLevel->downSignature(metaModule, interpreter))
    {
      //
      //	We need to get object level versions of header and imports
      //	inserted into a MetaPreModule.
      //
      FreeDagNode* f = safeCast(FreeDagNode*, metaModule);
      int id;
      DagNode* metaParameterDeclList;
      metaLevel->downHeader(f->getArgument(0), id, metaParameterDeclList);

      MetaPreModule* pm = new MetaPreModule(id,
					    metaModule,
					    metaLevel,
					    m,
					    interpreter);
      
      metaLevel->downParameterDeclList2(metaParameterDeclList, pm);
      metaLevel->downImports2(f->getArgument(1), pm);
      interpreter->insertModule(id, pm); 

      Vector<DagNode*> reply(2);
      DagNode* target = message->getArgument(1);
      reply[0] = target;
      reply[1] = message->getArgument(0);
      return insertedModuleMsg->makeDagNode(reply);
    }
  return makeErrorReply("Bad module.", message);
}

DagNode*
InterpreterManagerSymbol::showModule(FreeDagNode* message,
				     ObjectSystemRewritingContext& context,
				     Interpreter* interpreter)
{
  int id;
  if (metaLevel->downQid(message->getArgument(2), id))
    {
      bool flat;
      if (metaLevel->downBool(message->getArgument(3), flat))
	{
	  if (PreModule* pm = interpreter->getModule(id))
	    {
	      Vector<DagNode*> reply(3);
	      DagNode* target = message->getArgument(1);
	      reply[0] = target;
	      reply[1] = message->getArgument(0);
	      PointerMap qidMap;
	      reply[2] = metaLevel->upModule(flat, pm, qidMap);
	      return showingModuleMsg->makeDagNode(reply);
	    }
	  return makeErrorReply("Nonexistent module.", message);
	}
      return makeErrorReply("Bad option.", message);
    }
  return makeErrorReply("Bad module name.", message);
}

DagNode*
InterpreterManagerSymbol::insertView(FreeDagNode* message,
				     ObjectSystemRewritingContext& context,
				     Interpreter* interpreter)
{
  if (View* v = metaLevel->downView(message->getArgument(2), interpreter))
    {
      // FIXME: need to sanity check v before inserting it
      interpreter->insertView(v->id(), v);

      Vector<DagNode*> reply(2);
      DagNode* target = message->getArgument(1);
      reply[0] = target;
      reply[1] = message->getArgument(0);
      return insertedViewMsg->makeDagNode(reply);
    }
  return makeErrorReply("Bad view.", message);
}

DagNode*
InterpreterManagerSymbol::showView(FreeDagNode* message,
				   ObjectSystemRewritingContext& context,
				   Interpreter* interpreter)
{
  int id;
  if (metaLevel->downQid(message->getArgument(2), id))
    {
      if (View* v = interpreter->getView(id))
	{
	  Vector<DagNode*> reply(3);
	  DagNode* target = message->getArgument(1);
	  reply[0] = target;
	  reply[1] = message->getArgument(0);
	  PointerMap qidMap;
	  reply[2] = metaLevel->upView(v, qidMap);
	  return showingViewMsg->makeDagNode(reply);
	}
      return makeErrorReply("Nonexistent view.", message);
    }
  return makeErrorReply("Bad view name.", message);
}
