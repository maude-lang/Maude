/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for fixing up special symbols.
//

int
SyntacticPreModule::findHook(const Vector<Hook>& hookList, HookType type, int name)
{
  int nrHooks = hookList.length();
  for (int i = 0; i < nrHooks; i++)
    {
      const Hook& h = hookList[i];
      if (h.type == type && h.name == name)
	return i;
    }
  return NONE;
}

Symbol*
SyntacticPreModule::findHookSymbol(const Vector<Token>& fullName)
{
  Vector<Token> name;
  int len = fullName.size();
  for (int pos = 0; pos < len; pos++)
    {
      if (pos != 0 && fullName[pos].code() == colon)
	{
	  int prefixName = Token::bubbleToPrefixNameCode(name);
	  Vector<ConnectedComponent*> domain;
	  for (pos++; pos < len; pos++)
	    {
	      int code = fullName[pos].code();
	      if (code == partial)  // ~> token
		{
		  if (++pos < len)  // range sort given
		    {
		      if (Sort* s = flatModule->findSort(fullName[pos].code()))  // range sort found
			return flatModule->findSymbol(prefixName, domain, s->component());
		    }
		  break;
		}
	      else
		{
		  Sort* s = flatModule->findSort(fullName[pos].code());
		  if (s == 0)
		    break;
		  domain.append(s->component());
		}
	    }
	  break;
	}
      else
	name.append(fullName[pos]);
    }
  return 0;
}

bool
SyntacticPreModule::defaultFixUp(OpDef& opDef, Symbol* symbol)
{
  const Vector<Hook>& hookList = opDef.special;
  int nrHooks = hookList.length();
  for (int i = 0; i < nrHooks; i++)
    {
      const Hook& h = hookList[i];
      const char* name = Token::name(h.name);
      switch (h.type)
	{
	case ID_HOOK:
	  {
	    int nrDetails = h.details.length();
	    Vector<const char*> hd(nrDetails);
	    for (int j = 0; j < nrDetails; j++)
	      hd[j] = h.details[j].name();
	    if (!(symbol->attachData(opDef.domainAndRange, name, hd)))
	      {
		DebugAdvisory("ID_HOOK " << name << " failed");
		return false;
	      }
	    break;
	  }
	case OP_HOOK:
	  {
	    Symbol* hs = findHookSymbol(h.details);
	    if (hs == 0 || !(symbol->attachSymbol(name, hs)))
	      {
		DebugAdvisory("OP_HOOK " << name << " failed");
		return false;
	      }
	    break;
	  }
	case TERM_HOOK:
	  {
	    Term* ht = flatModule->parseTerm(h.details); // potential component problem
	    if (ht == 0 || !(symbol->attachTerm(name, ht)))
	      {
		DebugAdvisory("TERM_HOOK " << name << " failed");
		return false;
	      }
	    break;
	  }
	}
    }
  return true;
}

bool
SyntacticPreModule::defaultFixUp(OpDef& opDef, int index)
{
  const Vector<Hook>& hookList = opDef.special;
  int nrHooks = hookList.length();
  for (int i = 0; i < nrHooks; i++)
    {
      const Hook& h = hookList[i];
      switch (h.type)
	{
	case ID_HOOK:
	  {
	    int nrDetails = h.details.length();
	    Vector<int> hd(nrDetails);
	    for (int j = 0; j < nrDetails; j++)
	      hd[j] = h.details[j].code();
	    flatModule->addIdHookToPolymorph(index, h.name, hd);
	    break;
	  }
	case OP_HOOK:
	  {
	    Symbol* hs = findHookSymbol(h.details);
	    if (hs == 0)
	      return false;
	    flatModule->addOpHookToPolymorph(index, h.name, hs);
	    break;
	  }
	case TERM_HOOK:
	  {
	    Term* ht = flatModule->parseTerm(h.details); // potential component problem
	    if (ht == 0)
	      return false;
	    flatModule->addTermHookToPolymorph(index, h.name, ht);
	    break;
	  }
	}
    }
  return true;
}

void
SyntacticPreModule::fixUpSymbols()
{
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      OpDecl& opDecl = opDecls[i];
      OpDef& opDef = opDefs[opDecl.defIndex];
      if (opDef.symbolType.hasFlag(SymbolType::POLY))
	fixUpPolymorph(opDecl);
      else
	fixUpSymbol(opDecl);
    }
}

#define FIX_UP_FAILED	{flatModule->markAsBad(); return;}

