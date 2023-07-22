/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for parser tokens.
//
#ifndef _token_hh_
#define _token_hh_
#include <gmpxx.h>
#include <vector>
#include "rope.hh"
#include "stringTable.hh"
#include "lineNumber.hh"

class Token
{
public:
  //
  //	Can't have a constructor or destructor because
  //	we want to be in the bison stack union.
  //
  enum SpecialProperties
  {
    SMALL_NAT,
    SMALL_NEG,
    ZERO,
    QUOTED_IDENTIFIER,
    STRING,
    FLOAT,
    CONTAINS_COLON,
    ENDS_IN_COLON,
    ITER_SYMBOL,
    RATIONAL,
    LAST_PROPERTY
  };

  enum AuxProperties
  {
    AUX_SORT,
    AUX_STRUCTURED_SORT,
    AUX_VARIABLE,
    AUX_CONSTANT,
    AUX_KIND
  };

  void tokenize(const char* tokenString, int lineNumber);
  void tokenize(int code, int lineNumber);
  static int makeParameterInstanceName(int parameterCode, int originalCode);
  void fixUp(const char* tokenString, int& lineNumber);
  void dropChar(const Token& original);

  void makePrefixName(const Vector<Token>& opBubble);
  const char* name() const;
  int code() const;
  int lineNumber() const;
  LineNumber getLineNr() const;
  int specialProperty() const;
  int auxProperty() const;
  bool getInt(int& value) const;
  bool containsUnderscore() const;

  static const char* name(int code);
  static Rope sortName(int code);

  static int specialProperty(int code);
  static int auxProperty(int code);
  static int encode(const char* tokenString);
  static int dotNameCode(int sortNameCode);
  static int quoteNameCode(int idCode);
  static int extractMixfix(int prefixNameCode, Vector<int>& mixfixSyntax);
  static bool specialChar(char c);

  static int backQuoteSpecials(int code);
  static int unBackQuoteSpecials(int code);
  static void printTokenVector(ostream& s,
			       const Vector<Token>& tokens,
			       int first,
			       int last,
			       bool fancySpacing);

  static bool split(int code, int& prefix, int& suffix);
  static bool split(int code, int& opName, mpz_class& number);
  static bool splitKind(int code, Vector<int>& codes);
  static void splitParameterizedSort(int code, Vector<int>& codes);
  static void splitParameterList(int code, int& header, Vector<int>& parameters);
  static int joinParameterList(int header, const Vector<int>& parameters);
  static Int64 codeToInt64(int code);
  static int int64ToCode(Int64 i);
  static double codeToDouble(int code);
  static int doubleToCode(double d);
  static Rope codeToRope(int code);
  static Rope stringToRope(const char* string);
  static void ropeToString(const Rope& rope, string& result);
  static int ropeToCode(const Rope& r);
  static int ropeToPrefixNameCode(const Rope& r);
  static int bubbleToPrefixNameCode(const Vector<Token>& opBubble);
  void getRational(mpz_class& numerator, mpz_class& denominator);
  static void printTokens(ostream& s,
			  const Vector<Token>& tokens,
			  const char* seperator);
  static void peelParens(Vector<Token>& tokens);
  static void peelParen(Vector<Token>& tokens);
  static int flaggedCode(int code);
  static bool isFlagged(int code);
  static int unflaggedCode(int code);
  static int fixUp(const char* tokenString);
  static Rope removeBoundParameterBrackets(int code);
  static string prettyOpName(int prefixNameCode);

  static string latexName(const char* name);
  static string latexName(int code);
  static string latexName(const string& name);
  static string latexName(const Rope& name);
  static string latexIdentifier(int code);

private:


  enum SpecialValues
    {
      FLAG_BIT = 0x40000000	// we set this bit to create flagged codes
    };

  static void checkForSpecialProperty(const char* tokenString);
  static int computeSpecialProperty(const char* tokenString);
  static int computeAuxProperty(const char* tokenString);
  static const char* skipSortName(const char* tokenString, bool& parameterized);
  static bool looksLikeRational(const char* s);
  static bool hasNonAlphanumeric(const char* name);

  static StringTable stringTable;
  static Vector<int> specialProperties;
  static Vector<int> auxProperties;
  static Vector<char> buffer;

  int codeNr;
  int lineNr;
};

inline void
Token::makePrefixName(const Vector<Token>& opBubble)
{
  codeNr = bubbleToPrefixNameCode(opBubble);
  lineNr = opBubble[0].lineNr;
}

inline int
Token::encode(const char* tokenString)
{
  int code = stringTable.encode(tokenString);
  if (code == specialProperties.length())
    checkForSpecialProperty(tokenString);
  return code;
}

inline int
Token::specialProperty() const
{
  return specialProperties[codeNr];
}

inline int
Token::auxProperty() const
{
  return auxProperties[codeNr];
}

inline int
Token::specialProperty(int code)
{
  return specialProperties[code];
}

inline int
Token::auxProperty(int code)
{
  return auxProperties[code];
}

inline bool
Token::specialChar(char c)
{
  return c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c  == ',';
}

inline int
Token::backQuoteSpecials(int code)
{
  const char* s = stringTable.name(code);
  char c = s[0];
  if (specialChar(s[0]) && s[1] == '\0')
    {
      char t[3];
      t[0] = '`';
      t[1] = c;
      t[2] = '\0';
      return encode(t);
    }
  return code;
}

inline int
Token::unBackQuoteSpecials(int code)
{
  const char* s = stringTable.name(code);
  if (s[0] == '`' && specialChar(s[1]) && s[2] == '\0')
    return encode(s + 1);
  return code;
}

inline void
Token::tokenize(const char* tokenString, int lineNumber)
{
  codeNr = encode(tokenString);
  lineNr = lineNumber;
}

inline void
Token::tokenize(int code, int lineNumber)
{
  Assert(code >= 0, "bad code = " << code);
  codeNr = code;
  lineNr = lineNumber;
}

inline const char*
Token::name() const
{
  return stringTable.name(codeNr);
}

inline int
Token::code() const
{
  return codeNr;
}

inline int
Token::lineNumber() const
{
  return lineNr;
}

inline LineNumber
Token::getLineNr() const
{
  return LineNumber(lineNr);
}

inline const char*
Token::name(int code)
{
  return stringTable.name(code);
}

inline bool
Token::containsUnderscore() const
{
  // might want to cache this as a flag in specialProperties
  return index(name(), '_') != 0;
}

inline int
Token::flaggedCode(int code)
{
  return code | FLAG_BIT;
}

inline bool
Token::isFlagged(int code)
{
  return code & FLAG_BIT;
}

inline int
Token::unflaggedCode(int code)
{
  return code & ~FLAG_BIT;
}

inline Rope
Token::codeToRope(int code)
{
  return stringToRope(stringTable.name(code));
}

ostream& operator<<(ostream& s, const Token& token);
ostream& operator<<(ostream& s, const Vector<Token>& tokens);
ostream& operator<<(ostream& s, const Token* token);

#endif
