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
#include "SMT.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"
#include "extensionInfo.hh"

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
#include "narrowingSequenceSearch3.hh"

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

//	SMT class definitions
#include "SMT_RewriteSequenceSearch.hh"
//#include "SMT_Info.hh"

//	front end class definitions
#include "token.hh"
#include "timer.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "visibleModule.hh"
#include "interpreter.hh"
#include "maudeLatexBuffer.hh"

//	our stuff
#include "latexCommand.cc"
#include "latexResult.cc"

MaudeLatexBuffer::MaudeLatexBuffer(const char* fileName)
: output(fileName),
  commentSettings(PrintSettings::PLAIN_PRINT_FLAGS)  // no color, format or graph printing in comments
{
  needNewline = false;
  output << "\\documentclass{article}\n";
  output << "\\usepackage[top=30pt,bottom=50pt,left=30pt,right=30pt]{geometry}\n";
  output << "\\usepackage{maude}\n";
  output << "\\begin{document}\n";
}

MaudeLatexBuffer::~MaudeLatexBuffer()
{
  while (!pendingCloseStack.empty())
    {
      output << pendingCloseStack.top();
      pendingCloseStack.pop();
    }
  output << "\\end{document}\n";
}

void
MaudeLatexBuffer::generateBanner(const char* date, const char* time, time_t seconds)
{
  output << "\\begin{maudeBanner}\n";
  output << "\\maudeBannerIndent\\hspace{6em}\\textbackslash||||||||||||||||||/\n";
  output << "\\maudeBannerIndent\\hspace{5em}--- Welcome to \\color{red}M\\color{cyan}a\\color{blue}u\\color{magenta}d\\color{green}e\\color{black} ---\n";
  output << "\\maudeBannerIndent\\hspace{6em}/||||||||||||||||||\\textbackslash\n";
  output << "\\maudeBannerIndent\\hspace{2em}" <<  PACKAGE_STRING << " built: " << date << ' ' << time << '\n';
  output << "\\maudeBannerIndent\\hspace{2em}Copyright 1997-2024 SRI International\n";
  output << "\\maudeBannerIndent\\hspace{5em}" << ctime(&seconds);
  output << "\\end{maudeBanner}\n";
}

void
MaudeLatexBuffer::generateModuleName(NamedEntity* module)
{
  output << "\\maudeModule{" << Token::latexName(module->id()) << "}";
}

