/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for search command.
//

#include "equalityConditionFragment.hh"
#include "SMT_RewriteSequenceSearch.hh"
#include "narrowingSequenceSearch3.hh"
#include "narrowing.cc"
#include "smtSearch.cc"

bool
Interpreter::checkSearchRestrictions(SearchKind searchKind,
				     int searchType,
				     Term* target,				     
				     const Vector<ConditionFragment*>& condition,
				     MixfixModule* module)
{
  switch (searchKind)
    {
    case NARROW:
    case XG_NARROW:
    case VU_NARROW:
    case FVU_NARROW:
     {
	//
	//	Narrowing does not support conditions.
	//
	if (!condition.empty())
	  {
	    IssueWarning(*target << ": conditions are not currently supported for narrowing.");
	    return false;
	  }
	break;
      }
    case SMT_SEARCH:
      {
	//
	//	SMT search does not support =>! mode since states are symbolic.
	//
	if (searchType == SequenceSearch::NORMAL_FORM)
	  {
	     IssueWarning(*target << ": =>! mode is not supported for searching modulo SMT.");
	     return false;
	  }
	//
	//	Only equational condition fragments are supported since they need to
	//	pushed in to the SMT solver.
	//

	//
	//	Module must satisfy many restrictions.
	//
	if (!(module->validForSMT_Rewriting()))
	  {
	    IssueWarning(*module << ": module " << QUOTE(module) <<
			 " does not satisfy restrictions for rewriting modulo SMT.");
	    return false;
	  }
	//
	//	Target can't contain SMT operators.
	//
	if (Symbol* s = module->findSMT_Symbol(target))
	  {
	    IssueWarning(*target << ": pattern contains SMT symbol " << QUOTE(s) << ".");
	    return false;
	  }
	//
	//	Target can't contain nonlinear variables.
	//
	VariableInfo variableInfo;
	if (Term* v = MixfixModule::findNonlinearVariable(target, variableInfo))
	{
	  IssueWarning(*target << ": pattern contains a nonlinear variable " << QUOTE(v) << ".");
	  return false;
	}
	break;
      }
    default:
      break;
    }
  return true;
}
  
