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

//
//      Implementation for class Renaming.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "core.hh"
#include "interface.hh"
#include "strategyLanguage.hh"
#include "mixfix.hh"

//      interface class definitions
#include "term.hh"

//		core class definitions
#include "rewriteStrategy.hh"

//	front end class definitions
#include "token.hh"
#include "syntacticPreModule.hh"
#include "importModule.hh"
#include "renaming.hh"

void
Renaming::setType(set<int>& type, const ConnectedComponent* component)
{
  int nrSorts = component->nrSorts();
  for (int i = 1; i < nrSorts; i++)
    type.insert(component->sort(i)->id());
}

Rope
Renaming::makeTypeName(const set<int>& type)
{
  Rope name;
  FOR_EACH_CONST(i, set<int>, type)
    {
      name += name.empty() ? "[" : ",";
      name += Token::sortName(*i);
    }
  return name + "]";
}

Rope
Renaming::makeCanonicalName() const
{
  Rope name;
  {
    FOR_EACH_CONST(i, IdMap, sortMap)
      {
	if (!name.empty())
	  name += ", ";
	name += "sort ";
	name += Token::sortName(i->first);
	name += " to ";
	name += Token::sortName(i->second);
      }
  }
  {
    FOR_EACH_CONST(i, IdMap, labelMap)
      {
	if (!name.empty())
	  name += ", ";
	name += "label ";
	name += Token::name(i->first);
	name += " to ";
	name += Token::name(i->second);
      }
  }
  int nrOpMappings = opMap.size();
  if (nrOpMappings > 0)
    {
      Vector<Rope> opRopes(nrOpMappings);
      {
	Vector<Rope>::iterator j = opRopes.begin();
	FOR_EACH_CONST(i, OpMap, opMap)
	  {
	    *j = "op ";
	    *j += Token::name(i->first);
	    if (!(i->second.types.empty()))
	      {
		*j += " :";
		int nrArgs = i->second.types.size() - 1;
		for (int k = 0; k < nrArgs; k++)
		  {
		    *j += " ";
		    *j += makeTypeName(i->second.types[k]);
		  }
		*j += " -> ";
		*j += makeTypeName(i->second.types[nrArgs]);
	      }
	    *j += " to ";
	    *j += Token::name(i->second.name);
	    bool attr = false;
	    if (i->second.prec >= MixfixModule::MIN_PREC)
	      {
		*j += attr ? " " : " [";
		*j += "prec ";
		*j += int64ToString(i->second.prec);
		attr = true;
	      }
	    if (!(i->second.gather.empty()))
	      {
		*j += attr ? " " : " [";
		*j += "gather ";
		int nrArgs = i->second.gather.size();
		for (int k = 0; k < nrArgs; k++)
		  {
		    *j += (k == 0) ? "(" : " ";
		    switch (i->second.gather[k])
		      {
		      case MixfixModule::GATHER_AMP:
			{
			  *j += '&';
			  break;
			}
		      case MixfixModule::GATHER_E:
			{
			  *j += 'E';
			  break;
			}
		      case MixfixModule::GATHER_e:
			{
			  *j += 'e';
			  break;
			}
		      }
		  }
		*j += ")";
		attr = true;
	      }
	    if (!(i->second.format.empty()))
	      {
		*j += attr ? " " : " [";
		*j += "format ";
		int nrWords = i->second.format.size();
		for (int k = 0; k < nrWords; k++)
		  {
		    *j += (k == 0) ? "(" : " ";
		    *j += Token::name(i->second.format[k]);
		  }
		*j += ")";
		attr = true;
	      }
	    if (!(i->second.latexMacro.empty()))
	      {
		*j += attr ? " " : " [";
		*j += "latex \"";
		*j += i->second.latexMacro.c_str();
		*j += "\"";
		attr = true;
	      }
	    if (attr)
	      *j += "]";
	    ++j;
	  }
      }
      sort(opRopes.begin(), opRopes.end());
      {
	FOR_EACH_CONST(i, Vector<Rope>, opRopes)
	  {
	    if (!name.empty())
	      name += ", ";
	    name += *i;
	  }
      }
    }
  int nrStratMappings = stratMap.size();
  if (nrStratMappings > 0)
    {
      Vector<Rope> stratRopes(nrStratMappings);
      {
	Vector<Rope>::iterator j = stratRopes.begin();
	for (const auto& mapping : stratMap)
	  {
	    *j = "strat ";
	    *j += Token::name(mapping.first);
	    if (!mapping.second.types.empty())
	      {
		*j += " :";
		int nrArgs = mapping.second.types.size() - 1;
		for (int k = 0; k < nrArgs; k++)
		  {
		    *j += " ";
		    *j += makeTypeName(mapping.second.types[k]);
		  }
	      }
	    *j += " to ";
	    *j += Token::name(mapping.second.name);
	    ++j;
	  }
      }
      sort(stratRopes.begin(), stratRopes.end());
      {
	for (const Rope& rope : stratRopes)
	  {
	    if (!name.empty())
	      name += ", ";
	    name += rope;
	  }
      }
    }
  return name;
}

