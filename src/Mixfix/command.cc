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
//	Code for general top level commands.
//

void
PreModule::loseFocus()
{
  if (flatModule != 0)
    {
      flatModule->clearMemo();
      flatModule->reset();
      flatModule->economize();
    }
}

void
PreModule::dump()
{
#ifdef DUMP
  getFlatModule()->dump(cout);
#endif
}

void
PreModule::printSortTokenVector(ostream& s, const Vector<Token>& sorts)
{
  int nrTokens = sorts.size();
  s << Token::sortName(sorts[0].code());
  for (int i = 1; i < nrTokens; ++i)
    s << ' ' << Token::sortName(sorts[i].code());
}

void
PreModule::showModule(ostream& s)
{
  s << MixfixModule::moduleTypeString(moduleType) << ' ' << this;
  int nrParameters = parameters.size();
  if (nrParameters > 0)
    {
      s << '{' << parameters[0].name << " :: " << parameters[0].theory;
      for (int i = 1; i < nrParameters; ++i)
	s << ", " << parameters[i].name << " :: " << parameters[i].theory;
      s << '}';
    }
  s << " is\n";

  int nrImports = imports.length();
  for (int i = 0; i < nrImports; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << imports[i].mode << ' ' << imports[i].expr << " .\n";
    }

  int nrSortDecls = sortDecls.length();
  for (int i = 0; i < nrSortDecls; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  sorts ";
      printSortTokenVector(s, sortDecls[i]);
      s << " .\n";
    }

  int nrSubsortDecls = subsortDecls.length();
  for (int i = 0; i < nrSubsortDecls; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  subsorts ";
      printSortTokenVector(s, subsortDecls[i]);
      s << " .\n";
    }

  bool follow = false;
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      OpDecl& opDecl = opDecls[i];
      int defIndex = opDecl.defIndex;
      bool newFollow = (i + 1 < nrOpDecls) && (opDecls[i + 1].defIndex == defIndex);
      if (!follow)
	{
	  s << ((opDefs[defIndex].symbolType.getBasicType() == SymbolType::VARIABLE) ?
		"  var" : "  op") <<
	    (newFollow ? "s " : " ");
	}
      s << opDecls[i].prefixName << ' ';
      follow = newFollow;
      if (!follow)
	{
	  printOpDef(s, defIndex);
	  if (UserLevelRewritingContext::interrupted())
	    return;
	}
    }

  int nrStatements = statements.length();
  for (int i = 0; i < nrStatements; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << statements[i] << " .\n";
    }

  s << MixfixModule::moduleEndString(moduleType) << '\n';
}

void
PreModule::printOpDef(ostream&s, int defIndex)
{
  OpDef& opDef = opDefs[defIndex];
  s << ": ";
  if (opDef.symbolType.getBasicType() == SymbolType::VARIABLE)
    s << opDef.types[0] << ' ';
  else
    {
      int nrArgs = opDef.types.length() - 1;
      for (int i = 0; i < nrArgs; i++)
	s << opDef.types[i] << ' ';
      s << "-> " << opDef.types[nrArgs] << ' ';
    }
  printAttributes(s, opDef);
  s << ".\n";
}

