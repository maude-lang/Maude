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
//      Class for building instances of right hand sides.
//
#ifndef _rhsBuilder_hh_
#define _rhsBuilder_hh_
#include "rhsAutomaton.hh"

class RhsBuilder
{
public:
  RhsBuilder();
  ~RhsBuilder();

  void addRhsAutomaton(RhsAutomaton* automaton);
  bool empty();
  void remapIndices(VariableInfo& variableInfo);
  DagNode* construct(Substitution& matcher) const;
  void safeConstruct(Substitution& matcher) const;
  void replace(DagNode* old, Substitution& matcher) const;

  bool recordInfo(StackMachineRhsCompiler& compiler);

#ifdef DUMP
  void dump(ostream& s, const VariableInfo& variableInfo, int indentLevel = 0);
#endif

private:
  Vector<RhsAutomaton*> automata;
  RhsAutomaton* lastAutomaton;
};

inline
RhsBuilder::RhsBuilder()
{
  lastAutomaton = 0;
}

inline bool
RhsBuilder::empty()
{
  return lastAutomaton == 0;
}

inline void
RhsBuilder::addRhsAutomaton(RhsAutomaton* automaton)
{
  if (lastAutomaton != 0)
    automata.append(lastAutomaton);
  lastAutomaton = automaton;
}

inline DagNode*
RhsBuilder::construct(Substitution& matcher) const
{
  if (!automata.isNull())
    {
      Vector<RhsAutomaton*>::const_iterator i = automata.begin();
      const Vector<RhsAutomaton*>::const_iterator e = automata.end();
      do
	(void) (*i)->construct(matcher);
      while (++i != e);
    }
  return lastAutomaton->construct(matcher);
}

inline void
RhsBuilder::safeConstruct(Substitution& matcher) const
{
  if (!automata.isNull())
    {
      Vector<RhsAutomaton*>::const_iterator i = automata.begin();
      const Vector<RhsAutomaton*>::const_iterator e = automata.end();
      do
	(void) (*i)->construct(matcher);
      while (++i != e);
    }
  if (lastAutomaton != 0)
    (void) lastAutomaton->construct(matcher);
}

inline void
RhsBuilder::replace(DagNode* old, Substitution& matcher) const
{
  if (!automata.isNull())
    {
      Vector<RhsAutomaton*>::const_iterator i = automata.begin();
      const Vector<RhsAutomaton*>::const_iterator e = automata.end();
      do
	(void) (*i)->construct(matcher);
      while (++i != e);
    }
  lastAutomaton->replace(old, matcher);
}

#endif
