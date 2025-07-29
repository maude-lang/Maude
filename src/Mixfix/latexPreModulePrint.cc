/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	LaTeX pretty printer for PreModules.
//

void
SyntacticPreModule::latexSortTokenVector(ostream& s, const Vector<Token>& sorts)
{
  for (const Token& t : sorts)
    {
      int code = t.code();
      s << "\\maudeSpace";
      if (code == lessThan)
	s << "\\maudeLessThan";
      else
	s << MixfixModule::latexSort(code, flatModule);
    }
}

void
SyntacticPreModule::latexType(ostream& s, const Type& type)
{
  if (type.kind)
    {
      const char* sep = "\\maudeLeftBracket";
      for (const Token& t : type.tokens)
	{
	  s << sep << MixfixModule::latexSort(t.code(), flatModule);
	  sep = "\\maudeComma";
	}
      s << "\\maudeRightBracket";
    }
  else
    s << MixfixModule::latexSort(type.tokens[0].code(), flatModule);
}

void
SyntacticPreModule::latexShowModule(ostream& s)
{
  s << "\\par\\maudeKeyword{" << MixfixModule::moduleTypeString(getModuleType()) << "}\\maudeSpace";
  s << "\\maudeModule{" << Token::latexName(id()) << "}";
  Index nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << "$\\maudeLeftBrace\\maudeParameter{" << Token::latexName(getParameterName(0)) << "}\\maudeParameterColon";
      getParameter(0)->latexPrint(s);
      for (Index i = 1; i < nrParameters; ++i)
	{
	  s << "\\maudeComma\\maudeParameter{" << Token::latexName(getParameterName(i)) << "}\\maudeParameterColon";
	  getParameter(i)->latexPrint(s);
	}
      s << "\\maudeRightBrace$";
    }
  s << "\\maudeSpace\\maudeKeyword{is}\n";

  Index nrImports = getNrImports();
  for (Index i = 0; i < nrImports; ++i)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{" << ImportModule::importModeString(getImportMode(i)) << "}\\maudeSpace";
      getImport(i)->latexPrint(s, flatModule);
      s << "$\\maudeEndStatement\n";
    }

  for (const Vector<Token>& sorts : sortDecls)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{sort" << pluralize(sorts.size()) << "}";
      latexSortTokenVector(s, sorts);
      s << "$\\maudeEndStatement\n";
    }

  for (const Vector<Token>& subsorts : subsortDecls)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{subsort" << pluralize(subsorts.size() - 2) << "}";
      latexSortTokenVector(s, subsorts);
      s << "$\\maudeEndStatement\n";
    }

  for (const ClassDecl& classDecl : classDecls)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{class}\\maudeSpace" << MixfixModule::latexSort(classDecl.name.code(), flatModule);
      const char* sep = "\\maudePipe";
      for (const AttributePair& p : classDecl.attributes)
	{
	  s << sep << Token::latexIdentifier(p.attributeName.code()) << ":";
	  latexType(s, p.type);
	  sep = "\\maudeComma";
	}
      s << "$\\maudeEndStatement\n";
    }

  for (const Vector<Token>& subclass : subclassDecls)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{" << ((subclass.size() <= 3) ? "subclass" : "subclasses") << "}";
      latexSortTokenVector(s, subclass);
      s << "$\\maudeEndStatement\n";
    }

  bool follow = false;
  Index nrOpDecls = opDecls.size();
  for (Index i = 0; i < nrOpDecls; ++i)
    {
      const OpDecl& opDecl = opDecls[i];
      Index defIndex = opDecl.defIndex;
      bool newFollow = (i + 1 < nrOpDecls) && (opDecls[i + 1].defIndex == defIndex);
      const OpDef& opDef = opDefs[defIndex];
      SymbolType st = opDef.symbolType;
      int basicType = st.getBasicType();
      //
      //	If we not a follow-on declaration, start a new line.
      //
      if (!follow)
	{
	  s << "\\par$\\maudeIndent\\maudeKeyword{";
	  if (basicType == SymbolType::VARIABLE)
	    s << "var";
	  else if (st.hasFlag(SymbolType::MSG_STATEMENT))
	    s << "msg";
	  else
	    s << "op";
	  s << (newFollow ? "s}" : "}");
	  s << "\\maudeSpace";
	}
      //
      //	Deal with variable/message/operator name.
      //
      int id = opDecl.prefixName.code();
      if (basicType == SymbolType::VARIABLE)
	{
	  //
	  //	We don't prettify variable names - they must remain single tokens.
	  //
	  s << Token::latexIdentifier(id);
	}
      else
	{
	  //
	  //	Check if we're on an ops/msgs line and need parens for multiple token op names.
	  //
	  bool multiDecl = follow || newFollow;
	  if (opDef.types.size() == 1 && (Token::auxProperty(id) == Token::AUX_STRUCTURED_SORT))
	    {
	      //
	      //	Not a variable, no arguments, and looks like a parameterized constant or constant message.
	      //
	      if (multiDecl)
		s << "\\maudeLeftParen";
	      s << flatModule->latexStructuredConstant(id);
	      if (multiDecl)
		s << "\\maudeRightParen";
	    }
	  else
	    {
	      //
	      //	Prettify op name, pass flags to detect an exposed colon, and for ops/msgs, multiple tokens.
	      //
	      s << MixfixModule::latexPrettyOpName(id, multiDecl ? (Token::EXPOSED_COLON | Token::MULTIPLE_TOKENS) : Token::EXPOSED_COLON);
	    }
	}
      follow = newFollow;
      if (follow)
	s << "\\maudeSpace";
      else
	{
	  latexOpDef(s, opDef);
	  s << "$\\maudeEndStatement\n";
	}
    }
  
  for (const StratDecl& sd : stratDecls)
    latexStratDecl(s, sd);
  
  for (const Vector<Token>& tokens : statements)
    {
      s << "\\par$\\maudeIndent\\maudeKeyword{" << tokens[0] << "}\\maudeSpace" <<
	MixfixModule::latexTokenVector(tokens, 1, tokens.size() - 1) << "$\\maudeEndStatement\n";
    }

  s << "\\par\\maudeKeyword{" << MixfixModule::moduleEndString(getModuleType()) << "}\n";
}

