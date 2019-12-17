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
//	Class for info we have to keep around when compiling to C++.
//
#ifndef _compilationContext_hh_
#define _compilationContext_hh_
#include "natSet.hh"

class CompilationContext
{
public:
  CompilationContext(ostream& hhFile, ostream& ccFile, bool countRewrites);
  ostream& head() const;
  ostream& body() const;
  void generateIncrement(int indent);
  void generateSortVectors(Module* module);
  void generateEval(Module* module);
  void beginNewFunction();
  void setTailRecursive();
  void setNrSafeSlots(int nrSlots);
  bool isTailRecursive() const;
  int getNrSafeSlots() const;
  void usedSort(const Sort* sort);

private:
  //
  //	Info global to compilation; set at construction and never changes.
  //
  ostream& hhFile;
  ostream& ccFile;
  const bool countRewrites;
  NatSet usedSorts;
  //
  //	Info local to symbol currently being compiled.
  //
  bool tailRecursive;
  int nrSafeSlots;
};

inline ostream&
CompilationContext::head() const
{
  return hhFile;
}

inline ostream&
CompilationContext::body() const
{
  return ccFile;
}

inline void
CompilationContext::beginNewFunction()
{
  tailRecursive = false;
  nrSafeSlots = 0;
}

inline void
CompilationContext::setTailRecursive()
{
  tailRecursive = true;
}

inline void
CompilationContext::setNrSafeSlots(int nrSlots)
{
  if (nrSlots > nrSafeSlots)
    nrSafeSlots = nrSlots;
}

inline bool
CompilationContext::isTailRecursive() const
{
  return tailRecursive;
}

inline int
CompilationContext::getNrSafeSlots() const
{
  return nrSafeSlots;
}

inline void
CompilationContext::usedSort(const Sort* sort)
{
  usedSorts.insert(sort->getIndexWithinModule());
}

#endif
