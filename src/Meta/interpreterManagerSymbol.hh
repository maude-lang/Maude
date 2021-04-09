/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to handle meta-interpreter objects.
//
#ifndef _interpreterManagerSymbol_hh_
#define _interpreterManagerSymbol_hh_
#include <sys/types.h>
#include <map>
#include "externalObjectManagerSymbol.hh"
#include "pseudoThread.hh"
#include "freeDagNode.hh"

class InterpreterManagerSymbol :
  public ExternalObjectManagerSymbol,
  public PseudoThread
{
  NO_COPYING(InterpreterManagerSymbol);

public:
  InterpreterManagerSymbol(int id);
  ~InterpreterManagerSymbol();

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  bool attachTerm(const char* purpose, Term* term);
  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  void getTermAttachments(Vector<const char*>& purposes, Vector<Term*>& terms);

  void postInterSymbolPass();
  void reset();

  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  struct RemoteInterpreter
  {
    pid_t processId;
    int ioSocket;
    int errSocket;
    //
    //	To handle nonblocking send.
    //
    char* charArray;  // full message
    const char* unsent;  // point to first unsent character
    Rope::size_type nrUnsent;  // number of unsent characters
    //
    //	To handle async replies from remote interpreter.
    //
    int nrPendingReplies;
    Rope incomingMessage;
    Rope incomingErrorLine;
    ObjectSystemRewritingContext* savedContext;
    MixfixModule* module;
    //
    //	This is the object that requested the remote interpreter's creation.
    //	We need it to send an intepreterExit() message should the remote
    //	interpreter process exit unexpectedly.
    //
    DagRoot owner;
  };

  typedef map<int, RemoteInterpreter> RemoteInterpreterMap;

  //
  //	Overridden methods from PseudoThread.
  //
  void doRead(int fd);
  void doWrite(int fd);
  void doChildExit(pid_t childPid);

  static RewritingContext* term2RewritingContext(Term* term, RewritingContext& context);

  bool okToBind();

  RewriteSequenceSearch* makeRewriteSequenceSearch(ImportModule* m,
						   FreeDagNode* message,
						   RewritingContext& context) const;
  MatchSearchState* makeMatchSearchState(ImportModule* m,
					 FreeDagNode* message,
					 RewritingContext& context) const;
  MatchSearchState* makeMatchSearchState2(ImportModule* m,
					  FreeDagNode* subject,
					  RewritingContext& context) const;

  RewriteSearchState* makeRewriteSearchState(ImportModule* m,
					     FreeDagNode* message,
					     RewritingContext& context,
					     bool atTop) const;
  NarrowingSearchState2* makeNarrowingSearchState2(ImportModule* m,
						   FreeDagNode* message,
						   RewritingContext& context,
						   int variantFlags) const;
  NarrowingSequenceSearch3* makeNarrowingSequenceSearch3(ImportModule* m,
							 FreeDagNode* message,
							 RewritingContext& context,
							 int variantFlags) const;
  StrategicSearch* makeStrategicSearch(MetaModule* m,
				       FreeDagNode* message,
				       RewritingContext& context,
				       bool depthFirst) const;
  
  DagNode* makeNarrowingSearchPath(ImportModule* m, NarrowingSequenceSearch3* state) const;

  bool quit(FreeDagNode* message, ObjectSystemRewritingContext& context);

  Interpreter* getInterpreter(DagNode* interpreterArg);

  bool deleteInterpreter(DagNode* interpreterArg);
  bool createInterpreter(FreeDagNode* originalMessage, ObjectSystemRewritingContext& context);
  bool createRemoteInterpreter(FreeDagNode* originalMessage,
			       ObjectSystemRewritingContext& context,
			       int id);
  RemoteInterpreter* getRemoteInterpreter(DagNode* interpreterArg);
  bool remoteHandleMessage(FreeDagNode* message,
			   ObjectSystemRewritingContext& context,
			   RemoteInterpreter* r);
  void remoteHandleReply(RemoteInterpreter* r, const Rope& reply);

  void nonblockingSendMessage(RemoteInterpreter& ri, const Rope& message);
  Rope receiveMessage(int socketId);
  void sendMessage(int socketId, const Rope& message);
  bool outputWholeErrorLines(RemoteInterpreter* ri);
  
  void becomeRemoteInterpreter(int socketId,
			       MixfixModule* m,
			       ObjectSystemRewritingContext& context);
  DagNode* handleMessage(FreeDagNode* message,
			 ObjectSystemRewritingContext& context,
			 Interpreter* interpreter);
  MetaModule* getMetaModule(FreeDagNode* message,
			    int metaModuleArgIndex,
			    Interpreter* interpreter,
			    DagNode*& errorMessage);
  DagNode* getMetaModule(FreeDagNode* message,
			 Interpreter* interpreter,
			 MetaModule*& mm);
  DagNode* insertModule(FreeDagNode* message,
			ObjectSystemRewritingContext& context,
			Interpreter* interpreter);
  DagNode* showModule(FreeDagNode* message,
		      ObjectSystemRewritingContext& context,
		      Interpreter* interpreter);
  DagNode* insertView(FreeDagNode* message,
		      ObjectSystemRewritingContext& context,
		      Interpreter* interpreter);
  DagNode* showView(FreeDagNode* message,
		    ObjectSystemRewritingContext& context,
		    Interpreter* interpreter);
  DagNode* getLesserSorts(FreeDagNode* message,
			  ObjectSystemRewritingContext& context,
			  Interpreter* interpreter);
  DagNode* getMaximalSorts(FreeDagNode* message,
			   ObjectSystemRewritingContext& context,
			   Interpreter* interpreter);
  DagNode* getMinimalSorts(FreeDagNode* message,
			   ObjectSystemRewritingContext& context,
			   Interpreter* interpreter);
  DagNode* compareTypes(FreeDagNode* message,
			ObjectSystemRewritingContext& context,
			Interpreter* interpreter);
  DagNode* getKind(FreeDagNode* message,
		   ObjectSystemRewritingContext& context,
		   Interpreter* interpreter);
  DagNode* getKinds(FreeDagNode* message,
		    ObjectSystemRewritingContext& context,
		    Interpreter* interpreter);
  DagNode* getGlbTypes(FreeDagNode* message,
		       ObjectSystemRewritingContext& context,
		       Interpreter* interpreter);
  DagNode* getMaximalAritySet(FreeDagNode* message,
			      ObjectSystemRewritingContext& context,
			      Interpreter* interpreter);
  DagNode* normalizeTerm(FreeDagNode* message,
			 ObjectSystemRewritingContext& context,
			 Interpreter* interpreter);
  DagNode* reduceTerm(FreeDagNode* message,
		      ObjectSystemRewritingContext& context,
		      Interpreter* interpreter);
  DagNode* rewriteTerm(FreeDagNode* message,
		       ObjectSystemRewritingContext& context,
		       Interpreter* interpreter);
  DagNode* frewriteTerm(FreeDagNode* message,
			ObjectSystemRewritingContext& context,
			Interpreter* interpreter);
  DagNode* erewriteTerm(FreeDagNode* message,
			ObjectSystemRewritingContext& context,
			Interpreter* interpreter);
  DagNode* srewriteTerm(FreeDagNode* message,
			ObjectSystemRewritingContext& context,
			Interpreter* interpreter);
  DagNode* getSearchResult(FreeDagNode* message,
			   ObjectSystemRewritingContext& context,
			   Interpreter* interpreter);
  DagNode* getUnifier(FreeDagNode* message,
		      ObjectSystemRewritingContext& context,
		      bool disjoint,
		      bool irredundant,
		      Interpreter* interpreter);
  DagNode* getVariant(FreeDagNode* message,
		      ObjectSystemRewritingContext& context,
		      Interpreter* interpreter);
  DagNode* getVariantUnifier(FreeDagNode* message,
			     ObjectSystemRewritingContext& context,
			     bool disjoint,
			     Interpreter* interpreter);
  DagNode* getVariantMatcher(FreeDagNode* message,
			     ObjectSystemRewritingContext& context,
			     Interpreter* interpreter);
  DagNode* printTerm(FreeDagNode* message,
		     ObjectSystemRewritingContext& context,
		     Interpreter* interpreter);
  DagNode* parseTerm(FreeDagNode* message,
		     ObjectSystemRewritingContext& context,
		     Interpreter* interpreter);
  DagNode* getMatch(FreeDagNode* message,
		    ObjectSystemRewritingContext& context,
		    Interpreter* interpreter);
  DagNode* getXmatch(FreeDagNode* message,
		     ObjectSystemRewritingContext& context,
		     Interpreter* interpreter);
  DagNode* applyRule(FreeDagNode* message,
		     ObjectSystemRewritingContext& context,
		     bool atTop,
		     Interpreter* interpreter);
  DagNode* getOneStepNarrowing(FreeDagNode* message,
			       ObjectSystemRewritingContext& context,
			       Interpreter* interpreter);
  DagNode* getNarrowingSearchResult(FreeDagNode* message,
				    ObjectSystemRewritingContext& context,
				    bool returnPath,
				    Interpreter* interpreter);
  DagNode* upRewriteCount(const RewritingContext* context);
  void errorReply(const Rope& errorMessage,
		  FreeDagNode* originalMessage,
		  ObjectSystemRewritingContext& context);
  DagNode* makeErrorReply(const Rope& errorMessage,
			  FreeDagNode* originalMessage);

  MetaLevel* metaLevel;
  MetaLevelOpSymbol* shareWith;  // if this is nonzero then it points to the
  				 // true owner of the MetaLevel object

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "interpreterSignature.cc"
#undef MACRO

  Vector<Interpreter*> interpreters;
  RemoteInterpreterMap remoteInterpreters;
};

inline RewritingContext*
InterpreterManagerSymbol::term2RewritingContext(Term* term, RewritingContext& context)
{
  term = term->normalize(false);
  DagNode* d = term->term2DagEagerLazyAware();
  term->deepSelfDestruct();
  return context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
}

inline void
InterpreterManagerSymbol::errorReply(const Rope& errorMessage,
				     FreeDagNode* originalMessage,
				     ObjectSystemRewritingContext& context)
{
  context.bufferMessage(originalMessage->getArgument(1),
			makeErrorReply(errorMessage, originalMessage));
}

#endif
