/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Code for generating LaTeX for module expressions derived from ImportModules.
//

string
ImportModule::latexModuleExpression(bool parameterBrackets) const
{
  switch (origin)
    {
    case TEXT:
      {
	return "\\maudeModule{" + Token::latexName(id()) + "}";
      }
    case SUMMATION:
      {
	string result;
	const char* sep = "";
	for (const ImportModule* i : importedModules)
	  {
	    result += sep;
	    sep = "\\maudeSummation";
	    result += i->latexModuleExpression(parameterBrackets);
	  }
	return result;
      }
    case RENAMING:
      {
	string result = baseModule->latexModuleExpression(parameterBrackets);
	if (baseModule->origin == SUMMATION)
	  result = "\\maudeLeftParen" + result + "\\maudeRightParen";  // (M + N) * R
	result += "\\maudeRenaming";
	result += canonicalRenaming->latexRenaming("\\maudeLeftParen", "\\maudeComma", baseModule,
						   this, true);
	result += "\\maudeRightParen";
	return result;
      }
    case PARAMETER:
      {
	//
	//	We need to latex the name of a parameter copy of a theory like X :: T for show modules.
	//
	string result = "\\maudeParameter{";
	result += Token::latexName(parameterCopyParameterName);
	result += "}\\maudeParameterColon";
	result += baseModule->latexModuleExpression(parameterBrackets);
	return result;
      }
    case INSTANTIATION:
      {
	string result = baseModule->latexModuleExpression(parameterBrackets);
	if (baseModule->origin == RENAMING)  // can't be SUMMATION
	  result = "\\maudeLeftParen" + result + "\\maudeRightParen";  // (M * R){...}
	const char* sep = "\\maudeLeftBrace";
	for (const Argument* a : savedArguments)
	  {
	    result += sep;
	    sep = "\\maudeComma";
	    if (const View* v = dynamic_cast<const View*>(a))
	      result += v->latexViewExpression(parameterBrackets);
	    else if (const Parameter* p = dynamic_cast<const Parameter*>(a))
	      {
		if (parameterBrackets)
		  result += "[";
		result += "\\maudeParameter{";
		result += Token::latexName(p->id());
		result += "}";
		if (parameterBrackets)
		  result += "]";
	      }
	  }
	result += "\\maudeRightBrace";
	return result;
      }
    case TRANSFORMATION:
      {
	//
	//	Transformer specification.
	//
	string result("\\maudeModule{");
	result += Token::latexName(transformModule->id());
	result += "}";
	//
	//	Input modules.
	//
	if (!inputModules.empty())
	  {
	    const char* sep = "\\maudeLeftBracket ";
	    for (const ImportModule* m : inputModules)
	      {
		result += sep;
		result += m->latexModuleExpression(parameterBrackets);
		sep = "\\maudeComma ";
	      }
	    result += "\\maudeRightBracket";
	  }
	//
	//	Options.
	//
	if (!transformOptions.empty() || inputModules.empty())
	  {
	    result += "\\maudeLeftParen";
	    const char* sep = "";
	    for (int a : transformOptions)
	      {
		result += sep;
		result += "\\maudeQid{";
		result += Token::latexName(a);
		result += "}";
		sep = "\\maudeSpace";
	      }
	    result +=  "\\maudeRightParen";
	  }
	break;
      }
    }
  CantHappen("bad module origin");
  return "";
}
