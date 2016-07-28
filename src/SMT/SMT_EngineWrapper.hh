/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Abstract base class for wrapping SMT engines.
//
#ifndef _SMT_EngineWrapper_hh_
#define _SMT_EngineWrapper_hh_
#include "gmpxx.h"

class SMT_EngineWrapper
{
public:
  enum Result
    {
      BAD_DAG = -2,
      SAT_UNKNOWN = -1,
      UNSAT = 0,
      SAT = 1
    };

  virtual ~SMT_EngineWrapper() {}

  //
  //	Extra functionality for when we do full abstraction of SMT solving.
  //
  virtual Result assertDag(DagNode* dag) = 0;
  virtual Result checkDag(DagNode* dag) = 0;
  virtual void clearAssertions() = 0;
  virtual void push() = 0;
  virtual void pop() = 0;
  //
  //	Make a Maude variable corresponding to a fresh SMT variable.
  //
  virtual VariableDagNode* makeFreshVariable(Term* baseVariable, const mpz_class& number) = 0;
};

#endif
