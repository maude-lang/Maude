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
#define LINE_NUMBER	LineNumber(opDef.types[0].tokens[0].lineNumber())

void
SyntacticPreModule::addVarDecl(Token varName)
{
  if (!lastSawOpDecl)
    {
      int nrOpDefs = opDefs.length();
      opDefs.expandBy(1);
      opDefs[nrOpDefs].symbolType.setBasicType(SymbolType::VARIABLE);
    }
  int nrOpDecls = opDecls.length();
  opDecls.expandBy(1);
  opDecls[nrOpDecls].prefixName = varName;
  opDecls[nrOpDecls].defIndex = opDefs.length() - 1;
  lastSawOpDecl = true;
}

void
SyntacticPreModule::addOpDecl(const Vector<Token>& opName)
{
  if (!lastSawOpDecl)
    opDefs.expandBy(1);

  int nrOpDecls = opDecls.length();
  opDecls.expandBy(1);
  opDecls[nrOpDecls].prefixName.makePrefixName(opName);
  opDecls[nrOpDecls].defIndex = opDefs.length() - 1;
  lastSawOpDecl = true;
}

void
SyntacticPreModule::addType(bool kind, const Vector<Token>& tokens)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  opDef.types.expandBy(1);
  Type& type = opDef.types[opDef.types.length() - 1];
  type.kind = kind;
  type.tokens = tokens;  // deep copy
  lastSawOpDecl = false;
}

void
SyntacticPreModule::convertSortsToKinds()
{
  Assert(!lastSawOpDecl, "missing sort specs");
  OpDef& opDef = opDefs[opDefs.length() - 1];
  int nrTypes = opDef.types.length();
  for (int i = 0; i < nrTypes; i++)
    {
      if (opDef.types[i].kind)
	{
	  IssueWarning(LineNumber(opDef.types[i].tokens[0].lineNumber()) <<
		       ": superfluous [] notation in operation declaration");
	}
      else
	opDef.types[i].kind = true;
    }
}

void
SyntacticPreModule::setIdentity(const Vector<Token>& identity)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.identity.empty())
    opDef.identity = identity;  // deep copy
  else
    IssueWarning(LINE_NUMBER << ": multiple identity declarations.");
}

void
SyntacticPreModule::setFlag(int flag)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(flag))
    {
      if (flag & SymbolType::SIMPLE_ATTRIBUTES)
	IssueWarning(LINE_NUMBER << ": duplicate attributes.");
    }
  else
    opDef.symbolType.setFlags(flag);
}

void
SyntacticPreModule::setPrec(Token precTok)
{
  int prec;
  if (precTok.getInt(prec) &&
      prec >= MixfixModule::MIN_PREC && prec <= MixfixModule::MAX_PREC)
    {
      OpDef& opDef = opDefs[opDefs.length() - 1];
      if (opDef.symbolType.hasFlag(SymbolType::PREC))
	IssueWarning(LINE_NUMBER << ": multiple precedence attributes.");
      else
	{
	  opDef.prec = prec;
	  opDef.symbolType.setFlags(SymbolType::PREC);
	}
    }
  else
    {
      IssueWarning(LineNumber(precTok.lineNumber()) <<
		   ": bad value " << QUOTE(precTok) << " for prec attribute.");
    }
}

void
SyntacticPreModule::setGather(const Vector<Token>& gather)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::GATHER))
    {
      IssueWarning(LINE_NUMBER << ": multiple gather attributes.");
      return;
    }
  int length = gather.length();
  if (length != opDef.types.length() - 1)
    {
      IssueWarning(LINE_NUMBER << ": bad gather length.");
      return;
    }
  for (int i = 0; i < length; i++)
    {
      const char* string = gather[i].name();
      if (string[0] != '\0' && string[1] == '\0')
	{
	  switch(string[0])
	    {
	    case '&':
	      opDef.gather.append(MixfixModule::GATHER_AMP);
	      continue;
	    case 'E':
	      opDef.gather.append(MixfixModule::GATHER_E);
	      continue;
	    case 'e':
	      opDef.gather.append(MixfixModule::GATHER_e);
	      continue;
	    }
	}
      IssueWarning(LineNumber(gather[i].lineNumber()) <<
		   ": bad value " << QUOTE(string) <<
		   " in gather attribute. Recovering by ignoring gather attribute.");
      opDef.gather.clear();  // for safety
      return;
    }
  opDef.symbolType.setFlags(SymbolType::GATHER);
}

