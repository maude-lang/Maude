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
//      Forward declarations for free theory classes.
//
#ifndef _freeTheory_hh_
#define _freeTheory_hh_

class FreeSymbol;
class FreeNullarySymbol;
class FreeUnarySymbol;
class FreeBinarySymbol;
class FreeTernarySymbol;

class FreeTerm;
class FreeArgumentIterator;
class FreeDagNode;
class FreeDagArgumentIterator;
class FreeLhsAutomaton;

class FreeRhsAutomaton;
class FreeNullaryRhsAutomaton;
class FreeUnaryRhsAutomaton;
class FreeBinaryRhsAutomaton;
class FreeTernaryRhsAutomaton;
class FreeFast2RhsAutomaton;
class FreeFast3RhsAutomaton;

class FreeOccurrence;
class FreeNet;
class FreeRemainder;
class FreePositionTable;
class FreeLhsStructs;

class FreeInstruction;
class FreeFastInstruction;
class FreeGeneralCtor;
class FreeGeneralCtorFinal;
class FreeGeneralExtor;
class FreeGeneralExtorFinal;

#endif
