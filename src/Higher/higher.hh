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
//	Forward declarations for higher level functionality classes.
//
#ifndef _higher_hh_
#define _higher_hh_

class Pattern;
class PositionState;
class SearchState;
class RewriteSearchState;
class MatchSearchState;

class CacheableState;
class SequenceSearch;
class RewriteSequenceSearch;

class StateTransitionGraph3;
class StateTransitionGraph;

class EqualityConditionFragment;
class SortTestConditionFragment;
class AssignmentConditionFragment;
class RewriteConditionFragment;
class AssignmentConditionState;
class RewriteConditionState;

class TemporalSymbol;
class ModelCheckerSymbol;
class SatSolverSymbol;

class UnificationProblem;
class IrredundantUnificationProblem;

class NarrowingUnificationProblem;
class NarrowingSearchState;
class NarrowingSequenceSearch;

class UnifierFilter;
class VariantNarrowingSearchState;
class VariantSearch;
class VariantFolder;

class VariantUnificationProblem;
class NarrowingSearchState2;

class NarrowingSearchState3;
class NarrowingSequenceSearch3;
class NarrowingFolder;

class VariantUnifierFilter;
class FilteredVariantUnifierSearch;

class VariantMatchingProblem;

#endif
