/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class Token.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	core stuff
#include "lineNumber.hh"

//	mixfix stuff
#include "token.hh"

//	our stuff
#include "auxProperty.cc"
#include "latexToken.cc"

StringTable Token::stringTable;
Vector<int> Token::specialProperties;
Vector<int> Token::auxProperties;
Vector<char> Token::buffer;

ostream&
operator<<(ostream& s, const Token& token)
{
  s << token.name();
  return s;
}

ostream&
operator<<(ostream& s, const Vector<Token>& tokens)
{
  Token::printTokenVector(s, tokens, 0, tokens.length() - 1, true);
  return s;
}

ostream&
operator<<(ostream& s, const Token* token)
{
  s << LineNumber(token->lineNumber());
  return s;
}

void
Token::printTokens(ostream& s,
		   const Vector<Token>& tokens,
		   const char* seperator)
{
  const char* sep = "";
  int nrTokens = tokens.length();
  for (int i = 0; i < nrTokens; i++)
    {
      s << sep << tokens[i];
      sep = seperator;
    }
}

void
Token::printTokenVector(ostream& s,
			const Vector<Token>& tokens,
			int first,
			int last,
			bool fancySpacing)
{
  if (fancySpacing)
    {
      bool needSpace = false;
      for (int i = first; i <= last; i++)
	{
	  bool nextNeedSpace = true;
	  const char* name = tokens[i].name();
	  char c = name[0];
	  if (c != '\0' && name[1] == '\0')
	    {
	      if (c == ')' || c == ']' || c == '}' || c == ',')
		needSpace = false;
	      else if (c == '(' || c == '[' || c == '{')
		nextNeedSpace = false;
	    }
	  if (needSpace)
	    s << ' ';
	  s << name;
	  needSpace = nextNeedSpace;
	}
    }
  else
    {
      for (int i = first; i <= last; i++)
	{
	  if (i != first)
	    s << ' ';
	  s << tokens[i];
	}
    }
}

int
Token::makeParameterInstanceName(int parameterCode, int originalCode)
{
  string newName(stringTable.name(parameterCode));
  newName += '$';
  newName += stringTable.name(originalCode);
  return encode(newName.c_str());
}

void
Token::fixUp(const char* tokenString, int& lineNumber)
{
  //
  //	This essentially a version of tokenize() that removes
  //	\ newline sequences from the tokenString before tokenizing.
  //	We also convert \t characters to spaces.
  //
  int nrBackslashNewlineCombos = 0;
  buffer.clear();
  for (int i = 0;; i++)
    {
      char c = tokenString[i];
      if (c == '\\' && tokenString[i + 1] == '\n')
	{
	  //
	  //	Skip over \ newline pair.
	  //
	  ++i;
	  ++nrBackslashNewlineCombos;
	}
      else
	{
	  if (c == '\t')
	    {
	      IssueWarning(LineNumber(lineNumber + nrBackslashNewlineCombos) <<
		   ": tab character in string literal - replacing it with space");
	      c = ' ';
	    }
	  buffer.push_back(c);
	  if (c == '\0')
	    break;
	}
    } 
  codeNr = encode(buffer.data());
  lineNr = lineNumber;
  lineNumber += nrBackslashNewlineCombos;
}

int
Token::fixUp(const char* tokenString)
{
  //
  //	Remove \ newline sequences.
  //
  buffer.clear();
  for (int i = 0;; i++)
    {
      char c = tokenString[i];
      if (c == '\\' && tokenString[i + 1] == '\n')
	{
	  //
	  //	Skip over \ newline pair.
	  //
	  ++i;
	}
      else
	{
	  buffer.push_back(c);
	  if (c == '\0')
	    break;
	}
    }
  return encode(buffer.data());
}

void
Token::dropChar(const Token& original)
{
  string truncated(stringTable.name(original.codeNr));
  truncated.pop_back();
  codeNr = encode(truncated.c_str());
  lineNr = original.lineNr;
}

int
Token::dotNameCode(int sortNameCode)
{
  string dottedName(".");
  dottedName += stringTable.name(sortNameCode);
  return encode(dottedName.c_str());
}

