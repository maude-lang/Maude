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
//	Code for fixing up special symbols.
//

int
PreModule::findHook(const Vector<Hook>& hookList, HookType type, int name)
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
PreModule::findHookSymbol(const Vector<Token>& fullName)
{
  int colon = Token::encode(":");
  int arrow = Token::encode("~>");
  
  static Vector<Token> name;
  name.contractTo(0);
  int pos;
  for (pos = 0; fullName[pos].code() != colon; pos++)
    name.append(fullName[pos]);
  int prefixName = Token::bubbleToPrefixNameCode(name);

  static Vector<ConnectedComponent*> domain;
  domain.contractTo(0);
  for (pos++; fullName[pos].code() != arrow; pos++)
    {
      Sort* s = flatModule->findSort(fullName[pos].code());
      if (s == 0)
	return 0;
      domain.append(s->component());
    }
  ++pos;
  return flatModule->findSymbol(prefixName,
				domain,
				flatModule->findSort(fullName[pos].code())->component());
}

bool
PreModule::defaultFixUp(OpDef& opDef, Symbol* symbol)
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
	      return false;
	    break;
	  }
	case OP_HOOK:
	  {
	    Symbol* hs = findHookSymbol(h.details);
	    if (hs == 0 || !(symbol->attachSymbol(name, hs)))
	      return false;
	    break;
	  }
	case TERM_HOOK:
	  {
	    Term* ht = flatModule->parseTerm(h.details); // potential component problem
	    if (ht == 0 || !(symbol->attachTerm(name, ht)))
	      return false;
	    break;
	  }
	}
    }
  return true;
}

#define FIX_UP_FAILED	{flatModule->markAsBad(); continue;}

void
PreModule::fixUpSymbols()
{
  int nrOpDecls = opDecls.length();
  for (int i = 0; i < nrOpDecls; i++)
    {
      OpDecl& opDecl = opDecls[i];
      OpDef& opDef = opDefs[opDecl.defIndex];
      //
      //	Handle identities.
      //
      if (opDef.identity.length() != 0)
	{
	  if (BinarySymbol* s = dynamic_cast<BinarySymbol*>(opDecl.symbol))
	    {
	      Sort* wanted =
		opDef.domainAndRange[opDef.symbolType.hasFlag(SymbolType::LEFT_ID) ? 0 : 1];
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
	      IssueWarning("bad special for " << QUOTE(opDecl.symbol) << '.');
	      FIX_UP_FAILED;
	    }
	}
      else if (opDecl.originator)
	{
	  //
	  //	We have responsibility for fixing up symbol.
	  //
	  switch (opDef.symbolType.getBasicType())
	    {
	    case SymbolType::BRANCH_SYMBOL:
	      {
		Vector<Term*> terms;
		ConnectedComponent* component = opDef.domainAndRange[0]->component();
		int h = findHook(opDef.special, TERM_HOOK, Token::encode("trueTerm"));
		if (h == NONE)
		  FIX_UP_FAILED;
		terms.append(flatModule->parseTerm(opDef.special[h].details, component));
		h = findHook(opDef.special, TERM_HOOK, Token::encode("falseTerm"));
		if (h == NONE)
		  FIX_UP_FAILED;
		terms.append(flatModule->parseTerm(opDef.special[h].details, component));
		flatModule->fixUpPolymorph(opDecl.polymorphIndex, terms);
		if (terms[0] == 0 || terms[1] == 0)
		  FIX_UP_FAILED;
		break;
	      }
	    case SymbolType::EQUALITY_SYMBOL:
	      {
		Vector<Term*> terms;
		ConnectedComponent* component = opDef.domainAndRange[2]->component();
		int h = findHook(opDef.special, TERM_HOOK, Token::encode("equalTerm"));
		if (h == NONE)
		  FIX_UP_FAILED;
		terms.append(flatModule->parseTerm(opDef.special[h].details, component));
		h = findHook(opDef.special, TERM_HOOK, Token::encode("notEqualTerm"));
		if (h == NONE)
		  FIX_UP_FAILED;
		terms.append(flatModule->parseTerm(opDef.special[h].details, component));
		flatModule->fixUpPolymorph(opDecl.polymorphIndex, terms);
		if (terms[0] == 0 || terms[1] == 0)
		  FIX_UP_FAILED;
		break;
	      }
	    case SymbolType::BUBBLE:
	      {
		int h = findHook(opDef.special, OP_HOOK, Token::encode("qidSymbol"));
		if (h == NONE)
		  FIX_UP_FAILED;
		QuotedIdentifierSymbol* quotedIdentifierSymbol =
		  static_cast<QuotedIdentifierSymbol*>(findHookSymbol(opDef.special[h].details));
		if (quotedIdentifierSymbol == 0)
		  FIX_UP_FAILED;
		Symbol* nilQidListSymbol = 0;
		Symbol* qidListSymbol = 0;
		h = findHook(opDef.special, OP_HOOK, Token::encode("nilQidListSymbol"));
		if (h != NONE)
		  nilQidListSymbol = findHookSymbol(opDef.special[h].details);
		h = findHook(opDef.special, OP_HOOK, Token::encode("qidListSymbol"));
		if (h != NONE)
		  qidListSymbol = findHookSymbol(opDef.special[h].details);
		flatModule->fixUpBubbleSpec(opDecl.bubbleSpecIndex,
					    quotedIdentifierSymbol,
					    nilQidListSymbol,
					    qidListSymbol);
		break;
	      }
	    }
	}
    }
}
