/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2003 SRI International, Menlo Park, CA 94025, USA.

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

#include "token.hh"

StringTable Token::stringTable;
Vector<int> Token::specialProperties;
Vector<int> Token::auxProperties;
char* Token::buffer = 0;
int Token::bufferLength = 0;

ostream&
operator<<(ostream& s, const Token& token)
{
  s << token.name();
  return s;
}

ostream&
operator<<(ostream& s, const Vector<Token>& tokens)
{
  /*
  bool needSpace = false;
  int nrTokens = tokens.length();
  for (int i = 0; i < nrTokens; i++)
    {
      const char* name = tokens[i].name();
      bool special = Token::specialChar(name[0]) && name[1] == '\0';
      if (needSpace && !special)
	s << ' ';
      s << name;
      needSpace = !special;
    }
  */
  Token::printTokenVector(s, tokens, 0, tokens.length() - 1, true);
  return s;
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

void
Token::reallocateBuffer(int length)
{
  length *= 2;  // to avoid piecemeal expansion
  char* newBuffer = new char[length];
  (void) memcpy(newBuffer, buffer, bufferLength);
  delete [] buffer;
  buffer = newBuffer;
  bufferLength = length;
}

void
Token::fixUp(const char* tokenString, int& lineNumber)
{
  int nrBackslashNewlineCombos = 0;
  int j = 0;
  for (int i = 0;; i++)
    {
      char c = tokenString[i];
      if (c == '\\' && tokenString[i + 1] == '\n')
	{
	  //
	  //	Fix up \ newline case.
	  //      if (c == '\0')
	  //
	  ++i;
	  ++nrBackslashNewlineCombos;
	}
      else
	{  
	  bufferExpandTo(j + 1);
	  buffer[j] = c;
	  ++j;
	  if (c == '\0')
	    break;
	}
    } 
  codeNr = encode(buffer);
  lineNr = lineNumber;
  lineNumber += nrBackslashNewlineCombos;
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
/*
int
Token::extractMixfix(int prefixNameCode, Vector<int>& mixfixSyntax)
{
  
  if (specialProperties[prefixNameCode] != NONE)
    return 0;
  const char* name = stringTable.name(prefixNameCode);
  const char* p = name;
  for (;; p++)
    {
      char c = *p;
      if (c == '\0')
	return 0;
      if (c == '`' || c == '_')
	break;
    }
  //
  //	Has mixfix syntax.
  //
  int nrUnderscores = 0;
  bufferExpandTo(strlen(name) + 1);
  int j = p - name;
  strncpy(buffer, name, j);
  for (;; p++)
    {
      char c = *p;
      if (c == '\0')
	break;
      else if (c == '`')
	{
	  if (j > 0)
	    {
	      buffer[j] = '\0';
	      mixfixSyntax.append(encode(buffer));
	      j = 0;
	    }
	  continue;
	}
      else if (c == '_')
	++nrUnderscores;
      else if (!specialChar(c))
	{
	  buffer[j++] = c;
	  continue;
	}
      if (j > 0)
	{
	  buffer[j] = '\0';
	  mixfixSyntax.append(encode(buffer));
	  j = 0;
	}
      buffer[0] = c;
      buffer[1] = '\0';
      mixfixSyntax.append(encode(buffer));
    }
  if (j > 0)
    {
      buffer[j] = '\0';
      mixfixSyntax.append(encode(buffer));
    }
  return nrUnderscores;
}

*/

int
Token::extractMixfix(int prefixNameCode, Vector<int>& mixfixSyntax)
{
  
  int sp = specialProperties[prefixNameCode];
  if (sp != NONE && sp != CONTAINS_COLON && sp != ENDS_IN_COLON &&
      sp != ITER_SYMBOL)
    return 0;
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
  //	Has mixfix syntax.
  //
  int nrUnderscores = 0;
  bool stringMode = false;
  bool seenBS = false;
  string token(name, p - name);
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
	    ++nrUnderscores;
	  else if (!specialChar(c))
	    {
	      token += c;
	      if (c == '"')
		stringMode = true;
	      continue;
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
  return nrUnderscores;
}

const char*
Token::skipSortName(const char* tokenString)
{
  for (const char* p = tokenString;; p++)
    {
      switch (*p)
	{
	case '\0':
	  return p == tokenString ? 0 : p;
	case '.':
	case ':':
	  return p;
	case '`':
	  {
	    switch (*(p + 1))
	      {
	      case '[':
	      case ']':
	      case ',':
		return p;
	      case '\0':
		return 0;
	      }
	    break;
	  }
	}
    }
}

int
Token::computeAuxProperty(const char* tokenString)
{
  {
    //
    //	Check for kind.
    //
    const char* p = tokenString;
    if (*p++ == '`' && *p == '[')
      {
	for(;;)
	  {
	    p = skipSortName(p);
	    if (p != 0 && *p++ == '`')
	      {
		switch (*p++)
		  {
		  case ']':
		    {
		      if (*p == '\0')
			return AUX_KIND;
		      break;
		    }
		  case ',':
		    continue;
		  }
	      }
	    break;
	  }
      }
  }
  {
    //
    //	Check for sort.
    //
    const char* p = skipSortName(tokenString);
    if (p != 0 && *p == '\0')
      return AUX_SORT;
  }
  {
    //
    //	Check for constant or variable.
    //
    int len = strlen(tokenString);
    for (int i = len - 1; i > 0; i--)
      {
	char c = tokenString[i];
	if (c == '.')
	  {
	    int t = computeAuxProperty(tokenString + i + 1);
	    if (t == AUX_SORT || t == AUX_KIND)
	      return AUX_CONSTANT;
	    break;
	  }
	else if (c == ':')
	  {
	    int t = computeAuxProperty(tokenString + i + 1);
	    if (t == AUX_SORT || t == AUX_KIND)
	      return AUX_VARIABLE;
	    break;
	  }
      }
  }
  return NONE;
}

void
Token::checkForSpecialProperty(const char* tokenString)
{
  int tokenNr = specialProperties.length();
  specialProperties.expandBy(1);
  specialProperties[tokenNr] = NONE;
  auxProperties.expandBy(1);
  auxProperties[tokenNr] = computeAuxProperty(tokenString);

  const char* p = tokenString;
  if (*p == 0)
    return;  // handle null strings correctly
  if (*p == '\'')
    {
      specialProperties[tokenNr] = QUOTED_IDENTIFIER;
      return;
    }
  if (*p == '"')
    {
      bool seenBackslash = false;
      for (p++; *p; p++)
	{
	  char c = *p;
	  if (c == '\\')
	    seenBackslash = !seenBackslash;
	  else
	    {
	      if (c == '"' && !seenBackslash)
		{
		  if (*(p + 1) == '\0')
		    specialProperties[tokenNr] = STRING;
		  return;
		}
	      seenBackslash = false;
	    }
	}
      return;
    }
  {
    size_t len = strlen(tokenString);
    for (size_t i = len - 1; i > 0; --i)
      {
	char c = tokenString[i];
	if (c == '^')
	  {
	    if (i == len - 1 || tokenString[i + 1] == '0')
	      break; 
	    specialProperties[tokenNr] = ITER_SYMBOL;
	    return;
	  }
	if (!isdigit(c))
	  break;
      }
  }
  for (++p; *p; ++p)
    {
      if (*p == ':')
        specialProperties[tokenNr] = *(p + 1) ? CONTAINS_COLON : ENDS_IN_COLON;
    }
  if (specialProperties[tokenNr] != NONE)
    return;
  if (looksLikeFloat(tokenString))
    {
      specialProperties[tokenNr] = FLOAT;
      return;
    }

  mpz_class i;
  int error = mpz_set_str(i.get_mpz_t(), tokenString, 10);
  if (error == 0)
    {
      specialProperties[tokenNr] = (i == 0) ? ZERO : ((i < 0) ? SMALL_NEG : SMALL_NAT);
      return;
    }

  if (looksLikeRational(tokenString))
    {
      specialProperties[tokenNr] = RATIONAL;
      return;
    }
    
  /*
  bool error;
  Int64 i = stringToInt64(tokenString, error, 10);
  if (!error)
    specialProperties[tokenNr] = (i == 0) ? ZERO : ((i < 0) ? SMALL_NEG : SMALL_NAT);
  */
}

bool
Token::looksLikeRational(const char* s)
{
  if (*s == '-')
    s++;
  {
    char c = *s++;
    if (!isdigit(c) || c == '0')
      return false;
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
  const char* p = stringTable.name(code);
  size_t len = strlen(p);
  char* t = new char[len + 1];
  strcpy(t, p);
  for (size_t i = len - 1; i > 0; --i)
    {
      char c = t[i];
      if (c == ':' || c == '.')
	{
	  t[i] = '\0';
	  prefix = encode(t);
	  suffix = (i == len - 1) ? NONE : encode(t + i + 1);
	  delete [] t;
	  return true;
	}
    }
  delete [] t;
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
  codes.contractTo(0);
  const char* p = stringTable.name(code);
  size_t len = strlen(p);
  char* t = new char[len + 1];
  p = strcpy(t, p);
  if (*p++ =='`' && *p++ == '[')
    {
      for(;;)
	{
	  char *p2 = const_cast<char*>(skipSortName(p));
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
		      {
			delete [] t;
			return true;
		      }
		  }
		case ',':
		  continue;
		}
	    }
	  break;
	}
    }
  delete [] t;
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
      specialProperties.append((i < 0) ? SMALL_NEG : SMALL_NAT);
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
      auxProperties.append(computeAuxProperty(stringTable.name(code)));
    }
  return code;
}

