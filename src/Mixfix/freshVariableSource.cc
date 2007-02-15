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
  : module(module)
{
  counter = 0;
  name[1 + INT_TEXT_SIZE] = '\0';
}

int
FreshVariableSource::getFreshVariableName()
{
  char* p = name + INT_TEXT_SIZE;
  ++counter;
  for (int i = counter; i != 0; i /= 10, --p)
    *p = '0' + i % 10;
  *p = '#';
  return Token::encode(p);
}

Symbol*
FreshVariableSource::getBaseVariableSymbol(Sort* sort)
{
  return module->instantiateVariable(sort);
}

void
FreshVariableSource::reset()
{
  counter = 0;
}
