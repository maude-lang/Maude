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
//      Class for user level rewriting contexts
//
#ifndef _userLevelRewritingContext_hh_
#define _userLevelRewritingContext_hh_
#include <signal.h>
#ifdef USE_LIBSIGSEGV
#include "sigsegv.h"
#endif
#include "objectSystemRewritingContext.hh"
#include "metadataStore.hh"
#include "rule.hh"
class Token;  // HACK

class UserLevelRewritingContext : public ObjectSystemRewritingContext
{
  NO_COPYING(UserLevelRewritingContext);

public:
  enum OtherPurpose
  {
    TOP_LEVEL_EVAL = OTHER + 1,
    META_EVAL
  };

  enum ParseResult
  {
    NORMAL,
    QUIT,
    RESUME,
    ABORT,
    STEP,
    WHERE
  };

  UserLevelRewritingContext(DagNode* root);
  UserLevelRewritingContext(DagNode* root,
			    UserLevelRewritingContext* parent,
			    int purpose,
			    bool localTraceFlag);


  static void setHandlers(bool handleCtrlC);
  static ParseResult commandLoop();
  static bool interrupted();
  static bool aborted();
  static void setInteractive(bool status);
  static void beginCommand();
  static void setDebug();
  static void clearDebug();
  static void clearInterrupt();
  static void clearTrialCount();

  RewritingContext* makeSubcontext(DagNode* root, int purpose);
  void beAdoptedBy(UserLevelRewritingContext* newParent);
  int traceBeginEqTrial(DagNode* subject, const Equation* equation);
  int traceBeginRuleTrial(DagNode* subject, const Rule* rule);
  int traceBeginScTrial(DagNode* subject, const SortConstraint* sc);
  void traceEndTrial(int trailRef, bool success);
  void traceExhausted(int trialRef);

  void tracePreEqRewrite(DagNode* redex, const Equation* equation, int type);
  void tracePostEqRewrite(DagNode* replacement);
  void tracePreRuleRewrite(DagNode* redex, const Rule* rule);
  void tracePostRuleRewrite(DagNode* replacement);
  void tracePreScApplication(DagNode* subject, const SortConstraint* sc);
  bool traceAbort();
  void traceBeginFragment(int trialRef,
			  const PreEquation* preEquation,
			  int fragmentIndex,
			  bool firstAttempt);
  void traceEndFragment(int trialRef,
			const PreEquation* preEquation,
			int fragmentIndex,
			bool success);

  void traceNarrowingStep(Rule* rule,
			  DagNode* redex,
			  DagNode* replacement,
			  const NarrowingVariableInfo* variableInfo,
			  const Substitution* substitution,
			  DagNode* newState);

  static void printSubstitution(const Substitution& substitution,
				const VariableInfo& varInfo);



private:
  static void interruptHandler(int);
  static void interruptHandler2(...);

#ifdef USE_LIBSIGSEGV
  static void stackOverflowHandler(int emergency, stackoverflow_context_t scp);
  static int sigsegvHandler(void *fault_address, int serious);
#endif
  static void internalErrorHandler(int signalNr);

  static void changePrompt();
  static bool dontTrace(const DagNode* redex, const PreEquation* pe);
  void checkForPrintAttribute(MetadataStore::ItemType itemType, const PreEquation* item);
  bool handleDebug(const DagNode* subject, const PreEquation* pe);
  void where();

  static bool tracePostFlag;
  static int trialCount;
  static const char header[];

  static bool interactiveFlag;
  static bool ctrlC_Flag;
  static bool stepFlag;
  static bool abortFlag;
  static int debugLevel;

  static AutoWrapBuffer* wrapOut;
  static AutoWrapBuffer* wrapErr;

  UserLevelRewritingContext* parent;
  const int purpose;
  bool localTraceFlag;
};

inline void
UserLevelRewritingContext::clearTrialCount()
{
  trialCount = 0;
}

inline bool
UserLevelRewritingContext::interrupted()
{
  return ctrlC_Flag;
}

inline bool
UserLevelRewritingContext::aborted()
{
  return abortFlag;
}

inline void
UserLevelRewritingContext::setDebug()
{
  setTraceStatus(true);
  stepFlag = true;
}

inline void
UserLevelRewritingContext::beAdoptedBy(UserLevelRewritingContext* newParent)
{
  parent = newParent;
}

#endif