crope
Token::codeToRope(int code)
{
  crope result;
  bool seenBackslash = false;
  for (const char* p = stringTable.name(code) + 1; *p; p++)
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
      result.append(c);
      seenBackslash = false;
    }
  CantHappen("bad end to string");
  return result;
}

void
Token::ropeToString(const crope& r, string& result)
{
  result = '"';
  for (crope::const_iterator i = r.begin(); i != r.end(); ++i)
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
Token::ropeToPrefixNameCode(const crope& r)
{
  string result;
  bool needBQ = false;
  bool lastCharSpecial = false;
  bool stringMode = false;
  bool seenBS = false;
  for (crope::const_iterator i = r.begin(); i != r.end(); ++i)
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
  int r1;
  //#if GOOD
  {
  int nrTokens = opBubble.length();
  if (nrTokens == 1)
    {
      int code = opBubble[0].codeNr;
      if (!specialChar(stringTable.name(code)[0]))
	return code;
    }
  string result;
  bool lastCharSpecial = true;
  for (int i = 0; i < nrTokens; i++)
    {
      const char* name = stringTable.name(opBubble[i].codeNr);
      char c = name[0];
      Assert(c != '`', "can't start token with `");
      if (specialChar(c) || (!lastCharSpecial && c != '_'))
	result += '`';
      result += name;
      char l = result[result.length() - 1];
      lastCharSpecial = specialChar(l) || l == '_';
    }
  r1 = encode(result.c_str());
  //  return encode(result.c_str());
  }
  //#else
  int r2;
  {
  int nrTokens = opBubble.length();
  if (nrTokens == 1)
    {
      int code = opBubble[0].codeNr;
      //      if (specialProperties[code] == STRING)
      //        return code;  // NEED TO FIX QUOTED STRINGS
      if (!specialChar(stringTable.name(code)[0]))
        return code;
    }
  int pos = 0;
  bool needBQ = false;
  for (int i = 0; i < nrTokens; i++)
    {
      const char* name = stringTable.name(opBubble[i].codeNr);
      char c = name[0];
      if (specialChar(c))
        needBQ = true;
      else if (c == '_' || c == '`')
        needBQ = false;
      if (needBQ)
        {
          bufferExpandTo(pos + 1);
          buffer[pos++] = '`';
        }
      while (*name != '\0')
        {
          c = *name++;
          bufferExpandTo(pos + 1);
          buffer[pos++] = c;
        }
      needBQ = !(specialChar(c) || c == '_');
    }
  bufferExpandTo(pos + 1);
  buffer[pos] = '\0';
  r2 = encode(buffer);
  //  return encode(buffer);
  }
  //#endif
  if (r1 != r2)
    cerr << "PROBLEM: " << opBubble << '\n' << stringTable.name(r1) << '\n' <<
      stringTable.name(r2) << '\n';
  return r2;
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