Renaming*
Renaming::makeCanonicalVersion(ImportModule* module) const
{
  Renaming* canonical = new Renaming;
  {
    //
    //	Prune sort mappings to those that actually occur
    //	in the module. We do not allow sorts that come from
    //	a parameter to be mapped.
    //
    FOR_EACH_CONST(i, IdMap, sortMap)
      {
	Sort* s = module->findSort(i->first);
	DebugAdvisory("looking for sort " << s << " in module to be renamed " << module);
	if (s != 0)
	  {
	    if (module->parameterDeclared(s))
	      {
		IssueAdvisory("Ignoring a sort mapping because sort " <<
			      QUOTE(s) << " comes from a parameter.");
	      }
	    else
	      {
		DebugAdvisory("added sort mapping " << s << " to " << Token::name(i->second));
		pair<IdMap::iterator, bool> p = canonical->sortMap.insert(*i);
		Assert(p.second, "dup sort mapping");
		canonical->sortMapIndex.append(p.first);
	      }
	  }
      }
  }
  {
    //
    //	Prune label mappings to those that actually occur
    //	in the module.
    //
    const set<int>& labels = module->getLabels();
    FOR_EACH_CONST(i, IdMap, labelMap)
      {
	if (labels.find(i->first) != labels.end())
	  {
	    pair<IdMap::iterator, bool> p = canonical->labelMap.insert(*i);
	    Assert(p.second, "dup label mapping");
	    canonical->labelMapIndex.append(p.first);
	  }
      }
  }
  {
    IdSet genericsToConsider;	// generics that affect an operator
    IdSet genericsToAvoid;	// subset that affect an operator from a parameter
    {
      //
      //	For each operator in the module that is not from a parameter
      //	and would be remapped by an orginal mapping, we add a mapping
      //	to the canonical renaming unless the mapping is a generic one
      //	in which case we add the id to genericsToConsider. For operators
      //	from parameter theories, if the mapping is specific we emit an
      //	advisory otherwise we add it to both genericsToConsider and
      //	genericsToAvoid so as to be able to generate an advisory.
      //
      const Vector<Symbol*> symbols = module->getSymbols();
      int nrUserSymbols = module->getNrUserSymbols();
      for (int i = 0; i < nrUserSymbols; i++)
	{
	  Symbol* symbol = symbols[i];
	  int id = symbol->id();
	  const OpMap::const_iterator e = opMap.upper_bound(id);
	  for (OpMap::const_iterator j = opMap.lower_bound(id); j != e; ++j)
	    {
	      const Vector<IdSet>& types = j->second.types;
	      if (module->parameterDeclared(symbol))
		{
		  if (types.empty())
		    {
		      genericsToConsider.insert(id);
		      genericsToAvoid.insert(id);
		    }
		  else
		    {
		      IssueAdvisory("Ignoring a specific op mapping because operator " <<
				    QUOTE(symbol) << " comes from a parameter.");
		    }
		}
	      else
		{
		  if (types.empty())
		    genericsToConsider.insert(id);
		  else
		    {
		      //
		      //	arity specific case
		      //
		      if (typeMatch(types, symbol))
			{
			  OpMap::iterator n =
			    canonical->opMap.insert(OpMap::value_type(id, OpMapping()));
			  int nrTypes = types.size();
			  n->second.types.resize(nrTypes);
			  --nrTypes;
			  for (int i = 0; i < nrTypes; i++)
			    setType(n->second.types[i], symbol->domainComponent(i));
			  setType(n->second.types[nrTypes], symbol->rangeComponent());
			  n->second.name = j->second.name;
			  n->second.term = 0;
			  n->second.prec = j->second.prec;
			  n->second.gather = j->second.gather;
			  n->second.format = j->second.format;
			  n->second.latexMacro = j->second.latexMacro;
			  n->second.index = canonical->opMapIndex.size();
			  canonical->opMapIndex.append(n);
			}
		    }
		}
	    }
	}
    }
    {
      //
      //	For each polymorph in the module, add its id to genericsToConsider.
      //	If it came from a parameter, also add its id to genericsToAvoid.
      //
      int nrPolymorphs = module->getNrPolymorphs();
      for (int i = 0; i < nrPolymorphs; i++)
	{
	  int id = module->getPolymorphName(i).code();
	  genericsToConsider.insert(id);
	  if (module->parameterDeclaredPolymorph(i))
	    genericsToAvoid.insert(id);
	}
    }
    {
      //
      // 	Finally we add the generics which affect an operator and which
      //	which we did not decide to avoid.
      //
      FOR_EACH_CONST(i, IdSet, genericsToConsider)
	{
	  int id = *i;
	  const OpMap::const_iterator e = opMap.upper_bound(id);
	  for (OpMap::const_iterator j = opMap.lower_bound(id); j != e; ++j)
	    {
	      if (j->second.types.empty())
		{
		  if (genericsToAvoid.find(id) != genericsToAvoid.end())
		    {
		      IssueAdvisory("Ignoring a generic op mapping because operator " <<
				    QUOTE(Token::name(id)) << " comes from a parameter.");
		    }
		  else
		    {
		      OpMap::iterator n = canonical->opMap.insert(*j);
		      n->second.index = canonical->opMapIndex.size();
		      canonical->opMapIndex.append(n);
		    }
		}
	    }
	}
    }
  }
  if (module->isStrategic())
    {
      IdSet genericsToConsider;	// generics that affect an strategy
      IdSet genericsToAvoid;	// subset that affect an strategy from a parameter
      {
	//
	//	Strategies mappings are filtered as the operators.
	//
	const Vector<RewriteStrategy*> &strategies = module->getStrategies();
	int nrStrategies = strategies.length();
	for (int i = 0; i < nrStrategies; i++)
	  {
	    RewriteStrategy* strat = strategies[i];
	    int id = strat->id();
	    const StratMap::const_iterator e = stratMap.upper_bound(id);
	    for (StratMap::const_iterator j = stratMap.lower_bound(id); j != e; ++j)
	      {
		const Vector<IdSet>& types = j->second.types;
		if (module->parameterDeclared(strat))
		  {
		    if (types.empty())
		      {
			genericsToConsider.insert(id);
			genericsToAvoid.insert(id);
		      }
		    else
		      {
			IssueAdvisory("Ignoring a specific strat mapping because strategy " <<
				      QUOTE(0) << " comes from a parameter.");
		      }
		  }
		else
		  {
		    if (types.empty())
		      genericsToConsider.insert(id);
		    else
		      {
			//
			//	arity specific case
			//
			if (typeMatch(types, strat))
			  {
			    StratMap::iterator n =
			      canonical->stratMap.insert(StratMap::value_type(id, StratMapping()));
			    int nrTypes = types.size();
			    n->second.types.resize(nrTypes);
			    --nrTypes;
			    for (int i = 0; i < nrTypes; i++)
			      setType(n->second.types[i], strat->getDomain()[i]->component());
			    setType(n->second.types[nrTypes], strat->getSubjectSort()->component());
			    n->second.expr = 0;
			    n->second.index = canonical->stratMapIndex.size();
			    canonical->stratMapIndex.append(n);
			  }
		      }
		  }
	      }
	  }


      }
      {
	//
	// 	Finally we add the generics which affect an strategy and which
	//	which we did not decide to avoid.
	//
	FOR_EACH_CONST(i, IdSet, genericsToConsider)
	  {
	    int id = *i;
	    const StratMap::const_iterator e = stratMap.upper_bound(id);
	    for (StratMap::const_iterator j = stratMap.lower_bound(id); j != e; ++j)
	      {
		if (j->second.types.empty())
		  {
		    if (genericsToAvoid.find(id) != genericsToAvoid.end())
		      {
			IssueAdvisory("Ignoring a generic strat mapping because strategy " <<
				      QUOTE(Token::name(id)) << " comes from a parameter.");
		      }
		    else
		      {
			StratMap::iterator n = canonical->stratMap.insert(*j);
			n->second.index = canonical->stratMapIndex.size();
			canonical->stratMapIndex.append(n);
		      }
		  }
	      }
	  }
      }
    }
  if (canonical->sortMap.empty() &&
      canonical->opMap.empty() &&
      canonical->labelMap.empty() &&
      canonical->stratMap.empty())
    {
      delete canonical;
      DebugAdvisory("renaming " << this << " on " << module << " resulted in empty canonical renaming");
      return 0;
    }
  return canonical;
}

