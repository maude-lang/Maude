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
//      Implementation for class StrategyModelCheckerSymbol.
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
#include "strategyModelCheckerSymbol.hh"

StrategyModelCheckerSymbol::StrategyModelCheckerSymbol(int id)
  : TemporalSymbol(id, 5)
{
  satisfiesSymbol = nullptr;
  qidSymbol = nullptr;
  unlabeledSymbol = nullptr;
  solutionSymbol = nullptr;
  opaqueSymbol = nullptr;
  transitionSymbol = nullptr;
  transitionListSymbol = nullptr;
  nilTransitionListSymbol = nullptr;
  counterexampleSymbol = nullptr;
}

bool
StrategyModelCheckerSymbol::attachData(const Vector<Sort*>& opDeclaration,
			       const char* purpose,
			       const Vector<const char*>& data)
{
  NULL_DATA(purpose, StrategyModelCheckerSymbol, data);
  return TemporalSymbol::attachData(opDeclaration, purpose, data);
}

bool
StrategyModelCheckerSymbol::attachSymbol(const char* purpose, Symbol* symbol)
{
  BIND_SYMBOL(purpose, symbol, satisfiesSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, qidSymbol, QuotedIdentifierSymbol*);
  BIND_SYMBOL(purpose, symbol, unlabeledSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, solutionSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, opaqueSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, transitionSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, transitionListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, nilTransitionListSymbol, Symbol*);
  BIND_SYMBOL(purpose, symbol, counterexampleSymbol, Symbol*);
  return TemporalSymbol::attachSymbol(purpose, symbol);
}

bool
StrategyModelCheckerSymbol::attachTerm(const char* purpose, Term* term)
{
  BIND_TERM(purpose, term, trueTerm);
  return TemporalSymbol::attachTerm(purpose, term);
}

void
StrategyModelCheckerSymbol::copyAttachments(Symbol* original, SymbolMap* map)
{
  StrategyModelCheckerSymbol* orig = safeCast(StrategyModelCheckerSymbol*, original);
  COPY_SYMBOL(orig, satisfiesSymbol, map, Symbol*);
  COPY_SYMBOL(orig, qidSymbol, map, QuotedIdentifierSymbol*);
  COPY_SYMBOL(orig, unlabeledSymbol, map, Symbol*);
  COPY_SYMBOL(orig, solutionSymbol, map, Symbol*);
  COPY_SYMBOL(orig, opaqueSymbol, map, Symbol*);
  COPY_SYMBOL(orig, transitionSymbol, map, Symbol*);
  COPY_SYMBOL(orig, transitionListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, nilTransitionListSymbol, map, Symbol*);
  COPY_SYMBOL(orig, counterexampleSymbol, map, Symbol*);

  COPY_TERM(orig, trueTerm, map);
  TemporalSymbol::copyAttachments(original, map);
}

void
StrategyModelCheckerSymbol::getDataAttachments(const Vector<Sort*>& opDeclaration,
				       Vector<const char*>& purposes,
				       Vector<Vector<const char*> >& data)
{
  APPEND_DATA(purposes, data, StrategyModelCheckerSymbol);
  TemporalSymbol::getDataAttachments(opDeclaration, purposes, data);
}

void
StrategyModelCheckerSymbol::getSymbolAttachments(Vector<const char*>& purposes,
					 Vector<Symbol*>& symbols)
{
  APPEND_SYMBOL(purposes, symbols, satisfiesSymbol);
  APPEND_SYMBOL(purposes, symbols, qidSymbol);
  APPEND_SYMBOL(purposes, symbols, unlabeledSymbol);
  APPEND_SYMBOL(purposes, symbols, solutionSymbol);
  APPEND_SYMBOL(purposes, symbols, opaqueSymbol);
  APPEND_SYMBOL(purposes, symbols, transitionSymbol);
  APPEND_SYMBOL(purposes, symbols, transitionListSymbol);
  APPEND_SYMBOL(purposes, symbols, nilTransitionListSymbol);
  APPEND_SYMBOL(purposes, symbols, counterexampleSymbol);
  TemporalSymbol::getSymbolAttachments(purposes, symbols);
}

void
StrategyModelCheckerSymbol::getTermAttachments(Vector<const char*>& purposes,
				       Vector<Term*>& terms)
{
  APPEND_TERM(purposes, terms, trueTerm);
  TemporalSymbol::getTermAttachments(purposes, terms);
}

