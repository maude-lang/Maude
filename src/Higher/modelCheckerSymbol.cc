/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class ModelCheckerSymbol.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "temporal.hh"
#include "interface.hh"
#include "core.hh"
#include "freeTheory.hh"
#include "builtIn.hh"
#include "higher.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "dagArgumentIterator.hh"
#include "substitution.hh"
#include "rewritingContext.hh"
#include "symbolMap.hh"
#include "rule.hh"
#include "rewriteStrategy.hh"
#include "strategyDefinition.hh"

//	mixfix class definitions
#include "token.hh"

//      free theory class definitions
#include "freeDagNode.hh"

//      built in class definitions
#include "bindingMacros.hh"

//	temporal class definitions
#include "logicFormula.hh"

//	strategy class definitions
#include "applicationStrategy.hh"
#include "callStrategy.hh"

//HACK
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierDagNode.hh"

//      higher class definitions
#include "modelCheckerSymbol.hh"

ModelCheckerSymbol::ModelCheckerSymbol(int id)
  : TemporalSymbol(id, 3)
{
  satisfiesSymbol = nullptr;
  qidSymbol = nullptr;
  unlabeledSymbol = nullptr;
  deadlockSymbol = nullptr;
  solutionSymbol = nullptr;
  stratSymbol = nullptr;
  transitionSymbol = nullptr;
  transitionListSymbol = nullptr;
  nilTransitionListSymbol = nullptr;
  counterexampleSymbol = nullptr;
  usingSymbol = nullptr;
  opaqueSymbol = nullptr;
  fullMatchrewSymbol = nullptr;
  noneOptionSetSymbol = nullptr;
  optionSetSymbol = nullptr;
}

bool
ModelCheckerSymbol::attachData(const Vector<Sort*>& opDeclaration,
			       const char* purpose,
			       const Vector<const char*>& data)
{
  NULL_DATA(purpose, ModelCheckerSymbol, data);
  return  TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
ModelCheckerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
  BIND_SYMBOL(purpose, symbol, unlabeledSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, deadlockSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, solutionSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, stratSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, transitionSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, transitionListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, nilTransitionListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, counterexampleSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, usingSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, opaqueSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, fullMatchrewSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, noneOptionSetSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, optionSetSymbol, Symbol*);
  return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
ModelCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, trueTerm);
  return TemporalSymbol::attachTerm(purpose, term);
}

void
ModelCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  ModelCheckerSymbol* orig = safeCastNonNull<ModelCheckerSymbol*>(original);
  COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
  COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
  COPY_SYMBOL(orig, unlabeledSymbol, map, Symbol*);
  COPY_SYMBOL(orig, deadlockSymbol, map, Symbol*);
  COPY_SYMBOL(orig, solutionSymbol, map, Symbol*);
  COPY_SYMBOL(orig, stratSymbol, map, Symbol*);
  COPY_SYMBOL(orig, transitionSymbol, map, Symbol*);
  COPY_SYMBOL(orig, transitionListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, nilTransitionListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, counterexampleSymbol, map, Symbol*);
  COPY_SYMBOL(orig, usingSymbol, map, Symbol*);
  COPY_SYMBOL(orig, opaqueSymbol, map, Symbol*);
  COPY_SYMBOL(orig, fullMatchrewSymbol, map, Symbol*);
  COPY_SYMBOL(orig, noneOptionSetSymbol, map, Symbol*);
  COPY_SYMBOL(orig, optionSetSymbol, map, Symbol*);

  COPY_TERM(orig, trueTerm, map);
  TemporalSymbol::copyAttachments(original, map);
}

void
ModelCheckerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				       Vector<const char*>& purposes,
				       Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, ModelCheckerSymbol);
  TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
ModelCheckerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
  APPEND_SYMBOL(purposes, symbols, qidSymbol);
  APPEND_SYMBOL(purposes, symbols, unlabeledSymbol);
  APPEND_SYMBOL(purposes, symbols, deadlockSymbol);
  APPEND_SYMBOL(purposes, symbols, solutionSymbol);
  APPEND_SYMBOL(purposes, symbols, stratSymbol);
  APPEND_SYMBOL(purposes, symbols, transitionSymbol);
  APPEND_SYMBOL(purposes, symbols, transitionListSymbol);
  APPEND_SYMBOL(purposes, symbols, nilTransitionListSymbol);
  APPEND_SYMBOL(purposes, symbols, counterexampleSymbol);
  APPEND_SYMBOL(purposes, symbols, usingSymbol);
  APPEND_SYMBOL(purposes, symbols, opaqueSymbol);
  APPEND_SYMBOL(purposes, symbols, fullMatchrewSymbol);
  APPEND_SYMBOL(purposes, symbols, noneOptionSetSymbol);
  APPEND_SYMBOL(purposes, symbols, optionSetSymbol);
  TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
ModelCheckerSymbol::getTermAttachments(Vector<const char*>& purposes,
				       Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, trueTerm);
  TemporalSymbol::getTermAttachments(purposes, terms);
}

void
ModelCheckerSymbol::postInterSymbolPass()
{
  PREPARE_TERM(trueTerm);
  TemporalSymbol::postInterSymbolPass();
}

void
ModelCheckerSymbol::reset()
{
  trueTerm.reset();  // so true dag can be garbage collected
  TemporalSymbol::reset();  // parents reset() tasks
}

void
ModelCheckerSymbol::compileEquations()
{
  TemporalSymbol::compileEquations();
  setEqRewrite(&ModelCheckerSymbol::eqRewrite);
}

void
ModelCheckerSymbol::dump(const StateTransitionGraph& states, const list<int>& path)
{
  cout << "begin{StateList}\n";
  for (int stateNr : path)
    {
      cout << states.getStateDag(stateNr) << '\n';
    }
  cout << "end{StateList}\n";
}

void
ModelCheckerSymbol::dump(const StrategyTransitionGraph& states, const list<int>& path)
{
  cout << "begin{StateList}\n";
  for (int stateNr : path)
    {
      cout << states.getStateDag(stateNr) << '\n';
    }
  cout << "end{StateList}\n";
}

DagNode*
ModelCheckerSymbol::makeTransition(const StateTransitionGraph& states, int stateNr, int targetNr)
{
  static Vector<DagNode*> args(2);

  args[0] = states.getStateDag(stateNr);
  const StateTransitionGraph::ArcMap& fwdArcs = states.getStateFwdArcs(stateNr);
  StateTransitionGraph::ArcMap::const_iterator i = fwdArcs.find(targetNr);
  if (i == fwdArcs.end())
    args[1] = deadlockSymbol->makeDagNode();
  else
    {
      int id = (*(i->second.begin()))->getLabel().id();
      args[1] = (id == NONE) ? unlabeledSymbol->makeDagNode() :
	new QuotedIdentifierDagNode(qidSymbol, id);
    }
 return transitionSymbol->makeDagNode(args);
}

DagNode*
ModelCheckerSymbol::makeTransition(const StrategyTransitionGraph& states, int stateNr, int targetNr)
{
  static Vector<DagNode*> args(2);

  args[0] = states.getStateDag(stateNr);
  const StrategyTransitionGraph::ArcMap& fwdArcs = states.getStateFwdArcs(stateNr);
  StrategyTransitionGraph::ArcMap::const_iterator i = fwdArcs.find(targetNr);

  Assert(i != fwdArcs.end(), "missing transition information");

  const StrategyTransitionGraph::Transition &transition = *(i->second.begin());

  switch (transition.getType())
    {
      case StrategyTransitionGraph::RULE_APPLICATION:
	{
	  int label = transition.getRule()->getLabel().id();

	  args[1] = (label == NONE) ? unlabeledSymbol->makeDagNode() :
		      new QuotedIdentifierDagNode(qidSymbol, label);
	  break;
	}
      case StrategyTransitionGraph::OPAQUE_STRATEGY:
	{
	  Vector<DagNode*> args2(1);
	  int label = transition.getStrategy()->id();

	  args2[0] = new QuotedIdentifierDagNode(qidSymbol, label);
	  args[1] = stratSymbol->makeDagNode(args2);
	  break;
	}
      case StrategyTransitionGraph::SOLUTION :
      default :
	args[1] = solutionSymbol->makeDagNode();
    }

 return transitionSymbol->makeDagNode(args);
}

