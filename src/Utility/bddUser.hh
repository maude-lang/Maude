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
//	Class to manage a single BDD package for multiple users.
//
#ifndef _bddUser_hh_
#define _bddUser_hh_
#include "bdd.h"

class BddUser
{
public:
  typedef void ErrorHandler(int errorNr);

  BddUser();
  ~BddUser();

  static bdd ithvar(int i);
  static bdd nithvar(int i);
  static void setNrVariables(int nrVariables);
  static void dump(ostream& s, bdd root);
  static void setErrorHandler(ErrorHandler* errHandler);

private:
  enum Constants
  {
    DEFAULT_NODE_SIZE = 1000,
    DEFAULT_CACHE_SIZE = 100,
    DEFAULT_NR_VARIABLES = 10
  };

  static void gc_handler(int pre, bddGbcStat* stat);
  static void err_handler(int errcode);

  static int nrUsers;

  static ErrorHandler* errorHandler;
};

inline bdd
BddUser::ithvar(int i)
{
  if (i >= bdd_varnum())
    bdd_setvarnum(i + 1);
  return bdd_ithvar(i);
}

inline bdd
BddUser::nithvar(int i)
{
  if (i >= bdd_varnum())
    bdd_setvarnum(i + 1);
  return bdd_nithvar(i);
}

inline void
BddUser::setNrVariables(int nrVariables)
{
  if (nrVariables > bdd_varnum())
    bdd_setvarnum(nrVariables);
}

inline void
BddUser::setErrorHandler(ErrorHandler* errHandler)
{
  errorHandler = errHandler;
}

#endif