void
Interpreter::search(const Vector<Token>& bubble,
		    Int64 limit,
		    Int64 depth,
		    SearchKind searchKind,
		    bool debug,
		    int variantFlags)
{
  VisibleModule* fm = currentModule->getFlatModule();
  Term* initial;
  int searchType;
  Term* target;
  Vector<ConditionFragment*> condition;
  if (!(fm->parseSearchCommand(bubble, initial, searchType, target, condition)))
    return;

  if (!checkSearchRestrictions(searchKind, searchType, target, condition, fm))
    {
      initial->deepSelfDestruct();
      target->deepSelfDestruct();
      for (ConditionFragment* cf : condition)
	delete cf;
      return;
    }
  Pattern* pattern = (searchKind == VU_NARROW ||
		      searchKind == FVU_NARROW ||
		      searchKind == SMT_SEARCH) ? 0 :
    new Pattern(target, false, condition);
  //
  //	Regular seach cannot have unbound variables.
  //
  if (searchKind == SEARCH && !(pattern->getUnboundVariables().empty()))
    {
      IssueWarning(*target << ": variable " <<
		   QUOTE(pattern->index2Variable(pattern->getUnboundVariables().min())) <<
		   " is used before it is bound in the condition of a search command.\n");
      initial->deepSelfDestruct();
      delete pattern;
      return;
    }

  DagNode* subjectDag = makeDag(initial);

  static const char* searchTypeSymbol[] = { "=>1", "=>+", "=>*", "=>!" };
  bool showCommand = getFlag(SHOW_COMMAND);
  if (showCommand)
    {
      static const char* searchKindName[] =
	{ "search", "narrow", "xg-narrow",
	  "smt-search", "vu-narrow", "fvu-narrow"};

      UserLevelRewritingContext::beginCommand();
      if (debug)
	cout << "debug ";
      if (variantFlags & NarrowingSequenceSearch3::FOLD)
	cout << "{fold} ";
      cout << searchKindName[searchKind] << ' ';
      if (variantFlags &
	  (VariantSearch::IRREDUNDANT_MODE | VariantUnificationProblem::FILTER_VARIANT_UNIFIERS))
	{
	  cout << '{';
	  const char* sep = "";
	  if (variantFlags & VariantSearch::IRREDUNDANT_MODE)
	    {
	      cout << "delay";
	      sep = ", ";
	    }
	  if (variantFlags & VariantUnificationProblem::FILTER_VARIANT_UNIFIERS)
	    cout << sep << "filter";
	  cout << "} ";
	}
      printModifiers(limit, depth);
      cout << subjectDag << ' ' << searchTypeSymbol[searchType] << ' ' << target;
      if (!condition.empty())
	{
	  cout << " such that ";
	  MixfixModule::printCondition(cout, condition);	  
	}
      cout << " ." << endl;

      if (xmlBuffer != 0)
	xmlBuffer->generateSearch(subjectDag, pattern, searchTypeSymbol[searchType], limit, depth);  // does this work for narrowing?
    }
  if (latexBuffer != 0)
    {
      latexBuffer->generateSearch(showCommand,
				  searchKind,
				  subjectDag,
				  searchType,
				  target,
				  condition,
				  variantFlags,
				  limit,
				  depth,
				  debug);
    }

  startUsingModule(fm);
  if (debug)
    UserLevelRewritingContext::setDebug();
  QUANTIFY_START();
  
  if (searchKind == SEARCH)
    {
      RewriteSequenceSearch* state =
	new RewriteSequenceSearch(new UserLevelRewritingContext(subjectDag),
				  static_cast<RewriteSequenceSearch::SearchType>(searchType),
				  pattern,
				  depth);
      Timer timer(getFlag(SHOW_TIMING));
      doSearching(timer, fm, state, 0, limit);
    }
  else if (searchKind == SMT_SEARCH)
    {
      const SMT_Info& smtInfo = fm->getSMT_Info();
      VariableGenerator* vg = new VariableGenerator(smtInfo);
      RewritingContext* initial = new UserLevelRewritingContext(subjectDag);
      //
      //	SMT_RewriteSequenceSearch takes responsibility for deleting
      //	vg and initial.
      //
      SMT_RewriteSequenceSearch* smtSearch =
	new SMT_RewriteSequenceSearch(initial,
				      static_cast<RewriteSequenceSearch::SearchType>(searchType),
				      target,
				      condition,
				      smtInfo,
				      vg,
				      depth,
				      0);
       Timer timer(getFlag(SHOW_TIMING));
       doSmtSearch(timer, fm, smtSearch, 0, limit);
    }
  else if (searchKind == VU_NARROW || searchKind == FVU_NARROW)
    {
      //
      //	We don't really need to normalize but we do need to set hash values.
      //
      target = target->normalize(true);
      DagNode* goal = target->term2Dag();
      target->deepSelfDestruct();

      if (searchKind == FVU_NARROW)
	variantFlags |= NarrowingSequenceSearch3::FOLD;
      NarrowingSequenceSearch3* state =
	new NarrowingSequenceSearch3(new UserLevelRewritingContext(subjectDag),
				     static_cast<NarrowingSequenceSearch::SearchType>(searchType),  // HACK
				     goal,
				     depth,
				     new FreshVariableSource(fm),
				     variantFlags);
      Timer timer(getFlag(SHOW_TIMING));
      doVuNarrowing(timer, fm, state, 0, limit);
    }
  else
    {
      NarrowingSequenceSearch* state =
	new NarrowingSequenceSearch(new UserLevelRewritingContext(subjectDag),
				    static_cast<NarrowingSequenceSearch::SearchType>(searchType),  // HACK
				    pattern,
				    depth,
				    (searchKind == XG_NARROW ?
				     (NarrowingSearchState::ALLOW_NONEXEC |
				      NarrowingSearchState::SINGLE_POSITION) :
				     NarrowingSearchState::ALLOW_NONEXEC),
				    new FreshVariableSource(fm));
      Timer timer(getFlag(SHOW_TIMING));
      doNarrowing(timer, fm, state, 0, limit);
    }
}

