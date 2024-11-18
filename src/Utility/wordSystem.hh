/*

    This file is part of the Maude 2 interpreter.

    Copyright 1997-2015 SRI International, Menlo Park, CA 94025, USA.

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
//	Class to solve systems of word equations using the Plotkin/PIG-PUG
//	algorithm for single equations with forward and backward substitution,
//	together with various heuristics: left and right cancellation, orienting,
//	ordering and known solutions to special forms.
//
//	In order to ensure termination, non-unifier preserving transformations
//	might be made in which case a incompleteness flag is set.
//
#include "pigPug.hh"
#include "wordLevel.hh"

#ifndef _wordSystem_hh_
#define _wordSystem_hh_

class WordSystem
{
public:
  enum OutcomeFlags
    {
      FAILURE = 0,	// no solution was found
      SUCCESS = 1,	// solution was found
      INCOMPLETE = 2	// solutions may have been missed
    };
  //
  //	Local names for PigPug data type.
  //
  typedef PigPug::Word Word;
  typedef PigPug::Subst Subst;

  WordSystem(int nrVariables, int nrEquations, bool identityOptimizations = false);

  void setTheoryConstraint(int variable, int theoryIndex);
  void setUpperBound(int variable, int upperBound);
  void setTakeEmpty(int variable);
  void addAssignment(int variable, const Word& value);
  void addEquation(int index, const Word& lhs, const Word& rhs);
  void addNullEquation(const Word& word);
  
  int findNextSolution();
  const Word& getAssignment(int variable) const;
  int getNrVariables() const;

private:
  typedef Vector<std::unique_ptr<WordLevel>> LevelStack;

  std::unique_ptr<WordLevel> current;
  LevelStack levelStack;
  int incompletenessFlag;
};

inline void
WordSystem::setTheoryConstraint(int variable, int theoryIndex)
{
  current->setTheoryConstraint(variable, theoryIndex);
}

inline void
WordSystem::setUpperBound(int variable, int upperBound)
{
  current->setUpperBound(variable, upperBound);
}

inline void
WordSystem::setTakeEmpty(int variable)
{
  current->setTakeEmpty(variable);
}

inline void
WordSystem::addAssignment(int variable, const Word& value)
{
  current->addAssignment(variable, value);
}

inline void
WordSystem::addEquation(int index, const Word& lhs, const Word& rhs)
{
  current->addEquation(index, lhs, rhs);
}

inline void
WordSystem::addNullEquation(const Word& word)
{
  current->addNullEquation(word);
}

inline const WordSystem::Word&
WordSystem::getAssignment(int variable) const
{
  return current->getAssignment(variable);
}

inline int
WordSystem::getNrVariables() const
{
  return current->getNrVariables();
}

#endif
