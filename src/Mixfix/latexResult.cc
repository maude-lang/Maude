/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Code for printing results.
//
void
MaudeLatexBuffer::generateDecisionTime(int64_t cpuTime, int64_t realTime)
{
  output << "\\par\\maudeResponse{Decision time:} \\maudeNumber{" << cpuTime / 1000 <<
    "} \\maudeResponse{ms cpu} \\maudePunctuation{(}\\maudeNumber{" <<
    realTime / 1000 << "} \\maudeResponse{ms real}\\maudePunctuation{)}";
}

void
MaudeLatexBuffer::generateNonResult(const string& message)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{" << message << "}\n";
  needNewline = false;
}

void
MaudeLatexBuffer::generateResult(const string& message, int64_t solutionNr)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{" << message << "}\\maudeSpace\\maudeNumber{" << solutionNr << "}\n";
  needNewline = true;
}

void
MaudeLatexBuffer::generateMatchResult(MatchSearchState* state, int64_t matchNr)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{Matcher}\\maudeSpace\\maudeNumber{" << matchNr << "}\n";
  needNewline = true;
  ExtensionInfo* extensionInfo = state->getExtensionInfo();
  if (extensionInfo != 0)
    {
      output << "\\par\\maudeResponse{Matched portion =}\\maudeSpace";
      if (extensionInfo->matchedWhole())
	output << "\\maudeMisc{(whole)}\n";
      else
	{
	  output << "$";
	  MixfixModule::latexPrintDagNode(output, extensionInfo->buildMatchedPortion());
	  output << "$\n";
	}
    }
}

void
MaudeLatexBuffer::generateStats(RewritingContext& context,
				int64_t cpuTime,
				int64_t realTime,
				bool showTiming,
				bool showBreakdown,
				int64_t nrStates)
{
  //
  //	Rewrites.
  //
  Int64 nrRewrites = context.getTotalCount();
  output << "\\par";
  if (nrStates != NONE)
    output << "\\maudeResponse{states:} \\maudeNumber{" << nrStates << "}\\maudeSpace\\maudeSpace";
  output << "\\maudeResponse{rewrites:} \\maudeNumber{" << nrRewrites << "}\n";
  //
  //	Timing.
  //
  if (showTiming)
    {
      output << " \\maudeResponse{in} \\maudeNumber{" << cpuTime / 1000 <<
	"} \\maudeResponse{ms cpu} \\maudePunctuation{(}\\maudeNumber{" <<
	realTime / 1000 << "} \\maudeResponse{ms real}\\maudePunctuation{)} \\maudePunctuation{(}\\maudeNumber{";
      if (cpuTime > 0)
	output << (1000000 * nrRewrites) / cpuTime;
      else
	output << "\\textasciitilde";
      output << "} \\maudeResponse{rewrites/second}\\maudePunctuation{)}\n";
    }
  //
  //	Break down of rewrites.
  //
  if (showBreakdown)
    {
      output << "\\par\\maudeResponse{mb applications:} \\maudeNumber{" << context.getMbCount() << "}\n" <<
	"\\hspace{0.5em}\\maudeResponse{equational rewrites:} \\maudeNumber{" << context.getEqCount() << "}\n" <<
	"\\hspace{0.5em}\\maudeResponse{rule rewrites:} \\maudeNumber{" << context.getRlCount() << "}\n" <<
	"\\hspace{0.5em}\\maudeResponse{variant narrowing steps:} \\maudeNumber{" << context.getVariantNarrowingCount() << "}\n" <<
	"\\hspace{0.5em}\\maudeResponse{narrowing steps:} \\maudeNumber{" << context.getNarrowingCount() << "}\n";
    }
}

void
MaudeLatexBuffer::generateNonResult(RewritingContext& context,
				    const string& message,
				    int64_t cpuTime,
				    int64_t realTime,
				    bool showStats,
				    bool showTiming,
				    bool showBreakdown)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{" << message << "}\n";
  if (showStats)
    generateStats(context, cpuTime, realTime, showTiming, showBreakdown);
  needNewline = false;
}