void
Interpreter::doSearching(Timer& timer,
			 VisibleModule* module,
			 RewriteSequenceSearch* state,
			 Int64 solutionCount,
			 Int64 limit)
{
  const VariableInfo* variableInfo = state->getGoal();
  Int64 i = 0;
  for (; i != limit; i++)
    {
      bool result = state->findNextMatch();
      if (UserLevelRewritingContext::aborted())
	break;  // HACK: Is this safe - shouldn't we destroy context?
      Int64 real = 0;
      Int64 virt = 0;
      Int64 prof = 0;
      bool showTiming = getFlag(SHOW_TIMING) && timer.getTimes(real, virt, prof);
      bool showStats = getFlag(SHOW_STATS);
      if (!result)
	{
	  const char* reply = (solutionCount == 0) ? "No solution." : "No more solutions.";
	  cout << "\n" << reply << endl;
	  if (showStats)
	    printStats(*(state->getContext()), prof, real, showTiming, state->getNrStates());
	  if (xmlBuffer != 0)
	    {
	      xmlBuffer->generateSearchResult(NONE,
					      state,
					      timer,
					      getFlag(SHOW_STATS),
					      getFlag(SHOW_TIMING),
					      getFlag(SHOW_BREAKDOWN));
	    }
	  if (latexBuffer != 0)
	    {
	      latexBuffer->generateSearchNonResult(state,
						   reply,
						   prof,
						   real,
						   showStats,
						   showTiming,
						   getFlag(SHOW_BREAKDOWN));
	    }
	  break;
	}

      ++solutionCount;
      cout << "\nSolution " << solutionCount << " (state " << state->getStateNr() << ")\n";
      if (showStats)
	printStats(*(state->getContext()), prof, real, showTiming, state->getNrStates());
      UserLevelRewritingContext::printSubstitution(*(state->getSubstitution()), *variableInfo);
      if (xmlBuffer != 0)
	{
	  xmlBuffer->generateSearchResult(solutionCount,
					  state,
					  timer,
					  getFlag(SHOW_STATS),
					  getFlag(SHOW_TIMING),
					  getFlag(SHOW_BREAKDOWN));
	}
      if (latexBuffer != 0)
	{
	  latexBuffer->generateSearchResult(state,
					    solutionCount,
					    prof,
					    real,
					    showStats,
					    showTiming,
					    getFlag(SHOW_BREAKDOWN));
	  latexBuffer->generateSubstitution(*(state->getSubstitution()), *variableInfo);
	}
    }
  QUANTIFY_STOP();
  if (latexBuffer)
    latexBuffer->cleanUp();
  clearContinueInfo();  // just in case debugger left info
  //
  //	We always save these things even if we can't continue
  //	in order to allow inspection of the search graph.
  //
  savedState = state;
  savedModule = module;
  if (i == limit)  
    {
      //
      //	The loop terminated because we hit user's limit so 
      //	continuation is still possible. We save the state,
      //	solutionCount and module, and set a continutation function.
      //
      state->getContext()->clearCount();
      savedSolutionCount = solutionCount;
      continueFunc = &Interpreter::searchCont;
    }
  UserLevelRewritingContext::clearDebug();
}

