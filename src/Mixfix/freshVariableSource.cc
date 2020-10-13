/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2007 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class FreshVariableSource.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "higher.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//	core class definitions
#include "symbolMap.hh"

//	front end class definitions
#include "token.hh"
#include "mixfixModule.hh"
#include "freshVariableSource.hh"

FreshVariableSource::FreshVariableSource(MixfixModule* module)
  : module(module),
    baseNumber(0)
{
}

FreshVariableSource::FreshVariableSource(MixfixModule* module, const mpz_class& baseNumber)
  : module(module),
    baseNumber(baseNumber)
{
}

int
FreshVariableSource::getFreshVariableName(int index, int family)
{
  Vector<int>& cacheToUse = caches[family];
  char charToUse = "#%@"[family];

  int nrCached = cacheToUse.size();
  if (index < nrCached)
    {
      int t = cacheToUse[index];
      if (t >= 0)
	return t;
    }
  //
  //	In order to avoid allocating the name twice we convert the negative index to a
  //	string and replace the minus sign with a '#', '%' or '@'
  //
  //	We ultimately map index to (index + baseNumber + 1).
  //
  int negIndex = -(index + 1);
  mpz_class negativeIndex = negIndex - baseNumber;
  char* name = mpz_get_str(0, 10, negativeIndex.get_mpz_t());
  name[0] = charToUse;
  int code = Token::encode(name);
  free(name);
  //
  //	Cache newly computed name index.
  //
  if (index >= nrCached)
    {
      cacheToUse.resize(index + 1);
      for (int i = nrCached; i < index; ++i)
	cacheToUse[i] = -1;
    }
  cacheToUse[index] = code;
  return code;
}

Symbol*
FreshVariableSource::getBaseVariableSymbol(Sort* sort)
{
  return module->instantiateVariable(sort);
}

bool
FreshVariableSource::variableNameConflict(int id, int okFamily)
{
  //
  //	If an identifier looks like an internally generated variable name, check to see if
  //	the index > baseNumber as this will produce a potential conflict with fresh variables.
  //	We now allow variable of okFamily (which could be NONE).
  //
  const char* name = Token::name(Token::unflaggedCode(id));
  char c = name[0];
  if (c == '#')
    {
      if (okFamily == 0)
	return false;
    }
  else if (c == '%')
    {
      if (okFamily == 1)
	return false;
    }
  else if (c == '@')
    {
      if (okFamily == 2)
	return false;
    }
  else
    return false;

  if (name[1] == '0' || name[1] == '\0')
    return false;

  for (const char* p = name + 1; *p; ++p)
    {
      if (!isdigit(*p))
	return false;
    }
  //
  //	name looks like a fresh variable name so we need to get its index.
  //
  mpz_class index(name + 1);
  return index > baseNumber;
}

int
FreshVariableSource::getBaseName(int index)
{
  const char* rootChars[] = {"#", "%", "@"};
  return Token::encode(rootChars[index]);
}

int
FreshVariableSource::getFamily(int id)
{
  const char* name = Token::name(id);
  if (name[0] == '\0')
    return NONE;  // empty string
  if (name[1] != '\0')
    return NONE;  // more than 1 char
  if (name[0] == '#')
    return 0;
  if (name[0] == '%')
    return 1;
  if (name[0] == '@')
    return 2;
  return NONE;
}

bool
FreshVariableSource::belongsToFamily(int id, int family)
{
  const char* name = Token::name(id);
  const char rootChars[] = {'#', '%', '@'};
  if (name[0] != rootChars[family])
    return false;
  if (!(isdigit(name[1]) || name[1] == '0'))
    return false;
  for (int i = 2; name[i] != '\0'; ++i)
    {
      if (!(isdigit(name[i])))
	return false;
    }
  return true;
}

bool
FreshVariableSource::isFreshVariableName(int id, int& index, int& family)
{
  const char* name = Token::name(id);
  char firstChar = name[0];
  if (firstChar == '#')
    family = 0;
  else if (firstChar == '%')
    family = 1;
  else if (firstChar == '@')
    family = 2;
  else
    return false;
  char secondChar = name[1];
  if (!isdigit(secondChar) || secondChar == '0')
    return false;  // index must start with 1 thru 9
  mpz_class fullIndex;
  mpz_set_str(fullIndex.get_mpz_t(), name + 1, 10);
  --fullIndex;
  if (fullIndex > INT_MAX)
    {
      //
      //	We don't care about indices > INT_MAX because
      //	we never generate such names ourselves.
      //
      return false;
    }
  index = fullIndex.get_si();
  return true;
}
