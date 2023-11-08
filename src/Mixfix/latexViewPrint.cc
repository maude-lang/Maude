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
//	LaTeX pretty printer for views.
//

string
SyntacticView::latexType(const Type& type, const Module* module)
{
  if (type.kind)
    {
      string result = "\\maudeLeftBracket";
      const char* sep = "";
      for (auto& t : type.tokens)
	{
	  result += sep;
	  result += MixfixModule::latexSort(t.code(), module);
	  sep = "\\maudeComma";
	}
      return result + "\\maudeRightBracket";
    }
  return MixfixModule::latexSort(type.tokens[0].code(), module);
}

void
SyntacticView::latexShowView(ostream& s)
{
  //
  //	Generate header.
  //
  s << "\\par\\maudeKeyword{view}\\maudeSpace\\maudeView{" << Token::latexName(id()) << "}";
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << "$\\maudeLeftBrace\\maudeParameter{" << Token::latexName(getParameterName(0)) << "}\\maudeParameterColon";
      getParameterTheoryExpression(0)->latexPrint(s);
      for (int i = 1; i < nrParameters; ++i)
	{
	  s << "\\maudeComma\\maudeSpace\\maudeParameter{" << Token::latexName(getParameterName(i)) << "}\\maudeParameterColon";
	  getParameterTheoryExpression(i)->latexPrint(s);
	}
      s << "\\maudeRightBrace$";
    }
  s << "\\maudeSpace\\maudeKeyword{from}\\maudeSpace";
  getFrom()->latexPrint(s);
  s << "\\maudeSpace\\maudeKeyword{to}\\maudeSpace";
  ImportModule* toModule = getToModule();
  getTo()->latexPrint(s, toModule);  // the target must have the same set of parameters as the view
  s << "\\maudeSpace\\maudeKeyword{is}\n";
  //
  //	Generate sort/op/class mappings.
  //
  ImportModule* fromTheory = getFromTheory();
  if (getNrSortMappings() + getNrOpMappings() + getNrClassMappings() > 0)
    {
      string renaming = latexRenaming("\\par\\maudeIndent $",
				      "$\\maudeEndStatement\n\\par\\maudeIndent $",
				      fromTheory,
				      toModule,
				      false);
      s << renaming << "$\\maudeEndStatement\n";
    }
  //
  //	Generate variable declarations.
  //
  if (!varDecls.empty())
    {
      bool startNew = true;
      TypeList::const_iterator j = varDefs.begin();
      for (const VarDecl& i : varDecls)
	{
	  if (startNew)
	    {
	      s << "\\par\\maudeIndent" << (i.lastWithCurrentDef ? "\\maudeKeyword{var}$" : "\\maudeKeyword{vars}$");
	      startNew = false;
	    }
	  s << "\\maudeSpace" << Token::latexIdentifier(i.varName.code());
	  if (i.lastWithCurrentDef)
	    {
	      s << "\\maudeHasSort" << latexType(*j, fromTheory) << "$\\maudeEndStatement\n";
	      ++j;
	      startNew = true;
	    }
	}
    }
  //
  //	We generate op->term and  msg->term mappings as unparsed bubbles.
  //
  for (const auto& i : opTermList)
    {
      s << "\\par\\maudeIndent\\maudeKeyword{" << (i.msg ? "msg" : "op") << "}\\maudeSpace" <<
	MixfixModule::latexTokenVector(i.fromBubble, 0, i.fromBubble.size() - 1) <<
	"\\maudeSpace\\maudeKeyword{to}\\maudeSpace\\maudeKeyword{term}\\maudeSpace" <<
	MixfixModule::latexTokenVector(i.toBubble, 1, i.toBubble.size() - 1) << "\\maudeEndStatement\n";
    }
  //
  //	We generate strat->expr mappings as unparsed bubbles.
  //
  for (const auto& i : stratExprList)
    {
      s << "\\par\\maudeIndent\\maudeKeyword{strat}\\maudeSpace" <<
	MixfixModule::latexTokenVector(i.fromBubble, 0, i.fromBubble.size() - 1) <<
	"\\maudeSpace\\maudeKeyword{to}\\maudeSpace\\maudeKeyword{expr}\\maudeSpace" <<
	MixfixModule::latexTokenVector(i.toBubble, 1, i.toBubble.size() - 1) << "\\maudeEndStatement\n";
    }
  s << "\\par\\maudeKeyword{endv}\n";
}

