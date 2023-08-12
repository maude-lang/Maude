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
//      Class to hold interpreter state.
//
#ifndef _interpreter_hh_
#define _interpreter_hh_
#include <set>
#include "environment.hh"
#include "moduleDatabase.hh"
#include "moduleCache.hh"
#include "compiler.hh"
#include "viewDatabase.hh"
#include "viewCache.hh"
//#include "syntacticView.hh"
#include "parameterDatabase.hh"
#include "SMT.hh"

class Interpreter
  : public Environment,
    public ModuleDatabase,
    public ModuleCache,
#ifdef COMPILER
    public Compiler,
#endif
    public ViewDatabase,
    public ViewCache,
    public ParameterDatabase
{
  NO_COPYING(Interpreter);

public:
  enum SearchKind
    {
      SEARCH,
      NARROW,
      XG_NARROW,
      SMT_SEARCH,
      VU_NARROW,
      FVU_NARROW
    };

  enum Flags
  {
    //
    //	Show (information) flags.
    //
    SHOW_COMMAND = 0x1,
    SHOW_STATS = 0x2,
    SHOW_TIMING = 0x4,
    SHOW_BREAKDOWN = 0x8,
    //
    //	Loop mode flags.
    //
    SHOW_LOOP_STATS = 0x10,
    SHOW_LOOP_TIMING = 0x20,
    EREWRITE_LOOP_MODE = 0x40,
    //
    //	Memoization flags.
    //
    AUTO_CLEAR_MEMO = 0x100,
    //
    //	Profiler flags.
    //
    PROFILE = 0x200,
    AUTO_CLEAR_PROFILE = 0x400,
    //
    //	Debugger flags.
    //
    BREAK = 0x800,
    //
    //	Tracer flags.
    //
    TRACE = 0x1000,
    TRACE_CONDITION = 0x2000,
    TRACE_WHOLE = 0x4000,
    TRACE_SUBSTITUTION = 0x8000,
    TRACE_SELECT = 0x10000,
    TRACE_MB = 0x20000,
    TRACE_EQ = 0x40000,
    TRACE_RL = 0x80000,
    TRACE_SD = 0x100000,
    TRACE_REWRITE = 0x200000,
    TRACE_BODY = 0x400000,
    TRACE_BUILTIN = 0x800000,
    //
    //	Print attribute flags
    //
    PRINT_ATTRIBUTE = 0x1000000,
    PRINT_ATTRIBUTE_NEWLINE = 0x2000000,
    /*
    PRINT_ATTRIBUTE_MB = 0x2000000,
    PRINT_ATTRIBUTE_EQ = 0x4000000,
    PRINT_ATTRIBUTE_RL = 0x8000000,
    */
    //
    //	Counter flags.
    //
    AUTO_CLEAR_RULES = 0x40000000,
    //
    //	Compiler flags.
    //
    COMPILE_COUNT = 0x80000000,
    //
    //	Flag groups.
    //
    EXCEPTION_FLAGS = TRACE | BREAK | PROFILE | PRINT_ATTRIBUTE,

    DEFAULT_FLAGS = SHOW_COMMAND | SHOW_STATS | SHOW_TIMING | SHOW_LOOP_TIMING |
    COMPILE_COUNT |
    TRACE_CONDITION | TRACE_SUBSTITUTION | TRACE_MB | TRACE_EQ | TRACE_RL | TRACE_SD | TRACE_REWRITE | TRACE_BODY | TRACE_BUILTIN |
    AUTO_CLEAR_PROFILE | AUTO_CLEAR_RULES | PRINT_ATTRIBUTE_NEWLINE
  };

  enum PrintFlags
  {
    //
    //	General prettyprinter flags.
    //
    PRINT_GRAPH = 0x1,		// print as a set of DAG nodes
    PRINT_CONCEAL = 0x2,	// respect concealed argument lists
    PRINT_FORMAT = 0x4,		// respect format attribute
    PRINT_MIXFIX = 0x8,		// mixfix notation
    PRINT_WITH_PARENS = 0x10,	// maximal parens
    PRINT_COLOR = 0x20,		// dag node coloring based on ctor/reduced status
    PRINT_DISAMBIG_CONST = 0x40,	// (c).s for every constant c
    //
    //	Prettyprinter flags for particular symbol types.
    //
    PRINT_WITH_ALIASES = 0x100,	// for variables
    PRINT_FLAT = 0x200,		// for assoc symbols
    PRINT_NUMBER = 0x400,	// for nats & ints
    PRINT_RAT = 0x800,		// for rats

    DEFAULT_PRINT_FLAGS = PRINT_FORMAT | PRINT_MIXFIX | PRINT_WITH_ALIASES |
    PRINT_FLAT | PRINT_NUMBER | PRINT_RAT
  };

  Interpreter();
  ~Interpreter();

  void outputBanner(const char* date, const char* time, time_t seconds);

  void beginXmlLog(const char* fileName);
  void endXmlLog();
  void beginLatexLog(const char* fileName);
  void endLatexLog();
  MaudemlBuffer* getXmlBuffer() const;

  void cleanCaches();
  void setFlag(Flags flag, bool polarity);
  bool getFlag(Flags flag) const;
  void setPrintFlag(PrintFlags flag, bool polarity);
  bool getPrintFlag(PrintFlags flag) const;
  int getPrintFlags() const;

  SyntacticPreModule* getCurrentModule() const;
  bool setCurrentModule(const Vector<Token>& moduleExpr, int start = 0);
  void setCurrentModule(SyntacticPreModule* module);
  void makeClean(int lineNumber);

  SyntacticView* getCurrentView() const;
  bool setCurrentView(const Vector<Token>& viewExpr);
  void setCurrentView(SyntacticView* view);

  void parse(const Vector<Token>& subject);
  void reduce(const Vector<Token>& subject, bool debug);
  void creduce(const Vector<Token>& subject);
  void sreduce(const Vector<Token>& subject);
  void rewrite(const Vector<Token>& subject, Int64 limit, bool debug);
  void fRewrite(const Vector<Token>& subject, Int64 limit, Int64 gas, bool debug);
  void eRewrite(const Vector<Token>& subject, Int64 limit, Int64 gas, bool debug);
  void sRewrite(const Vector<Token>& subjectAndStrategy, Int64 limit, bool debug,
		bool depthSearch = false);
  void cont(Int64 limit, bool debug);
  void check(const Vector<Token>& subject);
  //
  //	This is just a utility command for development.
  //
  void test(const Vector<Token>& subject);

  void match(const Vector<Token>& bubble, bool withExtension, Int64 limit);
  void unify(const Vector<Token>& bubble, Int64 limit, bool irredundant);
  void search(const Vector<Token>& bubble,
	      Int64 limit,
	      Int64 depth,
	      SearchKind searchKind,
	      bool debug,
	      int variantFlags = 0);
  void getVariants(const Vector<Token>& bubble, Int64 limit, bool irredundant, bool debug);
  void variantUnify(const Vector<Token>& bubble, Int64 limit, bool filtered, bool debug);
  void variantMatch(const Vector<Token>& bubble, Int64 limit, bool debug);
  void smtSearch(const Vector<Token>& subject, int limit, int depth);

  void showSearchPath(int stateNr, bool showRule);
  void showSearchPathLabels(int stateNr);
  void showSearchGraph();

  void loop(const Vector<Token>& subject);
  void contLoop(const Vector<Token>& input);

  void addSelected(const Vector<Token>& opName);
  void traceSelect(bool add);
  void breakSelect(bool add);
  void traceExclude(bool add);
  void printConceal(bool add);

  bool traceId(int id);
  bool breakId(int id);
  bool excludedModule(int id);
  bool concealedSymbol(Symbol* symbol);

  void showProfile() const;
  void showKinds() const;
  void showSummary() const;
  void showSortsAndSubsorts() const;
  void showModule(bool all = true) const;
  void showModules(bool all) const;
  void showView() const;
  void showProcessedView() const;
  void showViews(bool all) const;
  void showVars() const;
  void showOps(bool all = true) const;
  void showMbs(bool all = true) const;
  void showEqs(bool all = true) const;
  void showRls(bool all = true) const;
  void showStrats(bool all = true) const;
  void showSds(bool all = true) const;

  ImportModule* getModuleOrIssueWarning(int name, const LineNumber& lineNumber);
  ImportModule* makeModule(const ModuleExpression* expr, EnclosingObject* enclosingObject = 0);

private:
  typedef void (Interpreter::*ContinueFuncPtr)(Int64 limit, bool debug);

  static DagNode* makeDag(Term* subjectTerm);
  static void printTiming(Int64 nrRewrites, Int64 cpu, Int64 real);
  static void printBubble(ostream& s, const Vector<int>& bubble);

  Argument* handleArgument(const ViewExpression* expr,
		      EnclosingObject* enclosingObject,
		      ImportModule* requiredParameterTheory,
		      int argNr);
  void clearContinueInfo();
  DagNode* makeDag(const Vector<Token>& subject);
  void startUsingModule(VisibleModule* module);
  void printModifiers(Int64 number, Int64 number2);
  void printStats(const Timer& timer, RewritingContext& context, bool timingFlag = false);
  void printStats(RewritingContext& context,
		  int64_t cpuTime,
		  int64_t realTime,
		  bool timingFlag,
		  int64_t nrStates = NONE);
  void beginRewriting(bool debug);
  void endRewriting(Timer& timer,
		    CacheableRewritingContext* context,
		    VisibleModule* module,
		    ContinueFuncPtr cf = 0);
  void rewriteCont(Int64 limit, bool debug);
  void fRewriteCont(Int64 limit, bool debug);
  void eRewriteCont(Int64 limit, bool debug);
  bool contLoop2(const Vector<Token>& input);
  void doLoop(CacheableRewritingContext* context, VisibleModule* module);
  void searchCont(Int64 limit, bool debug);
  void sRewriteCont(Int64 limit, bool debug);
  void dsRewriteCont(Int64 limit, bool debug);
  void doSearching(Timer& timer,
		   VisibleModule* module,
		   RewriteSequenceSearch* state,
		   Int64 solutionCount,
		   Int64 limit);
  void doNarrowing(Timer& timer,
		   VisibleModule* module,
		   NarrowingSequenceSearch* state,
		   Int64 solutionCount,
		   Int64 limit);
  void narrowingCont(Int64 limit, bool debug);
  void doVuNarrowing(Timer& timer,
		     VisibleModule* module,
		     NarrowingSequenceSearch3* state,
		     Int64 solutionCount,
		     Int64 limit);
  void vuNarrowingCont(Int64 limit, bool debug);
  /*
  void doFvuNarrowing(Timer& timer,
		      VisibleModule* module,
		      NarrowingSequenceSearch3* state,
		      Int64 solutionCount,
		      Int64 limit);
  */
  void doGetVariants(Timer& timer,
		     VisibleModule* module,
		     VariantSearch* state,
		     Int64 solutionCount,
		     Int64 limit);
  void getVariantsCont(Int64 limit, bool debug);
  void doVariantUnification(Timer& timer,
			    VisibleModule* module,
			    VariantSearch* state,
			    Int64 solutionCount,
			    Int64 limit);
  void variantUnifyCont(Int64 limit, bool debug);
  void doVariantMatching(Timer& timer,
			 VisibleModule* module,
			 VariantSearch* state,
			 Int64 solutionCount,
			 Int64 limit);
  void variantMatchCont(Int64 limit, bool debug);
  void doSmtSearch(Timer& timer,
		   VisibleModule* module,
		   SMT_RewriteSequenceSearch* state,
		   Int64 solutionCount,
		   Int64 limit);
  void smtSearchCont(Int64 limit, bool debug);

  void doStrategicSearch(Timer& timer,
			 VisibleModule* module,
			 StrategicSearch* state,
			 Int64 solutionCount,
			 Int64 limit,
			 bool depthSearch);
  void printDecisionTime(const Timer& timer);
  void printSearchTiming(const Timer& timer,  RewriteSequenceSearch* state);
  void doMatching(Timer& timer,
		  VisibleModule* module,
		  MatchSearchState* state,
		  int solutionCount,
		  int limit);

  void matchCont(Int64 limit, bool debug);
  void doUnification(Timer& timer,
		     VisibleModule* module,
		     UnificationProblem* problem,
		     int solutionCount,
		     int limit);
  void unifyCont(Int64 limit, bool debug);
  void updateSet(set<int>& target, bool add);
  bool checkSearchRestrictions(SearchKind searchKind,
			       int searchType,
			       Term* target,				     
			       const Vector<ConditionFragment*>& condition,
			       MixfixModule* module);

  ofstream* xmlLog;
  MaudemlBuffer* xmlBuffer;
  MaudeLatexBuffer* latexBuffer;

  int flags;
  int printFlags;
  SyntacticPreModule* currentModule;
  SyntacticView* currentView;
  //
  //	Continuation information.
  //
  CacheableState* savedState;
  Int64 savedSolutionCount;
  VisibleModule* savedModule;
  ContinueFuncPtr continueFunc;
  Vector<Token> savedLoopSubject;

  set<int> selected;		// temporary for building set of identifiers
  set<int> traceIds;		// names of symbols/labels selected for tracing
  set<int> breakIds;		// names of symbols/labels selected as break points
  set<int> excludedModules;	// names of modules to be excluded from tracing
  set<int> concealedSymbols;	// names of symbols to have their arguments concealed during printing
};

