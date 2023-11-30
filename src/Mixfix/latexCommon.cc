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
//	Common functionality for LaTeX pretty printing.
//
const char* MixfixModule::latexRed = "{\\color{red}";
const char* MixfixModule::latexGreen = "{\\color{green}";
const char* MixfixModule::latexBlue = "{\\color{blue}";
const char* MixfixModule::latexCyan = "{\\color{cyan}";
const char* MixfixModule::latexMagenta = "{\\color{magenta}";
const char* MixfixModule::latexYellow = "{\\color{yellow}";
const char* MixfixModule::latexResetColor = "}";

string
MixfixModule::latexStructuredName(const Vector<int>& codes, const Module* m)
{
  //
  //	We expect to see
  //	  Foo{TRIV,TRIV}{Bar{X,Baz},Z}
  //	split into tokens
  //	  Foo { TRIV , TRIV } { Bar { X , Baz } , Z }
  //	i.e. alternating identifiers and separators { } ,
  //	An identifier could be a parameter and we check that in the current module;
  //	otherwise it is a view or pseudo-parameter which we treat like a view.
  //	We assume that codes[0] will be handled by the caller.
  //
  string result;
  Index nrCodes = codes.size();
  for (Index i = 1; i < nrCodes; ++i)
    {
      int code = codes[i];
      if (code == leftBrace)
	result += "\\maudeLeftBrace";
      else if (code == rightBrace)
	result += "\\maudeRightBrace";
      else if (code == comma)
	result += "\\maudeComma";
      else
	{
	  //
	  //	An identifier. Need to decide if it is a view or a parameter.
	  //
	  result += (safeCastNonNull<const ImportModule*>(m)->findParameterIndex(code) == NONE) ?
	    "\\maudeView{" : "\\maudeParameter{";
	  result += Token::latexName(code);
	  result += "}";
	}
    }
  return result;
}

string
MixfixModule::latexSort(int code, const Module* module)
{
  if (Token::auxProperty(code) == Token::AUX_STRUCTURED_SORT)
    {
      if (module == nullptr)
	{
	  //
	  //	We don't have a module to determine what is a parameter and what is
	  //	is a view so do the naive thing.
	  //
	  return "\\maudeSort{" + Token::latexName(Token::sortName(code)) +  "}";
	}
      Vector<int> codes;
      Token::splitParameterizedSort(code, codes);
      return "\\maudeSort{" + Token::latexName(codes[0]) + "}" + latexStructuredName(codes, module);
    }
  return "\\maudeSort{" + Token::latexName(code) +  "}";
}

string
MixfixModule::latexSort(const Sort* sort)
{
  Assert(sort->index() != Sort::KIND, "kind");
  int code = sort->id();
  if (Token::auxProperty(code) == Token::AUX_STRUCTURED_SORT)
    {
      Vector<int> codes;
      Token::splitParameterizedSort(code, codes);
      return "\\maudeSort{" + Token::latexName(codes[0]) + "}" + latexStructuredName(codes, sort->getModule());
    }
  return "\\maudeSort{" + Token::latexName(code) +  "}";
}

string
MixfixModule::latexType(const Sort* sort)
{
  Assert(sort != 0, "null sort");
  ConnectedComponent* c = sort->component();
  if (c != 0 && sort->index() == Sort::KIND)
    {
      string kind("\\maudeLeftBracket");
      kind += latexType(c->sort(1));
      int nrMax = c->nrMaximalSorts();
      for (int i = 2; i <= nrMax; i++)
	{
	  kind += "\\maudeComma";
	  kind += latexSort(c->sort(i));
	}
      return kind + "\\maudeRightBracket";
    }
  return latexSort(sort);
}

string
MixfixModule::latexConstant(int code, const Module* module)
{
  if (Token::auxProperty(code) == Token::AUX_STRUCTURED_SORT)
    {
      if (module == nullptr)
	{
	  //
	  //	We don't have a module to determine what is a parameter and what is
	  //	is a view so do the naive thing.
	  //
	  return "\\maudeSymbolic{" + Token::latexName(Token::sortName(code)) +  "}";
	}
      return safeCastNonNull<const MixfixModule*>(module)->latexStructuredConstant(code);
    }
  return latexPrettyOp(code);
}

string
MixfixModule::latexStructuredConstant(int code) const
{
  Assert(Token::auxProperty(code) == Token::AUX_STRUCTURED_SORT, "not structured");
  Vector<int> codes;
  Token::splitParameterizedSort(code, codes);
  return Token::latexIdentifier(codes[0]) + latexStructuredName(codes, this);
}

string
MixfixModule::latexPrettyOp(int code)
{
  string pretty = Token::prettyOpName(code);
  if (pretty.empty())
    return Token::latexIdentifier(code);
  return "\\maudeSymbolic{" + Token::latexName(pretty)  + "}";
}

