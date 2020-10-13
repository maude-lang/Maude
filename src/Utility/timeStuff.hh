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
//	Put the time inclusion mess in a single place
//	(code taken from the autoconf manual).
//
#ifndef _timeStuff_hh_
#define _timeStuff_hh_
#if TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# if HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

inline long
timespecCompare(const timespec* first, const timespec* second)
{
  //
  //	Compare two timespec structs
  //
  //	Return < 0 if first < second
  //	       = 0 if first = second
  //	       > 0 if first > second
  //
  long diff = first->tv_sec - second->tv_sec;
  return (diff == 0) ? (first->tv_nsec - second->tv_nsec) : diff;
}

inline void
timespecSubtract(const timespec* first,
		 const timespec* second,
		 timespec* result)
{
  //
  //	Subtract two timespec structs, assuming first >= second
  //
  long diff = first->tv_sec - second->tv_sec;
  long ndiff = first->tv_nsec - second->tv_nsec;
  if (ndiff < 0)
    {
      ndiff += 1000000000L;
      diff -= 1;
    }
  result->tv_sec = diff;
  result->tv_nsec = ndiff;
}

#endif
