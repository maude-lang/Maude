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

ostream&
operator<<(ostream& s, const PreModule::Type& type)
{
  if (type.kind)
    {
      s << '[' << type.tokens[0];
      int nrTokens = type.tokens.length();
      for (int i = 1; i < nrTokens; i++)
	s << ',' << type.tokens[i];
      s << ']';
    }
  else
    s << type.tokens[0];
  return s;
}

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
PreModule::showModule(ostream& s)
{
  
  if (moduleType == MixfixModule::FUNCTIONAL_MODULE)
    s << 'f';
  s << "mod " << this << " is\n";

  int nrImports = imports.length();
  for (int i = 0; i < nrImports; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << imports[i] << " .\n";
    }

  int nrSortDecls = sortDecls.length();
  for (int i = 0; i < nrSortDecls; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  sorts " << sortDecls[i] << " .\n";
    }

  int nrSubsortDecls = subsortDecls.length();
  for (int i = 0; i < nrSubsortDecls; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  subsorts " << subsortDecls[i] << " .\n";
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
  s << ((moduleType == MixfixModule::FUNCTIONAL_MODULE) ? "endfm\n" : "endm\n");
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
  if (int flags = opDef.symbolType.getFlags())
    {
      char c = '[';
      if (flags & SymbolType::MESSAGE)
	{
	  s << c << "msg";
	  c = ' ';
	}
      if (flags & SymbolType::OBJECT)
	{
	  s << c << "obj";
	  c = ' ';
	}
      if (flags & SymbolType::CONFIG)
	{
	  s << c << "config";
	  c = ' ';
	}
      if (flags & SymbolType::ASSOC)
	{
	  s << c << "assoc";
	  c = ' ';
	}
      if (flags & SymbolType::COMM)
	{
	  s << c << "comm";
	  c = ' ';
	}
      if (flags & (SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	{
	  s << c;
	  c = ' ';
	  if (!(flags & SymbolType::LEFT_ID))
	    s << "left ";
	  else if (!(flags & SymbolType::RIGHT_ID))
	    s << "right ";
	  s << "id: " << opDef.identity;
	}
      if (flags & SymbolType::IDEM)
	{
	  s << c << "idem";
	  c = ' ';
	}
      if (flags & SymbolType::STRAT)
	{
	  s << c << "strat (";
	  int stratLen = opDef.strategy.length();
	  for (int i = 0; i < stratLen; i++)
	    s << opDef.strategy[i] << ((i == stratLen - 1) ? ')' : ' ');
	  c = ' ';
	}
      if (flags & SymbolType::MEMO)
	{
	  s << c << "memo";
	  c = ' ';
	}
      if (flags & SymbolType::FROZEN)
	{
	  s << c << "frozen";
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
	  c = ' ';
	}
      if (flags & SymbolType::PREC)
	{
	  s << c << "prec " << opDef.prec;
	  c = ' ';
	}
      if (flags & SymbolType::DITTO)
	{
	  s << c << "ditto";
	  c = ' ';
	}
      if (flags & SymbolType::GATHER)
	{
	  static char gatherSymbols[] = {'e', 'E', '&'};
	  s << c << "gather (";
	  int gatherLen = opDef.gather.length();
	  for (int i = 0; i < gatherLen; i++)
	    {
	      s << gatherSymbols[opDef.gather[i] - MixfixModule::GATHER_e] <<
		((i == gatherLen - 1) ? ')' : ' ');
	    }
	  // c = ' ';
	}
      s << "] ";
    }
  s << ".\n";
}
