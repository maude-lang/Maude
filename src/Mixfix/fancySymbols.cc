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

Symbol*
MixfixModule::newAssociativeSymbol(int name,
				   const Vector<Sort*>& domainAndRange,
				   SymbolType symbolType,
				   const Vector<int>& strategy)
{
  //
  //	Handles A, AUl, AUr, AU, AC, ACU, and ConfigSymbol.
  //
  Assert(domainAndRange.length() - 1 == 2, "need 2 args");
  Assert(domainAndRange[0]->component() == domainAndRange[2]->component(), "kind clash");
  Assert(domainAndRange[1]->component() == domainAndRange[2]->component(), "kind clash");
  
  bool memo = symbolType.hasFlag(SymbolType::MEMO);
  if (symbolType.hasFlag(SymbolType::COMM))
    {
      if (symbolType.hasFlag(SymbolType::CONFIG))
	return new ConfigSymbol(name, strategy, memo);
      else
	return new ACU_Symbol(name, strategy, memo);
    }
  return new AU_Symbol(name,
		       strategy,
		       memo,
		       symbolType.hasFlag(SymbolType::LEFT_ID),
		       symbolType.hasFlag(SymbolType::RIGHT_ID));
}

Symbol*
MixfixModule::newAxiomSymbol(int name,
			     const Vector<Sort*>& domainAndRange,
			     SymbolType symbolType,
			     const Vector<int>& strategy)
{
  //
  //	Handles symbols with axioms other than associativity:
  //	  C, CU, Ul, Ur, CI, CUI, UlI, UrI, and CUI_NumberOpSymbol.
  //
  Assert(domainAndRange.length() - 1 == 2, "need 2 args");
  int axioms = 0;
  if (symbolType.hasFlag(SymbolType::COMM))
    {
      Assert(domainAndRange[0]->component() == domainAndRange[1]->component(), "kind clash");
      axioms |= CUI_Symbol::COMM;
    }
  if (symbolType.hasFlag(SymbolType::IDEM))
    {
      Assert(domainAndRange[0]->component() == domainAndRange[2]->component(), "kind clash");
      Assert(domainAndRange[1]->component() == domainAndRange[2]->component(), "kind clash");
      axioms |= CUI_Symbol::IDEM;
    }
  if (symbolType.hasFlag(SymbolType::LEFT_ID) ||
      (symbolType.hasAllFlags(SymbolType::RIGHT_ID | SymbolType::COMM)))
    {
      Assert(domainAndRange[1]->component() == domainAndRange[2]->component(), "kind clash");
      axioms |= CUI_Symbol::LEFT_ID;
    }
  if (symbolType.hasFlag(SymbolType::RIGHT_ID) ||
      (symbolType.hasAllFlags(SymbolType::LEFT_ID | SymbolType::COMM)))
    {
      Assert(domainAndRange[0]->component() == domainAndRange[2]->component(), "kind clash");
      axioms |= CUI_Symbol::RIGHT_ID;
    }
  return (symbolType.getBasicType() == SymbolType::CUI_NUMBER_OP_SYMBOL) ?
    new CUI_NumberOpSymbol(name, static_cast<CUI_Symbol::Axioms>(axioms)) :
    new CUI_Symbol(name,
		   strategy,
		   symbolType.hasFlag(SymbolType::MEMO),
		   static_cast<CUI_Symbol::Axioms>(axioms));
}

