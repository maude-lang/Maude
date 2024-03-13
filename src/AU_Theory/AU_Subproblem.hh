/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for subproblems in the the A, AUl, AUr and AU theories.
//	They consist of a directed acyclic graph and a set of sequence
//	partitioning problems.
//
#ifndef _AU_Subproblem_hh_
#define _AU_Subproblem_hh_
#include "subproblem.hh"
#include "AU_Layer.hh"

class AU_Subproblem : public Subproblem
{
public:
  AU_Subproblem(AU_DagNode* subject,
		int firstSubterm,
		int lastSubterm,
		int nrBlockPairs,
		AU_ExtensionInfo* extensionInfo);

  bool solve(bool findFirst, RewritingContext& solution);
  void addTopVariable(int layerNr,
		      int index,
		      int lowerBound,
		      int upperBound,
		      const Sort* sort);
  void addNode(int layerNr,
	       LocalBinding* difference,
	       Subproblem* subproblem,
	       int firstSubterm,
	       int lastSubterm);
  void complete();

private:
  bool solvePatterns(bool findFirst, RewritingContext& solution);
  bool solveVariables(bool findFirst, RewritingContext& solution);

  AU_ExtensionInfo* const extensionInfo;	// extension information object to be filled in
  Vector<AU_Layer> layers;		// layers of directed acyclic graph
};

inline void
AU_Subproblem::addTopVariable(int layerNr,
			      int index,
			      int lowerBound,
			      int upperBound,
			      const Sort* sort)
{
  layers[layerNr].addTopVariable(index, lowerBound, upperBound, sort);
}

inline void
AU_Subproblem::addNode(int layerNr,
		       LocalBinding* difference,
		       Subproblem* subproblem,
		       int firstSubterm,
		       int lastSubterm)
{
  layers[layerNr].addNode(difference, subproblem, firstSubterm, lastSubterm);
}

#endif

