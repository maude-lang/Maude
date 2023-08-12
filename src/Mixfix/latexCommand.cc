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

void
MaudeLatexBuffer::generateSearch(bool showCommand,
				 Interpreter::SearchKind searchKind,
				 DagNode* subject,
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
  //	  <command and options> in <module> : <subject> <searchType> <target> .
  //
  static const char* searchKindName[] = { "search", "narrow", "xg-narrow", "smt-search", "vu-narrow", "fvu-narrow"};
  static const char* searchTypeSymbol[] = { "=>1", "=>+", "=>*", "=>!" };
  static const char* searchTypeLatex[] = { "\\maudeOneStep", "\\maudeAtLeastOneStep", "\\maudeAnySteps", "\\maudeToNormalForm" };

  Module* module = subject->symbol()->getModule();
  //
  //	Print comment.
  //
  Tty::blockEscapeSequences();
  output << "\\begin{comment}\n%\n%  ";
  if (debug)
    output << "debug ";
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
  output << subject << ' ' << searchTypeSymbol[searchType] << ' ' << target;
  if (!condition.empty())
    {
      output << " such that ";
      MixfixModule::printCondition(output, condition);	  
    }
  output << " .\n%\n\\end{comment}\n";
  Tty::unblockEscapeSequences();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$";
      if (debug)
	output << "\\maudeKeyword{debug}\\maudeSpace";
      if (variantFlags & NarrowingSequenceSearch3::FOLD)
	output << "\\maudeLeftBrace\\maudeKeyword{fold}\\maudeRightBrace\\maudeSpace";
      output << "\\maudeKeyword{" << searchKindName[searchKind] << "}\\maudeSpace";

      if (variantFlags & (VariantSearch::IRREDUNDANT_MODE | VariantUnificationProblem::FILTER_VARIANT_UNIFIERS))
	{
	  output << "\\maudeLeftBrace";
	  const char* sep = "";
	  if (variantFlags & VariantSearch::IRREDUNDANT_MODE)
	    {
	      output << "\\maudeKeyWord{delay}";
	      sep = "\\maudePunctuation{,}\\maudeSpace";
	    }
	  if (variantFlags & VariantUnificationProblem::FILTER_VARIANT_UNIFIERS)
	    output << sep << "\\maudeKeyWord{filter}";
	  output << "\\maudeRightBrace";
	}

      if (limit != NONE || depth != NONE)
	{
	  output << "\\maudePunctuation{[}";
	  if (limit != NONE)
	    output << "\\maudeNumber{" << limit << "}";
	  if (depth != NONE)
	    output << "\\maudePunctuation{,}\\maudeSpace\\maudeNumber{" << depth << "}";
	  output << "\\maudePunctuation{]}\\maudeSpace";
	}

      output << "\\maudeKeyword{in}\\maudeSpace\\maudeModule{" << Token::latexName(module->id()) <<
	"}\\maudeSpace\\maudePunctuation{:}\\maudeSpace\n";

      MixfixModule::latexPrintDagNode(output, subject);
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
  pendingClose = "\\end{maudeResultParagraph}\n%\n%  End of ";
  pendingClose += searchKindName[searchKind];
  pendingClose += "\n%\n";
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
  Tty::blockEscapeSequences();
  output << "\\begin{comment}\n%\n%  " << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, number, number2);
  output << subject << " .\n%\n\\end{comment}\n";
  Tty::unblockEscapeSequences();
  //
  //	Print latex version of command.
  //
  output << "\\begin{maudeResultParagraph}\n";
  if (showCommand)
    {
      output << "$\\maudeKeyword{" << command << "}\\maudeSpace";
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
      MixfixModule::latexPrintDagNode(output, subject);
      if (strategy != 0)
	{
	  output << "\\maudeSpace\\maudeKeyword{using}\\maudeSpace";
	  safeCastNonNull<const VisibleModule*>(subject->symbol()->getModule())->latexPrintStrategy(output, strategy);
	}
      output << "$\\maudeEndCommand\n";
    }
  needNewline = showCommand;  // for results that need a newline after the command
  pendingClose = "\\end{maudeResultParagraph}\n%\n%  End of " + command + "\n%\n";
}

void
MaudeLatexBuffer::generateContinue(bool showCommand, Int64 limit, bool debug)
{
  Tty::blockEscapeSequences();
  output << "\\begin{comment}\n%\n%  continue " << limit << " .\n%\n\\end{comment}\n\\begin{maudeResultParagraph}\n";
  Tty::unblockEscapeSequences();
  if (showCommand)
    output << "\\maudeKeyword{continue} \\maudeNumber{" << limit << "}\\maudeEndCommand\n";
  //
  //	We might be continuing a command that separates solutions by a blank line.
  //
  needNewline = showCommand;
  pendingClose = "\\end{maudeResultParagraph}\n%\n%  End of continue\n%\n";
}

void
MaudeLatexBuffer::generateShow(bool showCommand, const string& command, Module* module)
{
  output << "\\begin{comment}\n%\n%  " << command << " " << module << " .\n%\n\\end{comment}\n\\begin{maudeShowParagraph}";
  if (showCommand)
    {
      output << "\\maudeKeyword{" << command << "}\\maudeSpace";
      generateModuleName(module);
      output << "\\maudeEndCommand\\newline\n";
    }
  pendingClose = "\\end{maudeShowParagraph}\n%\n%  End of " + command + "\n%\n";
}

void
MaudeLatexBuffer::generateShow(bool showCommand, const string& command, View* view)
{
  output << "\\begin{comment}\n%\n%  " << command << " " << view << " .\n%\n\\end{comment}\n\\begin{maudeShowParagraph}";
  if (showCommand)
    output << "\\maudeKeyword{" << command << "}\\maudeSpace\\maudeView{" << view << "}\\maudeEndCommand\\newline\n";
  pendingClose = "\\end{maudeShowParagraph}\n%\n%  End of " + command + "\n%\n";
}