void
Interpreter::searchCont(Int64 limit, bool debug)
{
  RewriteSequenceSearch* state = safeCast(RewriteSequenceSearch*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("search", fm, limit);
  if (latexBuffer)
    latexBuffer->generateContinue(getFlag(SHOW_COMMAND), limit, debug);

  if (debug)
    UserLevelRewritingContext::setDebug();

  QUANTIFY_START();
  Timer timer(getFlag(SHOW_TIMING));
  doSearching(timer, fm, state, savedSolutionCount, limit);
}

void
Interpreter::showSearchPath(int stateNr, bool showRule)
{
  RewriteSequenceSearch* savedRewriteSequenceSearch = dynamic_cast<RewriteSequenceSearch*>(savedState);
  if (savedRewriteSequenceSearch == 0)
    {
      IssueWarning("no state graph.");
      return;
    }
  if (stateNr < 0 || stateNr >= savedRewriteSequenceSearch->getNrStates())
    {
      IssueWarning("bad state number.");
      return;
    }
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateShowSearchPath(stateNr);
  Vector<int> steps;
  for (int i = stateNr; i != NONE; i = savedRewriteSequenceSearch->getStateParent(i))
    steps.append(i);

  for (int i = steps.length() - 1; i >= 0; i--)
    {
      int sn = steps[i];
      if (sn != 0)
	{
	  Rule* rule = savedRewriteSequenceSearch->getStateRule(sn);
	  if (showRule)
	    cout << "===[ " << rule << " ]===>\n";
	  else
	    {
	      const Label& l = rule->getLabel();
	      cout << "---";
	      if (l.id() != NONE)
		cout << ' ' << &l << ' ';
	      cout << "--->\n";
	    }
	}
      DagNode* d = savedRewriteSequenceSearch->getStateDag(sn);
      cout << "state " << sn << ", " << d->getSort() << ": " << d << '\n';
    }
  if (latexBuffer)
    {
      latexBuffer->generateSearchPath(savedRewriteSequenceSearch,
				      steps,
				      stateNr,
				      getFlag(SHOW_COMMAND),
				      showRule);
    }
  if (xmlBuffer != 0)
    xmlBuffer->generateSearchPath(savedRewriteSequenceSearch, stateNr);
}

void
Interpreter::showSearchPathLabels(int stateNr)
{
  RewriteSequenceSearch* savedRewriteSequenceSearch = dynamic_cast<RewriteSequenceSearch*>(savedState);
  if (savedRewriteSequenceSearch == 0)
    {
      IssueWarning("no state graph.");
      return;
    }
  if (stateNr < 0 || stateNr >= savedRewriteSequenceSearch->getNrStates())
    {
      IssueWarning("bad state number.");
      return;
    }
  Vector<int> steps;
  for (int i = stateNr; i != NONE; i = savedRewriteSequenceSearch->getStateParent(i))
    steps.append(i);

  int i = steps.length() - 2;
  if (i < 0)
    cout << "Empty path.\n";
  else
    { 
      for (; i >= 0; i--)
	{
	  const Label& label = savedRewriteSequenceSearch->getStateRule(steps[i])->getLabel();
	  if (label.id() == NONE)
	    cout << "(unlabeled rule)\n";
	  else
	    cout << &label << '\n';
	}
    }
  if (latexBuffer)
    {
      latexBuffer->generateSearchPathLabels(savedRewriteSequenceSearch,
					    steps,
					    stateNr,
					    getFlag(SHOW_COMMAND));
    }
}

void
Interpreter::showSearchGraph()
{
  RewriteSequenceSearch* savedRewriteSequenceSearch = dynamic_cast<RewriteSequenceSearch*>(savedState);
  if (savedRewriteSequenceSearch == 0)
    {
      IssueWarning("no state graph.");
      return;
    }

  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateShowSearchGraph();
  int nrStates = savedRewriteSequenceSearch->getNrStates();
  for (int i = 0; i < nrStates; i++)
    {
      if (i > 0)
	cout << '\n';
      DagNode* d = savedRewriteSequenceSearch->getStateDag(i);
      cout << "state " << i << ", " << d->getSort() << ": " << d << '\n';
      const RewriteSequenceSearch::ArcMap& fwdArcs =
	savedRewriteSequenceSearch->getStateFwdArcs(i);
      int arcNr = 0;
      for (RewriteSequenceSearch::ArcMap::const_iterator j = fwdArcs.begin();
	   j != fwdArcs.end(); j++, arcNr++)
	{
	  cout << "arc " << arcNr << " ===> state " << (*j).first;
	  const set<Rule*>& r = (*j).second;
	  for (set<Rule*>::const_iterator k = r.begin(); k != r.end(); k++)
	    cout << " (" << *k << ')';
	  cout << '\n';
	}
    }
  if (xmlBuffer != 0)
    xmlBuffer->generateSearchGraph(savedRewriteSequenceSearch);
  if (latexBuffer)
    latexBuffer->generateSearchGraph(savedRewriteSequenceSearch, getFlag(SHOW_COMMAND));
}