int
Renaming::renameSort(int oldId) const
{
  IdMap::const_iterator i = sortMap.find(oldId);
  return (i == sortMap.end()) ? oldId : i->second;
}

int
Renaming::renameLabel(int oldId) const
{
  IdMap::const_iterator i = labelMap.find(oldId);
  return (i == labelMap.end()) ? oldId : i->second;
}

bool
Renaming::typeMatch(const set<int>& type, const ConnectedComponent* component)
{
  //
  //	Check that a type matches a connected component; i.e. at least
  //	on sort in the connect component appears in the type.
  //
  set<int>::const_iterator e = type.end();
  int nrSorts = component->nrSorts();
  for (int i = 1; i < nrSorts; i++)
    {
      if (type.find(component->sort(i)->id()) != e)
	return true;
    }
  return false;
}

bool
Renaming::typeMatch(const Vector<set<int> >& types, Symbol* oldSymbol)
{
  //
  //	Check that each connected component in the symbol arity/co-arity
  //	matches the corresponding type in the Vector of types.
  //
  int nrArgs = types.size() - 1;
  if (oldSymbol->arity() != nrArgs)
    return false;
  for (int i = 0; i < nrArgs; i++)
    {
      if (!typeMatch(types[i], oldSymbol->domainComponent(i)))
	return false;
    }
  return typeMatch(types[nrArgs], oldSymbol->rangeComponent());
}