bool
MixfixModule::latexFancySpace(ostream& s, int spaceToken, const PrintSettings& printSettings)
{
  //
  //	Handle user specified formating between tokens. Return true if we emit
  //	something that can be treated like a space between tokens.
  //
  bool space = false;
  for (const char* cmd = Token::name(spaceToken); *cmd; ++cmd)
    {
      char c = *cmd;
      switch (c)
	{
	case '+':
	  {
	    ++globalIndent;
	    break;
	  }
	case '-':
	  {
	    if (globalIndent > 0)
	      --globalIndent;
	    break;
	  }
	case 'n':
	  {
	    s << "\\newline";
	    space = true;
	    break;
	  }
	case 't':
	  {
	    s << "\\;";  // no good way to do a tab without a tabbing environment - do a thick space
	    space = true;
	    break;
	  }
	case 's':
	  {
	    s << "\\:";  // math mode medium space
	    space = true;
	    break;
	  }
	case 'i':
	  {
	    if (globalIndent > 0)
	      {
		for (int i = 0; i < globalIndent; i++)
		  s << "\\:";
		space = true;
	      }
	    break;
	  }
	default:
	  {
	    if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	      break;
	    switch (c)
	      {
	      case 'r':
		{
		  s << "\\color{red}";
		  break;
		}
	      case 'g':
		{
		  s << "\\color{green}";
		  break;
		}
	      case 'b':
		{
		  s << "\\color{blue}";
		  break;
		}
	      case 'c':
		{
		  s << "\\color{cyan}";
		  break;
		}
	      case 'm':
		{
		  s << "\\color{magenta}";
		  break;
		}
	      case 'y':
		{
		  s << "\\color{yellow}";
		  break;
		}
	      case 'p':
	      case 'o':
		{
		  s << "\\color{black}";
		  break;
		}
	      }
	    /* FIXME
	    switch (c)
	      {
#define MACRO(m, t) \
case m: { s << Tty(Tty::t); attributeUsed = true; break; }
#include "ansiEscapeSequences.cc"
#undef MACRO
	      case 'o':
		{
		  s << Tty(Tty::RESET);
		  break;
		}
	      }
	    */
	  }
	}
    }
  return space;
}

void
MixfixModule::latexPrefix(ostream& s, bool needDisambig, const char* color)
{
  if (needDisambig)
    s << "\\maudeLeftParen";
  if (color != 0)
    s << color;
}

void
MixfixModule::latexPrintPrefixName(ostream& s, const char* prefixName, const SymbolInfo& si, const PrintSettings& printSettings)
{
  if (printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() == 2))
    {
      latexFancySpace(s, si.format[0], printSettings);
      s << prefixName;
      latexFancySpace(s, si.format[1], printSettings);
    }
  else
    s << prefixName;
}

int
MixfixModule::latexPrintTokens(ostream& s, const SymbolInfo& si, int pos, const char* color, const PrintSettings& printSettings)
{
  //
  //	We start with a space if we are following an argument and don't start with a special
  //	character and we end with a space if we don't end with a special character.
  //
  bool noSpace = (pos == 0);
  bool previousOpenOrComma = false;
  bool hasFormat = printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() > 0);
  for (;;)
    {
      int token = si.mixfixSyntax[pos++];
      if (token == underscore)
	break;
      bool open = token == leftParen || token == leftBracket || token == leftBrace;
      bool close = token == rightParen || token == rightBracket || token == rightBrace;
      bool isComma = token == comma;
      if (!(hasFormat && latexFancySpace(s, si.format[pos - 1], printSettings)))
	{
	  //
	  //	format didn't produce a space; do we need one?
	  //
	  if (open || close || isComma || noSpace)
	    {
	      //
	      //	We don't want to output a space before one of our 7 special characters,
	      //	for example:
	      //	  f(   m[   s{   a)   v]   a}   a,
	      //	or after a special character except comma (unless format is in play).
	      //
	      //	But we want to allow a line break
	      //	  if we're about to print ) ] {
	      //	  or the previous token was ( [ { ,
	      //	in order to reduce the likelihood of an overfull \hbox
	      //
	      if (close || previousOpenOrComma)
		s << "\\maudeAllowLineBreak";
	    }
	  else
	    s << "\\maudeSpace";
	}
      //
      //	If we don't generate a space after ( [ { , we want to allow a linebreak.
      //
      previousOpenOrComma = open || isComma;
      //
      //	We don't follow with a space if the token is an open or close; or if the token is comma
      //	and format is being used (to allow the user to explicitly get rid of spaces after commas).
      //	For example:  (a   [b   {c   d)   e]   a}
      //
      noSpace = open || close || (hasFormat && isComma);
      if (color != 0)
      	s << color;
      //
      //	We're dealing with a mixfix operator so all parts of it must be typeset with \maudeSymbolic{}
      //
      s << "\\maudeSymbolic{" << Token::latexName(token) << "}";
      if (color != 0)
      s << latexResetColor;
    }
  if (!(hasFormat && latexFancySpace(s, si.format[pos - 1], printSettings)))
    {
      //
      //	format didn't produce a space; do we need one?
      //
      if (noSpace)
	{
	  //
	  //	We don't output a space in this cases but we want to allow a linebreak,
	  //	if the previous token was ( [ { , to reduce to likelihood of overfull lines.
	  //
	  if (previousOpenOrComma)
	    s << "\\maudeAllowLineBreak";
	}
      else
	s << "\\maudeSpace";
    }
  return pos;
}