void
SyntacticPreModule::setMetadata(Token metaDataTok)
{
  if (metaDataTok.specialProperty() == Token::STRING)
    {
      OpDef& opDef = opDefs[opDefs.length() - 1];
      if (opDef.metadata == NONE)
	opDef.metadata = metaDataTok.code();
      else
	IssueWarning(LINE_NUMBER << ": multiple metadata attributes.");
      //opDef.symbolType.setFlags(SymbolType::METADATA);
    }
  else
    {
      IssueWarning(LineNumber(metaDataTok.lineNumber()) <<
		   ": bad value " << QUOTE(metaDataTok) << " for metadata attribute.");
    }
}

bool
SyntacticPreModule::checkFormatString(const char* string)
{
  if (string[0] == 'd' && string[1] == '\0')
    return true;
  for(;;)
    {
      switch (*string++)
	{
	case '\0':
	  return true;
	case '+':
	case '-':
	case 's':
	case 't':
	case 'n':
	case 'i':
#define MACRO(m, t)	case m:
#include "ansiEscapeSequences.cc"
#undef MACRO
	case 'o':
	  continue;
	default:
	  return false;
	}
    }
}

void
SyntacticPreModule::setFormat(const Vector<Token>& format)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::FORMAT))
    {
      IssueWarning(LINE_NUMBER << ": multiple format attributes.");
      return;
    }
  int length = format.length();
  for (int i = 0; i < length; i++)
    {
      const char* str = format[i].name();
      if (checkFormatString(str))
	opDef.format.append(format[i].code());
      else
	{
	  IssueWarning(LineNumber(format[i].lineNumber()) <<
		       ": bad value " << QUOTE(str) <<
		       " in format attribute. Recovering by ignoring format attribute.");
	  opDef.format.clear();  // for safety
	  return;
	}
    }
  opDef.symbolType.setFlags(SymbolType::FORMAT);
}

void
SyntacticPreModule::setFrozen(const Vector<Token>& frozen)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::FROZEN))
    {
      IssueWarning(LINE_NUMBER << ": multiple frozen attributes.");
      return;
    }
  int maxArgPos = opDef.types.length() - 1;
  int length = frozen.length();
  if (length == 0)
    {
      if (maxArgPos == 0)
	{
	  IssueWarning("cannot have frozen attribute for constant.");
	  return;
	}
      for (int i = 0; i < maxArgPos; i++)
	opDef.frozen.insert(i);
    }
  else
    {
      for (int i = 0; i < length; i++)
	{
	  const char* str = frozen[i].name();
	  char* pointer;
	  long argPos = strtol(str, &pointer, 10);
	  if (pointer != str && *pointer == '\0' &&
	      argPos >= 1 && argPos <= maxArgPos)
	    {
	      WarningCheck(!(opDef.frozen.contains(argPos - 1)),
			   LineNumber(frozen[i].lineNumber()) <<
			   ": argument " << QUOTE(argPos) <<
			   " mentioned twice in frozen attribute.");
	      opDef.frozen.insert(argPos - 1);
	    }
	  else
	    {
	      IssueWarning(LineNumber(frozen[i].lineNumber()) <<
			   ": bad value " << QUOTE(str) <<
			   " in frozen attribute. Recovering by ignoring frozen attribute.");
	      opDef.frozen.clear();  // for safety
	      return;
	    }
	}
    }
  opDef.symbolType.setFlags(SymbolType::FROZEN);
}