bool
Renaming::typeMatch(const Vector<set<int> >& types, RewriteStrategy* oldStrat)
{
  int nrArgs = types.size();
  if (oldStrat->arity() != nrArgs)
    return false;

  const Vector<Sort*> &domain = oldStrat->getDomain();
  for (int i = 0; i < nrArgs; i++)
  {
    if (!typeMatch(types[i], domain[i]->component()))
      return false;
    }
  return true;
}

int
Renaming::renameOp(Symbol* oldSymbol) const
{
  //
  //	Rename a concrete symbol.
  //
  //cerr << "renaming << " << oldSymbol << endl;
  int oldId = oldSymbol->id();
  int index = NONE;
  const OpMap::const_iterator e = opMap.end();
  for (OpMap::const_iterator i = opMap.find(oldId); i != e && i->first == oldId; ++i)
    {
      //cerr << "considering " << Token::name( i->first) << " to " <<  Token::name(i->second.name) << endl;
      const Vector<set<int> >& types = i->second.types;
      if (types.empty() || typeMatch(types, oldSymbol))
	{
	  //cerr << "success" << endl;
	  if (index == NONE)
	    index = i->second.index;
	  else
	    {
	      IssueWarning("multiple renamings apply to " << QUOTE(oldSymbol));
	      break;
	    }
	}
    }
  return index;
}

bool
Renaming::typeMatch(const Vector<set<int> >& types, const Vector<int>& sortNames)
{
  //
  //	Check that each sort name in sortNames appears in the corresponding type
  //	in types.
  //
  int nrTypes = types.size();
  for (int i = 0; i < nrTypes; ++i)
    {
      //cerr << "looking for a match at arg " << i << " for sort " << Token::name(sortNames[i]) << endl;
      const set<int>& type = types[i];
      if (type.find(sortNames[i]) == type.end())
	return false;
    }
  return true;
}