void
MaudeLatexBuffer::generateResult(RewritingContext& context,
				 DagNode* result,
				 int64_t cpuTime,
				 int64_t realTime,
				 bool showStats,
				 bool showTiming,
				 bool showBreakdown)
{
  if (showStats)
    generateStats(context, cpuTime, realTime, showTiming, showBreakdown);
  output << "\\par\\maudeResponse{result}\n";
  generateType(result->getSort());
  output << "\\maudePunctuation{:}$\\maudeSpace\n";
  MixfixModule::latexPrintDagNode(output, result);
  output << "$\n";
}

void
MaudeLatexBuffer::generateResult(Term* result)
{
  output << "\\par";
  generateType(result->getSort());
  output << "\\maudePunctuation{:}$\\maudeSpace\n";
  MixfixModule::latexPrettyPrint(output, result);
  output << "$\n";
}

void
MaudeLatexBuffer::generateResult(const string& message, DagNode* result)
{
  output << "\\par\\maudeResponse{" << message << "}\n";
  generateType(result->getSort());
  output << "\\maudePunctuation{:}$\\maudeSpace\n";
  MixfixModule::latexPrintDagNode(output, result);
  output << "$\n";
}

void
MaudeLatexBuffer::generateBubbleResult(const Vector<int>& bubble)
{
  if (!bubble.empty())
    {
      output << "\\par$";
      MixfixModule::latexPrintBubble(output, bubble);
      output << "$\n";
    }
}

void
MaudeLatexBuffer::generateSearchNonResult(RewriteSequenceSearch* state,
					  const string& message,
					  int64_t cpuTime,
					  int64_t realTime,
					  bool showStats,
					  bool showTiming,
					  bool showBreakdown)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{" << message << "}\n";
  if (showStats)
    generateStats(*(state->getContext()), cpuTime, realTime, showTiming, showBreakdown, state->getNrStates());
  needNewline = false;
}

void
MaudeLatexBuffer::generateSmtResult(SMT_RewriteSequenceSearch* state,
				    int64_t solutionNr,
				    int64_t cpuTime,
				    int64_t realTime,
				    bool showStats,
				    bool showTiming,
				    bool showBreakdown)
{
  generateSolutionNr(solutionNr);
  if (showStats)
    generateStats(*(state->getContext()), cpuTime, realTime, showTiming, showBreakdown);
  int stateNr = state->getCurrentStateNumber();
  DagNode* d = state->getState(stateNr);

  output << "\\par\\maudeResponse{state:}\n";
  MixfixModule::latexPrintDagNode(output, d);

  generateSubstitution(*(state->getSubstitution()),
		       *state,
		       state->getSMT_VarIndices());

  output << "\\par\\maudeResponse{where}\n";
  MixfixModule::latexPrintDagNode(output, state->getFinalConstraint());
}

void
MaudeLatexBuffer::generateSearchResult(RewriteSequenceSearch* state,
				       int64_t solutionNr,
				       int64_t cpuTime,
				       int64_t realTime,
				       bool showStats,
				       bool showTiming,
				       bool showBreakdown)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{Solution}\\maudeSpace\\maudeNumber{" << solutionNr <<
    "}\\maudeSpace\\maudePunctuation{(}\\maudeResponse{state}\\maudeSpace\\maudeNumber{" <<
    state->getStateNr() << "}\\maudePunctuation{)}\n";
  if (showStats)
    generateStats(*(state->getContext()), cpuTime, realTime, showTiming, showBreakdown, state->getNrStates());
  needNewline = true;
}