void
StrategyModelCheckerSymbol::postInterSymbolPass()
{
  PREPARE_TERM(trueTerm);
  TemporalSymbol::postInterSymbolPass();
}

void
StrategyModelCheckerSymbol::reset()
{
  trueTerm.reset();  // so true dag can be garbage collected
  TemporalSymbol::reset();  // parents reset() tasks
}

void
StrategyModelCheckerSymbol::compileEquations()
{
  TemporalSymbol::compileEquations();
  setEqRewrite(&StrategyModelCheckerSymbol::eqRewrite);
}

DagNode*
StrategyModelCheckerSymbol::makeTransition(const StrategyTransitionGraph& states, int stateNr, int targetNr)
{
  static Vector<DagNode*> args(2);

  args[0] = states.getStateDag(stateNr);
  const StrategyTransitionGraph::ArcMap& fwdArcs = states.getStateFwdArcs(stateNr);
  StrategyTransitionGraph::ArcMap::const_iterator i = fwdArcs.find(targetNr);

  Assert(i != fwdArcs.end(), "missing transition information");

  const StrategyTransitionGraph::Transition &transition = *(i->second.begin());

  switch (transition.getType())
    {
      case StrategyTransitionGraph::RULE_APPLICATION :
	{
	  int label = transition.getRule()->getLabel().id();

	  args[1] = (label == NONE) ? unlabeledSymbol->makeDagNode() :
		      new QuotedIdentifierDagNode(qidSymbol, label);
	  break;
	}
      case StrategyTransitionGraph::OPAQUE_STRATEGY :
	{
	  Vector<DagNode*> args2(1);
	  int label = transition.getStrategy()->id();

	  args2[0] = new QuotedIdentifierDagNode(qidSymbol, label);
	  args[1] = opaqueSymbol->makeDagNode(args2);
	  break;
	}
      case StrategyTransitionGraph::SOLUTION :
      default :
	args[1] = solutionSymbol->makeDagNode();
    }

 return transitionSymbol->makeDagNode(args);
}

