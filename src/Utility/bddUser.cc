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
//	Implementation for class BddUser.
//

//	utility stuff
#include "macros.hh"
#include "bddUser.hh"

int BddUser::nrUsers = 0;
BddUser::ErrorHandler* BddUser::errorHandler = 0;
bddPair* BddUser::cachedPairing = 0;

BddUser::BddUser()
{
  if (nrUsers == 0)
    {
      bdd_init(DEFAULT_NODE_SIZE, DEFAULT_CACHE_SIZE);
      bdd_setvarnum(DEFAULT_NR_VARIABLES);
      bdd_gbc_hook(gc_handler);
      bdd_error_hook(err_handler);
    }
  ++nrUsers;
}

void
BddUser::gc_handler(int pre, bddGbcStat* stat)
{
  // We don't output any message.
}

void
BddUser::err_handler(int errcode)
{
  if (errorHandler != 0)
    (*errorHandler)(errcode);
  else
    CantHappen("buddy error " << errcode);  // need an abort() for debugging
}

BddUser::~BddUser()
{
  --nrUsers;
  if (nrUsers == 0)
    {
      if (cachedPairing != 0)
	{
	  bdd_freepair(cachedPairing);
	  cachedPairing = 0;
	}
      bdd_done();
    }
}

void
BddUser::dump(ostream& s, bdd root)
{
  if (root == bdd_false())
    s << "false";
  else if (root == bdd_true())
    s << "true";
  else
    {
      int var = bdd_var(root);
      bdd lo = bdd_low(root);
      if (lo != bdd_false())
	{
	  s << "~x" << var;
	  if (lo != bdd_true())
	    {
	      s << ".(";
	      dump(s, lo);
	      s << ')';
	    }
	}
      bdd hi = bdd_high(root);
      if (hi != bdd_false())
	{
	  if (lo != bdd_false())
	    s << " + ";
	  s << "x" << var;
	  if (hi != bdd_true())
	    {
	      s << ".(";
	      dump(s, hi);
	      s << ')';
	    }
	}
    }
}
