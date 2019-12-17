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
//      Class for operator level strategies.
//
#ifndef _strategy_hh_
#define _strategy_hh_
#include "natSet.hh"

class Strategy
{
public:
  Strategy();

  void setStrategy(const Vector<int>& userStrategy, int nrArgs, bool memoFlag);
  virtual void setFrozen(const NatSet& frozen);
  const Vector<int>& getStrategy() const;
  const NatSet& getFrozen() const;
  bool hasFrozenArguments() const;
  bool standardStrategy() const;
  bool unevaluatedArguments() const;
  bool eagerArgument(int argNr) const;
  bool evaluatedArgument(int argNr) const;

protected:
  static const Vector<int> standard;

private:
  Bool stdStrategy;
  Bool unevalArgs;
  Vector<int> strategy;
  NatSet eager;
  NatSet evaluated;
  NatSet frozen;
};

inline const NatSet&
Strategy::getFrozen() const
{
  return frozen;
}

inline const Vector<int>&
Strategy::getStrategy() const
{
  return strategy;
}

inline bool
Strategy::standardStrategy() const
{
  return stdStrategy;
}

inline bool
Strategy::unevaluatedArguments() const
{
  return unevalArgs;
}

inline bool
Strategy::eagerArgument(int argNr) const
{
  return stdStrategy || eager.contains(argNr);
}

inline bool
Strategy::evaluatedArgument(int argNr) const
{
  return stdStrategy || evaluated.contains(argNr);
}

#endif
