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
  Vector<Token> name;
  int len = fullName.size();
  for (int pos = 0; pos < len; pos++)
    {
      if (fullName[pos].code() == colon)
	{
	  int prefixName = Token::bubbleToPrefixNameCode(name);
	  Vector<ConnectedComponent*> domain;
	  for (pos++; pos < len; pos++)
	    {
	      int code = fullName[pos].code();
	      if (code == partial)
		{
		  if (++pos < len)
		    {
		      if (Sort* s = flatModule->findSort(fullName[pos].code()))
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
PreModule::defaultFixUp(OpDef& opDef, int index)
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
	  if (opDef.symbolType.hasFlag(SymbolType::POLY))
	    {
	      int nonPolyIndex = opDef.symbolType.hasFlag(SymbolType::LEFT_ID) ? 0 : 1;
	      Sort* wanted = opDef.domainAndRange[nonPolyIndex];
	      if (wanted == 0)
		{
		  static const char* lr[] = {"left", "right"};
		  IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
			       ": polymorphic operator " << QUOTE(opDecl.prefixName) <<
			       " cannot have a " << lr[nonPolyIndex] << " identity.");
		  FIX_UP_FAILED;
		}
	      Term* id = flatModule->parseTerm(opDef.identity, wanted->component());
	      if (id == 0)
		FIX_UP_FAILED;
	      int index = opDecl.polymorphIndex;
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
		flatModule->addIdentityToPolymorph(opDecl.polymorphIndex, id);
	    }
	  else
	    {
	      //
	      //	Might not be a binary symbol if it got converted to
	      //	a free symbol during error recovery.
	      //
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
	}
      else
	{
	  if (opDef.symbolType.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
	    {
	      IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
			   ": missing identity for operator " <<
			   QUOTE(opDecl.prefixName) << '.');
	      FIX_UP_FAILED;
	    }      
	}
      //
      //	Handle specials.
      //
      if (opDef.symbolType.hasAttachments())
	{
	  if (opDef.symbolType.hasFlag(SymbolType::POLY))
	    {
	      if (!defaultFixUp(opDef, opDecl.polymorphIndex))
		{
		  IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
			       ": bad special for polymorphic operator " <<
			       QUOTE(opDecl.prefixName) << '.');
		  FIX_UP_FAILED;
		}
	    }
	  else
	    {
	      if (!defaultFixUp(opDef, opDecl.symbol))
		{
		  IssueWarning(LineNumber(opDecl.prefixName.lineNumber()) <<
			       ": bad special for operator " <<
			       QUOTE(opDecl.prefixName) << '.');
		  FIX_UP_FAILED;
		}
	    }
	}
      else if (opDef.symbolType.getBasicType() == SymbolType::BUBBLE)
	{
	  Symbol* quotedIdentifierSymbol = 0;
	  Symbol* nilQidListSymbol = 0;
	  Symbol* qidListSymbol = 0;
	  int h = findHook(opDef.special, OP_HOOK, Token::encode("qidSymbol"));
	  if (h != NONE)
	    quotedIdentifierSymbol = findHookSymbol(opDef.special[h].details);
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
	}
    }
}