int
Renaming::renameOp(int id, const Vector<int>& sortNames) const
{
  //
  //	Rename an abstract operation arising from module expression evaluation.
  //	This is needed because a concrete symbol might be pushed through a sequence
  //	of renamings.
  //
  int index = NONE;
  const OpMap::const_iterator e = opMap.end();
  for (OpMap::const_iterator i = opMap.find(id); i != e && i->first == id; ++i)
    {
      //cerr << "considering " << Token::name( i->first) << " to " <<  Token::name(i->second.name) << endl;
      const Vector<set<int> >& types = i->second.types;
      if (types.empty() || typeMatch(types, sortNames))
	{
	  //cerr << "success" << endl;
	  if (index == NONE)
	    index = i->second.index;
	  else
	    {
	      IssueWarning("multiple renamings apply to " << QUOTE(Token::name(id)));
	      break;
	    }
	}
    }
  return index;
}

int
Renaming::renamePolymorph(int oldId) const
{
  const OpMap::const_iterator e = opMap.end();
  for (OpMap::const_iterator i = opMap.find(oldId); i != e && i->first == oldId; ++i)
    {
      if (i->second.types.empty())
	return i->second.index;  // check for multiple renamings?
    }
  return NONE;
}

int
Renaming::renameStrat(RewriteStrategy* oldStrat) const
{
  int oldId = oldStrat->id();
  int index = NONE;
  const StratMap::const_iterator e = stratMap.end();
  for (StratMap::const_iterator i = stratMap.find(oldId); i != e && i->first == oldId; ++i)
  {
    const Vector<set<int> >& types = i->second.types;
    if (types.empty() || typeMatch(types, oldStrat))
    {
      if (index == NONE)
        index = i->second.index;
      else
      {
        IssueWarning("multiple renamings apply to strategy " << QUOTE(oldStrat));
        break;
      }
    }
  }
  return index;
}

int
Renaming::renameStrat(int id, const Vector<int>& sortNames) const
{
  int index = NONE;
  const StratMap::const_iterator e = stratMap.end();
  for (StratMap::const_iterator i = stratMap.find(id); i != e && i->first == id; ++i)
    {
      const Vector<set<int> >& types = i->second.types;
      if (types.empty() || typeMatch(types, sortNames))
	{
	  if (index == NONE)
	    index = i->second.index;
	  else
	    {
	      IssueWarning("multiple renamings apply to strategy " << QUOTE(Token::name(id)));
	      break;
	    }
	}
    }
  return index;
}

void
Renaming::addSortAndLabelMappings(const Renaming* original)
{
  {
    FOR_EACH_CONST(i, IdMap, original->sortMap)
      {
	pair<IdMap::iterator, bool> p = sortMap.insert(*i);
	if (p.second)
	  sortMapIndex.append(p.first);
      }
  }
  {
    FOR_EACH_CONST(i, IdMap, original->labelMap)
      {
	pair<IdMap::iterator, bool> p = labelMap.insert(*i);
	if (p.second)
	  labelMapIndex.append(p.first);
      }
  }
}

void
Renaming::addOpMappingPartialCopy(const Renaming* original, int index)
{
  //
  //	Add a copy of a given op mapping, leaving out any type info.
  //
  OpMap::const_iterator from = original->opMapIndex[index];
  lastOpMapping = opMap.insert(OpMap::value_type(from->first, OpMapping()));
  lastOpMapping->second.name = from->second.name;
  lastOpMapping->second.term = 0;
  lastOpMapping->second.prec = from->second.prec;
  lastOpMapping->second.gather = from->second.gather;
  lastOpMapping->second.format = from->second.format;
  lastOpMapping->second.latexMacro = from->second.latexMacro;
  lastOpMapping->second.index = opMapIndex.size();
  opMapIndex.append(lastOpMapping);
  lastSeenWasStrategy = false;
}

