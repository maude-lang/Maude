/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
SyntacticPreModule::loseFocus(bool clearCaches)
{
  if (flatModule != 0)
    {
      flatModule->clearMemo();
      if (clearCaches)
	flatModule->reset();
      flatModule->economize();
    }
}

void
SyntacticPreModule::dump()
{
#ifdef DUMP
  getFlatModule()->dump(cout);
#endif
}

void
SyntacticPreModule::printSortTokenVector(ostream& s, const Vector<Token>& sorts)
{
  int nrTokens = sorts.size();
  s << Token::sortName(sorts[0].code());
  for (int i = 1; i < nrTokens; ++i)
    s << ' ' << Token::sortName(sorts[i].code());
}

void
SyntacticPreModule::showModule(ostream& s)
{
  s << MixfixModule::moduleTypeString(getModuleType()) << ' ' << this;
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << '{' << Token::name(getParameterName(0)) << " :: " << getParameter(0); // FIX NAME
      for (int i = 1; i < nrParameters; ++i)
	s << ", " << Token::name(getParameterName(i)) << " :: " << getParameter(i); // FIX NAME
      s << '}';
    }
  s << " is\n";

  int nrImports = getNrImports();
  for (int i = 0; i < nrImports; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << ImportModule::importModeString(getImportMode(i)) <<
	' ' << getImport(i) << " .\n";
    }

  for (const Vector<Token>& sorts : sortDecls)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  sort" << pluralize(sorts.size()) << " ";
      printSortTokenVector(s, sorts);
      s << " .\n";
    }

  for (const Vector<Token>& subsorts : subsortDecls)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  subsort" << pluralize(subsorts.size() - 2) << " ";
      printSortTokenVector(s, subsorts);
      s << " .\n";
    }

  for (auto& cd : classDecls)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  class " << Token::sortName(cd.name.code());
      int nrAttributes = cd.attributes.size();
      for (int i = 0; i < nrAttributes; ++i)
	{
	  AttributePair& p = cd.attributes[i];
	  s << (i == 0 ? " | " : ", ") << p.attributeName << " : " << p.type;
	}
      s << " .\n";
    }

  for (const auto& sc : subclassDecls)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << ((sc.size() <= 3) ? "  subclass " : "  subclasses ");
      printSortTokenVector(s, sc);
      s << " .\n";
    }

  bool follow = false;
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      OpDecl& opDecl = opDecls[i];
      int defIndex = opDecl.defIndex;
      OpDef& opDef = opDefs[defIndex];
      SymbolType st = opDef.symbolType;
      bool newFollow = (i + 1 < nrOpDecls) && (opDecls[i + 1].defIndex == defIndex);
      if (!follow)
	{
	  if (st.getBasicType() == SymbolType::VARIABLE)
	    s << "  var";
	  else if (st.hasFlag(SymbolType::MSG_STATEMENT))
	    s << "  msg";
	  else
	    s << "  op";
	  s << (newFollow ? "s " : " ");
	}
      
      if (st.getBasicType() == SymbolType::VARIABLE)
	s << opDecl.prefixName;  // don't prettify variable names
      else
	{
	  s << MixfixModule::prettyOpName(opDecl.prefixName.code(),
					  (follow || newFollow) ? (Token::EXPOSED_COLON | Token::MULTIPLE_TOKENS) : Token::EXPOSED_COLON);
	}
      s << ' ';
      follow = newFollow;
      if (!follow)
	{
	  printOpDef(s, defIndex);
	  if (UserLevelRewritingContext::interrupted())
	    return;
	}
    }

  int nrStratDecls = stratDecls.length();
  for (int i = 0; i < nrStratDecls; i++)
    {
      printStratDecl(s, stratDecls[i]);

      if (UserLevelRewritingContext::interrupted())
	return;
    }

  int nrStatements = statements.length();
  for (int i = 0; i < nrStatements; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "  " << statements[i] << " .\n";
    }

  s << MixfixModule::moduleEndString(getModuleType()) << '\n';
}

