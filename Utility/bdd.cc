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
//	Implementation for class Bdd.
//

//	utility stuff
#include "macros.hh"
#include "bdd.hh"

Bdd
Bdd::extractPrimeImplicant() const
{
  if (*this == bdd_false() || *this == bdd_true())
    return *this;

  Bdd hi = bdd_high(*this);
  Bdd lo = bdd_low(*this);
  int var = bdd_var(*this);

  if (hi == bdd_false())
    return bdd_nithvar(var) & lo.extractPrimeImplicant();
  
  Bdd pi = hi.extractPrimeImplicant();
  if (bdd_restrict(lo, pi) == bdd_true())
    return pi;

  return bdd_ithvar(var) & pi;
}
