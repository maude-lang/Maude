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
//	Common functionality for LaTeX pretty printing.
//
const char* MixfixModule::latexRed = "\\color{red}";
const char* MixfixModule::latexGreen = "\\color{green}";
const char* MixfixModule::latexBlue = "\\color{blue}";
const char* MixfixModule::latexCyan = "\\color{cyan}";
const char* MixfixModule::latexMagenta = "\\color{magenta}";
const char* MixfixModule::latexYellow = "\\color{yellow}";
const char* MixfixModule::latexResetColor = "\\color{black}";

const char* MixfixModule::restoreColor = "";

string
MixfixModule::latexNumber(const mpz_class& number)
{
  //
  //	We want to be able to break very large numbers over multiple lines but we want to
  //	avoid breaking smaller numbers in a arbitrary way.
  //
  const int BIG_NUM_CUTOFF = 30;
  string str = number.get_str();
  string result = (str.length() >= BIG_NUM_CUTOFF) ? "\\maudeBigNumber{" : "\\maudeNumber{";
  result += str;
  result += "}";
  return result;
}

string
MixfixModule::latexString(const string& str)
{
  const int BIG_STRING_CUTOFF = 30;
  string result = (str.length() >= BIG_STRING_CUTOFF) ? "\\maudeBigString{" : "\\maudeString{";
  result += Token::latexName(str);
  result += "}";
  return result;
}

string
MixfixModule::latexQid(int idCode)
{
  const int BIG_QID_CUTOFF = 29;
  const char* str = Token::name(idCode);
  string result = (strlen(str) >= BIG_QID_CUTOFF) ? "\\maudeBigQid{" : "\\maudeQid{";
  result += "\\maudeSingleQuote ";
  result += Token::latexName(str);
  result += "}";
  return result;
}

string
MixfixModule::latexRaw(int idCode)
{
  const int BIG_RAW_CUTOFF = 30;
  const char* str = Token::name(idCode);
  string result = (strlen(str) >= BIG_RAW_CUTOFF) ? "\\maudeBigRaw{" : "\\maudeRaw{";
  result += Token::latexName(str);
  result += "}";
  return result;
}

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
MixfixModule::latexConstant(int code, const Module* module, int situations)
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
  return latexPrettyOpName(code, situations);
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
MixfixModule::latexPrettyOpName(int code, int situations)
{
  auto pair = Token::makePrettyOpName(code, situations);
  //
  //	If making the pretty name returned the problematic flag, we need to fix the problem in some way.
  //
  if (pair.second)
    {
      //
      //	We have a problem. If we're concerned about BARE_COLON we're in an op-hook and we return the single token
      //	ugly name. Otherwise we just put a set of parentheses around the pretty name if it exists or the original
      //	name if it doesn't. For original names we use Token::latexIdentifier() which may use a different font.
      //
      if (situations & Token::BARE_COLON)
	return Token::latexIdentifier(code);
      string nameToUse = pair.first.empty() ? Token::latexIdentifier(code) : ("\\maudeSymbolic{" + Token::latexName(pair.first) + "}");
      return "\\maudeLeftParen" + nameToUse + "\\maudeRightParen";
    }
  //
  //	If we didn't prettify the name, use Token::latexIdentifier() on the original name; otherwise print the
  //	the prettified name using \maudeSymbolic.
  //
  return pair.first.empty() ? Token::latexIdentifier(code) : ("\\maudeSymbolic{" + Token::latexName(pair.first)  + "}");
}