DagNode*
ModelCheckerSymbol::makeTransitionList(const StateTransitionGraph& states,
				       const list<int>& path,
				       int lastTarget)
{
  Vector<DagNode*> args;
  if (path.empty())
    return nilTransitionListSymbol->makeDagNode(args);
  const list<int>::const_iterator e = path.end();
  for (list<int>::const_iterator i = path.begin();;)
    {
      int stateNr = *i;
      if (++i == e)
	{
	  args.append(makeTransition(states, stateNr, lastTarget));
	  break;
	}
      else
	args.append(makeTransition(states, stateNr, *i));
    }
  return (args.length() == 1) ? args[0] : transitionListSymbol->makeDagNode(args);
}

DagNode*
ModelCheckerSymbol::makeTransitionList(const StrategyTransitionGraph& states,
				       const list<int>& path,
				       int lastTarget,
				       bool firstPath)
{
  Vector<DagNode*> args;
  if (path.empty())
    return nilTransitionListSymbol->makeDagNode(args);
  const list<int>::const_iterator e = path.end();
  for (list<int>::const_iterator i = path.begin();;)
    {
      int stateNr = *i;
      if (++i == e)
	{
	  // This removes the transition to the fake self-loop state.
	  // Read StrategyTransitionGraph::getSelfLoop for an explanation.
	  if (!firstPath || states.getStateFwdArcs(stateNr).find(lastTarget)->second.begin()->getType()
		!= StrategyTransitionGraph::SOLUTION)
	    {
	      args.append(makeTransition(states, stateNr, lastTarget));
	    }
	  else if (args.empty())
	    return nilTransitionListSymbol->makeDagNode(args);
	  break;
	}
      else
	args.append(makeTransition(states, stateNr, *i));
    }
  return (args.length() == 1) ? args[0] : transitionListSymbol->makeDagNode(args);
}

DagNode*
ModelCheckerSymbol::makeCounterexample(const StateTransitionGraph& states,
				       const ModelChecker2& mc)
{
  Vector<DagNode*> args(2);
  int junction = mc.getCycle().front();
  args[0] = makeTransitionList(states, mc.getLeadIn(), junction);
  args[1] = makeTransitionList(states, mc.getCycle(), junction);
  return counterexampleSymbol->makeDagNode(args);
}

DagNode*
ModelCheckerSymbol::makeCounterexample(const StrategyTransitionGraph& states,
				       const ModelChecker2& mc)
{
  Vector<DagNode*> args(2);
  int junction = mc.getCycle().front();
  args[0] = makeTransitionList(states, mc.getLeadIn(), junction, true);
  args[1] = makeTransitionList(states, mc.getCycle(), junction, false);
  return counterexampleSymbol->makeDagNode(args);
}

bool
ModelCheckerSymbol::eqRewrite(Symbol* symbol, DagNode* subject, RewritingContext& context)
{
  Assert(symbol == subject->symbol(), "bad symbol");
  ModelCheckerSymbol* s = safeCastNonNull<ModelCheckerSymbol*>(symbol);
  //  This is a hack; eventually we'll inline this function.
  return s->eqRewrite(subject, context);
}

bool
ModelCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  //
  //	Compute normalization of negated formula.
  //
  FreeDagNode* d = safeCastNonNull<FreeDagNode*>(subject);
  RewritingContext* sysContext = context.makeSubcontext(d->getArgument(0));
  RewritingContext* newContext = context.makeSubcontext(negate(d->getArgument(1)));
  newContext->reduce();
#ifdef TDEBUG
  cout << "Negated formula: " << newContext->root() << endl;
