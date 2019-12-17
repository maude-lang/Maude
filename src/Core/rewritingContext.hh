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
//	Class for rewriting contexts.
//
//	A rewriting context keeps track of miscellaneous information needed
//	while rewriting. An important perfomance trick is that we derive it
//	from Substitution so that we can use the rewriting context to construct
//	matching substitutions in. This avoids creating a new substitution at
//	the start of each match attempt.
//
#ifndef _rewritingContext_hh_
#define _rewritingContext_hh_
#include "substitution.hh"
#include "simpleRootContainer.hh"
#include "dagNode.hh"
#include "module.hh"

class RewritingContext : public Substitution, private SimpleRootContainer
{
public:
  enum Purpose
  {
    CONDITION_EVAL,
    SORT_EVAL,
    OTHER
  };

  enum RewriteType
  {
    NORMAL,
    BUILTIN,
    MEMOIZED
  };

  RewritingContext(DagNode* root);
  RewritingContext(int substitutionSize);  // limited use RewritingContext for matching
  virtual ~RewritingContext();

  static bool getTraceStatus();
  static void setTraceStatus(bool state);

  DagNode* root();
  void incrementMbCount(Int64 i = 1);
  void incrementEqCount(Int64 i = 1);
  void incrementRlCount(Int64 i = 1);
  void incrementNarrowingCount(Int64 i = 1);
  void incrementVariantNarrowingCount(Int64 i = 1);

  void clearCount();
  void addInCount(const RewritingContext& other);
  void transferCountFrom(RewritingContext& other);
  Int64 getTotalCount() const;
  Int64 getMbCount() const;
  Int64 getEqCount() const;
  Int64 getRlCount() const;
  Int64 getNarrowingCount() const;
  Int64 getVariantNarrowingCount() const;
  void reduce();
  void ruleRewrite(Int64 limit = NONE);
  void fairRewrite(Int64 limit = NONE, Int64 gas = 1);
  void fairContinue(Int64 limit = NONE);

  void fairStart(Int64 limit, Int64 gas);
  void fairRestart(Int64 limit);
  bool fairTraversal();
  bool getProgress();

  bool builtInReplace(DagNode* old, DagNode* replacement);

  virtual RewritingContext* makeSubcontext(DagNode* root, int purpose = OTHER);
  virtual int traceBeginEqTrial(DagNode* subject, const Equation* equation);
  virtual int traceBeginRuleTrial(DagNode* subject, const Rule* rule);
  virtual int traceBeginScTrial(DagNode* subject, const SortConstraint* sc);
  virtual int traceBeginSdTrial(DagNode* subject, const StrategyDefinition* sc);
  virtual void traceEndTrial(int trialRef, bool success);
  virtual void traceExhausted(int trialRef);

  virtual void tracePreEqRewrite(DagNode* redex,
				 const Equation* equation,
				 int type);
  virtual void tracePostEqRewrite(DagNode* replacement);
  virtual void tracePreRuleRewrite(DagNode* redex, const Rule* rule);
  virtual void tracePostRuleRewrite(DagNode* replacement);
  virtual void tracePreScApplication(DagNode* subject, const SortConstraint* sc);
  virtual bool traceAbort();
  virtual void traceBeginFragment(int trialRef,
				  const PreEquation* preEquation,
				  int fragmentIndex,
				  bool firstAttempt);
  virtual void traceEndFragment(int trialRef,
				const PreEquation* preEquation,
				int fragmentIndex,
				bool success);

  virtual void traceNarrowingStep(Rule* rule,
				  DagNode* redex,
				  DagNode* replacement,
				  const NarrowingVariableInfo* variableInfo,
				  const Substitution* substitution,
				  DagNode* newState);

  virtual void traceVariantNarrowingStep(Equation* equation,
					 const Vector<DagNode*>& oldVariantSubstitution,
					 DagNode* redex,
					 DagNode* replacement,
					 const NarrowingVariableInfo& variableInfo,
					 const Substitution* substitution,
					 DagNode* newState,
					 const Vector<DagNode*>& newVariantSubstitution,
					 const NarrowingVariableInfo& originalVariables);
  //
  //	This exists so we can handle an interrupt that causes a blocking call to return early.
  //	If it returns true, the caller assumes it can continue; otherwise the caller should return.
  //
  virtual bool handleInterrupt();

  virtual void traceStrategyCall(StrategyDefinition* sdef,
				 DagNode* callDag,
				 DagNode* subject,
				 const Substitution* substitution);

#ifdef DUMP
  static void dumpStack(ostream& s, const Vector<RedexPosition>& stack);
#endif

protected:
  //
  //	We make this protected so that a derived class can override it and then call
  //	our version.
  //
  void markReachableNodes();

private:
  enum Special
  {
    ROOT_OK = -2
  };

