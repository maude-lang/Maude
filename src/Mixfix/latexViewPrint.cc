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
  string renaming =  latexRenaming("\\par\\maudeIndent $",
				   "$\\maudeEndStatement\n\\par\\maudeIndent $",
				   fromTheory,
				   toModule,
				   false);
  if (getNrSortMappings() + getNrOpMappings() > 0)
    s << renaming << "$\\maudeEndStatement\n";
  for (auto& p : fromTheoryVariableAliases)
    {
      s << "\\par\\maudeIndent\\maudeKeyword{var}$" << Token::latexIdentifier(p.first) <<
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
      s << "\\par\\maudeIndent\\maudeKeyword{op} $";
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