void
MaudeLatexBuffer::generateSolutionNr(int64_t solutionNr, int stateNr)
{
  if (needNewline)
    output << "\\newline";
  output << "\\par\\maudeResponse{Solution}\\maudeSpace\\maudeNumber{" << solutionNr << "}\n";
  if (stateNr != NONE)
    {
      output << "\\maudeSpace\\maudePunctuation{(}\\maudeResponse{state}\\maudeSpace\\maudeNumber{" <<
	stateNr << "}\\maudePunctuation{)}\n";
    }
  needNewline = true;
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
MaudeLatexBuffer::generateSubstitution(const Substitution& substitution,
				       const VariableInfo& varInfo,
				       const NatSet& ignoredIndices)
{
  int nrVars = varInfo.getNrRealVariables();
  bool printedVariable = false;
  for (int i = 0; i < nrVars; ++i)
    {
      if (ignoredIndices.contains(i))
	continue;

      Term* v = varInfo.index2Variable(i);
      DagNode* d = substitution.value(i);
      Assert(v != 0, "null variable");

      output << "\\par$";
      MixfixModule::latexPrettyPrint(output, v);
      output << "\\maudeIsAssigned";
      if (d == 0)
	output << "\\maudeMisc{(unbound)}\n";
      else
	MixfixModule::latexPrintDagNode(output, d);
      output << "$\n";
      printedVariable = true;
    }
  if (!printedVariable)
    output << "\\par\\maudeMisc{empty substitution}\n";
}

void
MaudeLatexBuffer::generateSubstitution(const Substitution& substitution,
				       const NarrowingVariableInfo& varInfo)
{
  int nrVariables = substitution.nrFragileBindings();
  for (int i = 0; i < nrVariables; ++i)
    {
      DagNode* v = varInfo.index2Variable(i);
      DagNode* b = substitution.value(i);
      output << "\\par$";
      MixfixModule::latexPrintDagNode(output, v);
      output << "\\maudeIsAssigned";
      MixfixModule::latexPrintDagNode(output, b);
      output << "$\n";
    }
}

void
MaudeLatexBuffer::generateSubstitution(const Vector<DagNode*>& substitution,
				       const NarrowingVariableInfo& variableInfo)
{
  Index nrVariables = substitution.size();
  for (Index i = 0; i < nrVariables; ++i)
    {
      DagNode* v = variableInfo.index2Variable(i);
      DagNode* b = substitution[i];
      output << "\\par$";
      MixfixModule::latexPrintDagNode(output, v);
      output << "\\maudeIsAssigned";
      MixfixModule::latexPrintDagNode(output, b);
      output << "$\n";
    }
}

void
MaudeLatexBuffer::generateVariant(const Vector<DagNode*>& variant, const NarrowingVariableInfo& variableInfo)
{
  Index nrVariables = variant.size() - 1;
  DagNode* d = variant[nrVariables];
  output << "\\par";
  generateType(d->getSort());
  output << "\\maudePunctuation{:}$\\maudeSpace\n";
  MixfixModule::latexPrintDagNode(output, d);
  output << "$\n";
  for (Index i = 0; i < nrVariables; ++i)
    {
      DagNode* v = variableInfo.index2Variable(i);
      DagNode* b = variant[i];
      output << "\\par$";
      MixfixModule::latexPrintDagNode(output, v);
      output << "\\maudeIsAssigned";
      MixfixModule::latexPrintDagNode(output, b);
      output << "$\n";
    }
}

void
MaudeLatexBuffer::generateWarning(const char* message)
{
  output << "\\par\\textcolor{red}{\\maudeResponse{Warning: }}\\maudeMisc{" << message << "}\n";
}

void
MaudeLatexBuffer::generateAdvisory(const char* message)
{
  if (globalAdvisoryFlag)
    output << "\\par\\textcolor{green}{\\maudeResponse{Advisory: }}\\maudeMisc{" << message << "}\n";
}

void
MaudeLatexBuffer::generateState(DagNode* stateDag, const char* message)
{
  if (message == nullptr)
    message = "state:";
  output << "\\par$\\maudeResponse{" << message << "}\\maudeSpace";
  MixfixModule::latexPrintDagNode(output, stateDag);
  output << "$\n";
}

void
MaudeLatexBuffer::generateCompoundSubstitution(const Substitution& substitution,
					       const VariableInfo& variableInfo,
					       const NarrowingVariableInfo& narrowingVariableInfo,
					       Module* m)
{
  //	We deal with a substitution that is broken into two parts.
  //	The first part is bindings to variables from a PreEquation and
  //	the variable names are given by Terms in variableInfo.
  //	The second part is bindings to variables from a term being narrowed
  //	and the variable names are given by DagNodes in narrowingVariableInfo.
  //
  int nrVariables1 = variableInfo.getNrRealVariables();
  int nrVariables2 = narrowingVariableInfo.getNrVariables();
  for (int i = 0; i < nrVariables1; ++i)
    {
      Term* v = variableInfo.index2Variable(i);
      DagNode* b = substitution.value(i);
      output << "\\par$";
      MixfixModule::latexPrettyPrint(output, v);
      output << "\\maudeIsAssigned";
      if (b == 0)
	output << "\\maudeMisc{(unbound)}\n";
      else
	MixfixModule::latexPrintDagNode(output, b);
      output << "$\n";
    }
  //
  //	Variables from the dag being narrowed start after the regular substitution.
  //
  int firstTargetSlot = m->getMinimumSubstitutionSize();
  for (int i = 0; i < nrVariables2; ++i)
    {
      DagNode* v = narrowingVariableInfo.index2Variable(i);
      DagNode* b = substitution.value(firstTargetSlot + i);
      output << "\\par$";
      MixfixModule::latexPrintDagNode(output, v);
      output << "\\maudeIsAssigned";
      MixfixModule::latexPrintDagNode(output, b);
      output << "$\n";
    }
}