  void rebuildUptoRoot();
  void remakeStaleDagNode(int staleIndex, int childIndex);
  bool ascend();
  void descend();
  bool doRewriting(bool argsUnstackable);

  static bool traceFlag;

  DagNode* rootNode;

  Int64 mbCount;
  Int64 eqCount;
  Int64 rlCount;

  Int64 narrowingCount;
  Int64 variantNarrowingCount;
  //
  //	For rule rewriting
  //
  Vector<RedexPosition> redexStack;
  int currentIndex;
  int staleMarker;
  bool progress;
  Int64 rewriteLimit;
  Int64 gasPerNode;
  Int64 currentGas;
  int lazyMarker;
};

inline
RewritingContext::RewritingContext(DagNode* root)
  : Substitution(root->symbol()->getModule()->getMinimumSubstitutionSize(), 0),
    rootNode(root)
{
  //cout << "sub size is " << root->symbol()->getModule()->getMinimumSubstitutionSize() << endl;
  Assert(root != 0, "null root");
  mbCount = 0;
  eqCount = 0;
  rlCount = 0;

  narrowingCount = 0;
  variantNarrowingCount = 0;
  staleMarker = ROOT_OK;
}

inline
RewritingContext::RewritingContext(int substitutionSize)
  : Substitution(substitutionSize),
    rootNode(0)
{
  //
  //	This constructor exists so we can build RewritingContexts for use in the solve()
  //	phase of matching where we don't otherwise have a RewritingContext to hand.
  //
}

inline
RewritingContext::~RewritingContext()
{
}

inline void
RewritingContext::reduce()
{
  rootNode->reduce(*this);
}

inline DagNode*
RewritingContext::root()
{
  if (staleMarker != ROOT_OK)
    rebuildUptoRoot();
  return rootNode;
}

inline bool
RewritingContext::getTraceStatus()
{
  return traceFlag;
}

inline void
RewritingContext::setTraceStatus(bool state)
{
  traceFlag = state;
}

inline Int64
RewritingContext::getTotalCount() const
{
  return mbCount + eqCount + rlCount + narrowingCount + variantNarrowingCount;
}
inline Int64
RewritingContext::getMbCount() const
{
  return mbCount;
}
inline Int64
RewritingContext::getEqCount() const
{
  return eqCount;
}

inline Int64
RewritingContext::getRlCount() const
{
  return rlCount;
}

inline Int64
RewritingContext::getNarrowingCount() const
{
  return narrowingCount;
}

inline Int64
RewritingContext::getVariantNarrowingCount() const
{
  return variantNarrowingCount;
}

inline void
RewritingContext::incrementMbCount(Int64 i)
{
  mbCount += i;
}

inline void
RewritingContext::incrementEqCount(Int64 i)
{
  eqCount += i;
}

inline void
RewritingContext::incrementRlCount(Int64 i)
{
  rlCount += i;
}

inline void
RewritingContext::incrementNarrowingCount(Int64 i)
{
  narrowingCount += i;
}

inline void
RewritingContext::incrementVariantNarrowingCount(Int64 i)
{
  variantNarrowingCount += i;
}


inline void
RewritingContext::clearCount()
{
  mbCount = 0;
  eqCount = 0;
  rlCount = 0;

  narrowingCount = 0;
  variantNarrowingCount = 0;
}

inline void
RewritingContext::addInCount(const RewritingContext& other)
{
  mbCount += other.mbCount;
  eqCount += other.eqCount;
  rlCount += other.rlCount;

  narrowingCount += other.narrowingCount;
  variantNarrowingCount += other.variantNarrowingCount;
}

inline void
RewritingContext::transferCountFrom(RewritingContext& other)
{
  mbCount += other.mbCount;
  other.mbCount = 0;
  eqCount += other.eqCount;
  other.eqCount = 0;
  rlCount += other.rlCount;
  other.rlCount = 0;

  narrowingCount += other.narrowingCount;
  other.narrowingCount = 0;
  variantNarrowingCount += other.variantNarrowingCount;
  other.variantNarrowingCount = 0;
}

inline bool
RewritingContext::builtInReplace(DagNode* old, DagNode* replacement)
{
   bool trace = traceFlag;
   if (trace)
     {
       tracePreEqRewrite(old, 0, BUILTIN);
       if (traceAbort())
	 return false;
     }
   replacement->overwriteWithClone(old);
   ++eqCount;
   if (trace)
     tracePostEqRewrite(old);
   return true;
}

inline bool
RewritingContext::getProgress()
{
  return progress;
}

inline void
RewritingContext::fairRestart(Int64 limit)
{
  rewriteLimit = limit;
}

#endif