void
SyntacticPreModule::printOpDef(ostream&s, int defIndex)
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
SyntacticPreModule::printStratDecl(ostream& s, const StratDecl& decl)
{
  size_t rangeIndex = decl.types.length() - 1;

  s << (decl.names.length() == 1 ? "  strat " : "  strats ");

  int nrNames = decl.names.length();
  for (int i = 0; i < nrNames; i++)
    s << decl.names[i] << ' ';

  if (rangeIndex > 0)
    {
      s << ": ";

      for (size_t i = 0; i < rangeIndex; i++)
	s << decl.types[i] << " ";
    }
  s << "@ " << decl.types[rangeIndex] << ' ';

  printAttributes(s, decl);
  s << ".\n";
}

void
SyntacticPreModule::printAttributes(ostream& s, const OpDef& opDef) const
{
  SymbolType st = opDef.symbolType;  // copy
  if (st.hasFlag(SymbolType::MSG_STATEMENT))
    {
      //
      //	msg statement implies msg and ctor so we don't want to print these attributes.
      //
      st.clearFlags(SymbolType::MESSAGE | SymbolType::CTOR);
    }
  if (!(st.hasFlag(SymbolType::ATTRIBUTES | SymbolType::CTOR |
		   SymbolType::POLY | SymbolType::DITTO)) &&
      opDef.special.empty() && opDef.metadata == NONE)
    return;

  const char* space = "";
  s << '[';
  if (st.hasFlag(SymbolType::POLY))
    {
      s << "poly (";
      for (int i : opDef.polyArgs)
	{
	  if  (i != 0)
	    {
	      s << space << i;
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
  if (st.hasFlag(SymbolType::CTOR))
    {
      s << space << "ctor";
      space = " ";
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
  if (st.hasFlag(SymbolType::ITER))
    {
      s << space << "iter";
      space = " ";
    }
  if (st.hasFlag(SymbolType::PCONST))
    {
      s << space << "pconst";
      space = " ";
    }
  if (st.hasFlag(SymbolType::OBJECT))
    {
      s << space << "obj";
      space = " ";
    }
  if (st.hasFlag(SymbolType::MESSAGE))
    {
      s << space << "msg";
      space = " ";
    }
  if (st.hasFlag(SymbolType::PORTAL))
    {
      s << space << "portal";
      space = " ";
    }
  if (st.hasFlag(SymbolType::CONFIG))
    {
      s << space << "config";
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
  if (st.hasFlag(SymbolType::LATEX))
    {
      s << space << "latex (" << Token::name(opDef.latexMacro) << ")";
      space = " ";
    }
  if (st.hasFlag(SymbolType::RPO))
    {
      s << space << "rpo " << opDef.rpo;
      space = " ";
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
      if (getOwner()->getPrintFlag(Interpreter::PRINT_HOOKS))
	{
	  for (const Hook& h : opDef.special)
	    {
	      static const char* hookTypes[] = {"id-hook", "op-hook", "term-hook"};
	      s << "\n    " << hookTypes[h.type] << ' ' <<
		Token::name(h.name);
	      if (!(h.details.empty()))
		s << " (" << h.details << ')';
	    }
	}
      else
	s << "...";
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
SyntacticPreModule::printGather(ostream& s, const Vector<int>& gather)
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
SyntacticPreModule::printFormat(ostream& s, const Vector<int>& format)
{
  s << "format (";
  int formatLen = format.length();
  for (int i = 0; i < formatLen; i++)
    s << Token::name(format[i]) << ((i == formatLen - 1) ? ')' : ' ');
}

void
SyntacticPreModule::printAttributes(ostream& s, const StratDecl& stratDecl)
{
  if (stratDecl.metadata == NONE)
    return;
  s << "[metadata " << Token::name(stratDecl.metadata) << ']';
}
