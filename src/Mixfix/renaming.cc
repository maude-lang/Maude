/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2025 SRI International, Menlo Park, CA 94025, USA.

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

//	our stuff
#include "ooRenaming.cc"
#include "latexRenaming.cc"

Renaming::~Renaming() 
{
  for (const auto& i : opMap)
    {
      if (i.second.gcToTerm)
	i.second.toTerm->deepSelfDestruct();
    }
}

void
Renaming::setType(set<int>& type, const ConnectedComponent* component)
{
  //
  //	We stuff all the sorts from the component into the type, not just the
  //	maximal ones. The rationale is as we apply to renaming to submodules,
  //	the maximal sorts might not exist and if we didn't have all the sorts
  //	the renaming of an operator might fail.
  //
  int nrSorts = component->nrSorts();
  for (int i = 1; i < nrSorts; i++)
    type.insert(component->sort(i)->id());
}

Rope
Renaming::makeTypeName(const set<int>& type)
{
  Rope name;
  for (int i : type)
    {
      name += name.empty() ? "[" : ",";
      name += Token::sortName(i);
    }
  return name + "]";
}

Rope
Renaming::makeCanonicalName() const
{
  Rope name;
  for (const auto& i : sortMap)
    {
      if (!name.empty())
	name += ", ";
      name += "sort ";
      name += Token::sortName(i.first);
      name += " to ";
      name += Token::sortName(i.second);
    }
  for (const auto& i : labelMap)
    {
      if (!name.empty())
	name += ", ";
      name += "label ";
      name += Token::name(i.first);
      name += " to ";
      name += Token::name(i.second);
    }
  int nrOpMappings = opMap.size();
  if (nrOpMappings > 0)
    {
      Vector<Rope> opRopes(nrOpMappings);
      {
	Vector<Rope>::iterator j = opRopes.begin();
	for (const auto& i : opMap)
	  {
	    *j = "op ";
	    *j += Token::name(i.first);
	    if (!(i.second.types.empty()))
	      {
		*j += " :";
		int nrArgs = i.second.types.size() - 1;
		for (int k = 0; k < nrArgs; k++)
		  {
		    *j += " ";
		    *j += makeTypeName(i.second.types[k]);
		  }
		*j += " -> ";
		*j += makeTypeName(i.second.types[nrArgs]);
	      }
	    *j += " to ";
	    *j += Token::name(i.second.name);
	    bool attr = false;
	    if (i.second.prec >= MixfixModule::MIN_PREC)
	      {
		*j += attr ? " " : " [";
		*j += "prec ";
		*j += int64ToString(i.second.prec);
		attr = true;
	      }
	    if (!(i.second.gather.empty()))
	      {
		*j += attr ? " " : " [";
		*j += "gather ";
		int nrArgs = i.second.gather.size();
		for (int k = 0; k < nrArgs; k++)
		  {
		    *j += (k == 0) ? "(" : " ";
		    switch (i.second.gather[k])
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
	    if (!(i.second.format.empty()))
	      {
		*j += attr ? " " : " [";
		*j += "format ";
		int nrWords = i.second.format.size();
		for (int k = 0; k < nrWords; k++)
		  {
		    *j += (k == 0) ? "(" : " ";
		    *j += Token::name(i.second.format[k]);
		  }
		*j += ")";
		attr = true;
	      }
	    if (i.second.latexMacro != NONE)
	      {
		*j += attr ? " " : " [";
		*j += "latex (";
		*j += Token::name(i.second.latexMacro);
		*j += ")";
		attr = true;
	      }
	    if (attr)
	      *j += "]";
	    ++j;
	  }
      }
      sort(opRopes.begin(), opRopes.end());
      {
	for (const auto& i : opRopes)
	  {
	    if (!name.empty())
	      name += ", ";
	    name += i;
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

void
Renaming::pruneSortMappings(ImportModule* module, Renaming* canonical) const
{
  //
  //	Prune sort mappings to those that actually occur in the module.
  //	We do not allow sorts that come from a parameter to be mapped.
  //
  for (auto& m : sortMap)
    {
      if (m.first == m.second)
	continue;  // skip identity mapping

      Sort* s = module->findSort(m.first);
      DebugAdvisory("looking for sort " << s << " in module to be renamed " << module);
      if (s != 0)
	{
	  if (module->parameterDeclared(s))
	    {
	      IssueAdvisory("Ignoring a sort mapping because sort " << QUOTE(s) << " comes from a parameter.");
	    }
	  else
	    {
	      DebugAdvisory("added sort mapping " << s << " to " << Token::name(m.second));
	      pair<IdMap::iterator, bool> p = canonical->sortMap.insert(m);
	      AdvisoryCheck(p.second, "sort mapping for " << QUOTE(s) << " occluded by class mapping.");
	      canonical->sortMapIndex.append(p.first);
	    }
	}
    }
}

void
Renaming::pruneLabelMappings(ImportModule* module, Renaming* canonical) const
{
  //
  //	Prune label mappings to those that actually occur in the module.
  //
  const set<int>& labels = module->getLabels();
  for (auto& m : labelMap)
    {
      if (m.first == m.second)
	continue;  // skip identity mapping

      if (labels.find(m.first) != labels.end())
	{
	  pair<IdMap::iterator, bool> p = canonical->labelMap.insert(m);
	  Assert(p.second, "dup label mapping");
	  canonical->labelMapIndex.append(p.first);
	}
    }
}

bool
Renaming::equal(const Vector<int>& left, const Vector<int>& right)
{
  if (left.size() != right.size())
    return false;
  Index nrElts = left.size();
  for (Index i = 0; i < nrElts; ++i)
    {
      if (left[i] != right[i])
	return false;
    }
  return true;
}

bool
Renaming::isIdentityOpMapping(const ImportModule* module, const OpMapping& om, const Symbol* symbol)
{
  //
  //	Is om an identity mapping on symbol?
  //
  if (om.name == symbol->id() &&
      (om.prec < MixfixModule::MIN_PREC || om.prec == module->getPrec(symbol)) &&
      (om.format.empty() || equal(om.format, module->getFormat(symbol))) &&
      om.latexMacro == module->getLatexMacro(symbol))
    {
      if (om.gather.empty())
	return true;
      //
      //	Gather is stored in an encoded form in module.
      //
      Vector<int> gather;
      module->getGather(symbol, gather);
      if (equal(om.gather, gather))
	return true;
    }
  return false;
}

bool
Renaming::isIdentityOpMapping(const ImportModule* module, const OpMapping& om, int index)
{
  //
  //	Is om an identity mapping on polymorph indexed by index?
  //
  if (om.name == module->getPolymorphName(index).code() &&
      (om.prec < MixfixModule::MIN_PREC || om.prec == module->getPolymorphPrec(index)) &&
      (om.format.empty() || equal(om.format, module->getPolymorphFormat(index))) &&
      om.latexMacro == module->getPolymorphLatexMacro(index))
    {
      if (om.gather.empty())
	return true;
      //
      //	Gather is stored in an encoded form in module.
      //
      Vector<int> gather;
      module->getPolymorphGather(index, gather);
      if (equal(om.gather, gather))
	return true;
    }
  return false;
}

void
Renaming::canonicalizeOpMappings(ImportModule* module, Renaming* canonical) const
{
  IdSet genericsToConsider;	// generics that affect an operator
  IdSet genericsToAvoid;	// subset that affect an operator from a parameter
  {
    //
    //	For each operator in the module that is not from a parameter
    //	and would be remapped by an original mapping, we add a mapping
    //	to the canonical renaming unless the mapping is a generic one
    //	in which case we add the id to genericsToConsider. For operators
    //	from parameter theories, if the mapping is specific we emit an
    //	advisory otherwise we add it to both genericsToConsider and
    //	genericsToAvoid so as to be able to generate an advisory.
    //
    const Vector<Symbol*> symbols = module->getSymbols();
    int nrUserSymbols = module->getNrUserSymbols();
    for (Index i = 0; i < nrUserSymbols; ++i)
      {
	Symbol* symbol = symbols[i];
	int id = symbol->id();
	const OpMap::const_iterator e = opMap.upper_bound(id);
	for (OpMap::const_iterator j = opMap.lower_bound(id); j != e; ++j)
	  {
	    if (isIdentityOpMapping(module, j->second, symbol))
	      continue;

	    const Vector<IdSet>& types = j->second.types;
	    if (types.empty())
	      {
		genericsToConsider.insert(id);
		if (module->parameterDeclared(symbol))
		  genericsToAvoid.insert(id);
		else
		  {		    
		    if (j->second.mappingType == MappingType::MSG)
		      {
			SymbolType st = module->getSymbolType(symbol);
			AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), "generic msg mapping renames operator " <<
					   QUOTE(symbol) << " which does not have the msg attribute.");
		      }
		  }
	      }
	    else
	      {
		if (typeMatch(types, symbol))
		  {
		    if (module->parameterDeclared(symbol))
		      {
			IssueAdvisory("Ignoring a specific op mapping because operator " <<
				      QUOTE(symbol) << " comes from a parameter.");
		      }
		    else
		      {
			if (j->second.mappingType == MappingType::MSG)
			  {
			    SymbolType st = module->getSymbolType(symbol);
			    AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), "specific msg mapping renames operator " <<
					  QUOTE(symbol) << " which does not have the msg attribute.");
			  }
			OpMap::iterator n = canonical->opMap.insert(OpMap::value_type(id, OpMapping()));
			int nrTypes = types.size();
			n->second.types.resize(nrTypes);
			--nrTypes;
			for (int i = 0; i < nrTypes; i++)
			  setType(n->second.types[i], symbol->domainComponent(i));
			setType(n->second.types[nrTypes], symbol->rangeComponent());
			n->second.name = j->second.name;
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
    //	For each polymorph in the module, if there is a generic mapping that is not the
    //	identity mapping, add its id to genericsToConsider.
    //	If it came from a parameter, also add its id to genericsToAvoid.
    //
    int nrPolymorphs = module->getNrPolymorphs();
    for (int i = 0; i < nrPolymorphs; i++)
      {
	int id = module->getPolymorphName(i).code();
	const OpMap::const_iterator e = opMap.upper_bound(id);
	for (OpMap::const_iterator j = opMap.lower_bound(id); j != e; ++j)
	  {
	    if (j->second.types.empty())
	      {
		if (!isIdentityOpMapping(module, j->second, i))
		  {
		    genericsToConsider.insert(id);
		    if (module->parameterDeclaredPolymorph(i))
		      genericsToAvoid.insert(id);
		    else
		      {
			if (j->second.mappingType == MappingType::MSG)
			  {
			    SymbolType st = module->getPolymorphType(i);
			    AdvisoryCheck(st.hasFlag(SymbolType::MESSAGE), "generic msg mapping renames polymorphic operator " <<
					  QUOTE(Token::name(id)) << " which does not have the msg attribute.");
			  }
		      }
		  }
		break;
	      }
	  }
      }
  }
  //
  // 	Finally we add the generics which affect an operator and which
  //	which we did not decide to avoid.
  //
  for (int id : genericsToConsider)
    {
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

void
Renaming::canonicalizeStrategyMappings(ImportModule* module, Renaming* canonical) const
{
  IdSet genericsToConsider;	// generics that affect an strategy
  IdSet genericsToAvoid;	// subset that affect an strategy from a parameter
  //
  //	Strategies mappings are filtered as the operators.
  //
  for (RewriteStrategy* strat : module->getStrategies())
    {
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
				QUOTE(Token::name(id)) << " comes from a parameter.");
		}
	    }
	  else
	    {
	      if (types.empty())
		genericsToConsider.insert(id);
	      else
		{
		  //
		  //	Arity specific case.
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
  //
  // 	Finally we add the generics which affect an strategy and which
  //	which we did not decide to avoid.
  //
  for (int id : genericsToConsider)
    {
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

Renaming*
Renaming::makeCanonicalVersion(ImportModule* module) const
{
  Renaming* canonical = new Renaming;

  convertClassMappings(module, canonical);
  convertAttrMappings(module, canonical);
  pruneSortMappings(module, canonical);
  pruneLabelMappings(module, canonical);
  canonicalizeOpMappings(module, canonical);

  if (module->isStrategic())
    canonicalizeStrategyMappings(module, canonical);

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
  int oldId = oldSymbol->id();
  int index = NONE;
  const OpMap::const_iterator e = opMap.end();
  for (OpMap::const_iterator i = opMap.find(oldId); i != e && i->first == oldId; ++i)
    {
      const Vector<set<int> >& types = i->second.types;
      if (types.empty() || typeMatch(types, oldSymbol))
	{
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
  int nrSortNames = sortNames.size();
  if (nrTypes != nrSortNames)
    return false;
  for (int i = 0; i < nrTypes; ++i)
    {
      DebugInfo("looking for a match at arg " << i << " for sort " << Token::name(sortNames[i]));
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
      //
      //	Abstract version only runs on canonicalized renamings.
      //
      const Vector<set<int> >& types = i->second.types;
      if (types.empty() || typeMatch(types, sortNames))
	{
	  if (index == NONE)
	    index = i->second.index;
	  else
	    {
	      IssueWarning("multiple renamings apply to " << QUOTE(Token::name(id)) << " : " <<
			   QUOTE(Token::name(getOpTo(index))) << " vs " <<
			   QUOTE(Token::name(i->second.name)) << ".");
	      DebugInfo("renaming is\n\n" << this << "\n");
	      break;
	    }
	}
    }
  return index;
}

int
Renaming::renamePolymorph(int oldId) const
{
  //
  //	For polymorphs, we only consider generic op mappings.
  //
  int index = NONE;
  const OpMap::const_iterator e = opMap.end();
  for (OpMap::const_iterator i = opMap.find(oldId); i != e && i->first == oldId; ++i)
    {
      if (i->second.types.empty())
	{
	  if (index == NONE)
	    index = i->second.index;
	  else
	    {
	      IssueWarning("multiple renamings apply to polymorphic operator " << QUOTE(Token::name(oldId)));
	      break;
	    }
	}
    }
  return index;
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
Renaming::addSortConstantAndLabelMappings(const Renaming* original)
{
  //
  //	This is used to copy the theory -> parameter copy mappings from an imported theory.
  //
  for (const auto& m : original->sortMap)
    {
      pair<IdMap::iterator, bool> p = sortMap.insert(m);
      if (p.second)
	sortMapIndex.append(p.first);
    }
  for (const auto& m : original->opMap)
    {
      //
      //	We should only have constant specific mappings generated by pconst.
      //
      Assert(m.second.types.size() == 1, "bad types.size() = " << m.second.types.size());
      Assert(m.second.mappingType == MappingType::OP, "bad mappingTyp");
      Assert(m.second.fromTerm == 0, "shouldn't have fromTerm");
      Assert(m.second.toTerm == 0, "shouldn't have term");
      Assert(m.second.prec == MixfixModule::MIN_PREC - 1, "shouldn't change prec");
      Assert(m.second.gather.empty(), "shouldn't change gather");
      Assert(m.second.format.empty(), "shouldn't change format");
      Assert(m.second.latexMacro == NONE, "shouldn't change latexMacro");
      if (opMap.find(m.first) == opMap.end())
	{
	  //
	  //	Because this is only used for a parameter X, we don't want to add
	  //	a second mapping c |-> X$c if we already have a mapping for c.
	  //
 	  auto i = opMap.insert(m);
	  i->second.index = opMapIndex.size();
	  opMapIndex.append(i);
	}
    }
  for (const auto& m : original->labelMap)
    {
      pair<IdMap::iterator, bool> p = labelMap.insert(m);
      if (p.second)
	labelMapIndex.append(p.first);
    }
}

void
Renaming::addOpMappingPartialCopy(const Renaming* original, int index, int newFromName, int newToName)
{
  //
  //	Add a copy of a given op mapping, leaving out any type info.
  //
  OpMap::const_iterator from = original->opMapIndex[index];
  lastOpMapping = opMap.insert(OpMap::value_type(newFromName, OpMapping()));
  lastOpMapping->second.name = newToName;
  lastOpMapping->second.prec = from->second.prec;
  lastOpMapping->second.gather = from->second.gather;
  lastOpMapping->second.format = from->second.format;
  lastOpMapping->second.latexMacro = from->second.latexMacro;
  lastOpMapping->second.index = opMapIndex.size();
  opMapIndex.append(lastOpMapping);
  lastSeenWasStrategy = false;
}

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
  Assert(p.second, "multiple mapping for sort: wanted to add " <<
	 Token::name(from) <<" to " << Token::name(to) << " but " <<
	 Token::name(p.first->first) << " to " << Token::name(p.first->second) <<
	 " already exists in\n" << this);
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
  lastOpMapping->second.index = opMapIndex.size();
  //
  //	We insert safe dummy values in case addOpTarget() never gets called (because of a syntax error).
  //
  lastOpMapping->second.name = name;  // map op to itself
  opMapIndex.append(lastOpMapping);
  lastSeenWasStrategy = false;
}

void
Renaming::addOpMapping(int code)
{
  lastOpMapping = opMap.insert(OpMap::value_type(code, OpMapping()));
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
  for (const Token& t : tokens)
    type.insert(t.code());
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
}

void
Renaming::addOpTarget(int code)
{
  lastOpMapping->second.name = code;
}

void
Renaming::addOpTargetTerm(Term* fromTerm, Term* targetTerm, bool gcTargetTerm)
{
  
  lastOpMapping->second.name = NONE;
  lastOpMapping->second.gcToTerm = gcTargetTerm;
  lastOpMapping->second.fromTerm = fromTerm;
  lastOpMapping->second.toTerm = targetTerm;
}

void
Renaming::addStratTarget(Token to)
{
  addStratTarget(to.code());

  // Removes the last type (the subject type) because it is not relevant
  // for strategy resolution
  // TODO The subject type should also be checked
  Vector<Renaming::IdSet>& types = lastStratMapping->second.types;
  if (!types.empty())
    types.pop_back();
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
  OpMapping& opMapping = lastOpMapping->second;
  int length = gather.size();
  int nrTypes = opMapping.types.size();
  if (nrTypes != 0 && length != nrTypes - 1)
    {
      IssueWarning(LineNumber(gather[0].lineNumber()) << ": bad gather length.");
      return;
    }
  for (const Token& t : gather)
    {
      const char* str = t.name();
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
      IssueWarning(LineNumber(t.lineNumber()) << ": bad value " << QUOTE(str) <<
		   " in gather attribute. Recovering by ignoring gather attribute.");
      opMapping.gather.clear();  // for safety
      return;
    }
}

void
Renaming::setFormat(const Vector<Token>& format)
{
  OpMapping& opMapping = lastOpMapping->second;
  for (const Token& t : format)
    {
      if (SyntacticPreModule::checkFormatString(t.name()))
	opMapping.format.append(t.code());
      else
	{
	  IssueWarning(LineNumber(t.lineNumber()) << ": bad value " << QUOTE(t) <<
		       " in format attribute. Recovering by ignoring format attribute.");
	  opMapping.format.clear();  // for safety
	  return;
	}
    }
}

void
Renaming::setLatexMacro(const string& latexMacro)
{
  if (latexMacro.empty())
    lastOpMapping->second.latexMacro = NONE;
  else
    lastOpMapping->second.latexMacro = Token::encode(latexMacro.c_str());
}

void
Renaming::purgeGeneratedMappings()
{
  //
  //	Discard generated sort mappings.
  //
  Index nrSortMappings = sortMapIndex.size();
  for (Index i = nrUserSortMappings; i < nrSortMappings; ++i)
    sortMap.erase(sortMapIndex[i]);
  sortMapIndex.resize(nrUserSortMappings);
  //
  //	Discard generated op mappings.
  //
  Index nrOpMappings = opMapIndex.size();
  for (Index i = nrUserOpMappings; i < nrOpMappings; ++i)
    opMap.erase(opMapIndex[i]);
  opMapIndex.resize(nrUserOpMappings);	  
}

void
Renaming::printRenamingType(ostream& s, const set<int>& sorts)
{
  char sep = '[';
  for (int i : sorts)
   {
      s << sep << Token::sortName(i);
      sep = ',';
    }
  s << ']';
}

void
Renaming::printRenaming(ostream& s, const char* sep, const char* sep2, bool showProcessed) const
{
  DebugEnter("this = " << (void*) this);
  //
  //	Problem situations that pretty op printing needs to handle.
  //
  const int fromOpProblems = Token::EXPOSED_COLON | Token::EXPOSED_TO; // colons and "to" in from operators names are always a problem
  int toOpProblems = (typeid(*this) == typeid(Renaming)) ?
    (Token::EXPOSED_COMMA | Token::EXPOSED_LEFT_BRACKET) :  // commas and left brackets in target operator names are a problem for renamings
    (Token::EXPOSED_TERM | Token::EXPOSED_DOT);  // "term" and dots in target operator names are a problem for views
  //
  //	If we are printing the unprocessing renaming, include class mappings.
  //
  if (!showProcessed)
    {
      for (const auto& c : classMap)
	{
	  s << sep << "class " << Token::sortName(c.fromClass.code()) << " to " <<
	    Token::sortName(c.toClass.code());
	  sep = sep2;
	}
      for (const auto& a : attrMap)
	{
	  s << sep << "attr " << a.fromAttr;
	  if (!(a.type.empty()))
	    {
	      s << " : ";
	      printRenamingType(s, a.type);
	    }
	  s << " to " << a.toAttr;
	  sep = sep2;
	}	      
    }
  //
  //	If we are printing the processed renaming, include generated sort mappings.
  //
  Index nrSortMappings = (nrUserSortMappings == NONE || showProcessed) ? sortMapIndex.size() : nrUserSortMappings;
  for (Index i = 0; i < nrSortMappings; ++i)
    {
      auto sm = sortMapIndex[i];
      s << sep << "sort " << Token::sortName(sm->first) << " to " <<
	Token::sortName(sm->second);
      sep = sep2;
    }
  //
  //	If we are printing the processed renaming, include generated op mappings.
  //
  Index nrOpMappings = (nrUserOpMappings == NONE || showProcessed) ? opMapIndex.size() : nrUserOpMappings;
  for (Index i = 0; i < nrOpMappings; ++i)
    {
      auto om = opMapIndex[i];
      s << sep << ((!showProcessed && om->second.mappingType == MappingType::MSG) ? "msg " : "op ") <<
	MixfixModule::prettyOpName(om->first, fromOpProblems);
      if (!om->second.types.empty())
	{
	  s << " :";
	  Index nrDomainTypes = om->second.types.size() - 1;
	  for (Index i = 0; i < nrDomainTypes; ++i)
	    {
	      s << ' ';
	      printRenamingType(s, om->second.types[i]);
	    }
	  s << " -> ";
	  printRenamingType(s, om->second.types[nrDomainTypes]);
	}
#ifndef NO_ASSERT
      //
      //	op->term mappings only occur in generated canonicalRenamings which are only printed out
      //	during debugging. We print them using illegal syntax (they should be term->term) because
      //	there isn't enough information retained to use legal syntax.
      //
      if (Term* t = om->second.toTerm)
	{
	  s << " to term " << t << " .";
	  continue;
	}
#endif
      s << " to " << MixfixModule::prettyOpName(om->second.name, toOpProblems);
      //
      //	If we have any attributes in target, print them inside []s.
      //
      if (om->second.prec >= MixfixModule::MIN_PREC ||
	  !om->second.gather.empty() ||
	  !om->second.format.empty() ||
	  om->second.latexMacro != NONE)
	{
	  sep = " [";
	  if (om->second.prec >= MixfixModule::MIN_PREC)
	    {
	      s << sep << "prec " << om->second.prec;
	      sep = " ";
	    }
	  if (!om->second.gather.empty())
	    {
	      s << sep;
	      SyntacticPreModule::printGather(s, om->second.gather);
	      sep = " ";
	    }
	  if (!om->second.format.empty())
	    {
	      s << sep;
	      SyntacticPreModule::printFormat(s, om->second.format);
	      sep = " ";
	    }
	  if (om->second.latexMacro != NONE)
	    {
	      s << sep;
	      s << "latex (" << Token::name(om->second.latexMacro) << ")";
	      sep = " ";
	    }
	  s << ']';
	}
      sep = sep2;
    }
  for (auto lm : labelMapIndex)
    {
      s << sep << "label " << Token::name(lm->first) << " to " <<
	Token::name(lm->second);
      sep = sep2;
    }
  for (auto sm : stratMapIndex)
    {
      s << sep << "strat " << Token::name(sm->first);
      if (!sm->second.types.empty())
	{
	  s << " :";
	  for (const IdSet& t : sm->second.types)
	    {
	      s << ' ';
	      printRenamingType(s, t);
	    }
	}
      Assert(sm->second.name != NONE && sm->second.expr == 0, "renamings with strat->expr mappings are not printable");
      s << " to " << Token::name(sm->second.name);
      sep = sep2;
    }
}

ostream&
operator<<(ostream& s, const Renaming* renaming)
{
  s << '(';
  renaming->printRenaming(s, "", ", ");
  return s << ')';
}
