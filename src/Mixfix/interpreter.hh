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
//      Class to hold interpreter state.
//
#ifndef _interpreter_hh_
#define _interpreter_hh_
#include "moduleDatabase.hh"
#include "moduleCache.hh"
#include "compiler.hh"
#include "intSet.hh"

class Interpreter : public ModuleDatabase, public ModuleCache
#ifdef COMPILER
, public Compiler
#endif
{
  NO_COPYING(Interpreter);

public:
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
    //
    //	General prettyprinter flags.
    //
    PRINT_GRAPH = 0x100000,
    PRINT_CONCEAL = 0x200000,
    PRINT_FORMAT = 0x400000,
    PRINT_MIXFIX = 0x800000,
    PRINT_WITH_PARENS = 0x1000000,
    //
    //	Prettyprinter flags for particular symbol types.
    //
    PRINT_WITH_ALIASES = 0x2000000,	// for variables
    PRINT_FLAT = 0x4000000,		// for assoc symbols
    //PRINT_ITER = 0x800000,		// for iter symbols
    PRINT_NUMBER = 0x8000000,		// for nats & ints
    PRINT_RAT = 0x10000000,		// for rats

    PRINT_COLOR = 0x20000000,		// for dag node coloring
    //
    //	Compiler flags.
    //
    COMPILE_COUNT = 0x80000000,
    //
    //	Flag groups.
    //
    EXCEPTION_FLAGS = TRACE | BREAK | PROFILE,

    DEFAULT_FLAGS = SHOW_COMMAND | SHOW_STATS | SHOW_TIMING | SHOW_LOOP_TIMING |
    COMPILE_COUNT |
    TRACE_CONDITION | TRACE_SUBSTITUTION | TRACE_MB | TRACE_EQ | TRACE_RL |
    AUTO_CLEAR_PROFILE |
    PRINT_FORMAT | PRINT_MIXFIX | PRINT_WITH_ALIASES |
    PRINT_FLAT | PRINT_NUMBER | PRINT_RAT
  };

  Interpreter();
  ~Interpreter();

  void beginXmlLog(const char* fileName);
  void endXmlLog();
  MaudemlBuffer* getXmlBuffer() const;

  void setFlag(Flags flag, bool polarity);
  bool getFlag(Flags flag) const;

  PreModule* getCurrentModule() const;
  bool setCurrentModule(const Vector<Token>& moduleExpr, int start = 0);
  void setCurrentModule(PreModule* module);
  void makeClean(int lineNumber);

  void parse(const Vector<Token>& subject);
  void reduce(const Vector<Token>& subject, bool debug);
  void creduce(const Vector<Token>& subject);
  void rewrite(const Vector<Token>& subject, Int64 limit, bool debug);
  void fRewrite(const Vector<Token>& subject, Int64 limit, Int64 gas, bool debug);
  void cont(Int64 limit, bool debug);

  void match(const Vector<Token>& bubble, bool withExtension, Int64 limit);
  void search(const Vector<Token>& bubble, Int64 limit);
  void showSearchPath(int stateNr);
  void showSearchGraph();

  void loop(const Vector<Token>& subject);
  void contLoop(const Vector<Token>& input);

  void addSelected(const Vector<Token>& opName);
  void traceSelect(bool add);
  void breakSelect(bool add);
  void traceExclude(bool add);
  void printConceal(bool polarity);

  void showProfile() const;
  void showKinds() const;
  void showSummary() const;
  void showSortsAndSubsorts() const;
  void showModule(bool all = true) const;
  void showModules(bool all) const;
  void showVars() const;
  void showOps(bool all = true) const;
  void showMbs(bool all = true) const;
  void showEqs(bool all = true) const;
  void showRls(bool all = true) const;

private:
  typedef void (Interpreter::*ContinueFuncPtr)(Int64 limit, bool debug);

  static DagNode* makeDag(Term* subjectTerm);
  static void beginRewriting(bool debug);
  static void printTiming(Int64 nrRewrites, Int64 cpu, Int64 real);
  static void printBubble(ostream& s, const Vector<int>& bubble);

  void clearContinueInfo();
  DagNode* makeDag(const Vector<Token>& subject);
  void startUsingModule(VisibleModule* module);
  void printStats(const Timer& timer, RewritingContext& context, bool timingFlag);
  void endRewriting(Timer& timer,
		    UserLevelRewritingContext* context,
		    VisibleModule* module,
		    ContinueFuncPtr cf = 0);
  void rewriteCont(Int64 limit, bool debug);
  void fRewriteCont(Int64 limit, bool debug);
  bool contLoop2(const Vector<Token>& input);
  void doLoop(DagNode* d, VisibleModule* module);
  void searchCont(Int64 limit, bool debug);
  void doSearching(Timer& timer,
		   VisibleModule* module,
		   RewriteSequenceSearch* state,
		   int solutionCount,
		   int limit);
  void printMatchTiming(const Timer& timer);
  void printSearchTiming(const Timer& timer,  RewriteSequenceSearch* state);
  void doMatching(Timer& timer,
		  VisibleModule* module,
		  MatchSearchState* state,
		  int solutionCount,
		  int limit);
  void matchCont(Int64 limit, bool debug);

  ofstream* xmlLog;
  MaudemlBuffer* xmlBuffer;

  int flags;
  PreModule* currentModule;
  //
  //	Continuation information.
  //
  UserLevelRewritingContext* savedContext;
  MatchSearchState* savedMatchSearchState;
  RewriteSequenceSearch* savedRewriteSequenceSearch;
  int savedSolutionCount;
  VisibleModule* savedModule;
  ContinueFuncPtr continueFunc;
  Vector<Token> savedLoopSubject;

  IntSet selected;
};

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

inline PreModule*
Interpreter::getCurrentModule() const
{
  return currentModule;
}

#endif