void
MaudeLatexBuffer::generateSearchPath(const RewriteSequenceSearch* graph,
				     const Vector<int>& steps,
				     int stateNr,
				     bool showCommand,
				     bool showRule)
{
  const char* command = (showRule ? "show path" : "show path state" );
  //
  //	Print comment.
  //
  startComment();
  output << command << " " << stateNr;
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    output << "\\par\\maudeKeyword{" << command << "}\\maudeSpace\\maudeNumber{" << stateNr << "}\\maudeEndCommand\\newline\n";
  //
  //	Print latex version of path.
  //
  const VisibleModule* m = safeCastNonNull<VisibleModule*>(graph->getStateDag(0)->symbol()->getModule());
  for (Index i = steps.size() - 1; i >= 0; --i)
    {
      int sn = steps[i];
      if (sn != 0)  // is this the initial state?
	{
	  Rule* rule = graph->getStateRule(sn);
	  if (showRule)
	    {
	      output << "\\par$\\maudePathLeft";
	      m->latexPrintRule(output, nullptr, rule);
	      output << "\\maudePathRight$\n";
	    }
	  else
	    {
	      int label = rule->getLabel().id();
	      if (label != NONE)
		output <<"\\par$\\maudePathWithLabel{\\maudeLabel{" << Token::latexName(label) << "}}$\n";
	      else
		output <<"\\par$\\maudePathWithoutLabel$\n";
	    }
	}
      DagNode* d = graph->getStateDag(sn);
      output << "\\par\\maudeResponse{state}\\maudeSpace\\maudeNumber{" << sn << "}\\maudePunctuation{,}\\maudeSpace";
      generateType(d->getSort());
      output << "\\maudePunctuation{:}$\\maudeSpace\n";
      MixfixModule::latexPrintDagNode(output, d);
      output << "$\n";
    }
  output <<"\\end{maudeResultParagraph}\n%\n%  End of show path\n%\n";    
}

void
MaudeLatexBuffer::generateSearchPathLabels(const RewriteSequenceSearch* graph,
					   const Vector<int>& steps,
					   int stateNr,
					   bool showCommand)
{
  //
  //	Print comment.
  //
  startComment();
  output << "show path labels " << stateNr;
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    output << "\\par\\maudeKeyword{show path labels}\\maudeSpace\\maudeNumber{" << stateNr << "}\\maudeEndCommand\\newline\n";
  //
  //	Print latex version of labels.
  //
  Index nrStates = steps.size();
  if (nrStates < 2)
    output << "\\par\\maudeResponse{Empty path.}\n";
  else
    {
      for (Index i = nrStates - 2; i >= 0; --i)  // skip the initial state
	{
	  const int label = graph->getStateRule(steps[i])->getLabel().id();
	  if (label == NONE)
	    output << "\\par\\maudeMisc{(unlabeled rule)}\n";
	  else
	    output << "\\par\\maudeLabel{" << Token::latexName(label) << "}\n";
	}
    }
  output <<"\\end{maudeResultParagraph}\n%\n%  End of show path labels\n%\n";    
}

void
MaudeLatexBuffer::generateSearchGraph(const RewriteSequenceSearch* graph, bool showCommand)
{
  //
  //	Print comment.
  //
  startComment();
  output << "show graph";
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    output << "\\par\\maudeKeyword{show graph}\\maudeEndCommand\\newline\n";
  //
  //	Print latex version of graph.
  //
  const VisibleModule* m = safeCastNonNull<VisibleModule*>(graph->getStateDag(0)->symbol()->getModule());
  int nrStates = graph->getNrStates();
  for (int i = 0; i < nrStates; ++i)
    {
      if (i != 0)
	output << "\\newline\n";
      DagNode* d = graph->getStateDag(i);
      output << "\\par\\maudeResponse{state}\\maudeSpace\\maudeNumber{" << i << "}\\maudePunctuation{,}\\maudeSpace";
	generateType(d->getSort());
      output << "\\maudePunctuation{:}$\\maudeSpace\n";
      MixfixModule::latexPrintDagNode(output, d);
      output << "$\n";
      
      int arcNr = 0;
      for (auto& j : graph->getStateFwdArcs(i))
	{
	  output << "\\par$\\maudeResponse{arc}\\maudeSpace\\maudeNumber{" << arcNr <<
	    "}\\maudeArcArrow\\maudeResponse{state}\\maudeSpace\\maudeNumber{" << j.first << "}$\n";
	  for (Rule* k : j.second)
	    {
	      output << "$\\maudeSpace\\maudeLeftParen";
	      m->latexPrintRule(output, nullptr, k);
	      output << "\\maudeRightParen$\n";
	    }
	  ++arcNr;
	}
    }
  output <<"\\end{maudeResultParagraph}\n%\n%  End of show graph\n%\n";    
}