void
MixfixModule::latexPrintStructuredConstant(ostream& s, Symbol* symbol, const char* color, const PrintSettings& printSettings) const
{
  const SymbolInfo& si = symbolInfo[symbol->getIndexWithinModule()];
  if (printSettings.getPrintFlag(PrintSettings::PRINT_FORMAT) && (si.format.length() > 0))
    {
      //
      //	We want to do the special fonts for a structured constant but we also want
      //	to support the format commands.
      //
      int nrTokens = si.mixfixSyntax.size();
      for (int i = 0;; ++i)
	{
	  (void) latexFancySpace(s, si.format[i], printSettings);
	  if (i == nrTokens)
	    break;
	  if (color != nullptr)
	    s << color;
	  int token = si.mixfixSyntax[i];
	  if (token == leftBrace)
	    s <<  "\\maudeLeftBrace";
	  else if (token == rightBrace)
	    s <<  "\\maudeRightBrace";
	  else if (token == comma)
	    s <<  "\\maudeComma";
	  else
	    {
	      if (i == 0)
		s << Token::latexIdentifier(token);
	      else
		{
		  s << ((safeCastNonNull<const ImportModule*>(this)->findParameterIndex(token) == NONE) ? "\\maudeView{" : "\\maudeParameter{");
		  s << Token::latexName(token) << "}";
		}
	    }
	  if (color != nullptr)
	    s << latexResetColor;
	}
    }
  else
    s << latexStructuredConstant(symbol->id());
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
	    //
	    //	\maudeNewline drops out of inline math mode to start a new paragraph and then restarts
	    //	inline math mode, forgetting about any color change, so we need to remember and restore
	    //	the current color.
	    //
	    s << "\\maudeNewline" << restoreColor;
	    space = true;
	    break;
	  }
	case 't':
	  {
	    s << "\\maudeIdent";  // no good way to do a tab without a tabbing environment - do a thick space
	    space = true;
	    break;
	  }
	case 's':
	  {
	    s << "\\maudeHardSpace";  // so it works at the start of a line
	    space = true;
	    break;
	  }
	case 'i':
	  {
	    if (globalIndent > 0)
	      {
		for (int i = 0; i < globalIndent; i++)
		  s << "\\maudeHardSpace";
		space = true;
	      }
	    break;
	  }
	default:
	  {
	    if (printSettings.getPrintFlag(PrintSettings::PRINT_COLOR))
	      break;  // if we have system provided color we switch off user provided format color
	    switch (c)
	      {
	      case 'r':
		{
		  s << (restoreColor = latexRed);
		  break;
		}
	      case 'g':
		{
		  s << (restoreColor = latexGreen);
		  break;
		}
	      case 'b':
		{
		  s << (restoreColor = latexBlue);
		  break;
		}
	      case 'c':
		{
		  s << (restoreColor = latexCyan);
		  break;
		}
	      case 'm':
		{
		  s << (restoreColor = latexMagenta);
		  break;
		}
	      case 'y':
		{
		  s << (restoreColor = latexYellow);
		  break;
		}
	      case 'p':
	      case 'o':
		{
		  latexClearColor(s);
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
  //	We output nrTails copies of the user syntax from pos to si.mixfixSyntax.size() - 1
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
	  if (!(hasFormat && latexFancySpace(s, si.format[j], printSettings)))
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

void
MixfixModule::latexPrintLatexMacro(ostream& s, int latexMacro)
{
  s << "\\maudeKeyword{latex}\\maudeSpace\\maudeLeftParen\\maudeSymbolic{" << Token::latexName(latexMacro) << "}\\maudeRightParen";
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
      else if (code == leftParen || code == leftBracket || code == leftBrace)
	nextNeedSpace = false;
      


      
      if (needSpace)
	bubble += "\\maudeSpace";
      bubble += latexRaw(code);
      needSpace = nextNeedSpace;
    }
  return bubble;
}

void
MixfixModule::latexPrintBubble(ostream& s, const Vector<int>& bubble)
{
  //
  //	We duplicate the idiosyncratic spacing conventions and character translations
  //	from Interpreter::printBubble() for loop mode support.
  //
  bool needSpace = false;
  for (int code : bubble)
    {
      if (code == rightParen || code == rightBracket || code == rightBrace || code == comma ||
	  code == leftParen || code == leftBracket || code == leftBrace)
	{
	  s << latexRaw(code);
	  needSpace = false;
	  continue;
	}
      const char* n = Token::name(code);
      if (n[0] == '\\')
	{
	  //
	  //	Special token starting with backslash.
	  //
	  if (n[2] == 0)
	    {
	      switch (n[1])
		{
		case 'n':
		  {
		    //
		    //	\maudeNewline drops out of inline math mode to start a new paragraph and then restarts
		    //	inline math mode, forgetting about any color change, so we need to remember and restore
		    //	the current color.
		    //
		    s << "\\maudeNewline" << restoreColor;
		    needSpace = false;
		    continue;
		  }
		case 't':
		  {
		    s << "\\maudeIdent";  // no good way to do a tab without a tabbing environment - do a thick space
		    needSpace = false;
		    continue;
		  }
		case 's':
		  {
		    s << "\\maudeHardSpace";  // for symmetry with format but \maudeSpace also seems to work
		    needSpace = false;
		    continue;
		  }
		case '\\':
		  {
		    if (needSpace)
		      s << "\\maudeSpace";
		    s << "\\maudeRaw{\\textbackslash}";
		    needSpace = true;
		    continue;
		  }
		case 'r':
		  {
		    s << (restoreColor = latexRed);
		    continue;
		  }
		case 'g':
		  {
		    s << (restoreColor = latexGreen);
		    continue;
		  }
		case 'b':
		  {
		    s << (restoreColor = latexBlue);
		    continue;
		  }
		case 'c':
		  {
		    s << (restoreColor = latexCyan);
		    continue;
		  }
		case 'm':
		  {
		    s << (restoreColor = latexMagenta);
		    continue;
		  }
		case 'y':
		  {
		    s << (restoreColor = latexYellow);
		    continue;
		  }
		case 'p':
		case 'o':
		  {
		    latexClearColor(s);
		    continue;
		  }
		}
	    }
	  else if (n[1] == '`' &&  n[3] == 0)
	    {
	      //
	      //	\`<char> is converted to <char> with non-special spacing if <char> is one of ( ) [ ] { } ,
	      //
	      switch (n[2])
		{
		case '(':
		  {
		    code = leftParen;
		    break;
		  }
		case ')':
		  {
		    code = rightParen;
		    break;
		  }
		case '[':
		  {
		    code = leftBracket;
		    break;
		  }
		case ']':
		  {
		    code = rightBracket;
		    break;
		  }
		case '{':
		  {
		    code = leftBrace;
		    break;
		  }
		case '}':
		  {
		    code = rightBrace;
		    break;
		  }
		case ',':
		  {
		    code = comma;
		    break;
		  }
		}
	    }
	}
      if (needSpace)
	s << "\\maudeSpace";
      s << latexRaw(code);
      needSpace = true;
    }
  //
  //	For cleanliness, but mostly to reset restoreColor if needed.
  //
  latexClearColor(s);
}