int
Token::quoteNameCode(int idCode)
{
  string quotedName("'");
  quotedName += stringTable.name(idCode);
  return encode(quotedName.c_str());
}

bool
Token::getInt(int& value) const
{
  const char* str = name();
  char* pointer;
  value = strtol(str, &pointer, 10);
  return pointer != str && *pointer == '\0';
}

pair<string, bool>
Token::makePrettyOpName(int prefixNameCode, int situations)
{
  //
  //	We try to prettify a single token operator name by removing backquotes that
  //	join special characters or indicate whitespace. This can cause ambiguities in
  //	a number of situations and we check for these and return the problematic flag
  //	indicating that the caller needs to wrap the result in parentheses.
  //
  //	Unbalanced parentheses are always an issue and blocks prettification.
  //	Having a result that is already wrapped in parenthese is always problematic.
  //
  const char* original = stringTable.name(prefixNameCode);
  DebugInfo("original = " << original);
  int sp = specialProperties[prefixNameCode];
  if (sp != NONE && sp != CONTAINS_COLON && sp != ENDS_IN_COLON && sp != ITER_SYMBOL)
    return {original, false};  // looks like something that doen't need prettifying

  bool problematic = false;
  //
  //	If the name starts with "term" and it is not connected to the succeeding characters, we have a parsing issue in views.
  //
  if ((situations & EXPOSED_TERM) && original[0] == 't' && original[1] == 'e' && original[2] == 'r' && original[3] == 'm')
    {
      char c = original[4];
      if (c == '`' || c == '\0')
	problematic = true;
    }
  //
  //	Prettify by removing backquotes, checking if we expose problematic tokens outside of parentheses.
  //
  string result;
  int parenDepth = 0;
  bool disconnected = true;
  for (const char* p = original; *p; ++p)
    {
      char c = *p;
      if (c == '`')
	{
	  char c = *(p + 1);
	  Assert(c != '\0', "can't end token with backquote");
	  if (c == '(')
	    ++parenDepth;
	  else if (c == ')')
	    {
	      --parenDepth;
	      if (parenDepth < 0)
		return {original, false};  // don't prettify unbalanced parentheses - leave as an unproblematic single token
	    }
	  if (specialChar(c))
	    {
	      //
	      //	Handle backquoted special character.
	      //
	      result += c;
	      ++p;
	      if (!problematic && parenDepth == 0)
		{
		  //
		  //	Special characters are always disconnected, so
		  //	  , [
		  //	outside of parentheses can be problematic.
		  //
		  if (((situations & EXPOSED_COMMA) && c == ',') ||
		      ((situations & EXPOSED_LEFT_BRACKET) && c == '['))
		    problematic = true;
		}
	    }
	  else
	    result += ' ';  // white space
	  if (!problematic && (situations & MULTIPLE_TOKENS))
	    {
	      //
	      //	Check if we will have multiple tokens.
	      //
	      if (result.length() > 1 || *(p + 1) != '\0')
		problematic = true;
	    }
	  disconnected = true;
	}
      else
	{
	  result += c;
	  if (!problematic && disconnected && parenDepth == 0)
	    {
	      //
	      //	The tokens
	      //	  . to
	      //	appearing outside of parentheses and not connected to preceeding or succeeding
	      //	characters can be problematic. Disconnected colons can be problematic even inside
	      //	parentheses
	      //
	      if ((c == ':' && (((situations & EXPOSED_COLON) && parenDepth == 0) || (situations & BARE_COLON))) ||
		  (c == '.' && (situations & EXPOSED_DOT) && parenDepth == 0))
		{
		  char c = *(p + 1);
		  if (c == '`' || c == '\0')
		    problematic = true;
		}
	      else if ((situations & EXPOSED_TO) && parenDepth == 0 && c == 't' && *(p + 1) == 'o')
		{
		  char c = *(p + 2);
		  if (c == '`' || c == '\0')
		    problematic = true;
		}
	    }
	  disconnected = false;
	}
    }
  if (parenDepth != 0)
    return {original, false};  // don't prettify unbalanced parentheses - leave as an unproblematic single token
  //
  //	If the first and last characters of the prettified name are ( and ), they will be eliminated
  //	during parsing which is always problematic.
  //
  if (!problematic && result.front() == '(' && result.back() == ')')
    problematic = true;
  DebugInfo("result = " << result << " problematic = " << problematic);
  return {result, problematic};
}