#endif
  //
  //  Parse model-checking options
  //
  StrategyExpression* strategy = nullptr;
  bool fullMatchrew = false;
  set<int> opaques;
  {
    RewritingContext* optContext = context.makeSubcontext(d->getArgument(2));
    optContext->reduce();
    context.addInCount(*optContext);

    if (!parseOptionSet(optContext->root(), strategy, fullMatchrew, opaques))
      {
	delete optContext;
	d->getArgument(0)->computeTrueSort(*sysContext);
	context.addInCount(*sysContext);
	delete sysContext;
	delete newContext;
	delete strategy;
	return tryEquations(subject, context);
      }
    delete optContext;
  }
  // Warnings if fullMatchrew or opaques are given without a strategy
  if (strategy == nullptr)
    {
      if (fullMatchrew)
	IssueAdvisory(QUOTE("full-matchrew") << " does not make sense when no " <<
	              "strategy is given to control rewriting.");
      if (!opaques.empty())
	IssueAdvisory(QUOTE("opaque") << " does not make sense when no " <<
	              "strategy is given to control rewriting.");
    }
  //
  //	Convert Dag into a LogicFormula.
  //
  BaseAutomaton* system = strategy == nullptr ? static_cast<BaseAutomaton*>(new SystemAutomaton)
                                              : static_cast<BaseAutomaton*>(new StrategyAutomaton);
  LogicFormula formula;
  int top = build(formula, system->propositions, newContext->root());
  if (top == NONE)
    {
      IssueAdvisory("negated LTL formula " << QUOTE(newContext->root()) <<
		    " did not reduce to a valid negative normal form.");
      d->getArgument(0)->computeTrueSort(*sysContext);
      context.addInCount(*sysContext);
      delete sysContext;
      delete newContext;
      delete strategy;
      delete system;
      return tryEquations(subject, context);
    }
  context.addInCount(*newContext);
#ifdef TDEBUG
  cout << "top = " << top << endl;
  formula.dump(cout);
#endif
  //
  //	Do the model check using a ModelChecker2 object.
  //
  system->satisfiesSymbol = satisfiesSymbol;
  system->parentContext = &context;
  system->trueTerm = trueTerm.getDag();
  if (strategy == nullptr)
    static_cast<SystemAutomaton*>(system)->systemStates = new StateTransitionGraph(sysContext);
  else
    static_cast<StrategyAutomaton*>(system)->systemStates = new StrategyTransitionGraph(sysContext, strategy,
											opaques, !fullMatchrew);
  ModelChecker2 mc(*system, formula, top);
  bool result = mc.findCounterexample();
  if (strategy == nullptr)
    {
      StateTransitionGraph* graph = static_cast<SystemAutomaton*>(system)->systemStates;
      int nrSystemStates = graph->getNrStates();
      Verbose("ModelCheckerSymbol: Examined " << nrSystemStates <<
	" system state" << pluralize(nrSystemStates) << '.');
#ifdef TDEBUG
      if (result)
	{
	  dump(*graph, mc.getLeadIn());
          dump(*graph, mc.getCycle());
	}
#endif
    }
  else
    {
      StrategyTransitionGraph* graph = static_cast<StrategyAutomaton*>(system)->systemStates;
      int nrSystemStates = graph->getNrStates();
      Verbose("ModelCheckerSymbol: Examined " << nrSystemStates <<
        " system state" << pluralize(nrSystemStates) << " (" <<
        graph->getNrRealStates() << " real).");
#ifdef TDEBUG
      if (result)
	{
	  dump(*graph, mc.getLeadIn());
          dump(*graph, mc.getCycle());
	}
#endif
    }
  delete newContext;
  DagNode* resultDag = result ? (strategy == nullptr ?
        makeCounterexample(*(static_cast<SystemAutomaton*>(system)->systemStates), mc)
      : makeCounterexample(*(static_cast<StrategyAutomaton*>(system)->systemStates), mc)
    ) : trueTerm.getDag();
  context.addInCount(*sysContext);
  delete system;  // deletes sysContext via ~StateTransitionGraph()
  return context.builtInReplace(subject, resultDag);
}

bool
ModelCheckerSymbol::parseOptionSet(DagNode* options, StrategyExpression*& strategy,
				   bool& fullMatchrew, set<int>& opaques)
{
  Symbol* mp = options->symbol();
  if (mp == optionSetSymbol)
    {
      for (DagArgumentIterator i(options); i.valid(); i.next())
	{
	  if (!parseOption(i.argument(), strategy, fullMatchrew, opaques))
	    return false;
	}
    }
  else if (mp != noneOptionSetSymbol)
    return parseOption(options, strategy, fullMatchrew, opaques);
  return true;
}

