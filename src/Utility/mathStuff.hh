/*

    This file is part of the Maude 3 interpreter.

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
//	Put the math inclusion mess in a single place.
//
#ifndef _mathStuff_hh_
#define _mathStuff_hh_
#include <cmath>
#if HAVE_IEEEFP_H
#include <ieeefp.h>
#endif
//
//	Darwin fix from Fabricio Chalub <fc@gnu.org>
//
#if defined(DARWIN) && !defined(isnan)
extern "C" int isnan(double);
#endif
#endif
