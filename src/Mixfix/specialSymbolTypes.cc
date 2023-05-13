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
//	List of all the special symbol types supported.
//
//	We generate various chunks of linear (branching) code to handle
//	this hetrogeneous collection by macro expansion.
//	The format is:
//		MACRO(HookName, SymbolBasicType)
//
  MACRO(SystemTrue, SymbolType::SYSTEM_TRUE)
  MACRO(SystemFalse, SymbolType::SYSTEM_FALSE)
  MACRO(BranchSymbol, SymbolType::BRANCH_SYMBOL)
  MACRO(EqualitySymbol, SymbolType::EQUALITY_SYMBOL)
  MACRO(FloatSymbol, SymbolType::FLOAT)
  MACRO(FloatOpSymbol, SymbolType::FLOAT_OP)
  MACRO(StringSymbol, SymbolType::STRING)
  MACRO(StringOpSymbol, SymbolType::STRING_OP)
  MACRO(QuotedIdentifierSymbol, SymbolType::QUOTED_IDENTIFIER)
  MACRO(QuotedIdentifierOpSymbol, SymbolType::QUOTED_IDENTIFIER_OP)
  MACRO(MetaLevelOpSymbol, SymbolType::META_LEVEL_OP_SYMBOL)
  MACRO(LoopSymbol, SymbolType::LOOP_SYMBOL)
  MACRO(Bubble, SymbolType::BUBBLE)
  MACRO(ModelCheckerSymbol, SymbolType::MODEL_CHECKER_SYMBOL)
  MACRO(SatSolverSymbol, SymbolType::SAT_SOLVER_SYMBOL)
  MACRO(SuccSymbol, SymbolType::SUCC_SYMBOL)
  MACRO(MinusSymbol, SymbolType::MINUS_SYMBOL)
  MACRO(NumberOpSymbol, SymbolType::NUMBER_OP_SYMBOL)
  MACRO(ACU_NumberOpSymbol, SymbolType::ACU_NUMBER_OP_SYMBOL)
  MACRO(CUI_NumberOpSymbol, SymbolType::CUI_NUMBER_OP_SYMBOL)
  MACRO(DivisionSymbol, SymbolType::DIVISION_SYMBOL)
  MACRO(RandomOpSymbol, SymbolType::RANDOM_OP_SYMBOL)
  MACRO(MatrixOpSymbol, SymbolType::MATRIX_OP_SYMBOL)
  MACRO(CounterSymbol, SymbolType::COUNTER_SYMBOL)
  MACRO(SocketManagerSymbol, SymbolType::SOCKET_MANAGER_SYMBOL)
  MACRO(InterpreterManagerSymbol, SymbolType::INTERPRETER_MANAGER_SYMBOL)
  MACRO(SMT_Symbol, SymbolType::SMT_SYMBOL)
  MACRO(SMT_NumberSymbol, SymbolType::SMT_NUMBER_SYMBOL)
  MACRO(FileManagerSymbol, SymbolType::FILE_MANAGER_SYMBOL)
  MACRO(StreamManagerSymbol, SymbolType::STREAM_MANAGER_SYMBOL)
  MACRO(DirectoryManagerSymbol, SymbolType::DIRECTORY_MANAGER_SYMBOL)
  MACRO(ProcessManagerSymbol, SymbolType::PROCESS_MANAGER_SYMBOL)
  MACRO(TimeManagerSymbol, SymbolType::TIME_MANAGER_SYMBOL)
  MACRO(PrngManagerSymbol, SymbolType::PRNG_MANAGER_SYMBOL)
  MACRO(ObjectConstructorSymbol, SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