void
MixfixModule::latexPrintTails(ostream& s,
			      const SymbolInfo& si,
			      int pos,
			      int nrTails,
			      bool needAssocParen,
			      bool checkForInterrupt,
			      const char* color,
			      const PrintSettings& printSettings)
{
  //
  //	We output nrTails copies of the user syntax from  pos to si.mixfixSyntax.size() - 1
  //	Usually nrTails will be 1 unless we are unflattening a flattened associative operator.
  //
  bool previousOpenOrComma = false;
  bool hasFormat = printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() > 0);
  Index mixfixLength = si.mixfixSyntax.size();
  for (int i = 0;;)
    {
      if (checkForInterrupt && UserLevelRewritingContext::interrupted())
	return;
      bool noSpace = (pos == 0);  // in case we are printing a constant
      for (Index j = pos; j < mixfixLength; ++j)
	{
	  int token = si.mixfixSyntax[j];
	  bool open = token == leftParen || token == leftBracket || token == leftBrace;
	  bool close = token == rightParen || token == rightBracket || token == rightBrace;
	  bool isComma = token == comma;
	  if (!(hasFormat && latexFancySpace(s, si.format[pos - 1], printSettings)))
	    {
	      //
	      //	format didn't produce a space; do we need one?
	      //
	      if (open || close || isComma || noSpace)
		{
		  //
		  //	We don't output a space in these cases but we want to allow a linebreak,
		  //	if we're about to print ( ] } or the previous token was ( [ { , to reduce
		  //	the likelihood of an overful \hbox
		  //
		  if (close || previousOpenOrComma)
		    s << "\\maudeAllowLineBreak";
		}
	      else
		s << "\\maudeSpace";
	    }
	  //
	  //	If we don't generate a space after ( [ { , we want to allow a linebreak.
	  //
	  previousOpenOrComma = open || isComma;
	  //
	  //	We don't follow with a space if the token is an open or close; or if the token is a comma
	  //	and  format is being used (to allow the user to explicitly get rid of spaces after commas).
	  //
	  noSpace = open || close || (hasFormat && isComma);
	  if (color != 0)
	    s << color;
	  //
	  //	We're dealing with a mixfix operator so all parts of it must be typeset with \maudeSymbolic{}
	  //
	  s << "\\maudeSymbolic{" << Token::latexName(token) << "}";
	  if (color != 0)
	    s << latexResetColor;
	}
      if (hasFormat)
	{
	  //
	  //	If we generate whitespace after the token, we don't want to allow a line break before
	  //	the next tail so we reset previousOpenOrComma
	  //
	  if (latexFancySpace(s, si.format[mixfixLength], printSettings))
	    previousOpenOrComma = false;
	}
      if (++i == nrTails)
	break;
      if (needAssocParen)
	s << "\\maudeRightParen";
    }
}

void
MixfixModule::latexPrintGather(ostream& s, const Vector<int>& gather)
{
  static const char* gatherSymbols[] = {"\\maudeNormal{e}", "\\maudeNormal{E}", "\\maudeNormal{\\&}"};
  s << "\\maudeKeyword{gather}\\maudeSpace\\maudeLeftParen";
  const char* sep = "";
  for (int i : gather)
    {
      s << sep << gatherSymbols[i - GATHER_e];
      sep = "\\maudeSpace";
    }
  s << "\\maudeRightParen";
}

void
MixfixModule::latexPrintFormat(ostream& s, const Vector<int>& format)
{
  s << "\\maudeKeyword{format}\\maudeSpace\\maudeLeftParen";
  const char* sep = "";
  for (int i : format)
    {
      s << sep << "\\maudeNormal{" << Token::name(i) << "}";
      sep = "\\maudeSpace";
    }
  s << "\\maudeRightParen";
}

string
MixfixModule::latexTokenVector(const Vector<Token>& tokens, Index first, Index last)
{
  string bubble;
  bool needSpace = false;
  for (Index i = first; i <= last; ++i)
    {
      bool nextNeedSpace = true;
      int code = tokens[i].code();
      if (code == rightParen || code == rightBracket || code == rightBrace || code == comma)
	needSpace = false;
      else if (code == leftParen)
	{
	  needSpace = false;
	  nextNeedSpace = false;
	}
      else if (code == leftBracket || code == leftBrace)
	nextNeedSpace = false;
      if (needSpace)
	bubble += "\\maudeSpace";
      bubble += "\\maudeRaw{" + Token::latexName(tokens[i].code()) +  "}";
      needSpace = nextNeedSpace;
    }
  return bubble;
}
