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
//      Class to handle meta-interpreter objects.
//
#ifndef _interpreterManagerSymbol_hh_
#define _interpreterManagerSymbol_hh_
#include "externalObjectManagerSymbol.hh"

class InterpreterManagerSymbol : public ExternalObjectManagerSymbol
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
						   RewritingContext& context) const;
  NarrowingSequenceSearch3* makeNarrowingSequenceSearch3(ImportModule* m,
							 FreeDagNode* message,
							 RewritingContext& context,
							 bool keepHistory) const;
  StrategicSearch* makeStrategicSearch(MetaModule* m,
				       FreeDagNode* message,
				       RewritingContext& context,
				       bool depthFirst) const;
  
  DagNode* makeNarrowingSearchPath(ImportModule* m, NarrowingSequenceSearch3* state) const;

  bool insertModule(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool showModule(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool insertView(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool showView(FreeDagNode* message, ObjectSystemRewritingContext& context);

  bool printTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool parseTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);

  bool reduceTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool rewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool frewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool erewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool srewriteTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);

  bool applyRule(FreeDagNode* message, ObjectSystemRewritingContext& context, bool atTop);
  bool getSearchResult(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getMatch(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getXmatch(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getUnifier(FreeDagNode* message, ObjectSystemRewritingContext& context, bool disjoint);
  bool getVariant(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getVariantUnifier(FreeDagNode* message, ObjectSystemRewritingContext& context, bool disjoint);
  bool getLesserSorts(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getMaximalSorts(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getMinimalSorts(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool compareTypes(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getKind(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getKinds(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getGlbTypes(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getMaximalAritySet(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool normalizeTerm(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getOneStepNarrowing(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool getNarrowingSearchResult(FreeDagNode* message, ObjectSystemRewritingContext& context, bool returnPath);

  bool quit(FreeDagNode* message, ObjectSystemRewritingContext& context);

  bool getInterpreter(DagNode* interpreterArg, Interpreter*& interpreter);
  bool getInterpreterAndModule(FreeDagNode* message,
			       Interpreter*& interpreter,
			       ImportModule*& module);
  bool deleteInterpreter(DagNode* interpreterArg);
  bool createInterpreter(FreeDagNode* originalMessage, ObjectSystemRewritingContext& context);

  DagNode* upRewriteCount(const RewritingContext* context);

  MetaLevel* metaLevel;
  MetaLevelOpSymbol* shareWith;  // if this is nonzero then it points to the true owner of the MetaLevel object

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "interpreterSignature.cc"
#undef MACRO

  Vector<Interpreter*> interpreters;
};

inline RewritingContext*
InterpreterManagerSymbol::term2RewritingContext(Term* term, RewritingContext& context)
{
  term = term->normalize(false);
  DagNode* d = term->term2DagEagerLazyAware();
  term->deepSelfDestruct();
  return context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
}

#endif
