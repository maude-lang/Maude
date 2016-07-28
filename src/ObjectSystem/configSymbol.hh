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
//      Class for symbols for object-message configurations.
//
#ifndef _configSymbol_hh_
#define _configSymbol_hh_
#include <map>
#include "natSet.hh"
#include "ACU_Symbol.hh"

class ConfigSymbol : public ACU_Symbol
{
public:
  ConfigSymbol(int id,
	       const Vector<int>& strategy,
	       bool memoFlag,
	       Term* identity = 0);
  //
  //	Override normal rule rewriting behavior.
  //
  void compileRules();
  DagNode* ruleRewrite(DagNode* subject, RewritingContext& context);
  void resetRules();
  //
  //	Functions particular to ConfigSymbol.
  //
  void addObjects(NatSet& objSymbols);
  void addMessages(NatSet& msgSymbols);

private:
  struct symbolLt
  {
    bool operator()(const Symbol* d1, const Symbol* d2) const
    {
      return d1->compare(d2) < 0;
    }
  };

  struct RuleSet
  {
    Vector<Rule*> rules;
    Vector<Rule*>::const_iterator next;
  };

  typedef map<Symbol*, RuleSet, symbolLt> RuleMap;

  struct MessageQueue;
  struct dagNodeLt;
  class ObjectMap;
  struct Remainder;

  bool checkArgs(Term* pattern, Term*& object, Term*& message);
  DagNode* objMsgRewrite(Symbol* messageSymbol,
			 DagNode* subject,
			 RewritingContext& context);
  DagNode* retrieveObject(DagNode* rhs, DagNode* name, Remainder& remainder);
  DagNode* leftOverRewrite(DagNode* subject,
			   RewritingContext& context,
			   ExtensionInfo* extensionInfo);

  NatSet objectSymbols;
  NatSet messageSymbols;
  RuleMap ruleMap;
  RuleSet leftOver;
};

#endif
