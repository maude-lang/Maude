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
  virtual ~RewritingContext();

  static bool getTraceStatus();
  static void setTraceStatus(bool state);

  DagNode* root();
  void incrementMbCount(Int64 i = 1);
  void incrementEqCount(Int64 i = 1);
  void incrementRlCount(Int64 i = 1);
  void clearCount();
  void addInCount(RewritingContext& other);
  Int64 getTotalCount() const;
  Int64 getMbCount() const;
  Int64 getEqCount() const;
  Int64 getRlCount() const;
  void reduce();
  void ruleRewrite(Int64 limit = NONE);
  void fairRewrite(Int64 limit = NONE, Int64 gas = 1);
  void fairContinue(Int64 limit = NONE);
  bool builtInReplace(DagNode* old, DagNode* replacement);

  virtual RewritingContext* makeSubcontext(DagNode* root, int purpose = OTHER);
  virtual int traceBeginEqTrial(DagNode* subject, const Equation* equation);
  virtual int traceBeginRuleTrial(DagNode* subject, const Rule* rule);
  virtual int traceBeginScTrial(DagNode* subject, const SortConstraint* sc);
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

#ifdef DUMP
  static void dumpStack(ostream& s, const Vector<RedexPosition>& stack);
#endif

private:
  enum Special
  {
    ROOT_OK = -2
  };

  void markReachableNodes();
  void rebuildUptoRoot();
  void remakeStaleDagNode(int staleIndex, int childIndex);
  bool ascend();
  void descend();
  bool doRewriting(bool argsUnstackable);
  bool fairTraversal(bool newTraversal);

  static bool traceFlag;

  DagNode* rootNode;
  Int64 mbCount;
  Int64 eqCount;
  Int64 rlCount;
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
  : rootNode(root)
{
  Assert(root != 0, "null root");
  mbCount = 0;
  eqCount = 0;
  rlCount = 0;
  staleMarker = ROOT_OK;
}

inline
RewritingContext::~RewritingContext()
{
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
  return mbCount + eqCount + rlCount;
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
RewritingContext::clearCount()
{
  mbCount = 0;
  eqCount = 0;
  rlCount = 0;
}

inline void
RewritingContext::addInCount(RewritingContext& other)
{
  mbCount += other.mbCount;
  eqCount += other.eqCount;
  rlCount += other.rlCount;
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

#endif