DagNode*
StrategyModelCheckerSymbol::makeTransitionList(const StrategyTransitionGraph& states,
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
StrategyModelCheckerSymbol::makeCounterexample(const StrategyTransitionGraph& states,
				       const ModelChecker2& mc)
{
  Vector<DagNode*> args(2);
  int junction = mc.getCycle().front();
  args[0] = makeTransitionList(states, mc.getLeadIn(), junction, true);
  args[1] = makeTransitionList(states, mc.getCycle(), junction, false);
  return counterexampleSymbol->makeDagNode(args);
}

bool
StrategyModelCheckerSymbol::eqRewrite(Symbol* symbol, DagNode* subject, RewritingContext& context)
{
  Assert(symbol == subject->symbol(), "bad symbol");
  StrategyModelCheckerSymbol* s = safeCastNonNull<StrategyModelCheckerSymbol*>(symbol);
  //  This is a hack; eventually we'll inline this function.
  return s->eqRewrite(subject, context);
}

bool
StrategyModelCheckerSymbol::eqRewrite(DagNode* subject, RewritingContext& context)
{
  Assert(this == subject->symbol(), "bad symbol");
  FreeDagNode* d = safeCast(FreeDagNode*, subject);
  //
  // Looks for the strategy with the given name (and without arguments) in the module.
  //
  StrategyExpression* strategy;
  {
    RewritingContext* auxContext = context.makeSubcontext(d->getArgument(2));
    auxContext->reduce();
    int strategyId = safeCast(QuotedIdentifierDagNode*, auxContext->root())->getIdIndex();
    context.addInCount(*auxContext);
    delete auxContext;

    RewriteStrategy* strat = nullptr;
    {
      const Vector<RewriteStrategy*> &strategies = subject->symbol()->getModule()->getStrategies();
      size_t nrStrats = strategies.length();
      for (size_t i = 0; i < nrStrats; i++)
	if (strategies[i]->id() == strategyId)
	  {
	    strat = strategies[i];
	    if (strategies[i]->arity() == 0) break;
	  }
    }

    if (strat == nullptr)
      {
	IssueWarning("no strategy named " << QUOTE(Token::name(strategyId)) << " in scope.");
	return tryEquations(subject, context);
      }
    if (strat->arity() != 0)
      {
	IssueWarning("the strategy given to " << QUOTE("modelCheck") << " must not have parameters.");
	return tryEquations(subject, context);
      }

    Vector<Term*> dummy;
    strategy = new CallStrategy(strat, strat->makeAuxiliaryTerm(dummy));
  }

  //
  //	Extracts the list of opaque functions
  //
  set<int> opaqueIds;
  {
    RewritingContext* auxContext = context.makeSubcontext(d->getArgument(3));
    auxContext->reduce();
    DagNode* opaqueList = auxContext->root();

    if (opaqueList->symbol() == qidSymbol) // singleton list
      opaqueIds.insert(safeCast(QuotedIdentifierDagNode*, opaqueList)->getIdIndex());

    else
      {
	for (DagArgumentIterator it(*opaqueList); it.valid(); it.next())
	  {
	    int id = safeCast(QuotedIdentifierDagNode*, it.argument())->getIdIndex();
	    opaqueIds.insert(id);
	  }
      }
    context.addInCount(*auxContext);
    delete auxContext;
  }
  bool useBiasedMatchrew = false;
  {
    RewritingContext* auxContext = context.makeSubcontext(d->getArgument(4));
    auxContext->reduce();
    if (auxContext->root()->equal(trueTerm.getDag()))
      useBiasedMatchrew = true;

    context.addInCount(*auxContext);
    delete auxContext;
  }

  //
  //	Compute normalization of negated formula.
  //
  RewritingContext* newContext = context.makeSubcontext(negate(d->getArgument(1)));
  newContext->reduce();
  #ifdef TDEBUG
  cout << "Negated formula: " << newContext->root() << endl;
  #endif
  //
  //	Convert Dag into a LogicFormula.
  //
  SystemAutomaton system;
  LogicFormula formula;
  int top = build(formula, system.propositions, newContext->root());
  if (top == NONE)
    {
      IssueAdvisory("negated LTL formula " << QUOTE(newContext->root()) <<
	            " did not reduce to a valid negative normal form.");
      delete strategy;
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
  system.satisfiesSymbol = satisfiesSymbol;
  system.parentContext = &context;
  system.trueTerm = trueTerm.getDag();
  RewritingContext* sysContext = context.makeSubcontext(d->getArgument(0));
  sysContext->reduce();
  system.systemStates = new StrategyTransitionGraph(sysContext, strategy, opaqueIds, useBiasedMatchrew);
  ModelChecker2 mc(system, formula, top);
  bool result = mc.findCounterexample();
  int nrSystemStates = system.systemStates->getNrStates();
  Verbose("StrategyModelCheckerSymbol: Examined " << nrSystemStates <<
	  " system state" << pluralize(nrSystemStates) << " (" <<
  system.systemStates->getNrRealStates() << " real).");
  #ifdef TDEBUG
  if (result == true)
    {
      dump(*(system.systemStates), mc.getLeadIn());
      dump(*(system.systemStates), mc.getCycle());
    }
  #endif
  delete newContext;
  DagNode* resultDag = result ? makeCounterexample(*(system.systemStates), mc)
			      : trueTerm.getDag();
  context.addInCount(*sysContext);
  delete system.systemStates;  // deletes sysContext via ~StateTransitionGraph()
  return context.builtInReplace(subject, resultDag);
}

int
StrategyModelCheckerSymbol::SystemAutomaton::getNextState(int stateNr, int transitionNr)
{
  return systemStates->getNextState(stateNr, transitionNr);
}

bool
StrategyModelCheckerSymbol::SystemAutomaton::checkProposition(int stateNr, int propositionIndex) const
{
  DagNode* stateDag = systemStates->getStateDag(stateNr);

  //
  // System automaton states represent a term and a point in the strategy
  // execution. Hence multiple distinct states an share a common State term.
  // To reduce properties only once per term instead of once per automaton
  // state, we introduce (another) proposition cache.
  //

  #ifndef NO_SMC_2NDCACHE
  PropositionCache::const_iterator cached = propositionCache.find(make_pair(stateDag, propositionIndex));
  if (cached != propositionCache.end())
    return cached->second;
  #endif

  Vector<DagNode*> args(2);
  args[0] = stateDag;
  args[1] = propositions.index2DagNode(propositionIndex);
  RewritingContext* testContext =
	parentContext->makeSubcontext(satisfiesSymbol->makeDagNode(args));
  testContext->reduce();
  bool result = trueTerm->equal(testContext->root());
  parentContext->addInCount(*testContext);
  delete testContext;
  #ifndef NO_SMC_2NDCACHE
  propositionCache[make_pair(stateDag, propositionIndex)] = result;
  #endif
  return result;
}
