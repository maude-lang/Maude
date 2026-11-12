/*

    This file is part of the Maude 3 strategy-aware model checker.

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
//	Class for building a strategy execution transition graph on-the-fly, with hash consing.
//
#ifndef _strategyTransitionGraph_hh_
#define _strategyTransitionGraph_hh_
#include <set>
#include <map>
#include "higher.hh"
#include "hashConsSet.hh"
#include "rewritingContext.hh"
#include "strategyLanguage.hh"
#include "strategicSearch.hh"
#include "strategicProcess.hh"

class StrategyTransitionGraph : protected StrategicSearch
{
  NO_COPYING(StrategyTransitionGraph);

public:

  // Transitions are usually rule applications, but other atomic
  // operations are considered.
  enum TransitionType
  {
    RULE_APPLICATION,
    OPAQUE_STRATEGY,	// opaque strategies
    SOLUTION		// self loops for solutions
  };

  // Transitions are described by a rule or strategy definition pointer.
  //
  // Since transitions are stored aplenty but consulted unfrequently,
  // a low-level hack is used to reduce the struct size: writing
  // the transition type in the lowest bit of the union, assuming
  // that the pointers it stores are aligned at least to two bytes.
  //
  struct Transition
  {
    Transition();	// for solutions
    Transition(Rule* rule);
    Transition(RewriteStrategy* strategy);

    bool operator ==(const Transition &other) const;
    bool operator <(const Transition &other) const;

    Rule* getRule() const;
    RewriteStrategy* getStrategy() const;
    TransitionType getType() const;

    private:

    union {
      Rule* rule;
      RewriteStrategy* strategy;
      uintptr_t raw;
    };

    static_assert(alignof(Rule*) >= 2, "pointers are not aligned, "
      "but the lowest bit is used as a flag");

    static constexpr uintptr_t MASK = 0x1;
  };

  typedef map<int, set<Transition> > ArcMap;

  StrategyTransitionGraph(RewritingContext* initial,
			  StrategyExpression* strategy,
			  const set<int> &opaqueIds,
			  bool biasedMatchrew = false,
			  bool makeSelfLoops = true);
  ~StrategyTransitionGraph();

  int getNrStates() const;
  int getNrRealStates() const;
  int getNextState(int stateNr, int index);
  DagNode* getStateDag(int stateNr) const;
  const ArcMap& getStateFwdArcs(int stateNr) const;
  StrategyExpression* getStrategyContinuation(int stateNr) const;
  int getRealStateNr(int stateNr) const;
  bool isSolutionState(int stateNr) const;

  //
  //	Stuff needed for search.
  //
  RewritingContext* getContext();
  void transferCount(RewritingContext& recipient);

  //
  //	Functions to be used by the strategy execution code
  //

  // A rewrite (or other atomic operation) has been done
  void commitState(int dagNode,
		   StrategyStackManager::StackId stackId,
		   StrategicExecution* taskSibling,
		   const Transition& transition);

  // This function are variation of commitState needed for an
  // implementation of the matchrew operator

  // Links the current state with the given state
  void linkState(int nextState,
		 const Transition& transition);

  // Creates a new state
  int newState(int dagNode,
	       int completeDagNode,
	       StrategyStackManager::StackId stackId,
	       StrategicProcess* initialProcess,
	       const Transition& transition);

  // Marks a states as a solution
  void markSolution(int state);

  // The following functions are called by the strategy execution
  // infrastructure in certain interesting situations

  // A convenient point for checking if the state was already visited
  // (it is requisite for avoiding loops and convenient for reducing
  // duplicate work)
  bool onCheckpoint(int dagNode, StrategicExecution* taskSibling,
		    int stackId, StrategicProcess* insertionPoint);
  // Just after a call task is created to run a tail call
  void onTailStrategyCall(StrategicTask* callTask, int varContext);

  // Gets group for context comparison
  void* getContextGroup(StrategicTask* task);

  // Opaque strategies
  bool isOpaque(int id) const;
  // Partial-order reduction by biased matchrews with multiple subterms
  bool useBiasedMatchrew() const;

  //
  //	Model checker information attached to the task
  //
  friend struct TaskInfo;

  //
  //	Functions to be used by the matchrew execution for the model checker
  //

  // Opens a new subgraphs and returns an identifier to it
  int newSubgraph(int initialDag, StackId pending, StrategicExecution* taskSibling);

  // Removes and frees a subgraph
  bool closeSubgraph(int subgraphId);

  // Selects a subgraph, returning the identifier of the current one
  int selectSubgraph(int subgraphId);

  std::pair<int,int> getStatePoint(int stateNr) const;

private:

  //
  //	Virtual in StrategicSearch
  //
  DagNode* findNextSolution() override;

  // Substates are system automaton states without identifying information
  struct Substate	// 112 bytes
  {
    // An auxiliary struct to hold dependencies information
    struct Dependency
    {
      Substate* dependee;
      size_t alreadyImported;
    };

    Substate(int dagNode, StrategyStackManager::StackId pending,
	     StrategicExecution* taskSibling);
    Substate();
    ~Substate();

    // The successors of the substate
    Vector<int> nextStates;
    ArcMap fwdArcs;
    bool hasSolution;	// To avoid adding self-loops twice

    // Sources that generate new successors
    StrategicProcess* nextProcess;
    std::list<Dependency> dependencies;

    // The number of elements that refer to this substate (memory management)
    size_t referenceCount;
    // The number of the state in the seen array, or NONE if it is a proper substate
    int stateNr;

    // Auxiliary functions to deal with dependencies
    size_t addDependency(Substate* dependee);
    size_t importDependency(std::list<Dependency>::iterator dependency);
    size_t importDependencies();

    bool finished() const;
    bool free();
  };

  // Auxiliary comparison functions for dependencies
  friend bool operator<(const Substate::Dependency &left,
			const Substate::Dependency &right);
  friend bool operator==(const Substate::Dependency &left,
			 const Substate::Dependency &right);

  // States are substates that are part of the rewrite graph
  // (they are associated with a term and a stack id)
  struct State : public Substate	// 120 bytes
  {
    State(int dagNode,
	  int completeDagNode,
	  StrategyStackManager::StackId pending,
	  StrategicExecution* taskSibling);
    State(StrategicProcess* initialProcess,
	  int dagNode,
	  int completeDagNode,
	  StrategyStackManager::StackId pending);
    State(int dagNode, int stateNr);		// For self-loop states
    State(DecompositionProcess* process);	// For the initial process

    int dagNode;
    int stackId;
    const int completeDagNode;	// For states inside matchrew
  };

  // Create a self loop in the given substates, which implements the
  // stuttering extension of the solution of a strategy
  int makeSelfLoop(Substate* substate, int dagNode);
  void solveCyclicDependency(const vector<Substate*> &dependencyStack, size_t loop);
  // Make a strategy absorb another one when they are equivalent
  void absorbState(int absorber, int absorbed);
  // Lightweight execution of trivial processes just after a rewrite to
  // avoid creating new states in the graph that will fail inmediately
  void descend();
  bool descendProcess(StrategicProcess* proc) const;
  bool importFirstDependency(Substate* dependent);

  // This variable communicate getNextState with commitState/onResumeTask
  Substate* currentSubstate;
  int nrNextStates;

  // Initial data (the state, the strategy and the opaque strategies)
  RewritingContext* initial;
  StrategyExpression* strategy;
  set<int> opaqueStrategies;
  bool biasedMatchrew;
  bool avoidSelfLoops;

  // The states of the graph
  Vector<State*>* seen;
  StrategicTask* root;

  // Subgraphs for matchrew
  int currentSubgraph;

  struct SubgraphData {
    SubgraphData(StrategicTask* root)
     : states(1), root(root), currentSubstate(0), nrNextStates(0) { }

    Vector<State*> states;
    StrategicTask* root;
    Substate* currentSubstate;
    int nrNextStates;
  };

  Vector<SubgraphData*> subgraphs;

  TaskInfo &getTaskInfo(StrategicTask* task);
};

// A function to delete TaskInfo without revealing its content
void deleteTaskInfo(TaskInfo* taskInfo);

inline int
StrategyTransitionGraph::getNrStates() const
{
  return seen->length();
}

inline DagNode*
StrategyTransitionGraph::getStateDag(int stateNr) const
{
  return getCanonical((*seen)[stateNr]->completeDagNode);
}

inline StrategyExpression*
StrategyTransitionGraph::getStrategyContinuation(int stateNr) const
{
  return top((*seen)[stateNr]->stackId);
}

inline int
StrategyTransitionGraph::getRealStateNr(int stateNr) const
{
  return (*seen)[stateNr]->stateNr;
}

inline const StrategyTransitionGraph::ArcMap&
StrategyTransitionGraph::getStateFwdArcs(int stateNr) const
{
  return (*seen)[stateNr]->fwdArcs;
}

inline std::pair<int,int>
StrategyTransitionGraph::getStatePoint(int stateNr) const
{
  const State *state = (*seen)[stateNr];

  return make_pair(state->completeDagNode, state->stackId);
}

inline void
StrategyTransitionGraph::markSolution(int stateNr)
{
  (*seen)[stateNr]->hasSolution = true;
}

inline bool
StrategyTransitionGraph::isSolutionState(int stateNr) const
{
  return (*seen)[stateNr]->hasSolution;
}

inline RewritingContext*
StrategyTransitionGraph::getContext()
{
  return initial;
}

inline void
StrategyTransitionGraph::transferCount(RewritingContext& recipient)
{
  recipient.addInCount(*initial);
  initial->clearCount();
}

inline
StrategyTransitionGraph::Transition::Transition()
 : raw(0)
{
}

inline
StrategyTransitionGraph::Transition::Transition(Rule* rule)
 : rule(rule)
{
}

inline
StrategyTransitionGraph::Transition::Transition(RewriteStrategy* strategy)
 : strategy(strategy)
{
  raw |= MASK;
}

inline bool
StrategyTransitionGraph::Transition::operator ==(const Transition &other) const
{
  return raw == other.raw;
}

inline bool
StrategyTransitionGraph::Transition::operator <(const Transition &other) const
{
  return raw < other.raw;
}

inline StrategyTransitionGraph::TransitionType
StrategyTransitionGraph::Transition::getType() const
{
  return raw == 0 ? TransitionType::SOLUTION : ((raw & MASK) == 0 ?
      TransitionType::RULE_APPLICATION
    : TransitionType::OPAQUE_STRATEGY);
}

inline Rule*
StrategyTransitionGraph::Transition::getRule() const
{
  return (raw & MASK) == 0 ? rule : nullptr;
}

inline RewriteStrategy*
StrategyTransitionGraph::Transition::getStrategy() const
{
  return (raw & MASK) == 0 ? nullptr :
    reinterpret_cast<RewriteStrategy*>(raw & ~ MASK);
}

inline bool
StrategyTransitionGraph::isOpaque(int id) const
{
  return opaqueStrategies.find(id) != opaqueStrategies.end();
}

inline bool
StrategyTransitionGraph::useBiasedMatchrew() const
{
  return biasedMatchrew;
}

inline
StrategyTransitionGraph::Substate::Substate()
 : hasSolution(false),
   nextProcess(nullptr),
   referenceCount(0),
   stateNr(NONE)
{
}

inline bool
StrategyTransitionGraph::Substate::finished() const
{
  return nextProcess == nullptr && dependencies.empty();
}

inline bool
StrategyTransitionGraph::Substate::free()
{
  if (--referenceCount == 0)
    {
      if (stateNr == NONE)
	delete this;
      else
	delete static_cast<State*>(this);
      return true;
    }
  return false;
}


#endif