int
Token::extractMixfix(int prefixNameCode, Vector<int>& mixfixSyntax)
{
  int sp = specialProperties[prefixNameCode];
  if (sp != NONE && sp != CONTAINS_COLON && sp != ENDS_IN_COLON && sp != ITER_SYMBOL)
    return 0;  // regular string literals exit here
  const char* name = stringTable.name(prefixNameCode);
  const char* p = name;
  for (;; p++)
    {
      char c = *p;
      if (c == '\0')
	return 0;
      if (c == '`' || c == '_' || c == '"')
	break;
    }
  //
  //	Potentially has mixfix syntax.
  //
  int nrUnderscores = 0;
  bool stringMode = false;
  bool seenBS = false;
  string token(name, p - name);
  int parenDepth = 0;
  for (;; p++)
    {
      char c = *p;
      if (c == '\0')
	break;
      if (stringMode)
	{
	  token += c;
	  if (c == '\\')
	    seenBS = !seenBS;
	  else
	    {
	      if (c == '"' && !seenBS)
		stringMode = false;
	      seenBS = false;
	    }
	}
      else
	{
	  if (c == '`')
	    {
	      if (!token.empty())
		{
		  mixfixSyntax.append(encode(token.c_str()));
		  token.erase();
		}
	      continue;
	    }
	  else if (c == '_')
	    ++nrUnderscores;  // fall thru to special character code
	  else if (!specialChar(c))
	    {
	      token += c;
	      if (c == '"')
		stringMode = true;
	      continue;
	    }
	  //
	  //	Special character: terminate current token and add special character as a token.
	  //
	  if (c == '(')
	    ++parenDepth;
	  else if (c == ')')
	    {
	      --parenDepth;
	      if (parenDepth < 0)
		{
		  mixfixSyntax.clear();
		  return PAREN_MISMATCH;
		}
	    }
	  if (!token.empty())
	    mixfixSyntax.append(encode(token.c_str()));
	  token = c;
	  mixfixSyntax.append(encode(token.c_str()));
	  token.erase();
	}
    }
  if (!token.empty())
    mixfixSyntax.append(encode(token.c_str()));
  //
  //	If the mixfix syntax turned out to be a single token, that wasn't different
  //	from the prefix syntax we treat it as not having mixfix syntax in order to
  //	avoid ambiguous parses.
  //
  if (mixfixSyntax.size() == 1 && mixfixSyntax[0] == prefixNameCode)
    {
      DebugAdvisory("deleting mixfixSyntax for " << name);
      mixfixSyntax.clear();
    }
  //
  //	If parentheses are mismatched, we ignore mixfix syntax since it won't be parsable.
  //
  if (parenDepth != 0)
    {
      mixfixSyntax.clear();
      return PAREN_MISMATCH;
    }
  return nrUnderscores;
}

void
Token::checkForSpecialProperty(const char* tokenString)
{
  specialProperties.push_back(computeSpecialProperty(tokenString));
  auxProperties.push_back(computeAuxProperty(tokenString));
}