void
SyntacticPreModule::setStrat(const Vector<Token>& strategy)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::STRAT))
    {
      IssueWarning(LINE_NUMBER << ": multiple strategy attributes.");
      return;
    }
  int maxArgPos = opDef.types.length();
  int length = strategy.length();
  for (int i = 0; i < length; i++)
    {
      const char* str = strategy[i].name();
      char* pointer;
      long argPos = strtol(str, &pointer, 10);
      if (pointer != str && *pointer == '\0' &&
	  argPos >= -maxArgPos && argPos <= maxArgPos)
	opDef.strategy.append(static_cast<int>(argPos));
      else
	{
	  IssueWarning(LineNumber(strategy[i].lineNumber()) <<
		       ": bad value " << QUOTE(str) <<
		       " in strategy attribute. Recovering by ignoring strategy attribute.");
	  opDef.strategy.clear();  // for safety
	  return;
	}
    }
  opDef.symbolType.setFlags(SymbolType::STRAT);
}

void
SyntacticPreModule::setPoly(const Vector<Token>& polyArgs)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::POLY))
    {
      IssueWarning(LINE_NUMBER << ": multiple polymorphic attributes.");
      return;
    }
  int maxArgPos = opDef.types.length();
  int length = polyArgs.length();
  for (int i = 0; i < length; i++)
    {
      const char* str = polyArgs[i].name();
      char* pointer;
      long argPos = strtol(str, &pointer, 10);
      if (pointer != str && *pointer == '\0' &&
	  argPos >= 0 && argPos <= maxArgPos)
	opDef.polyArgs.insert(argPos);
      else
	{
	  IssueWarning(LineNumber(polyArgs[i].lineNumber()) <<
		       ": bad value " << QUOTE(str) <<
		       " in polymorphic attribute. Recovering by ignoring value.");
	}
    }
  opDef.symbolType.setFlags(SymbolType::POLY);
}

void
SyntacticPreModule::setLatexMacro(const string& latexMacro)
{
  OpDef& opDef = opDefs[opDefs.length() - 1];
  if (opDef.symbolType.hasFlag(SymbolType::LATEX))
    IssueWarning(LINE_NUMBER << ": multiple latex attributes.");
  else
    {
      opDef.latexMacro = latexMacro;
      opDef.symbolType.setFlags(SymbolType::LATEX);
    }
}

void
SyntacticPreModule::addHook(HookType type, Token name, const Vector<Token>& details)
{
  int code = name.code();
  OpDef& opDef = opDefs[opDefs.length() - 1];
  int nrHooks = opDef.special.length();
  opDef.special.expandBy(1);
  Hook& hook = opDef.special[nrHooks];
  hook.type = type;
  hook.name = code;
  hook.details = details;  // deep copy
  if (type == ID_HOOK)
    {
      int b = SymbolType::specialNameToBasicType(name.name());
      if (b != SymbolType::STANDARD)
	opDef.symbolType.setBasicType(b);
    }
}

void
SyntacticPreModule::makeOpDeclsConsistent()
{
  //
  //	Call if we encounter a syntax error while parsing module to make sure
  //	opDefs/opDecls are left in a good state.
  //
  int nrOpDefs = opDefs.length();
  if (nrOpDefs == 0)
    return;
  int lastDefIndex = nrOpDefs - 1;
  if(opDefs[lastDefIndex].types.length() == 0)
    {
      //
      //	Problem: we have op decls for which op def has not been
      //	properly filled in; quietly delete them and reset
      //	lastSawOpDecl flag
      //
      int m = opDecls.length();
      while (m > 0 && opDecls[m - 1].defIndex == lastDefIndex)
	--m;
      opDecls.contractTo(m);
      lastSawOpDecl = false;
    }
}
