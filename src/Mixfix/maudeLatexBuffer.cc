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
//      Implementation for class MaudeLatexBuffer.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "higher.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "sort.hh"
#include "connectedComponent.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "conditionFragment.hh"
#include "module.hh"
#include "rewriteStrategy.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "variableDagNode.hh"

// S theory class definitions
#include "S_Symbol.hh"
#include "S_Term.hh"
#include "S_DagNode.hh"

//	builtin class definitions
#include "floatSymbol.hh"
#include "floatTerm.hh"
#include "floatDagNode.hh"
#include "stringSymbol.hh"
#include "stringTerm.hh"
#include "stringDagNode.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"
#include "rewriteSequenceSearch.hh"
#include "pattern.hh"

//	strategy language class definitions
#include "strategyLanguage.hh"
#include "strategyExpression.hh"
#include "trivialStrategy.hh"
#include "applicationStrategy.hh"
#include "branchStrategy.hh"
#include "callStrategy.hh"
#include "concatenationStrategy.hh"
#include "iterationStrategy.hh"
#include "oneStrategy.hh"
#include "subtermStrategy.hh"
#include "testStrategy.hh"
#include "unionStrategy.hh"

//	front end class definitions
#include "token.hh"
#include "timer.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "mixfixModule.hh"
#include "maudeLatexBuffer.hh"

MaudeLatexBuffer::MaudeLatexBuffer(const char* fileName)
: output(fileName)
{
  output << "\\documentclass{article}\n";
  output << "\\usepackage[top=30pt,bottom=50pt,left=30pt,right=30pt]{geometry}\n";
  output << "\\usepackage{maude}\n";
  output << "\\begin{document}\n";
}

MaudeLatexBuffer::~MaudeLatexBuffer()
{
  output << "\\end{document}\n";
}

void
MaudeLatexBuffer::generateBanner(const char* date, const char* time, time_t seconds)
{
  output << "\\begin{maudeBanner}\n";
  output << "\\maudeBannerIndent\\hspace{6em}\\textbackslash||||||||||||||||||/\n";
  output << "\\maudeBannerIndent\\hspace{5em}--- Welcome to \\color{red}M\\color{cyan}a\\color{blue}u\\color{magenta}d\\color{green}e\\color{black} ---\n";
  output << "\\maudeBannerIndent\\hspace{6em}/||||||||||||||||||\\textbackslash\n";
  output << "\\maudeBannerIndent\\hspace{0.5em}" <<  PACKAGE_STRING << " built: " << date << ' ' << time << '\n';
  output << "\\maudeBannerIndent\\hspace{1.5em}Copyright 1997-2023 SRI International\n";
  output << "\\maudeBannerIndent\\hspace{5em}" << ctime(&seconds);
  output << "\\end{maudeBanner}\n";
}

void
MaudeLatexBuffer::generateDag(DagNode* dagNode)
{
  MixfixModule::latexPrettyPrint(output, dagNode);
}

void
MaudeLatexBuffer::generateModuleName(Module* module)
{
  output << "\\maudeModule{" << Token::latexName(module->id()) << "}";
}

void
MaudeLatexBuffer::generateModifiers(Module* module, Int64 number, Int64 number2)
{
  if (number != NONE || number2 != NONE)
    {
      output  << "[";
      if (number != NONE)
	output << "\\maudeNumber{" << number << "}";
      if (number2 != NONE)
	output << ", \\maudeNumber{" << number2 << "}";
      output  << "] ";
    }
  output << "\\maudeKeyword{in} \\maudeModule{" << Token::latexName(module->id()) <<
    "} \\maudePunctuation{:}\n";
}

void
MaudeLatexBuffer::generateResult(RewritingContext& context,
				 const Timer& timer,
				 bool showStats,
				 bool showTiming,
				 bool showBreakdown)
{
  if (showStats)
    generateStats(context, timer, showTiming, showBreakdown);
  output << "\\par\\maudeResponse{result}\n";
  generateType(context.root()->getSort());
  output << "\\maudePunctuation{:}\n";
  generateDag(context.root());
  output << "\n\\end{maudeResultParagraph}\n";
}