void
SyntacticPreModule::latexOpDef(ostream&s, const OpDef& opDef)
{
  if (opDef.symbolType.getBasicType() == SymbolType::VARIABLE)
    {
      s << "\\maudeHasSort";
      latexType(s, opDef.types[0]);
    }
  else
    {
      Index nrArgs = opDef.types.size() - 1;
      if (nrArgs == 0)
	s << "\\maudeConstantDecl";
      else
	{
	  s << "\\maudeHasSort";
	  for (Index i = 0; i < nrArgs; ++i)
	    {
	      if (i > 0)
		s << "\\maudeSpace";
	      latexType(s, opDef.types[i]);
	    }
	  s << "\\maudeFunction";
	}
      latexType(s, opDef.types[nrArgs]);
      latexAttributes(s, opDef);
    }
}

void
SyntacticPreModule::latexAttributes(ostream& s, const OpDef& opDef)
{
  SymbolType st = opDef.symbolType;
  if (st.hasFlag(SymbolType::MSG_STATEMENT))
    {
      //
      //	msg statement implies msg and ctor so we don't want to print these attributes.
      //
      st.clearFlags(SymbolType::MESSAGE | SymbolType::CTOR);
    }
  if (!(st.hasFlag(SymbolType::ATTRIBUTES | SymbolType::CTOR | SymbolType::POLY | SymbolType::DITTO)) &&
      opDef.special.empty() && opDef.metadata == NONE)
    return;

  const char* space = "";
  s << "\\maudeSpace\\maudeLeftBracket";
  if (st.hasFlag(SymbolType::POLY))
    {
      s << "\\maudeKeyword{poly}\\maudeSpace\\maudeLeftParen";
      for (int i : opDef.polyArgs)
	{
	  if  (i != 0)
	    {
	      s << space << "\\maudeNumber{" << i << "}";
	      space = "\\maudeSpace";
	    }
	}
      if (opDef.polyArgs.contains(0))
	s << space << "\\maudeNumber{0}";
      s << "\\maudeRightParen";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::CTOR))
    {
      s << space << "\\maudeKeyword{ctor}";
      space = "\\maudeSpace";
    }
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
    {
      s << space << "\\maudeKeyword{assoc}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::COMM))
    {
      s << space << "\\maudeKeyword{comm}";
      space = "\\maudeSpace";
    }
  //
  //	Collapse axioms.
  //
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << space;
      space = "\\maudeSpace";
      if (!(st.hasFlag(SymbolType::LEFT_ID)))
	s << "\\maudeKeyword{right}\\maudeSpace";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID)))
	s << "\\maudeKeyword{left}\\maudeSpace";
      s << "\\maudeKeyword{id:}\\maudeSpace" << MixfixModule::latexTokenVector(opDef.identity, 0, opDef.identity.size() - 1);
    }
  if (st.hasFlag(SymbolType::IDEM))
    {
      s << space << "\\maudeKeyword{idem}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::ITER))
    {
      s << space << "\\maudeKeyword{iter}";
      space = "\\maudeSpace";
    }
  //
  //	Module system attribute.
  //
  if (st.hasFlag(SymbolType::PCONST))
    {
      s << space << "\\maudeKeyword{pconst}";
      space = "\\maudeSpace";
    }
  //
  //	Object-oriented attributes.
  //
  if (st.hasFlag(SymbolType::OBJECT))
    {
      s << space << "\\maudeKeyword{obj}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::MESSAGE))
    {
      s << space << "\\maudeKeyword{msg}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::PORTAL))
    {
      s << space << "\\maudeKeyword{portal}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::CONFIG))
    {
      s << space << "\\maudeKeyword{config}";
      space = "\\maudeSpace";
    }
  //
  //	Semantic attributes.
  //
  if (st.hasFlag(SymbolType::STRAT))
    {
      s << space << "\\maudeKeyword{strat}\\maudeSpace\\maudeLeftParen";
      space = "\\maudeSpace";
      const char* sep = "";
      for (int i : opDef.strategy)
	{
	  s << sep << "\\maudeNumber{" << i << "}";
	  sep = "\\maudeSpace";
	}
      s << "\\maudeRightParen";
    }
  if (st.hasFlag(SymbolType::MEMO))
    {
      s << space << "\\maudeKeyword{memo}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::FROZEN))
    {
      s << space << "\\maudeKeyword{frozen}";
      space = "\\maudeSpace";
      if (!(opDef.frozen.empty()))
	{
	  s << "\\maudeSpace\\maudeLeftParen";
	  const char* sep = "";
	  for (int i : opDef.frozen)
	    {
	      s << sep << "\\maudeNumber{" << i << "}";
	      sep = "\\maudeSpace";
	    }
	  s << "\\maudeRightParen";
	}
    }
  //
  //	Syntactic attributes.
  //
  if (st.hasFlag(SymbolType::PREC))
    {
      s << space << "\\maudeKeyword{prec}\\maudeSpace\\maudeNumber{" << opDef.prec << "}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::GATHER))
    {
      s << space;
      space = "\\maudeSpace";
      MixfixModule::latexPrintGather(s, opDef.gather);
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << space;
      space = "\\maudeSpace";
      MixfixModule::latexPrintFormat(s, opDef.format);
    }
  if (st.hasFlag(SymbolType::LATEX))
    {
      s << space;
      space = "\\maudeSpace";
      MixfixModule::latexPrintLatexMacro(s, opDef.latexMacro);
    }
  if (st.hasFlag(SymbolType::RPO))
    {
      s << space << "\\maudeKeyword{rpo}\\maudeSpace\\maudeNumber{" << opDef.rpo << "}";
      space = "\\maudeSpace";
    }
  if (opDef.metadata != NONE)
    {
      s << space << "\\maudeKeyword{metadata}\\maudeSpace" << MixfixModule::latexString(opDef.metadata);
      space = "\\maudeSpace";
    }
  if (!(opDef.special.empty()))
    {
      s << space << "\\maudeKeyword{special}\\maudeSpace\\maudeLeftParen";
      space = "\\maudeSpace";
      if (getOwner()->getPrintFlag(Interpreter::PRINT_HOOKS))
	{
	  for (const Hook& h : opDef.special)
	    {
	      static const char* hookTypes[] = {"id-hook", "op-hook", "term-hook"};
	      s << "\\newline\\maudeKeyword{" << hookTypes[h.type] << "}\\maudeSpace\\maudeSymbolic{" << Token::latexName(h.name) << "}";
	      if (!(h.details.empty()))
		{
		  s << "\\maudeSpace\\maudeLeftParen" <<
		    MixfixModule::latexTokenVector(h.details, 0, h.details.size() - 1) <<
		    "\\maudeRightParen";
		}
	    }
	}
      else
	s << "\\maudeEllipsis";
      s << "\\maudeRightParen";
    }
  //
  //	Misc attributes.
  //
  if (st.hasFlag(SymbolType::DITTO))
    s << space << "\\maudeKeyword{ditto}";
  s << "\\maudeRightBracket";
}

void
SyntacticPreModule::latexStratDecl(ostream& s, const StratDecl& stratDecl)
{
  s << "\\par$\\maudeIndent\\maudeKeyword{strat" << pluralize(stratDecl.names.size()) << "}";
  for (const Token& t : stratDecl.names)
    s << "\\maudeSpace" << Token::latexIdentifier(t.code());
  Index arity = stratDecl.types.size() - 1;
  if (arity > 0)
    {
      s << "\\maudeHasSort";
      for (Index i = 0; i < arity; ++i)
	{
	  s << (i == 0 ? "" : "\\maudeSpace");
	  latexType(s, stratDecl.types[i]);
	}
    }
  s << "\\maudeStratAt";
  latexType(s, stratDecl.types[arity]);
  
  if (stratDecl.metadata != NONE)
    {
      s << "\\maudeSpace\\maudeLeftBracket\\maudeKeyword{metadata}\\maudeSpace" <<
	MixfixModule::latexString(stratDecl.metadata) << "\\maudeRightBracket";
    }
  s << "$\\maudeEndStatement\n";
}