void
SyntacticPreModule::fixUpSymbol(const OpDecl& opDecl)
{
  OpDef& opDef = opDefs[opDecl.defIndex];
  //
  //	Might not have an identity to parse if it is a ditto declaration.
  //
  if (!opDef.identity.empty())
    {
      //
      //	The validation of attributes in the flattened module may have
      //	removed the identity attributes.
      //
      Symbol* symbol = opDecl.symbol;
      SymbolType symbolType = flatModule->getSymbolType(symbol);
      if (symbolType.hasAtLeastOneFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	{
	  //
	  //	Might not be a binary symbol if it got converted to
	  //	a free symbol during error recovery.
	  //
	  BinarySymbol* s = safeCast(BinarySymbol*, symbol);
	  Sort* wanted = opDef.domainAndRange[symbolType.hasFlag(SymbolType::LEFT_ID) ? 0 : 1];
	  Term* id = flatModule->parseTerm(opDef.identity, wanted->component());
	  if (id == 0)
	    FIX_UP_FAILED;
	  if (Term* oldId = s->getIdentity())
	    {
	      WarningCheck(id->equal(oldId), *id <<
			   ": declaration of identity " << QUOTE(id) <<
			   " for operator " << QUOTE(s) <<
			   " clashes with previously declared identity " <<
			   QUOTE(oldId) << " in " << *oldId << '.');
	      id->deepSelfDestruct();
	    }
	  else
	    s->setIdentity(id);
	}
    }
  //
  //	Handle specials.
  //
  if (opDef.symbolType.hasAttachments())
    {
      if (!defaultFixUp(opDef, opDecl.symbol))
	{
	  IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
		       ": bad special for operator " <<
		       QUOTE(opDecl.prefixName) << '.');
	  FIX_UP_FAILED;
	}
    }
  else if (opDef.symbolType.getBasicType() == SymbolType::BUBBLE)
    {
      Symbol* quotedIdentifierSymbol = 0;
      Symbol* nilQidListSymbol = 0;
      Symbol* qidListSymbol = 0;
      int h = findHook(opDef.special, OP_HOOK, qidSymbolToken);
      if (h != NONE)
	quotedIdentifierSymbol = findHookSymbol(opDef.special[h].details);
      h = findHook(opDef.special, OP_HOOK, nilQidListSymbolToken);
      if (h != NONE)
	nilQidListSymbol = findHookSymbol(opDef.special[h].details);
      h = findHook(opDef.special, OP_HOOK, qidListSymbolToken);
      if (h != NONE)
	qidListSymbol = findHookSymbol(opDef.special[h].details);
      flatModule->fixUpBubbleSpec(opDecl.bubbleSpecIndex,
				  quotedIdentifierSymbol,
				  nilQidListSymbol,
				  qidListSymbol);
    }
}

void
SyntacticPreModule::fixUpPolymorph(const OpDecl& opDecl)
{
  OpDef& opDef = opDefs[opDecl.defIndex];
  int index = opDecl.polymorphIndex;
  //
  //	Might not have an identity to parse if it is a ditto declaration.
  //
  if (!opDef.identity.empty())
    {
      //
      //	The validation of attributes in the flattened module may have
      //	removed some.
      //
      SymbolType polymorphType = flatModule->getPolymorphType(index);
      if (polymorphType.hasAtLeastOneFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	{
	  int nonPolyIndex = polymorphType.hasFlag(SymbolType::LEFT_ID) ? 0 : 1;
	  Sort* wanted = opDef.domainAndRange[nonPolyIndex];
	  Assert(wanted != 0, "illegal polymophic arg - should have be fixed by validation");
	  Term* id = flatModule->parseTerm(opDef.identity, wanted->component());
	  if (id == 0)
	    FIX_UP_FAILED;
	  if (Term* oldId = flatModule->getPolymorphIdentity(index))
	    {
	      WarningCheck(id->equal(oldId), *id <<
			   ": declaration of identity " << QUOTE(id) <<
			   " for polymorphic operator " << QUOTE(opDecl.prefixName) <<
			   " clashes with previously declared identity " <<
			   QUOTE(oldId) << " in " << *oldId << '.');
	      id->deepSelfDestruct();
	    }
	  else
	    flatModule->addIdentityToPolymorph(index, id);
	}
    }
  if (opDef.symbolType.hasAttachments())
    {
      if (!defaultFixUp(opDef, index))
	{
	  IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
		       ": bad special for polymorphic operator " <<
		       QUOTE(opDecl.prefixName) << '.');
	  FIX_UP_FAILED;
	}
    }
}