void
SyntacticView::latexShowProcessedView(ostream& s)
{
  s << "\\par\\maudeKeyword{view}\\maudeSpace\\maudeView{" << Token::latexName(id()) << "}";
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << "$\\maudeLeftBrace\\maudeParameter{" << Token::latexName(getParameterName(0)) <<
	"}\\maudeParameterColon" << getParameterTheory(0)->latexModuleExpression();
      for (int i = 1; i < nrParameters; ++i)
	{
	  s << "\\maudeComma\\maudeSpace\\maudeParameter{" << Token::latexName(getParameterName(i)) <<
	    "}\\maudeParameterColon" << getParameterTheory(i)->latexModuleExpression();
	}
      s << "\\maudeRightBrace$";
    }
  ImportModule* fromTheory = getFromTheory();
  ImportModule* toModule = getToModule();
  s << "\\maudeSpace\\maudeKeyword{from}\\maudeSpace" << fromTheory->latexModuleExpression();
  s << "\\maudeSpace\\maudeKeyword{to}\\maudeSpace" << toModule->latexModuleExpression();
  s << "\\maudeSpace\\maudeKeyword{is}\n";
  if (getNrSortMappings() + getNrOpMappings() > 0)
    {
      string renaming = latexRenaming("\\par\\maudeIndent $",
				      "$\\maudeEndStatement\n\\par\\maudeIndent $",
				      fromTheory,
				      toModule,
				      true);
      s << renaming << "$\\maudeEndStatement\n";
    }
  for (auto& p : fromTheoryVariableAliases)
    {
      s << "\\par\\maudeIndent\\maudeKeyword{var}$\\maudeSpace" << Token::latexIdentifier(p.first) <<
	"\\maudeHasSort" << MixfixModule::latexType(p.second) << "$\\maudeEndStatement\n";
    }
  //
  //	We need to swap in our own aliases, even if they're empty
  //	if only to get rid of the modules' own aliases.
  //
  MixfixParser* savedFromTheoryParser = 0;
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  MixfixParser* savedToModuleParser = 0;
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);

  for (const auto& i : getOpTermMap())
    {
      s << "\\par\\maudeIndent\\maudeKeyword{op}$\\maudeSpace";
      MixfixModule::latexPrettyPrint(s, i.second.first);
      s << "\\maudeSpace\\maudeKeyword{to}\\maudeSpace\\maudeKeyword{term}\\maudeSpace";
      MixfixModule::latexPrettyPrint(s, i.second.second);
      s << "$\\maudeEndStatement\n";
    }
  for (const auto& j : getStratExprMap())
    {
      const VisibleModule* vm = safeCastNonNull<const VisibleModule*>(toModule);
      s << "\\par\\maudeIndent\\maudeKeyword{strat} $";
      vm->latexPrintStrategy(s, j.second.call);
      s << "\\maudeSpace\\maudeKeyword{to expr}\\maudeSpace";
      vm->latexPrintStrategy(s, j.second.value);
      s << "$\\maudeEndStatement\n";
    }
  s << "\\par\\maudeKeyword{endv}\n";
  //
  //	Restore original variable aliases. Check that we didn't generate parsers.
  //
  fromTheory->swapVariableAliasMap(fromTheoryVariableAliases, savedFromTheoryParser);
  Assert(savedFromTheoryParser == 0, "unexpected new from-theory parser");
  toModule->swapVariableAliasMap(toModuleVariableAliases, savedToModuleParser);
  Assert(savedToModuleParser == 0, "unexpected new to module-parser");
}
