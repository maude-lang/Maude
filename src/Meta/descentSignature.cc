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
//	List of all descent functions supported by MetaLevelOpSymbol.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(symbols name, number of args)
//
  MACRO(metaReduce, 2)
  MACRO(metaNormalize, 2)
  MACRO(metaRewrite, 3)
  MACRO(metaFrewrite, 4)
  MACRO(metaApply, 5)
  MACRO(metaXapply, 7)
  MACRO(metaMatch, 5)
  MACRO(metaXmatch, 7)
  MACRO(metaUnify, 4)
  MACRO(metaDisjointUnify, 4)
  MACRO(metaSearch, 7)
  MACRO(metaSearchPath, 7)
  MACRO(metaNarrow, 6)
  MACRO(metaNarrow2, 6)
  MACRO(metaSortLeq, 3)
  MACRO(metaSameKind, 3)
  MACRO(metaLesserSorts, 2)
  MACRO(metaGlbSorts, 3)
  MACRO(metaLeastSort, 2)
  MACRO(metaCompleteName, 2)
  MACRO(metaParse, 3)
  MACRO(metaPrettyPrint, 3)
  MACRO(metaWellFormedModule, 1)
  MACRO(metaWellFormedTerm, 2)
  MACRO(metaGetKind, 2)
  MACRO(metaGetKinds, 1)
  MACRO(metaMaximalSorts, 2)
  MACRO(metaMinimalSorts, 2)
  MACRO(metaMaximalAritySet, 4)
  MACRO(metaWellFormedSubstitution, 2)
  MACRO(metaUpModule, 2)
  MACRO(metaUpImports, 1)
  MACRO(metaUpSorts, 2)
  MACRO(metaUpSubsortDecls, 2)
  MACRO(metaUpOpDecls, 2)
  MACRO(metaUpMbs, 2)
  MACRO(metaUpEqs, 2)
  MACRO(metaUpRls, 2)
  MACRO(metaUpView, 1)
  MACRO(metaUpTerm, 1)
  MACRO(metaDownTerm, 2)
