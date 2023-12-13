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
//      Code for generating LaTeX for Tokens that doesn't depend on more complex objects
//	such as modules.
//

string
Token::latexName(const char* name)
{
  //
  //	Translate the 10 characters that LaTeX considers special plus ' ` because the defaults look bad.
  //	We also follow < and > with an empty group {} to prevent << and >> being transformed into guillemets.
  //
  //	Note that \textasciicircum and \textbackslash don't exist in math mode so the latex code returned should
  //	always be wrapped with something like \maudeSymbolic{} or \maudeIdentifier{} that switches to a font
  //	where these characters exist.
  //
  string id;
  for (const char* p = name; *p; ++p)
    {
      char c = *p;
      switch (c)
	{
	case '~':
	  {
	    id += "\\maudeTilde ";
	    break;
	  }
	case '^':
	  {
	    id += "\\textasciicircum ";
	    break;
	  }
	case '\\':
	  {
	    id += "\\textbackslash ";
	    break;
	  }
	case '\'':
	  {
	    id += "\\maudeSingleQuote ";
	    break;
	  }
	case '`':
	  {
	    id += "\\maudeBackquote ";
	    break;
	  }
	case '<':
	  {
	    id += "<{}";
	    break;
	  }
	case '>':
	  {
	    id += ">{}";
	    break;
	  }
	case '&':
	case '%':
	case '$':
	case '#':
	case '_':
	case '{':
	case '}':
	  {
	    id += '\\';
	    // fall thru
	  }
	default:
	  {
	    id += c;
	  }	  
	}
    }
  return id;
}

string
Token::latexName(int code)
{
  return latexName(name(code));
}

string
Token::latexName(const string& name)
{
  return latexName(name.c_str());
}

string
Token::latexName(const Rope& name)
{
  const char* nameStr = name.makeZeroTerminatedString();
  string result = latexName(nameStr);
  delete [] nameStr;
  return result;
}

bool
Token::hasNonAlphanumeric(const char* name)
{
  for (const char* p = name; *p; ++p)
    {
      if (!isalnum(*p))
	return true;
    }
  return false;
}

string
Token::latexIdentifier(int code)
{
  //
  //	We generate LaTeX for the token given by code, assuming it is part of a term.
  //	We split tokens into 5 classes
  //	(1) Numbers such as "42.0"
  //	(2) Special single characters (some of which need escaping) such as "f", "$"
  //	(3) Commas, which are single characters and good places to end a line
  //	(4) Alphanumeric strings that print fine in italics such as "foo", "f42"
  //	(5) Everything else, which could include any printable ASCII symbols, such as "|f\/|"
  //
  int sp = specialProperty(code);
  const char* str = name(code);
  if (sp == SMALL_NAT || sp == SMALL_NEG || sp == Token::ZERO || sp == Token::FLOAT || sp == Token::RATIONAL)
    {
      //
      //	If it looks like a number, treat it like a number.
      //
      return string("\\maudeNumber{") + str + "}";
    }
  if (str[1] == '\0')
    {
      //
      //	Handle some single character tokens specially.
      //
      char c = str[0];
      if (isalpha(c) || c == '+' || c == '-' || c == '*' || c == '/' || c == '<' || c == '>' ||
	  c == '.' || c == ';' || c == ':' || c == '|' || c == '!' || c == '?' ||
	  c == '[' || c == ']' || c == '(' || c == ')')  // can't assume these balance
	return string("\\maudeSingleChar{") + c + '}';
      if (c == ',')
	return "\\maudeSingleChar{,}\\maudeAllowBreak";
      if (c == '&' || c == '%' || c == '$' || c == '#' || c == '_' || c == '{' || c == '}')
	return string("\\maudeSingleChar{\\") + c + '}';
    }
  return (hasNonAlphanumeric(str) ? "\\maudeSymbolic{" : "\\maudeIdentifier{") + latexName(str) + "}";
}