int
Token::computeSpecialProperty(const char* tokenString)
{
  const char* p = tokenString;
  if (*p == 0)
    return NONE;  // handle null strings correctly
  if (*p == '\'')
    return QUOTED_IDENTIFIER;  // anything starting with a ' is a quoted identifier
  if (*p == '"')
    {
      //
      //	Check for string.
      //
      bool seenBackslash = false;
      for (++p; *p; ++p)
	{
	  char c = *p;
	  if (c == '\\')
	    seenBackslash = !seenBackslash;
	  else
	    {
	      if (c == '"' && !seenBackslash)
		{
		  if (*(p + 1) == '\0')
		    return STRING;
		  break;  // can't be a STRING
		}
	      seenBackslash = false;
	    }
	}
    }
  //
  //	Anthing that isn't as quoted identifer and ends in : gets the ENDS_IN_COLON property.
  //
  size_t len = strlen(tokenString);
  if (len > 1 && tokenString[len - 1] == ':')
    return ENDS_IN_COLON;
  {
    //
    //	Tokens that look like f^123 get the ITER_SYMBOL property.
    //
    for (size_t i = len - 1; i > 0; --i)
      {
	char c = tokenString[i];
	if (c == '^')
	  {
	    if (i == len - 1 || tokenString[i + 1] == '0')
	      break; 
	    return ITER_SYMBOL;
	  }
	if (!isdigit(c))
	  break;
      }
  }
  {
    //
    //	Anything that isn't a quoted identifer, string or iter, or ends in a colon, that contains at least one : other than
    //	the first character get the CONTAINS_COLON property.
    //
    for (++p; *p; ++p)  // don't count first character
      {
	if (*p == ':')
	  return CONTAINS_COLON;
      }
  }
  if (looksLikeFloat(tokenString))
    return FLOAT;
  {
    mpz_class i;
    int error = mpz_set_str(i.get_mpz_t(), tokenString, 10);
    if (error == 0)
      return (i == 0) ? ZERO : ((i < 0) ? SMALL_NEG : SMALL_NAT);
  }
  if (looksLikeRational(tokenString))
    return RATIONAL;
  return NONE;
}

bool
Token::looksLikeRational(const char* s)
{
  bool neg = false;
  if (*s == '-')
    {
      s++;
      neg = true;
    }
  {
    char c = *s++;
    if (!isdigit(c))
      return false;
    if (c == '0')
      {
	//
	//	We allow 0/n but not -0/n or 00/n.
	//
	if (neg || *s != '/')
	  return false;
      }
  }
  for (;;)
    {
      char c = *s++;
      if (c == '/')
	break;
      if (!isdigit(c))
	return false;
    }
  {
    char c = *s++;
    if (!isdigit(c) || c == '0')
      return false;
  }
  for (;;)
    {
      char c = *s++;
      if (c == '\0')
	break;
      if (!isdigit(c))
	return false;
    }
  return true;
}

bool
Token::split(int code, int& prefix, int& suffix)
{
  buffer.clear();
  const char* p = stringTable.name(code);
  Index len = strlen(p);
  for (Index i = len - 1; i > 0; --i)  // don't consider ':' or '.' in p[0]
    {
      char c = p[i];
      if (c == ':' || c == '.')
	{
	  suffix = (i == len - 1) ? NONE : encode(p + i + 1);
	  buffer.resize(i + 1);
	  for (Index j = 0; j != i; ++j)
	    buffer[j] = p[j];
	  buffer[i] = '\0';
	  prefix = encode(buffer.data());
	  return true;
	}
    }
  return false;
}

bool
Token::split(int code, int& opName, mpz_class& number)
{
  const char* p = stringTable.name(code);
  size_t len = strlen(p);
  char* t = new char[len + 1];
  strcpy(t, p);
  for (size_t i = len - 1; i > 0; --i)
    {
      char c = t[i];
      if (c == '^')
	{
	  if (i == len - 1 || t[i + 1] == '0')
	    break; 
	  t[i] = '\0';
	  opName = encode(t);
	  number = t + i + 1;  // convert from const char*
	  delete [] t;
	  return true;
	}
      if (!isdigit(c))
	break;
    }
  delete [] t;
  return false;
}

bool
Token::splitKind(int code, Vector<int>& codes)
{
  codes.clear();
  const char* p = stringTable.name(code);
  buffer.resize(strlen(p) + 1);
  p = strcpy(buffer.data(), p);
  if (*p++ =='`' && *p++ == '[')
    {
      for(;;)
	{
	  bool dummy;
	  char *p2 = const_cast<char*>(skipSortName(p, dummy));
	  if (p2 != 0 && *p2 == '`')
	    {
	      *p2 = 0;
	      codes.append(encode(p));
	      p = p2 + 1;
	      switch (*p++)
		{
		case ']':
		  {
		    if (*p == '\0')
		      return true;
		  }
		case ',':
		  continue;
		}
	    }
	  break;
	}
    }
  return false;
}

