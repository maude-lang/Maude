/*

    This file is part of the Maude 2 interpreter.

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
FreshVariableSource::getFreshVariableName(int index)
{
  //
  //	Check to see if we've aready computed a variable name for this index.
  //
  int nrCached = cache.size();
  if (index < nrCached)
    {
      int t = cache[index];
      if (t >= 0)
	return t;
    }
  //
  //	In order to avoid allocating the name twice we convert the negative index to a
  //	string and replace the minus sign with a '#'.
  //
  int negIndex = -(index + 1);
  mpz_class negativeIndex = negIndex - baseNumber;
  char* name = mpz_get_str (0, 10, negativeIndex.get_mpz_t());
  name[0] = '#';
  int code = Token::encode(name);
  free(name);
  //
  //	Cache newly computed name index.
  //
  if (index >= nrCached)
    {
      cache.resize(index + 1);
      for (int i = nrCached; i < index; ++i)
	cache[i] = -1;
    }
  cache[index] = code;
  return code;
}

Symbol*
FreshVariableSource::getBaseVariableSymbol(Sort* sort)
{
  return module->instantiateVariable(sort);
}

bool
FreshVariableSource::variableNameConflict(int id)
{
  const char* name = Token::name(Token::unflaggedCode(id));
  if (name[0] != '#' || name[1] == '0' || name[1] == '\0')
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