/*
void
Renaming::addOpMappingSimpleCopy(const Renaming* original, int index)
{
  //
  //	Add a copy of a given op mapping, not dealing with op->term mappings.
  //
  OpMap::const_iterator from = original->opMapIndex[index];
  lastOpMapping = opMap.insert(OpMap::value_type(from->first, OpMapping()));
  lastOpMapping->second.types = from->second.types;  // deep copy of a Vector of sets
  lastOpMapping->second.name = from->second.name;
  lastOpMapping->second.term = 0;
  lastOpMapping->second.prec = from->second.prec;
  lastOpMapping->second.gather = from->second.gather;
  lastOpMapping->second.format = from->second.format;
  lastOpMapping->second.latexMacro = from->second.latexMacro;
  lastOpMapping->second.index = opMapIndex.size();
  opMapIndex.append(lastOpMapping);
}
*/

void
Renaming::addStratMappingPartialCopy(const Renaming* original, int index)
{
  //
  //	Add a copy of a given strat mapping, leaving out any type info.
  //
  StratMap::const_iterator from = original->stratMapIndex[index];
  lastStratMapping = stratMap.insert(StratMap::value_type(from->first, StratMapping()));
  lastStratMapping->second.name = from->second.name;
  lastStratMapping->second.expr = 0;
  lastStratMapping->second.index = stratMapIndex.size();
  lastSeenWasStrategy = true;
}

void
Renaming::addSortMapping(Token from, Token to)
{
  pair<IdMap::iterator, bool> p = sortMap.insert(IdMap::value_type(from.code(), to.code()));
  if (p.second)
    sortMapIndex.append(p.first);
  else
    IssueWarning(LineNumber(from.lineNumber()) <<
		 ": multiple mapping for sort " << QUOTE(from) <<
		 " in renaming.");
}

void
Renaming::addSortMapping(int from, int to)
{
  pair<IdMap::iterator, bool> p = sortMap.insert(IdMap::value_type(from, to));
  Assert(p.second, "multiple mapping for sort");
  sortMapIndex.append(p.first);
}

void
Renaming::addLabelMapping(Token from, Token to)
{
  pair<IdMap::iterator, bool> p = labelMap.insert(IdMap::value_type(from.code(), to.code()));
  if (p.second)
    labelMapIndex.append(p.first);
  else
    IssueWarning(LineNumber(from.lineNumber()) <<
		 ": multiple mapping for label " << QUOTE(from) <<
		 " in renaming.");
}

void
Renaming::addLabelMapping(int from, int to)
{
  pair<IdMap::iterator, bool> p = labelMap.insert(IdMap::value_type(from, to));
  Assert(p.second, "multiple mapping for label");
  labelMapIndex.append(p.first);
}

void
Renaming::addOpMapping(const Vector<Token>& tokens)
{
  int name = Token::bubbleToPrefixNameCode(tokens);
  lastOpMapping = opMap.insert(OpMap::value_type(name, OpMapping()));
  lastOpMapping->second.prec = MixfixModule::MIN_PREC - 1;  // initialize to invalid value
  lastOpMapping->second.index = opMapIndex.size();
  //
  //	We insert safe dummy values in case addOpTarget() never gets called (because of a syntax error).
  //
  lastOpMapping->second.name = name;  // map op to itself
  lastOpMapping->second.term = 0;
  opMapIndex.append(lastOpMapping);
  lastSeenWasStrategy = false;
}

void
Renaming::addOpMapping(int code)
{
  lastOpMapping = opMap.insert(OpMap::value_type(code, OpMapping()));
  lastOpMapping->second.prec = MixfixModule::MIN_PREC - 1;  // initialize to invalid value
  lastOpMapping->second.index = opMapIndex.size();
  opMapIndex.append(lastOpMapping);
  lastSeenWasStrategy = false;
}

void
Renaming::addStratMapping(Token from)
{
  addStratMapping(from.code());
}

void
Renaming::addStratMapping(int code)
{
  lastStratMapping = stratMap.insert(StratMap::value_type(code, StratMapping()));
  lastStratMapping->second.index = stratMapIndex.size();
  stratMapIndex.append(lastStratMapping);
  lastSeenWasStrategy = true;
}

void
Renaming::addStratMappingVarIndices(const Vector<int> &indexMap)
{
  lastStratMapping->second.varsMap = indexMap; // copy
}