inline void
Interpreter::traceSelect(bool add)
{
  updateSet(traceIds, add);
}

inline void
Interpreter::breakSelect(bool add)
{
  updateSet(breakIds, add);
}

inline void
Interpreter::traceExclude(bool add)
{
  updateSet(excludedModules, add);
}

inline void
Interpreter::printConceal(bool add)
{
  updateSet(concealedSymbols, add);
}

inline bool
Interpreter::traceId(int id)
{
  return traceIds.find(id) != traceIds.end();
}

inline bool
Interpreter::breakId(int id)
{
  return breakIds.find(id) != breakIds.end();
}

inline bool
Interpreter::excludedModule(int id)
{
  return excludedModules.find(id) != excludedModules.end();
}

inline MaudemlBuffer*
Interpreter::getXmlBuffer() const
{
  return xmlBuffer;
}

inline bool
Interpreter::getFlag(Flags flag) const
{
  return flags & flag;
}

inline bool
Interpreter::getPrintFlag(PrintFlags flag) const
{
  return printFlags & flag;
}

inline int
Interpreter::getPrintFlags() const
{
  return printFlags;
}

inline SyntacticPreModule*
Interpreter::getCurrentModule() const
{
  return currentModule;
}

inline SyntacticView*
Interpreter::getCurrentView() const
{
  return currentView;
}

inline void
Interpreter::setCurrentView(SyntacticView* view)
{
  currentView = view;
}

#endif
