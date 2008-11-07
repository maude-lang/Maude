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
//      Class for creating fresh variables for unification.
//
#ifndef _freshVariableSource_hh_
#define _freshVariableSource_hh_
#include <gmpxx.h>
#include "freshVariableGenerator.hh"

class FreshVariableSource : public FreshVariableGenerator
{
public:
  FreshVariableSource(MixfixModule* module);
  FreshVariableSource(MixfixModule* module, const mpz_class& baseNumber);
  int getFreshVariableName(int index);
  Symbol* getBaseVariableSymbol(Sort* sort);
  bool variableNameConflict(int id);

private:
  MixfixModule* const module;
  mpz_class baseNumber;
  Vector<int> cache;
};

#endif