bool
ModelCheckerSymbol::parseOption(DagNode* option, StrategyExpression*& strategy,
				bool& fullMatchrew, set<int>& opaques)
{
  Symbol* mp = option->symbol();
  if (mp == usingSymbol)
    {
      if (strategy != nullptr)
	{
	  IssueWarning("the " << QUOTE("using") << " option is given twice.");
	  return false;
	}
      DagNode* argument = safeCastNonNull<FreeDagNode*>(option)->getArgument(0);
      if (QuotedIdentifierDagNode* argQid = dynamic_cast<QuotedIdentifierDagNode*>(argument))
	{
	  int strategyId = argQid->getIdIndex();
	  RewriteStrategy* candidate = nullptr;
	  for (RewriteStrategy* st : mp->getModule()->getStrategies())
	    if (st->id() == strategyId)
	      {
		candidate = st;
		if (candidate->arity() == 0) break;
	      }
	  if (candidate == nullptr)
	    {
	      IssueWarning("no strategy named " << QUOTE(Token::name(strategyId)) << " in scope.");
	      return false;
	    }
	  if (candidate->arity() != 0)
	    {
	      IssueWarning("the strategy given to " << QUOTE("modelCheck") << " must not have parameters.");
	      return false;
	    }

	    Vector<Term*> dummy;
	    strategy = new CallStrategy(candidate, candidate->makeAuxiliaryTerm(dummy));
	}
      else
	{
	  IssueWarning("the argument " << QUOTE(argument) << " of the " << QUOTE("using") << " option is not a quoted identifier.");
	  return false;
	}
    }
  else if (mp == fullMatchrewSymbol)
    fullMatchrew = true;
  else if (mp == opaqueSymbol)
    {
      DagNode* argument = safeCastNonNull<FreeDagNode*>(option)->getArgument(0);
      if (QuotedIdentifierDagNode* argQid = dynamic_cast<QuotedIdentifierDagNode*>(argument))
	opaques.insert(argQid->getIdIndex());
    }
  else
    return false;
  return true;
}

ModelCheckerSymbol::SystemAutomaton::~SystemAutomaton()
{
  delete systemStates;
}

ModelCheckerSymbol::StrategyAutomaton::~StrategyAutomaton()
{
  delete systemStates;
}

int
ModelCheckerSymbol::SystemAutomaton::getNextState(int stateNr, int transitionNr)
{
  int n = systemStates->getNextState(stateNr, transitionNr);
  if (n == NONE && transitionNr == 0)
    return stateNr; // fake a self loop for deadlocked state
  return n;
}

int
ModelCheckerSymbol::StrategyAutomaton::getNextState(int stateNr, int transitionNr)
{
  return systemStates->getNextState(stateNr, transitionNr);
}

bool
ModelCheckerSymbol::BaseAutomaton::checkPropositionAux(DagNode* state, DagNode* proposition) const
{
  Vector<DagNode*> args(2);
  args[0] = state;
  args[1] = proposition;
  RewritingContext* testContext =
    parentContext->makeSubcontext(satisfiesSymbol->makeDagNode(args));
  testContext->reduce();
  bool result = trueTerm->equal(testContext->root());
  parentContext->addInCount(*testContext);
  delete testContext;
  return result;
}

bool
ModelCheckerSymbol::SystemAutomaton::checkProposition(int stateNr, int propositionIndex) const
{
  return checkPropositionAux(systemStates->getStateDag(stateNr),
			     propositions.index2DagNode(propositionIndex));
}

bool
ModelCheckerSymbol::StrategyAutomaton::checkProposition(int stateNr, int propositionIndex) const
{
  DagNode* stateDag = systemStates->getStateDag(stateNr);

  //
  // System automaton states represent a term and a point in the strategy
  // execution. Hence multiple distinct states an share a common State term.
  // To reduce properties only once per term instead of once per automaton
  // state, we introduce (another) proposition cache.
  //

  #ifndef NO_SMC_2NDCACHE
  PropositionCache::const_iterator cached = propositionCache.find({stateDag, propositionIndex});
  if (cached != propositionCache.end())
    return cached->second;
  #endif

  bool result = checkPropositionAux(stateDag, propositions.index2DagNode(propositionIndex));

  #ifndef NO_SMC_2NDCACHE
  propositionCache[{stateDag, propositionIndex}] = result;
  #endif

  return result;
}