void
Renaming::addParameter2(Token /* name */, ModuleExpression* /* theory */)
{
  CantHappen("renamings don't take parameters");
}

void
Renaming::addVarDecl(Token /* varName */)
{
  CantHappen("renamings don't take var decls");
}

void
Renaming::addType(bool /* kind */, const Vector<Token>& tokens)
{
  Vector<set<int> >& types = lastSeenWasStrategy ?
		lastStratMapping->second.types : lastOpMapping->second.types;
  int nrTypes = types.length();
  types.resize(nrTypes + 1);
  set<int>& type = types[nrTypes];
  FOR_EACH_CONST(i, Vector<Token>, tokens)
    type.insert(i->code());
}

void
Renaming::addType(const ConnectedComponent* component)
{
  Vector<set<int> >& types = lastSeenWasStrategy ?
		lastStratMapping->second.types : lastOpMapping->second.types;
  int nrTypes = types.length();
  types.resize(nrTypes + 1);
  setType(types[nrTypes], component);
}

void
Renaming::addOpTarget(const Vector<Token>& tokens)
{
  lastOpMapping->second.name = Token::bubbleToPrefixNameCode(tokens);
  lastOpMapping->second.fromTerm = 0;
  lastOpMapping->second.term = 0;
}

void
Renaming::addOpTarget(int code)
{
  lastOpMapping->second.name = code;
  lastOpMapping->second.fromTerm = 0;
  lastOpMapping->second.term = 0;
}

void
Renaming::addOpTargetTerm(Term* fromTerm, Term* targetTerm)
{
  
  lastOpMapping->second.name = NONE;
  lastOpMapping->second.fromTerm = fromTerm;
  lastOpMapping->second.term = targetTerm;
}

void
Renaming::addStratTarget(Token to)
{
  addStratTarget(to.code());

  // Removes the last type (the subject type) because it is not relevant
  // for strategy resolution
  // TODO The subject type should also be checked
  Vector<Renaming::IdSet> &types = lastStratMapping->second.types;
  if (!types.empty())
    types.contractTo(types.length() - 1);
}

void
Renaming::addStratTarget(int code)
{
  lastStratMapping->second.name = code;
  lastStratMapping->second.expr = (StrategyExpression*) 0;
}

void
Renaming::addStratTargetExpr(CallStrategy* fromCall, StrategyExpression* expr)
{
  lastStratMapping->second.name = NONE;
  lastStratMapping->second.fromCall = fromCall;
  lastStratMapping->second.expr = expr;
}

void
Renaming::setPrec(Token precTok)
{
  int prec;
  if (precTok.getInt(prec) &&
      prec >= MixfixModule::MIN_PREC && prec <= MixfixModule::MAX_PREC)
    lastOpMapping->second.prec = prec;
  else
    {
      IssueWarning(LineNumber(precTok.lineNumber()) <<
		   ": bad value " << QUOTE(precTok) << " for prec attribute.");
    }
}

void
Renaming::setGather(const Vector<Token>& gather)
{
  int length = gather.length();
  OpMapping& opMapping = lastOpMapping->second;
  if (opMapping.types.length() != 0 && length != opMapping.types.length() - 1)
    {
      IssueWarning(LineNumber(gather[0].lineNumber()) <<
		   ": bad gather length.");
      return;
    }
  for (int i = 0; i < length; i++)
    {
      const char* str = gather[i].name();
      if (str[0] != '\0' && str[1] == '\0')
	{
	  switch(str[0])
	    {
	    case '&':
	      opMapping.gather.append(MixfixModule::GATHER_AMP);
	      continue;
	    case 'E':
	      opMapping.gather.append(MixfixModule::GATHER_E);
	      continue;
	    case 'e':
	      opMapping.gather.append(MixfixModule::GATHER_e);
	      continue;
	    }
	}
      IssueWarning(LineNumber(gather[i].lineNumber()) <<
		   ": bad value " << QUOTE(str) <<
		   " in gather attribute. Recovering by ignoring gather attribute.");
      opMapping.gather.clear();  // for safety
      return;
    }
}

