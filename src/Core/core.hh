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
//	Forward declarations for core classes.
//
#ifndef _core_hh_
#define _core_hh_

class MemoryCell;
class NamedEntity;
class ModuleItem;
class LineNumber;
class Module;
class Sort;
class ConnectedComponent;
class ArgumentIterator;
class DagArgumentIterator;
class VariableInfo;
class Substitution;
class LocalBinding;
class SubproblemSequence;
class SubproblemDisjunction;
class SortCheckSubproblem;
class VariableAbstractionSubproblem;
class ExtensionMatchSubproblem;
class EqualitySubproblem;
class PreEquation;
class Equation;
class Rule;
class Label;
class SortConstraint;
class Strategy;
class SortTable;
class SortConstraintTable;
class EquationTable;
class RuleTable;
class RewritingContext;
class SubproblemAccumulator;
class DisjunctiveSubproblemAccumulator;
class RedexPosition;
class TermSet;
class DagRoot;
class SymbolMap;
class CachedDag;
class DagNodeSet;
class DagNodeCache;
class RootContainer;
class SimpleRootContainer;
class BindingLhsAutomaton;
class TrivialRhsAutomaton;
class CopyRhsAutomaton;
class TermBag;
class RhsBuilder;
class ConditionFragment;
class ConditionState;
class OpDeclaration;
class SortBdds;
class UnificationContext;
class FreshVariableGenerator;
class NarrowingVariableInfo;
class ProtectedDagNodeSet;
class PendingUnificationStack;

#endif
