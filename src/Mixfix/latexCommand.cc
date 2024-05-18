/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2024 SRI International, Menlo Park, CA 94025, USA.

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

void
MaudeLatexBuffer::generateModifiers(Module* module, int64_t  number, int64_t  number2)
{
  if (number != NONE || number2 != NONE)
    {
      output << "\\maudePunctuation{[}";
      if (number != NONE)
	output << "\\maudeNumber{" << number << "}";
      if (number2 != NONE)
	output << "\\maudePunctuation{,}\\maudeSpace\\maudeNumber{" << number2 << "}";
      output << "\\maudePunctuation{]}\\maudeSpace";
    }
  output << "\\maudeKeyword{in}\\maudeSpace\\maudeModule{" << Token::latexName(module->id()) <<
    "}\\maudeSpace\\maudePunctuation{:}\\maudeSpace\n";
}


void
MaudeLatexBuffer::generateGetVariants(bool showCommand,
				      bool irredundant,
				      DagNode* dag,
				      Vector<Term*> constraint,
				      int64_t limit,
				      bool debug)
{
  Module* module = dag->symbol()->getModule();
  string command = debug ? "debug get " : "get ";
  if (irredundant)
    command += "irredundant ";
  command += "variants";
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit);
  commentDagNode(dag);
  if (!constraint.empty())
    {
      output << " such that ";
      const char* sep = "";
      for (Term* t : constraint)
	{
	  output << sep;
	  commentTerm(t);
	  sep = ", ";
	}
      output << " irreducible" << endl;
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, limit);
      MixfixModule::latexPrintDagNode(output, dag);
      if (!constraint.empty())
	{
	  output << "\n\\maudeSpace\\maudeKeyword{such that}\\maudeSpace";
	  const char* sep = "";
	  for (Term* t : constraint)
	    {
	      output << sep;
	      sep = "\\maudePunctuation{,}";
	      MixfixModule::latexPrettyPrint(output, t);
	    }
	  output << "\\maudeSpace\\maudeKeyword{irreducible}";
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateVariantMatch(bool showCommand,
				       Vector<Term*> lhs,
				       Vector<Term*> rhs,
				       Vector<Term*> constraint,
				       int64_t limit,
				       bool debug)
{
  Index nrPairs = lhs.size();
  Module* module = lhs[0]->symbol()->getModule();
  string command = debug ? "debug " : "";
  command += "variant match";
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit);
  for (Index i = 0; i < nrPairs; ++i)
    {
      commentTerm(lhs[i]);
      output << " <=? ";
      commentTerm(rhs[i]);
      output << ((i == nrPairs - 1) ? "" : " /\\ ");
    }
  if (!constraint.empty())
    {
      output << " such that ";
      const char* sep = "";
      for (Term* t : constraint)
	{
	  output << sep;
	  commentTerm(t);
	  sep = ", ";
	}
      output << " irreducible" << endl;
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, limit);
      for (Index i = 0; i < nrPairs; ++i)
	{
	  MixfixModule::latexPrettyPrint(output, lhs[i]);
	  output << "\\maudeMatch";
	  MixfixModule::latexPrettyPrint(output, rhs[i]);
	  if (i < nrPairs - 1)
	    output << "\\maudeConjunction\n";
	}
      if (!constraint.empty())
	{
	  output << "\n\\maudeSpace\\maudeKeyword{such that}";
	  const char* sep = "\\maudeSpace";
	  for (Term* t : constraint)
	    {
	      output << sep;
	      sep = "\\maudePunctuation{,}";
	      MixfixModule::latexPrettyPrint(output, t);
	    }
	  output << "\\maudeSpace\\maudeKeyword{irreducible}";
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateVariantUnify(bool showCommand,
				       bool filtered,
				       Vector<Term*> lhs,
				       Vector<Term*> rhs,
				       Vector<Term*> constraint,
				       int64_t limit,
				       bool debug)
{
  Index nrPairs = lhs.size();
  Module* module = lhs[0]->symbol()->getModule();
  string command = debug ? "debug " : "";
  if (filtered)
    command += "filtered ";
  command += "variant unify";
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit);
  for (Index i = 0; i < nrPairs; ++i)
    {
      commentTerm(lhs[i]);
      output << " =? ";
      commentTerm(rhs[i]);
      output << ((i == nrPairs - 1) ? "" : " /\\ ");
    }
  if (!constraint.empty())
    {
      output << " such that ";
      const char* sep = "";
      for (Term* t : constraint)
	{
	  output << sep;
	  commentTerm(t);
	  sep = ", ";
	}
      output << " irreducible" << endl;
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, limit);
      for (Index i = 0; i < nrPairs; ++i)
	{
	  MixfixModule::latexPrettyPrint(output, lhs[i]);
	  output << "\\maudeUnify";
	  MixfixModule::latexPrettyPrint(output, rhs[i]);
	  if (i < nrPairs - 1)
	    output << "\\maudeConjunction\n";
	}
      if (!constraint.empty())
	{
	  output << "\n\\maudeSpace\\maudeKeyword{such that}";
	  const char* sep = "\\maudeSpace";
	  for (Term* t : constraint)
	    {
	      output << sep;
	      sep = "\\maudePunctuation{,}";
	      MixfixModule::latexPrettyPrint(output, t);
	    }
	  output << "\\maudeSpace\\maudeKeyword{irreducible}";
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateUnify(bool showCommand,
				bool irredundant,
			        Vector<Term*> lhs,
				Vector<Term*> rhs,
				int64_t limit)
{
  Index nrPairs = lhs.size();
  Module* module = lhs[0]->symbol()->getModule();
  const char* command = irredundant ? "irredundant unify" : "unify";
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit);
  for (Index i = 0; i < nrPairs; ++i)
    {
      commentTerm(lhs[i]);
      output << " =? ";
      commentTerm(rhs[i]);
      output << ((i == nrPairs - 1) ? "" : " /\\ ");
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, limit);
      for (Index i = 0; i < nrPairs; ++i)
	{
	  MixfixModule::latexPrettyPrint(output, lhs[i]);
	  output << "\\maudeUnify";
	  MixfixModule::latexPrettyPrint(output, rhs[i]);
	  if (i < nrPairs - 1)
	    output << "\\maudeConjunction";
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push(string("\\end{maudeResultParagraph}\n%\n%  End of ") + command + "\n%\n");
}

void
MaudeLatexBuffer::generateMatch(bool showCommand,
				bool withExtension,
			        Term* pattern,
				DagNode* subject,
				const Vector<ConditionFragment*>& condition,
				int64_t limit)
{
  Module* module = subject->symbol()->getModule();
  const char* command = withExtension ? "xmatch" : "match";
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit);
  commentTerm(pattern);
  output << " <=? ";
  commentDagNode(subject);
  if (!condition.empty())
    {
      output << " such that ";
      MixfixModule::printCondition(output, condition, commentSettings);
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, limit);
      MixfixModule::latexPrettyPrint(output, pattern);
      output << "\\maudeMatch";
      MixfixModule::latexPrintDagNode(output, subject);
      if (!condition.empty())
	{
	  output << "\\maudeSpace\\maudeKeyword{such that}\\maudeSpace";
	  VisibleModule::latexPrintCondition(output, condition);
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push(string("\\end{maudeResultParagraph}\n%\n%  End of ") + command + "\n%\n");
}

void
MaudeLatexBuffer::generateSearch(bool showCommand,
				 Interpreter::SearchKind searchKind,
				 const Vector<DagNode*>& subjects,
				 int searchType,
				 Term* target,
				 const Vector<ConditionFragment*>& condition,
				 int variantFlags,
				 int64_t limit,
				 int64_t depth,
				 bool debug)
{
  //
  //	These commands are for the form:
  //	  <command and options> in <module> : <subjects> <searchType> <target> .
  //
  static const char* searchKindName[] = { "search", "narrow", "xg-narrow", "smt-search", "vu-narrow", "fvu-narrow"};
  static const char* searchTypeSymbol[] = { "=>1", "=>+", "=>*", "=>!", "=>#" };
  static const char* searchTypeLatex[] = { "\\maudeOneStep", "\\maudeAtLeastOneStep", "\\maudeAnySteps", "\\maudeToNormalForm", "\\maudeToBranch"};

  Module* module = subjects[0]->symbol()->getModule();
  //
  //	Print comment.
  //
  startComment();
  if (debug)
    output << "debug ";
  if (variantFlags & (NarrowingSequenceSearch3::FOLD | NarrowingSequenceSearch3::VFOLD | NarrowingSequenceSearch3::KEEP_PATHS))
    {
      const char* sep = ", ";
      if (variantFlags & NarrowingSequenceSearch3::FOLD)
	output << "{fold";
      else if (variantFlags & NarrowingSequenceSearch3::VFOLD)
	output << "{vfold";
      else
	sep = "{";
      if (variantFlags & NarrowingSequenceSearch3::KEEP_PATHS)
	output << sep << "path";
      output << "} ";
    }
  if (variantFlags & NarrowingSequenceSearch3::FOLD)
    output << "{fold} ";
  output << searchKindName[searchKind] << ' ';
  if (variantFlags & (VariantSearch::IRREDUNDANT_MODE | VariantUnificationProblem::FILTER_VARIANT_UNIFIERS))
    {
      output << '{';
      const char* sep = "";
      if (variantFlags & VariantSearch::IRREDUNDANT_MODE)
	{
	  output << "delay";
	  sep = ", ";
	}
      if (variantFlags & VariantUnificationProblem::FILTER_VARIANT_UNIFIERS)
	output << sep << "filter";
      output << "} ";
    }
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, limit, depth);
  {
    const char* sep = "";
    for (DagNode* d : subjects)
      {
	output << sep << d;
	sep = " \\/ ";
      }
  }
  output << ' ' << searchTypeSymbol[searchType] << ' ';
  commentTerm(target);
  if (!condition.empty())
    {
      output << " such that ";
      MixfixModule::printCondition(output, condition, commentSettings);
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$";
      if (debug)
	output << "\\maudeKeyword{debug}\\maudeSpace";
      if (variantFlags & (NarrowingSequenceSearch3::FOLD | NarrowingSequenceSearch3::VFOLD | NarrowingSequenceSearch3::KEEP_PATHS))
	{
	  const char* sep = "\\maudePunctuation{,}\\maudeSpace";
	  if (variantFlags & NarrowingSequenceSearch3::FOLD)
	    output << "\\maudeLeftBrace\\maudeKeyword{fold}";
	  else if (variantFlags & NarrowingSequenceSearch3::VFOLD)
	    output << "\\maudeLeftBrace\\maudeKeyword{vfold}";
	  else
	    sep = "\\maudeLeftBrace";
	  if (variantFlags & NarrowingSequenceSearch3::KEEP_PATHS)
	    output << sep << "\\maudeKeyword{path}";
	  output << "\\maudeRightBrace\\maudeSpace";
	}

      output << "\\maudeKeyword{" << searchKindName[searchKind] << "}\\maudeSpace";

      if (variantFlags & (VariantSearch::IRREDUNDANT_MODE | VariantUnificationProblem::FILTER_VARIANT_UNIFIERS))
	{
	  output << "\\maudeLeftBrace";
	  const char* sep = "";
	  if (variantFlags & VariantSearch::IRREDUNDANT_MODE)
	    {
	      output << "\\maudeKeyword{delay}";
	      sep = "\\maudePunctuation{,}\\maudeSpace";
	    }
	  if (variantFlags & VariantUnificationProblem::FILTER_VARIANT_UNIFIERS)
	    output << sep << "\\maudeKeyword{filter}";
	  output << "\\maudeRightBrace\\maudeSpace";
	}
      generateModifiers(module, limit, depth);
      {
	const char* sep = "";
	for (DagNode* d : subjects)
	  {
	    output << sep;
	    MixfixModule::latexPrintDagNode(output, d);
	    sep = "\\maudeDisjunction";
	  }
      }	
      output << searchTypeLatex[searchType];
      MixfixModule::latexPrettyPrint(output, target);

      if (!condition.empty())
	{
	  output << "\\maudeSpace\\maudeKeyword{such that}\\maudeSpace";
	  VisibleModule::latexPrintCondition(output, condition);
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;
  pendingCloseStack.push(string("\\end{maudeResultParagraph}\n%\n%  End of ") + searchKindName[searchKind] + "\n%\n");
}

void
MaudeLatexBuffer::generateLoopTokens(bool showCommand, const Vector<Token>& tokens)
{
  //
  //	Print comment.
  //
  output << "%  loop tokens: (";
  int nrTokens = tokens.size();  // might be 0 so must be signed
  Token::printTokenVector(output, tokens, 0, nrTokens - 1, true);
  output << ")\n%\n";
  //
  //	Print latex version.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{\\maudeLeftParen}" <<
	MixfixModule::latexTokenVector(tokens, 0, nrTokens - 1) <<
	"\\maudeKeyword{\\maudeRightParen}$\n";
    }
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of loop execution\n%\n");
}

void
MaudeLatexBuffer::generateCommand(bool showCommand, const string& command, Term* subject)
{
  Module* module = subject->symbol()->getModule();
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output);
  commentTerm(subject);
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module);
      MixfixModule::latexPrettyPrint(output, subject);
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;  // for results that need a newline after the command
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateCommand(bool showCommand,
				  const string& command,
				  DagNode* subject,
				  Int64 number,
				  Int64 number2,
				  StrategyExpression* strategy)
{
  Module* module = subject->symbol()->getModule();
  //
  //	Print comment.
  //
  startComment();
  output << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, number, number2);
  commentDagNode(subject);
  if (strategy != nullptr)
    {
      output << " using ";
      MixfixModule::prettyPrint(output, strategy, UNBOUNDED, commentSettings);
    }
  endComment();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModifiers(module, number, number2);
      MixfixModule::latexPrintDagNode(output, subject);
      if (strategy != nullptr)
	{
	  output << "\\maudeSpace\\maudeKeyword{using}\\maudeSpace";
	  safeCastNonNull<const VisibleModule*>(subject->symbol()->getModule())->latexPrintStrategy(output, strategy);
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;  // for results that need a newline after the command
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateContinue(bool showCommand, Int64 limit, bool debug)
{
  string command = debug ? "debug continue" : "continue";
  startComment();
  output << command << " " << limit;
  endComment();
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    output << "\\maudeKeyword{" << command << "} \\maudeNumber{" << limit << "}\\maudeEndCommand\n";
  //
  //	We might be continuing a command that separates solutions by a blank line.
  //
  needNewline = showCommand;
  pendingCloseStack.push("\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateShow(bool showCommand, const string& command, NamedEntity* module)
{
  startComment();
  output << command << " " << module;
  endComment();
  output << "\\begin{maudeShowParagraph}";
  if (showCommand)
    {
      output << "\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModuleName(module);
      output << "\\maudeEndCommand\\maudeShowSpace\n";
    }
  pendingCloseStack.push("\\end{maudeShowParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateShow(bool showCommand, const string& command, View* view)
{
  startComment();
  output << command << " " << view;
  endComment();
  output << "\\begin{maudeShowParagraph}";
  if (showCommand)
    output << "\\maudeKeyword{" << command << "}\\maudeSpace\\maudeView{" << view << "}\\maudeEndCommand\\maudeShowSpace\n";
  pendingCloseStack.push("\\end{maudeShowParagraph}\n%\n%  End of " + command + "\n%\n");
}

void
MaudeLatexBuffer::generateShow(bool showCommand, const string& command)
{
  startComment();
  output << command;
  endComment();
  output << "\\begin{maudeShowParagraph}";
  if (showCommand)
    output << "\\maudeKeyword{" << command << "}\\maudeEndCommand\\maudeShowSpace\n";
  pendingCloseStack.push("\\end{maudeShowParagraph}\n%\n%  End of " + command + "\n%\n");
}
