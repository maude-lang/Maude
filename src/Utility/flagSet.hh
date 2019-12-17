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
//      Class for a set of 32 flags.
//
#ifndef _flagSet_hh_
#define _flagSet_hh_

class FlagSet
{
public:
  FlagSet(int initFlags = 0);

  bool getFlag(int flag) const;
  bool oneOrMoreSet(int activeFlags) const;
  bool allSet(int activeFlags) const;

  void clearFlags(int activeFlags);
  void setFlags(int activeFlags);

private:
  int flags;
};

inline
FlagSet::FlagSet(int initFlags)
{
  flags = initFlags;
}

inline bool
FlagSet::getFlag(int flag) const
{
  return flags & flag;
}

inline bool
FlagSet::oneOrMoreSet(int activeFlags) const
{
  return flags & activeFlags;
}

inline bool
FlagSet::allSet(int activeFlags) const
{
  return (flags & activeFlags) == activeFlags;
}

inline void
FlagSet::clearFlags(int activeFlags)
{
  flags &= ~activeFlags;
}

inline void
FlagSet::setFlags(int activeFlags)
{
  flags |= activeFlags;
}

#endif