void
PreModule::printAttributes(ostream& s, const OpDef& opDef)
{
  SymbolType st = opDef.symbolType;
  if (!(st.hasFlag(SymbolType::ATTRIBUTES | SymbolType::CTOR |
		   SymbolType::POLY | SymbolType::DITTO)) &&
      opDef.special.empty() && opDef.metadata == NONE)
    return;

  const char* space = "";
  s << '[';
  if (st.hasFlag(SymbolType::POLY))
    {
      s << "poly (";
      FOR_EACH_CONST(i, NatSet, opDef.polyArgs)
	{
	  if  (*i != 0)
	    {
	      s << space << *i;
	      space = " ";
	    }
	}
      if (opDef.polyArgs.contains(0))
	{
	  s << space << '0';
	  space = " ";
	}
      s << ')';
   }
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    {
      s << space << "assoc";
      space = " ";
    }
  if (st.hasFlag(SymbolType::COMM))
    {
      s << space << "comm";
      space = " ";
    }
  if (st.hasFlag(SymbolType::ITER))
    {
      s << space << "iter";
      space = " ";
    }
  if (st.hasFlag(SymbolType::MESSAGE))
    {
      s << space << "msg";
      space = " ";
    }
  if (st.hasFlag(SymbolType::OBJECT))
    {
      s << space << "obj";
      space = " ";
    }
  if (st.hasFlag(SymbolType::CONFIG))
    {
      s << space << "config";
      space = " ";
    }
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << space;
      space = " ";
      if (!(st.hasFlag(SymbolType::LEFT_ID)))
	s << "right ";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID)))
	s << "left ";
      s << "id: " << opDef.identity;
    }
  if (st.hasFlag(SymbolType::IDEM))
    {
      s << space << "idem";
      space = " ";
    }
  //
  //	Semantic attributes.
  //
  if (st.hasFlag(SymbolType::STRAT))
    {
      s << space << "strat (";
      space = " ";
      int stratLen = opDef.strategy.length();
      for (int i = 0; i < stratLen; i++)
	s << opDef.strategy[i] << ((i == stratLen - 1) ? ')' : ' ');
    }
  if (st.hasFlag(SymbolType::MEMO))
    {
      s << space << "memo";
      space = " ";
    }
  if (st.hasFlag(SymbolType::FROZEN))
    {
      s << space << "frozen";
      space = " ";
      if (!(opDef.frozen.empty()))
	{
	  s << " (";
	  const NatSet::const_iterator e = opDef.frozen.end();
	  for (NatSet::const_iterator i = opDef.frozen.begin();;)
	    {
	      s << *i + 1;
	      if (++i == e)
		break;
	      s << ' ';
	    }
	  s << ')';
	}
    }
  if (st.hasFlag(SymbolType::CTOR))
    {
      s << space << "ctor";
      space = " ";
    }
  //
  //	Syntactic attributes.
  //
  if (st.hasFlag(SymbolType::PREC))
    {
      s << space << "prec " << opDef.prec;
      space = " ";
    }
  if (st.hasFlag(SymbolType::GATHER))
    {
      s << space;
      space = " ";
      printGather(s, opDef.gather);
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << space;
      space = " ";
      printFormat(s, opDef.format);
    }
  if (opDef.metadata != NONE)
    {
      s << space << "metadata " << Token::name(opDef.metadata);
      space = " ";
    }
  if (!(opDef.special.empty()))
    {
      s << space << "special (";
      space = " ";
      FOR_EACH_CONST(i, Vector<Hook>, opDef.special)
	{
	  static const char* hookTypes[] = {
	    "id-hook", "op-hook", "term-hook"
	  };
	  s << "\n    " << hookTypes[i->type] << ' ' <<
	    Token::name(i->name) << " (" << i->details << ')';
	}
      s << ')';
    }
  //
  //	Misc attributes.
  //
  if (st.hasFlag(SymbolType::DITTO))
    {
      s << space << "ditto";
      space = " ";
    }

  s << "] ";
}

void
PreModule::printGather(ostream& s, const Vector<int>& gather)
{
  static char gatherSymbols[] = {'e', 'E', '&'};
  s << "gather (";
  int gatherLen = gather.length();
  for (int i = 0; i < gatherLen; i++)
    {
      s << gatherSymbols[gather[i] - MixfixModule::GATHER_e] <<
	((i == gatherLen - 1) ? ')' : ' ');
    }
}

void
PreModule::printFormat(ostream& s, const Vector<int>& format)
{
  s << "format (";
  int formatLen = format.length();
  for (int i = 0; i < formatLen; i++)
    s << Token::name(format[i]) << ((i == formatLen - 1) ? ')' : ' ');
}