Int64
Token::codeToInt64(int code)
{
  bool error;
  Int64 i = stringToInt64(stringTable.name(code), error, 10);
  if (error)  // HACK
    {
      DebugAdvisory("converting out of range integer to 0");
      return 0;
    }
  Assert(!error, "bad machine integer string");
  return i;
}

int
Token::int64ToCode(Int64 i)
{
  int code = stringTable.encode(int64ToString(i, 10));
  if (code == specialProperties.length())
    {
      specialProperties.append((i == 0) ? ZERO : ((i < 0) ? SMALL_NEG : SMALL_NAT));
      //
      //	Integers are always valid sorts.
      //
      auxProperties.append(AUX_SORT);
    }
  return code;
}

double
Token::codeToDouble(int code)
{
  bool error;
  double d = stringToDouble(stringTable.name(code), error);
  Assert(!error, "bad float string");
  return d;
}

int
Token::doubleToCode(double d)
{
  int code = stringTable.encode(doubleToString(d));
  if (code == specialProperties.length())
    {
      specialProperties.append(FLOAT);
      //
      //	Floats might look like a sort or a "constant.sort".
      //
      auxProperties.append(computeAuxProperty(stringTable.name(code)));
    }
  return code;
}

Rope
Token::stringToRope(const char* string)
{
  Assert(*string == '"', "string must start with \"");
  Rope result;
  bool seenBackslash = false;
  for (const char* p = string + 1; *p; p++)
    {
      char c = *p;
      switch (c)
	{
	case '\\':
	  {
	    if (!seenBackslash)
	      {
		seenBackslash = true;
		continue;
	      }
	    break;
	  }
	case '"':
	  {
	    if (!seenBackslash)
	      return result;
	    break;
	  }
	case 'a':
	  {
	    if (seenBackslash)
	      c = '\a';
	    break;
	  }
	case 'b':
	  {
	    if (seenBackslash)
	      c = '\b';
	    break;
	  }
	case 'f':
	  {
	    if (seenBackslash)
	      c = '\f';
	    break;
	  }
	case 'n':
	  {
	    if (seenBackslash)
	      c = '\n';
	    break;
	  }
	case 'r':
	  {
	    if (seenBackslash)
	      c = '\r';
	    break;
	  }
	case 't':
	  {
	    if (seenBackslash)
	      c = '\t';
	    break;
	  }
	case 'v':
	  {
	    if (seenBackslash)
	      c = '\v';
	    break;
	  }
	default:
	  {
	    if (seenBackslash && isdigit(c) && c != '8' && c != '9')
	      {
		int i = c - '0';
	        c = *(p + 1);
		if (isdigit(c) && c != '8' && c != '9')
		  {
		    ++p;
		    i = 8 * i + c - '0';
		    c = *(p + 1);
		    if (isdigit(c) && c != '8' && c != '9')
		      {
			++p;
			i = 8 * i + c - '0';
		      }
		  }
		c = i;
	      }
	  }
	}
      result += c;
      seenBackslash = false;
    }
  CantHappen("bad end to string");
  return result;
}

void
Token::ropeToString(const Rope& r, string& result)
{
  result = '"';
  for (Rope::const_iterator i = r.begin(); i != r.end(); ++i)
    {
      char c = *i;
      if (isprint(c))
	{
	  if (c == '\\' || c == '"')
	    result += '\\';
	  result += c;
	}
      else
	{
	  switch (c)
	    {
	    case '\a':
	      {
		result += "\\a";
		break;
	      }
	    case '\b':
	      {
		result += "\\b";
		break;
	      }
	    case '\f':
	      {
		result += "\\f";
		break;
	      }
	    case '\n':
	      {
		result += "\\n";
		break;
	      }
	    case '\r':
	      {
		result += "\\r";
		break;
	      }
	    case '\t':
	      {
		result += "\\t";
		break;
	      }
	    case '\v':
	      {
		result += "\\v";
		break;
	      }
	    default:
	      {
		result += '\\';
		int t = static_cast<unsigned char>(c);  // +ve int value of char
		result += '0' + t / 64;
		result += '0' + (t / 8) % 8;
		result += '0' + t % 8;
	      }
	    }
	}
    }
  result += '"';
}

