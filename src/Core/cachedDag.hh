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
//      Class dags that are constructed from terms as then cached for reuse.
//
#ifndef _cachedDag_hh_
#define _cachedDag_hh_
#include "dagRoot.hh"
#include "instruction.hh"

class CachedDag
{
public:
  CachedDag(Term* t = 0);
  ~CachedDag();

  void setTerm(Term* t);
  Term* getTerm() const;
  bool normalize();
  void prepare();
  DagNode* getDag();
  DagRoot* getDagRoot();
  void reset();
  //
  //	MVM stuff.
  //
  void generateInstructionSequence();
  Instruction* getInstructionSequence() const;

private:
  Term* term;
  DagRoot dag;
  Instruction* instructionSequence;
};

inline
CachedDag::CachedDag(Term* t)
{
  term = t;
  instructionSequence = 0;
}

inline
CachedDag::~CachedDag()
{
  delete instructionSequence;
  if (term != 0)
    term->deepSelfDestruct();
}

inline void
CachedDag::setTerm(Term* t)
{
  term = t;
  dag.setNode(0);
}

inline Term*
CachedDag::getTerm() const
{
  return term;
}

inline bool
CachedDag::normalize()
{
  bool changed;
  term = term->normalize(true, changed);
  return changed;
}

inline DagNode*
CachedDag::getDag()
{
  DagNode* d = dag.getNode();
  if (d == 0)
    {
      d = term->term2Dag(term->getSortIndex() != Sort::SORT_UNKNOWN);  // HACK
      dag.setNode(d);
    }
  return d;
}

inline DagRoot*
CachedDag::getDagRoot()
{
  (void) getDag();
  return &dag;
}

inline void
CachedDag::generateInstructionSequence()
{
  instructionSequence = term->term2InstructionSequence();
}

inline Instruction*
CachedDag::getInstructionSequence() const
{
  return instructionSequence;
}

inline void
CachedDag::reset()
{
  dag.setNode(0);  // so dag can be garbage collected
}

#endif