void
MaudeLatexBuffer::generateStats(RewritingContext& context,
				const Timer& timer,
				bool showTiming,
				bool showBreakdown)
{
  Int64 nrRewrites = context.getTotalCount();
  output << "\\par\\maudeResponse{rewrites:} \\maudeNumber{" << nrRewrites << "}";
  Int64 real;
  Int64 virt;
  Int64 prof;
  if (showTiming && timer.getTimes(real, virt, prof))
    generateTiming(nrRewrites, prof, real);
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
MaudeLatexBuffer::generateTiming(Int64 nrRewrites, Int64 cpu, Int64 real)
{
  output << " \\maudeResponse{in} \\maudeNumber{" << cpu / 1000 <<
    "} \\maudeResponse{ms cpu} \\maudePunctuation{(}\\maudeNumber{" <<
    real / 100 << "} \\maudeResponse{ms real}\\maudePunctuation{)} \\maudePunctuation{(}\\maudeNumber{";
  if (cpu > 0)
    output << (1000000 * nrRewrites) / cpu;
  else
    output << "\\textasciitilde";
  output << "} \\maudeResponse{rewrites/second}\\maudePunctuation{)}\n";
}

void
MaudeLatexBuffer::generateType(Sort* sort)
{
  if (sort == 0)
    {
      output << "\\maudeMisc{(sort not calculated)}";
      return;
    }
  output << "$" << MixfixModule::latexType(sort) << "$";
}


void
MaudeLatexBuffer::generateCommand(const string command, DagNode* subject, Int64 number, Int64 number2)
{
  Module* module = subject->symbol()->getModule();
  
  Tty::blockEscapeSequences();
  output << "\\begin{maudeResultParagraph}\n\\begin{comment}\n  " << command << " ";
  safeCastNonNull<MixfixModule*>(module)->printModifiers(output, number, number2);
  output << subject << " .\n\\end{comment}\n";
  Tty::unblockEscapeSequences();

  output << "\\par\\maudeKeyword{" << command << "} ";
  if (number != NONE || number2 != NONE)
    {
      output << "\\maudePunctuation{[}";
      if (number != NONE)
	output << "\\maudeNumber{" << number << "}";
      if (number2 != NONE)
	output << "\\maudePunctuation{,} \\maudeNumber{" << number2 << "}";
      output << "\\maudePunctuation{]} ";
    }
  output << "\\maudeKeyword{in} \\maudeModule{" << Token::latexName(module->id()) <<
    "} \\maudePunctuation{:}\n";
  generateDag(subject);
  output << " \\maudePunctuation{.}\n";
}

void
MaudeLatexBuffer::generateReduce(DagNode* subject)
{
  generateCommand("reduce", subject);
}

void
MaudeLatexBuffer::generateRewrite(DagNode* subject, Int64 limit)
{
  generateCommand("rewrite", subject, limit);
}

void
MaudeLatexBuffer::generateFrewrite(DagNode* subject, Int64 limit, Int64 gas)
{
  generateCommand("frewrite", subject, limit, gas);

}

void
MaudeLatexBuffer::generateErewrite(DagNode* subject, Int64 limit, Int64 gas)
{
  generateCommand("erewrite", subject, limit, gas);
}

void
MaudeLatexBuffer::generateContinue(Int64 limit)
{
  Tty::blockEscapeSequences();
  output << "\\begin{maudeResultParagraph}\n\\begin{comment}\n  continue " << limit << " .\n\\end{comment}\n";
  Tty::unblockEscapeSequences();
  output << "\\par\\maudeKeyword{continue} \\maudeNumber{" << limit << "} \\maudePunctuation{.}\n";
}