int
Token::ropeToCode(const Rope& r)
{
  //
  //	Might want to do the code computation directly at some point.
  //
  char* s = r.makeZeroTerminatedString();
  int code = encode(s);
  delete [] s;
  return code;
}

int
Token::ropeToPrefixNameCode(const Rope& r)
{
  string result;
  bool needBQ = false;
  bool lastCharSpecial = false;
  bool stringMode = false;
  bool seenBS = false;
  for (Rope::const_iterator i = r.begin(); i != r.end(); ++i)
    {
      char c = *i;
      if (stringMode)
	{
	  if(!isprint(c))
	    return NONE;
	  result += c;
	  if (c == '\\')
	    seenBS = !seenBS;
	  else
	    {
	      if (c == '"' && !seenBS)
		stringMode = false;
	      seenBS = false;
	    }
	}
      else
	{
	  if (isspace(c) || c == '`')
	    needBQ = !lastCharSpecial && !(result.empty());
	  else if(!isprint(c))
	    return NONE;
	  else
	    {
	      lastCharSpecial = specialChar(c) || c == '_';
	      if ((lastCharSpecial || needBQ) && (c != '_'))
		result += '`';
	      result += c;
	      if (c == '"')
		stringMode = true;
	      needBQ = false;
	    }
	}
    }
  return stringMode ? NONE : encode(result.c_str());
}

int
Token::bubbleToPrefixNameCode(const Vector<Token>& opBubble)
{
  int nrTokens = opBubble.size();
  if (nrTokens == 1)
    {
      int code = opBubble[0].codeNr;
      //      if (specialProperties[code] == STRING)
      //        return code;  // NEED TO FIX QUOTED STRINGS
      if (!specialChar(stringTable.name(code)[0]))
        return code;
    }
  bool needBQ = false;
  buffer.clear();
  for (int i = 0; i < nrTokens; i++)
    {
      const char* name = stringTable.name(opBubble[i].codeNr);
      char c = name[0];
      if (specialChar(c))
        needBQ = true;
      else if (c == '_' || c == '`')
        needBQ = false;
      if (needBQ)
	buffer.push_back('`');
      while (*name != '\0')
	{
	  c = *name++;
	  buffer.push_back(c);
	}
      needBQ = !(specialChar(c) || c == '_');
    }
  buffer.push_back('\0');
  return encode(buffer.data());
}

void
Token::getRational(mpz_class& numerator, mpz_class& denominator)
{
  const char* name = stringTable.name(codeNr);
  char* s = new char[strlen(name) + 1];
  strcpy(s, name);
  char* p = index(s, '/');
  Assert(p != 0, "no /");
  *p = '\0';
  mpz_set_str(numerator.get_mpz_t(), s, 10);
  mpz_set_str(denominator.get_mpz_t(), p + 1, 10);
}

void
Token::peelParens(Vector<Token>& tokens)
{
  //
  //	If tokens look like ( ... ) with middle part having balanced parens, peel outer parens.
  //
  int len = tokens.size();
  if (len <= 2)
    return;
  int open = encode("(");
  int close = encode(")");
  if (tokens[0].codeNr != open && tokens[len - 1].codeNr != close)
    return;
  int depth = 0;
  for (int i = 1; i < len - 1; ++i)
    {
      int codeNr = tokens[i].codeNr;
      if (codeNr == open)
	++depth;
      else if (codeNr == close)
	{
	  --depth;
	  if (depth < 0)
	    return;
	}
    }
  if (depth != 0)
    return;
  for (int i = 1; i < len - 1; ++i)
    tokens[i - 1] = tokens[i];
  tokens.resize(len - 2);
}