Symbol*
MixfixModule::newFancySymbol(Token prefixName,
			     const Vector<Sort*>& domainAndRange,
			     SymbolType symbolType,
			     const Vector<int>& strategy)
{
  int name = prefixName.code();
  int nrArgs = domainAndRange.length() - 1;
  int kindIndex = domainAndRange[nrArgs]->component()->getIndexWithinModule();
  //
  //	Handle special symbols.
  //
  switch (symbolType.getBasicType())
    {
    case SymbolType::BRANCH_SYMBOL:
      return new BranchSymbol(name, nrArgs);
    case SymbolType::EQUALITY_SYMBOL:
      return new EqualitySymbol(name, strategy);
    case SymbolType::FLOAT:
      return new FloatSymbol(name);
    case SymbolType::STRING:
      return new StringSymbol(name);
    case SymbolType::QUOTED_IDENTIFIER:
      return new QuotedIdentifierSymbol(name);
    case SymbolType::LOOP_SYMBOL:
      return new LoopSymbol(name);
    case SymbolType::FLOAT_OP:
      return new FloatOpSymbol(name, nrArgs);
    case SymbolType::STRING_OP:
      return new StringOpSymbol(name, nrArgs);
    case SymbolType::QUOTED_IDENTIFIER_OP:
      return new QuotedIdentifierOpSymbol(name, nrArgs);
    case SymbolType::META_LEVEL_OP_SYMBOL:
      return new MetaLevelOpSymbol(name, nrArgs, strategy);
    case SymbolType::MODEL_CHECKER_SYMBOL:
      return new ModelCheckerSymbol(name);
    case SymbolType::SAT_SOLVER_SYMBOL:
      return new SatSolverSymbol(name);
    case SymbolType::SUCC_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithSucc.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	      ": multiple SuccSymbols in same kind will cause pretty printing problems.");
	  }
	return new SuccSymbol(name);
      }
    case SymbolType::MINUS_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithMinus.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	      ": multiple MinusSymbols in same kind will cause pretty printing problems.");
	  }
	return new MinusSymbol(name);
      }
    case SymbolType::NUMBER_OP_SYMBOL:
      return new NumberOpSymbol(name, nrArgs);
    case SymbolType::ACU_NUMBER_OP_SYMBOL:
      return new ACU_NumberOpSymbol(name);
    case SymbolType::DIVISION_SYMBOL:
      {
	pair<set<int>::iterator, bool> p = kindsWithDivision.insert(kindIndex);
	if (!(p.second))
	  {
	    IssueWarning(LineNumber(prefixName.lineNumber()) <<
	    ": multiple DivisionSymbols in same kind will cause pretty printing problems.");
	  }
	return new DivisionSymbol(name);
      }
    case SymbolType::RANDOM_OP_SYMBOL:
      return new RandomOpSymbol(name);
    case SymbolType::MATRIX_OP_SYMBOL:
      return new MatrixOpSymbol(name, nrArgs);
    case SymbolType::COUNTER_SYMBOL:
      return new CounterSymbol(name);
    case SymbolType::SOCKET_MANAGER_SYMBOL:
      return new SocketManagerSymbol(name);
    case SymbolType::INTERPRETER_MANAGER_SYMBOL:
      return new InterpreterManagerSymbol(name);
    case SymbolType::SMT_SYMBOL:
      return new SMT_Symbol(name, nrArgs);
    case SymbolType::SMT_NUMBER_SYMBOL:
      return new SMT_NumberSymbol(name);
    case SymbolType::FILE_MANAGER_SYMBOL:
      return new FileManagerSymbol(name);
    case SymbolType::STREAM_MANAGER_SYMBOL:
      return new StreamManagerSymbol(name);
    case SymbolType::PROCESS_MANAGER_SYMBOL:
      return new ProcessManagerSymbol(name);
    case SymbolType::TIME_MANAGER_SYMBOL:
      return new TimeManagerSymbol(name);
    }
  //
  //	Handle regular symbols that have semantic attributes.
  //
  if (symbolType.hasFlag(SymbolType::ITER))
    {
      Assert(domainAndRange.length() - 1 == 1, "need 1 arg");
      Assert(domainAndRange[0]->component() == domainAndRange[1]->component(), "kind clash");
      return new S_Symbol(name, strategy, symbolType.hasFlag(SymbolType::MEMO));
    }
  if (symbolType.hasFlag(SymbolType::ASSOC))
    return newAssociativeSymbol(name, domainAndRange, symbolType, strategy);
  if (symbolType.hasFlag(SymbolType::AXIOMS))
    return newAxiomSymbol(name, domainAndRange, symbolType, strategy);
  return 0;
}