void
Renaming::setFormat(const Vector<Token>& format)
{
  int length = format.length();
  OpMapping& opMapping = lastOpMapping->second;
  for (int i = 0; i < length; i++)
    {
      if (SyntacticPreModule::checkFormatString(format[i].name()))
	opMapping.format.append(format[i].code());
      else
	{
	  IssueWarning(LineNumber(format[i].lineNumber()) <<
		       ": bad value " << QUOTE(format[i]) <<
		       " in format attribute. Recovering by ignoring format attribute.");
	  opMapping.format.clear();  // for safety
	  return;
	}
    }
}

void
Renaming::setLatexMacro(const string& latexMacro)
{
  lastOpMapping->second.latexMacro = latexMacro;
}

void
Renaming::printRenamingType(ostream& s, const set<int>& sorts) const
{
  char sep = '[';
  FOR_EACH_CONST(i, set<int>, sorts)
    {
      s << sep << Token::sortName(*i);
      sep = ',';
    }
  s << ']';
}

void
Renaming::printRenaming(ostream& s, const char* sep, const char* sep2) const
{
  {
    int nrSortMappings = getNrSortMappings();
    for (int i = 0; i < nrSortMappings; i++)
      {
	s << sep << "sort " << Token::sortName(getSortFrom(i)) <<
	  " to " << Token::sortName(getSortTo(i));
	sep = sep2;
      }
  }
  {
    int nrOpMappings = getNrOpMappings();
    for (int i = 0; i < nrOpMappings; i++)
      {
	s << sep << "op " << Token::name(getOpFrom(i));
	int nrTypes = getNrTypes(i);
	if (nrTypes > 0)
	  {
	    s << " :";
	    --nrTypes;
	    for (int j = 0; j < nrTypes; j++)
	      {
		s << ' ';
		printRenamingType(s, getTypeSorts(i, j));
	      }
	    s << " -> ";
	    printRenamingType(s, getTypeSorts(i, nrTypes));
	  }
#ifndef NO_ASSERT
	//
	//	op->term mappings only occur in generated
	//	canonicalRenamings which are only printed out
	//	during debugging. We print them using illegal
	//	syntax because there isn't enough information
	//	retained to use legal syntax.
	//
	if (Term* t = getOpTargetTerm(i))
	  {
	    s << " to term " << t << " .";
	    continue;
	  }
#endif
	s << " to " << Token::name(getOpTo(i));
	int prec = getPrec(i);
	const Vector<int>& gather = getGather(i);
	const Vector<int>& format = getFormat(i);
	if (prec >= MixfixModule::MIN_PREC || !(gather.empty()) || !(format.empty()))
	  {
	    sep = " [";
	    if (prec >= MixfixModule::MIN_PREC)
	      {
		s << sep << "prec " << prec;
		sep = " ";
	      }
	    if (!(gather.empty()))
	      {
		s << sep;
		sep = " ";
		SyntacticPreModule::printGather(s, gather);
	      }
	    if (!(format.empty()))
	      {
		s << sep;
		sep = " ";
		SyntacticPreModule::printFormat(s, format);
	      }
	    s << ']';
	  }
	sep = sep2;
      }
  }
  {
    int nrLabelMappings = getNrLabelMappings();
    for (int i = 0; i < nrLabelMappings; i++)
      {
	s << sep << "label " << Token::name(getLabelFrom(i)) <<
	  " to " << Token::name(getLabelTo(i));
	sep = sep2;
      }
  }
  {
    int nrStratMappings = getNrStratMappings();
    for (int i = 0; i < nrStratMappings; i++)
      {
        s << sep << "strat " << Token::name(getStratFrom(i));
        int nrTypes = getNrStratTypes(i);
        if (nrTypes > 0)
          {
            s << " :";
            for (int j = 0; j < nrTypes; j++)
              {
                s << ' ';
                printRenamingType(s, stratMapIndex[i]->second.types[j]);
              }
          }
         Assert(getStratTo(i) != NONE && getStratTargetExpr(i) == 0,
           "renamings with strat->expr mappings are not printable");
        s << " to " << Token::name(getStratTo(i));
        sep = sep2;
      }
  }
}

ostream&
operator<<(ostream& s, const Renaming* renaming)
{
  s << '(';
  renaming->printRenaming(s, "", ", ");
  return s << ')';
}
