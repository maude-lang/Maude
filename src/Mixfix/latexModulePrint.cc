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
//	LaTeX pretty printer for modules and parts thereof.
//

void
VisibleModule::latexPrintConditionFragment(ostream& s, const ConditionFragment* c)
{
  if (const EqualityConditionFragment* e = dynamic_cast<const EqualityConditionFragment*>(c))
    {
      latexPrettyPrint(s, e->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
      s << "\n\\maudeEquals\n";
      latexPrettyPrint(s, e->getRhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  else if (const SortTestConditionFragment* t = dynamic_cast<const SortTestConditionFragment*>(c))
    {
      latexPrettyPrint(s, t->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
      s << "\n\\maudeHasSort\n" << latexType(t->getSort());
    }
  else if(const AssignmentConditionFragment* a = dynamic_cast<const AssignmentConditionFragment*>(c))
    {
      latexPrettyPrint(s, a->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
      s << "\n\\maudeAssign\n";
      latexPrettyPrint(s, a->getRhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  else if(const RewriteConditionFragment* r = dynamic_cast<const RewriteConditionFragment*>(c))
    {
      latexPrettyPrint(s, r->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
      s << "\n\\maudeRewritesTo\n";
      latexPrettyPrint(s, r->getRhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  else
    CantHappen("bad condition fragment");
}

void
VisibleModule::latexPrintCondition(ostream& s, const Vector<ConditionFragment*>& condition)
{
  int nrFragments = condition.length();
  for (int i = 0; i < nrFragments; )
    {
      latexPrintConditionFragment(s, condition[i]);
      ++i;
      if (i < nrFragments)
	s << "\\maudeConjunction\n";
    }
}

void
VisibleModule::latexPrintCondition(ostream& s, const PreEquation* pe)
{
  s << "\n\\maudeSpace\\maudeKeyword{if}\\maudeSpace";
  latexPrintCondition(s, pe->getCondition());
}

void
VisibleModule::latexPrintAttributes(ostream& s,
				    const PreEquation* pe,
				    int metadata,
				    const PrintAttribute* printAttribute,
				    bool owise,
				    bool variant,
				    bool narrowing)
{
  int label = pe->getLabel().id();
  bool nonexec = pe->isNonexec();
  if (!nonexec && !owise && !variant && !narrowing && label == NONE && metadata == NONE && printAttribute == 0)
    return;

  s << "\\maudeSpace\\maudeLeftBracket";
  const char* space = "";
  if (nonexec)
    {
      s << "\\maudeKeyword{nonexec}";
      space = "\\maudeSpace";
    }
  if (owise)
    {
      s << space << "\\maudeKeyword{owise}";
      space = "\\maudeSpace";
    }
  if (variant)
    {
      s << space << "\\maudeKeyword{variant}";
      space = "\\maudeSpace";
    }
  if (narrowing)
    {
      s << space << "\\maudeKeyword{narrowing}";
      space = "\\maudeSpace";
    }
  if (label != NONE)
    {
      s << space << "\\maudeKeyword{label}\\maudeSpace\\maudeLabel{" << Token::latexName(label) << "}";
      space = "\\maudeSpace";
    }
  if (metadata != NONE)
    {
      s << space << "\\maudeKeyword{metadata}\\maudeSpace\\maudeString{" << Token::latexName(metadata) << "}";
      space = "\\maudeSpace";
    }
  if (printAttribute != 0)
    {
      s << space;
      printAttribute->latexPrint(s, *pe);
    }
  s << "\\maudeRightBracket";
}

void
VisibleModule::latexPrintMembershipAxiom(ostream& s, const char* indent, const SortConstraint* mb)
{
  s << "\\par" << indent <<  (mb->hasCondition() ? "\\maudeKeyword{cmb}" : "\\maudeKeyword{mb}");
  s << "\\maudeSpace $";
  latexPrettyPrint(s, mb->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
  s << "\\maudeHasSort" << latexType(mb->getSort());
  if (mb->hasCondition())
    latexPrintCondition(s, mb);
  latexPrintAttributes(s, mb, getMetadata(MetadataStore::MEMB_AX, mb), getPrintAttribute(MetadataStore::MEMB_AX, mb));
  s << "$\\maudeEndStatement\n";
}

void
VisibleModule::latexShowMbs(ostream& s, const char* indent, bool all)
{
  const Vector<SortConstraint*>& mbs = getSortConstraints();
  Index nrMbs = all ? mbs.length() : getNrOriginalMembershipAxioms();
  for (Index i = 0; i < nrMbs; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      latexPrintMembershipAxiom(s, indent, mbs[i]);
    }
}

void
VisibleModule::latexPrintEquation(ostream& s, const char* indent, const Equation* eq)
{
  s << "\\par" << indent <<  (eq->hasCondition() ? "\\maudeKeyword{ceq}" : "\\maudeKeyword{eq}");
  s << "\\maudeSpace $";
  latexPrettyPrint(s, eq->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
  s << "\\maudeEquals";
  latexPrettyPrint(s, eq->getRhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
  if (eq->hasCondition())
    latexPrintCondition(s, eq);
  latexPrintAttributes(s,
		       eq,
		       getMetadata(MetadataStore::EQUATION, eq),
		       getPrintAttribute(MetadataStore::EQUATION, eq),
		       eq->isOwise(),
		       eq->isVariant());
  s << "$\\maudeEndStatement\n";
}

void
VisibleModule::latexShowEqs(ostream& s, const char* indent, bool all)
{
  const Vector<Equation*>& equations = getEquations();
  Index nrEquations = all ? equations.length() : getNrOriginalEquations();
  for (Index i = 0; i < nrEquations; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      latexPrintEquation(s, indent, equations[i]);
    }
}

void
VisibleModule::latexPrintRule(ostream& s, const char* indent, const Rule* rl)
{
  s << "\\par" << indent <<  (rl->hasCondition() ? "\\maudeKeyword{crl}" : "\\maudeKeyword{rl}");
  s << "\\maudeSpace $";
  latexPrettyPrint(s, rl->getLhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
  s << "\\maudeRewritesTo";
  latexPrettyPrint(s, rl->getRhs(), UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
  if (rl->hasCondition())
    latexPrintCondition(s, rl);
  latexPrintAttributes(s,
		       rl,
		       getMetadata(MetadataStore::RULE, rl),
		       getPrintAttribute(MetadataStore::RULE, rl),
		       false,
		       false,
		       rl->isNarrowing());
  s << "$\\maudeEndStatement\n";
}

void
VisibleModule::latexShowRls(ostream& s, const char* indent, bool all)
{
  const Vector<Rule*>& rules = getRules();
  Index nrRules = all ? rules.length() : getNrOriginalRules();
  for (Index i = 0; i < nrRules; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      latexPrintRule(s, indent, rules[i]);
    }
}

void
VisibleModule::latexShowImports(ostream& s) const
{
  int nrImports = getNrImports();
  for (int i = 0; i < nrImports; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      //const char* moduleName = Token::removeBoundParameterBrackets(getImportedModule(i)->id()).makeZeroTerminatedString();
      //s << "\\par\\maudeIndent\\maudeKeyword{"<< importModeString(getImportMode(i)) << "}\\ \\maudeModule{" <<
      //	Token::latexName(moduleName) << "}\\maudeEndStatement\n";
      s << "\\par\\maudeIndent$\\maudeKeyword{"<< importModeString(getImportMode(i)) << "}\\maudeSpace" <<
	getImportedModule(i)->latexModuleExpression() <<"$\\maudeEndStatement\n";
      //delete [] moduleName;
    }
}

void
VisibleModule::latexShowSorts(ostream& s, bool all) const
{
  if (UserLevelRewritingContext::interrupted())
    return;
  Index begin = all ? 0 : getNrImportedSorts();
  Index end =  getNrUserSorts();
  Index nrSorts = end - begin;
  if (nrSorts > 0)
    {
      const Vector<Sort*>& sorts = getSorts();
      s << "\\par\\maudeIndent\\maudeKeyword{sort" << pluralize(nrSorts) << "}"; 
       for (Index i = begin; i < end; ++i)
	 s << "\\maudeSpace" << latexType(sorts[i]);
      s << "\\maudeEndStatement\n";
    }
}

void
VisibleModule::latexShowSubsorts(ostream& s, bool all) const
{
  const Vector<Sort*>& sorts = getSorts();
  Index nrUserSorts =  getNrUserSorts();
  for (Index i = 0; i < nrUserSorts; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      Index begin = all ? 0 : getNrImportedSubsorts(i);
      const Sort* sort = sorts[i];
      const Vector<Sort*>& subsorts = sort->getSubsorts();
      Index end = subsorts.size();
      Index nrSubsorts = end - begin;
      if (nrSubsorts > 0)
	{
	  s << "\\par\\maudeIndent\\maudeKeyword{subsort" << pluralize(nrSubsorts) << "}";
	  for (Index j = begin; j < end; ++j)
	    s << "\\maudeSpace" << latexType(subsorts[j]);
	  s << "\\maudeSpace\\maudeLessThan\\maudeSpace" << latexType(sort) << "\\maudeEndStatement\n";
	}
    }
}

void
VisibleModule::latexShowVars(ostream& s) const
{
  const AliasMap& variableAliases = getVariableAliases();
  for (const auto& p : variableAliases)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      s << "\\par\\maudeIndent\\maudeKeyword{var} $" << Token::latexIdentifier(p.first) << "\\maudeHasSort" <<
	latexType(p.second) << "$\\maudeEndStatement\n";
    }
}

void
VisibleModule::latexShowDecls(ostream& s, const char* indent, Index index, bool all)
{
  Symbol* symbol = getSymbols()[index];
  int id = symbol->id();
  Index begin = all ? 0 : getNrImportedDeclarations(index);
  Index end = getNrUserDeclarations(index);
  Assert(end > 0, "op " << symbol << " has no declarations");
  const Vector<OpDeclaration>& opDecls = symbol->getOpDeclarations();

  int nrArgs = symbol->arity();
  for (Index i = begin; i < end; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      const Vector<Sort*>& dec = opDecls[i].getDomainAndRange();
      s << "\\par" << indent << "\\maudeKeyword{op} $";
      if (nrArgs == 0)
	{
	  if (Token::auxProperty(id) == Token::AUX_STRUCTURED_SORT)
	    s << latexStructuredConstant(id);
	  else
	    s << latexPrettyOp(id);		   
	  s <<  "\\maudeHasSort\\maudeSpace";
	}
      else
	s << latexPrettyOp(id) << "\\maudeHasSort";

      for (Index j = 0; j < nrArgs; ++j)
	s << (j == 0 ? "" : "\\maudeSpace") << latexType(dec[j]);
       s << "\\maudeFunction" << latexType(dec[nrArgs]);
      latexShowAttributes(s, symbol, i);
      s << "$\\maudeEndStatement\n";
    }
}

void
VisibleModule::latexShowOps(ostream& s, const char* indent, bool all)
{
  Index begin = all ? 0 : getNrImportedSymbols();
  Index end = getNrUserSymbols();
  for (Index i = begin; i < end; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      latexShowDecls(s, indent, i, all);
    }
}

void
VisibleModule::latexShowPolymorphDecl(ostream& s, const char* indent, Index index)
{
  s << "\\par" << indent << "\\maudeKeyword{op} $";
  const Vector<Sort*>& domainAndRange = getPolymorphDomainAndRange(index);
  int nrArgs = domainAndRange.length() - 1;
  int id = getPolymorphName(index).code();
  if (nrArgs == 0)
    {
      if (Token::auxProperty(id) == Token::AUX_STRUCTURED_SORT)
	s << latexStructuredConstant(id);
     else
       s << latexPrettyOp(id);
      s <<  "\\maudeHasSort\\maudeSpace";
    }
  else
    s << latexPrettyOp(id) << "\\maudeHasSort";

  for (int i = 0; i < nrArgs; ++i)
    {
      s << (i == 0 ? "" : "\\maudeSpace");
      if (Sort* sort = domainAndRange[i])
	s << latexType(sort);
      else
	s << "\\maudeSort{Universal}";
    }
  s << "\\maudeFunction";
  if (Sort* sort = domainAndRange[nrArgs])
    s << latexType(sort);
  else
    s << "\\maudeSort{Universal}";
  
  s << "\\maudeSpace\\maudeLeftBracket\\maudeKeyword{poly}\\maudeSpace\\maudeLeftParen";
  const char* sep = "";
  for (int i = 0; i < nrArgs; ++i)
    {
      if (domainAndRange[i] == 0)
	{
	  s << sep << "\\maudeNumber{" << i + 1 << "}";
	  sep = "\\maudeSpace";
	}
    }
  if (domainAndRange[nrArgs] == 0)
    s << sep << "\\maudeNumber{0}";
  s << "\\maudeRightParen";
  latexShowPolymorphAttributes(s, index);
  s << "\\maudeRightBracket$\\maudeEndStatement\n";
}

void
VisibleModule::latexShowPolymorphs(ostream& s, const char* indent, bool all)
{
  Index nrPolymorphs = getNrPolymorphs();
  Index begin = all ? 0 : getNrImportedPolymorphs();
  for (Index i = begin; i < nrPolymorphs; ++i)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      latexShowPolymorphDecl(s, indent, i);
    }
}

void
VisibleModule::latexShowModule(ostream& s, bool all)
{
  s << "\\par\\maudeKeyword{" << moduleTypeString(getModuleType()) << "}\\maudeSpace";
  latexPrintModuleName(s, this);
  int nrParameters = getNrParameters();
  if (nrParameters > 0)
    {
      s << "$\\maudeLeftBrace\\maudeParameter{" << Token::latexName(getParameterName(0)) << "}\\maudeParameterColon" <<
	getParameterTheory(0)->latexModuleExpression();
      for (int i = 1; i < nrParameters; ++i)
	{
	  s << "\\maudeComma\\maudeParameter{" << Token::latexName(getParameterName(i)) << "}\\maudeParameterColon" <<
	    getParameterTheory(i)->latexModuleExpression();
	}
      s << "\\maudeRightBrace$";
    }
  s << "\\maudeSpace\\maudeKeyword{is}\n";
  if (!all)
    latexShowImports(s);
  
  latexShowSorts(s, all);
  latexShowSubsorts(s, all);
  latexShowPolymorphs(s, "\\maudeIndent", all);
  latexShowOps(s, "\\maudeIndent", all);
  latexShowVars(s);
  latexShowMbs(s, "\\maudeIndent", all);
  latexShowEqs(s, "\\maudeIndent", all);
  latexShowRls(s, "\\maudeIndent", all);
  //showStrats(s, true, all);
  //showSds(s, true, all);
  if (UserLevelRewritingContext::interrupted())
    return;
  s << "\\par\\maudeKeyword{" << moduleEndString(getModuleType()) << "}\n";
}

void
VisibleModule::latexShowSortsAndSubsorts(ostream& s) const
{
  const Vector<Sort*>& sorts = getSorts();
  Index nrUserSorts =  getNrUserSorts();
  if (nrUserSorts == 0)
    return;
  s << "\\par\n\\begin{tabular}{@{}ll}\n";
  for (Index i = 0; i < nrUserSorts; i++)
    {
      if (UserLevelRewritingContext::interrupted())
	return;
      const Sort* sort = sorts[i];
      s << "\\maudeKeyword{sort} " << latexType(sort) << "\\maudeEndStatement & ";
      
      Index index = sort->index();
      const ConnectedComponent* c = sort->component();
      Index nrComponentSorts = c->nrSorts();
      Index nrSupersorts = 0;
      for (Index j = 1; j < index; ++j)
	{
	  if (leq(sort, j))
	    ++nrSupersorts;
	}
      Index nrSubsorts = 0;
      for (Index j = index + 1; j < nrComponentSorts; ++j)
	{
	  if (leq(j, sort))
	    ++nrSubsorts;
	}

      Index nrComparableSorts = nrSubsorts + nrSupersorts;
      if (nrComparableSorts > 0)
	{
	  s << "\\maudeKeyword{subsort" << pluralize(nrComparableSorts) << "}";
	  if (nrSubsorts > 0)
	    {
	      for (Index j = nrComponentSorts - 1; j > index ; --j)
		{
		  const Sort* sort2 = c->sort(j);
		  if (leq(sort2, sort))
		    s << "\\maudeSpace" << latexType(sort2);
		}
	      s << "\\maudeSpace\\maudeLessThan";
	    }
	  s << "\\maudeSpace\\color{magenta}" << latexType(sort) << "\\color{black}";
	  if (nrSupersorts > 0)
	    {
	      s << "\\maudeSpace\\maudeLessThan";
	      for (Index j = index - 1; j > 0; --j)
		{
		  const Sort* sort2 = c->sort(j);
		  if (leq(sort, sort2))
		    s << "\\maudeSpace" << latexType(sort2);
		}
	    }
	  s << "\\maudeEndStatement";
	}
      s << "\\\\\n";
    }
  s << "\\end{tabular}\n";
}

void
VisibleModule::latexShowAttributes(ostream& s, Symbol* symbol, Index opDeclIndex)
{
  Vector<int> gather;
  getGather(symbol, gather);  // there will be a gather whenever there is mixfix syntax and args
  Index gatherLength = gather.size();
  SymbolType st = getSymbolType(symbol);
  const OpDeclaration& decl = symbol->getOpDeclarations()[opDeclIndex];
  bool ctor = decl.isConstructor();
  int metadata = getMetadata(symbol, opDeclIndex);

  if (gatherLength == 0 &&
      st.getBasicType() == SymbolType::STANDARD &&
      !(st.hasFlag(SymbolType::ATTRIBUTES)) &&
      !ctor &&
      metadata == NONE)
    return;  // no attributes;

  s << "\\maudeSpace\\maudeLeftBracket";
  const char* space = "";
  if (ctor)
    {
      s << "\\maudeKeyword{ctor}";
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
  if (st.hasFlag(SymbolType::ITER))
    {
      s << space << "\\maudeKeyword{iter}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << space;
      space = "\\maudeSpace";
      if (!(st.hasFlag(SymbolType::LEFT_ID | SymbolType::COMM)))
	s << "\\maudeKeyword{right}\\maudeSpace";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID | SymbolType::COMM)))
	s << "\\maudeKeyword{left}\\maudeSpace";
      s << "\\maudeKeyword{id:}\\maudeSpace";
      Term* id = safeCast(BinarySymbol*, symbol)->getIdentity();
      if (id != 0)
	latexPrettyPrint(s, id, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  if (st.hasFlag(SymbolType::IDEM))
    {
      s << space << "\\maudeKeyword{idem}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::PCONST))
    {
      s << space << "\\maudeKeyword{pconst}";
      space = "\\maudeSpace";
    }
  //
  //	Object-oriented attributes.
  //
  if (st.hasFlag(SymbolType::MESSAGE))
    {
      s << space << "\\maudeKeyword{msg}";
      space = "\\maudeSpace";
    }
  if (st.hasFlag(SymbolType::OBJECT))
    {
      s << space << "\\maudeKeyword{obj}";
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
  const Vector<int>& strat = symbol->getStrategy();
  Index stratLen = strat.size();
  if (stratLen > 0)
    {
      s << space << "\\maudeKeyword{strat}\\maudeSpace\\maudeLeftParen";
      space = "\\maudeSpace";
      for (int k = 0; k < stratLen; ++k)
	{
	  s << "\\maudeNumber{" << strat[k] << "}";
	  if (k + 1 < stratLen)
	    s << "\\maudeSpace";
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
      s << space << "\\maudeKeyword{frozen}\\maudeSpace\\maudeLeftParen";
      space = "\\maudeSpace";
      const NatSet& frozen = symbol->getFrozen();
      const NatSet::const_iterator e = frozen.end();
      for (NatSet::const_iterator i = frozen.begin();;)
	{
	  s << "\\maudeNumber{" << *i + 1 << "}";
	  if (++i == e)
	    break;
	  s << "\\maudeSpace";
	}
      s << "\\maudeRightParen";
    }
  //
  //	Syntactic attributes.
  //
  if (st.hasFlag(SymbolType::PREC | SymbolType::GATHER) || gatherLength > 0)
    {
      s << space << "\\maudeKeyword{prec}\\maudeSpace\\maudeNumber{" << getPrec(symbol) << "}";
      space = "\\maudeSpace";
      if (gatherLength > 0)
	{
	  s << "\\maudeSpace\\maudeKeyword{gather}\\maudeSpace\\maudeLeftParen";
	  for (int i = 0; i < gatherLength; i++)
	    {
	      if (i != 0)
		s << "\\maudeSpace";
	      switch (gather[i])
		{
		case GATHER_e:
		  s << "\\maudeNormal{e}";
		  break;
		case GATHER_E:
		  s << "\\maudeNormal{E}";
		  break;
		case GATHER_AMP:
		  s << "\\maudeNormal{\\&}";
		  break;
		}
	    }
	  s << "\\maudeRightParen";
	}
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << space << "\\maudeKeyword{format}\\maudeSpace\\maudeLeftParen";
      space = "\\maudeSpace";
      const Vector<int>& format = getFormat(symbol);
      int formatLength = format.length();
      for (int i = 0; i < formatLength; ++i)
	{
	  if (i != 0)
	    s << "\\maudeSpace";
	  s << "\\maudeNormal{" << Token::name(format[i]) << "}";
	}
      s << "\\maudeRightParen";
    }
  if (metadata != NONE)
    {
      s << space << "\\maudeKeyword{metadata}\\maudeSpace\\maudeString{" << Token::latexName(metadata) << "}";
      space = "\\maudeSpace";
    }
  if (st.hasSpecial())
    {
      s << space << "\\maudeKeyword{special}\\maudeSpace\\maudeLeftParen";
      Vector<const char*> purposes;
      {
	//
	//	id-hooks
	//
	Vector<Vector<const char*> > data;
	getDataAttachments(symbol, decl.getDomainAndRange(), purposes, data);
	Index nrHooks = purposes.size();
	for (Index i = 0; i < nrHooks; ++i)
	  {
	    s << "\\newline\\maudeKeyword{id-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purposes[i]) << "}";
	    const Vector<const char*>& items = data[i];
	    Index nrItems = items.size();
	    if (nrItems > 0)
	      {
		for (Index j = 0; j < nrItems; ++j)
		  s << ((j == 0) ? "\\maudeSpace\\maudeLeftParen" : "\\maudeSpace") <<
		    "\\maudeSymbolic{" << Token::latexName(items[j]) << "}";
		s << "\\maudeRightParen";
	      }
	    s << "\n";
	  }
      }
      purposes.clear();
      {
	//
	//	op-hooks
	//
	Vector<Symbol*> symbols;
	getSymbolAttachments(symbol, purposes, symbols);
	Index nrHooks = purposes.size();
	for (Index i = 0; i < nrHooks; ++i)
	  {
	    Symbol* op = symbols[i];
	    s << "\\newline\\maudeKeyword{op-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purposes[i]) << "}\\maudeSpace\\maudeLeftParen" <<
	      Token::latexIdentifier(op->id()) << "\\maudeSpace\\maudeHasSort\\maudeSpace";
	    const Vector<Sort*>& domainAndRange = op->getOpDeclarations()[0].getDomainAndRange();
	    Index nrSorts = domainAndRange.size() - 1;
	    //
	    //	We don't use fancy printing for sorts in an op-hook since they
	    //	should always be printed as single tokens.
	    //
	    for (Index j = 0; j < nrSorts; ++j)
	      {
		Sort* sort = domainAndRange[j];
		if (sort->index() == Sort::KIND)
		  sort = sort->component()->sort(1);
		s << "\\maudeSort{" << Token::latexName(sort->id()) << "}\\maudeSpace";
	      }
	    Sort* sort = domainAndRange[nrSorts];
	    if (sort->index() == Sort::KIND)
	      sort = sort->component()->sort(1);
	    s << "\\maudePartialFunction\\maudeSpace\\maudeSort{" << Token::name(sort->id()) <<
	      "}\\maudeRightParen\n";
	  }
      }
      purposes.clear();
      {
	//
	//	term hooks
	//
	Vector<Term*> terms;
	getTermAttachments(symbol, purposes, terms);
	Index nrHooks = purposes.size();
	for (Index i = 0; i < nrHooks; ++i)
	  {
	    s << "\\newline\\maudeKeyword{term-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purposes[i]) << "}\\maudeSpace\\maudeLeftParen";
	    latexPrettyPrint(s, terms[i], UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
	    s << "\\maudeRightParen\n";
	  }
      }
      s << "\\maudeRightParen";
    }
  s << "\\maudeRightBracket";
}

void
VisibleModule::latexShowPolymorphAttributes(ostream& s, int index)
{
  SymbolType st = getPolymorphType(index);
  if (st.hasFlag(SymbolType::CTOR))
    s << " ctor";
  //
  //	Theory attributes.
  //
  if (st.hasFlag(SymbolType::ASSOC))
      s << "\\maudeSpace\\maudeKeyword{assoc}";
  if (st.hasFlag(SymbolType::COMM))
      s << "\\maudeSpace\\maudeKeyword{comm}";
  if (st.hasFlag(SymbolType::ITER))
      s << "\\maudeSpace\\maudeKeyword{iter}";
  if (st.hasFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      s << "\\maudeSpace";
      if (!(st.hasFlag(SymbolType::LEFT_ID | SymbolType::COMM)))
	s << "\\maudeKeyword{right}\\maudeSpace";
      else if (!(st.hasFlag(SymbolType::RIGHT_ID | SymbolType::COMM)))
	s << "\\maudeKeyword{left}\\maudeSpace";
      s << "\\maudeKeyword{id:}\\maudeSpace";
      Term* id = getPolymorphIdentity(index);
      if (id != 0)
	latexPrettyPrint(s, id, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
    }
  if (st.hasFlag(SymbolType::IDEM))
      s << "\\maudeSpace\\maudeKeyword{idem}";
  if (st.hasFlag(SymbolType::PCONST))
      s << "\\maudeSpace\\maudeKeyword{pconst}";
  //
  //	Object-oriented attributes.
  //
  if (st.hasFlag(SymbolType::MESSAGE))
      s << "\\maudeSpace\\maudeKeyword{msg}";
  if (st.hasFlag(SymbolType::OBJECT))
      s << "\\maudeSpace\\maudeKeyword{obj}";
  if (st.hasFlag(SymbolType::CONFIG))
      s << "\\maudeSpace\\maudeKeyword{config}";
  //
  //	Semantic attributes.
  //
  const Vector<int>& strat = getPolymorphStrategy(index);
  Index stratLen = strat.size();
  if (stratLen > 0)
    {
      s << "\\maudeSpace\\maudeKeyword{strat}\\maudeSpace\\maudeLeftParen";
      for (int k = 0; k < stratLen; ++k)
	{
	  s << "\\maudeNumber{" << strat[k] << "}";
	  if (k + 1 < stratLen)
	    s << "\\maudeSpace";
	}
      s << "\\maudeRightParen";
    }
  if (st.hasFlag(SymbolType::MEMO))
    s << "\\maudeSpace\\maudeKeyword{memo}";
  if (st.hasFlag(SymbolType::FROZEN))
    {
      s << "\\maudeSpace\\maudeKeyword{frozen}\\maudeSpace\\maudeLeftParen";
      const NatSet& frozen = getPolymorphFrozen(index);
      const NatSet::const_iterator e = frozen.end();
      for (NatSet::const_iterator i = frozen.begin();;)
	{
	  s << "\\maudeNumber{" << *i + 1 << "}";
	  if (++i == e)
	    break;
	  s << "\\maudeSpace";
	}
      s << "\\maudeRightParen";
    }
  //
  //	Syntactic attributes.
  //
  Vector<int> gather;
  getPolymorphGather(index, gather);
  int gatherLength = gather.length();
  if (st.hasFlag(SymbolType::PREC | SymbolType::GATHER) || gatherLength > 0)
    {
      s << "\\maudeSpace\\maudeKeyword{prec}\\maudeSpace\\maudeNumber{" << getPolymorphPrec(index) << "}";
      if (gatherLength > 0)
	{
	  s << "\\maudeSpace\\maudeKeyword{gather}\\maudeSpace\\maudeLeftParen";
	  for (int i = 0; i < gatherLength; i++)
	    {
	      if (i != 0)
		s << "\\maudeSpace";
	      switch (gather[i])
		{
		case GATHER_e:
		  s << "\\maudeNormal{e}";
		  break;
		case GATHER_E:
		  s << "\\maudeNormal{E}";
		  break;
		case GATHER_AMP:
		  s << "\\maudeNormal{\\&}";
		  break;
		}
	    }
	  s << "\\maudeRightParen";
	}
    }
  if (st.hasFlag(SymbolType::FORMAT))
    {
      s << "\\maudeSpace\\maudeKeyword{format}\\maudeSpace\\maudeLeftParen";
      const Vector<int>& format = getPolymorphFormat(index);
      Index formatLength = format.size();
      for (Index i = 0; i < formatLength; ++i)
	{
	  if (i != 0)
	    s << "\\maudeSpace";
	  s << "\\maudeNormal{" << Token::name(format[i]) << "}";
	}
      s << "\\maudeRightParen";
    }
  int metadata = getPolymorphMetadata(index);
  if (metadata != NONE)
    s << "\\maudeSpace\\maudeKeyword{metadata}\\maudeSpace\\maudeString{" << Token::latexName(metadata) << "}";

  if (st.hasSpecial())
    {
      s << "\\maudeSpace\\maudeKeyword{special}\\maudeSpace\\maudeLeftParen";
      int purpose;
      {
	//
	//	id-hooks
	//
	Vector<int> items;
	for (int i = 0; getPolymorphDataAttachment(index, i, purpose, items); ++i)
	  {
	    s << "\\newline\\maudeKeyword{id-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purpose) << "}";
	    Index nrItems = items.size();
	    if (nrItems > 0)
	      {
		for (Index j = 0; j < nrItems; ++j)
		  {
		    s << ((j == 0) ? "\\maudeSpace\\maudeLeftParen" : "\\maudeSpace") << "\\maudeSymbolic{"
		      << Token::latexName(items[j]) << "}";
		  }
		s << "\\maudeRightParen";
	      }
	    s << "\n";
	  }
      }
      {
	//
	//	op-hooks
	//
	Symbol* op;
	for (int i = 0; getPolymorphSymbolAttachment(index, i, purpose, op); ++i)
	  {
	    s << "\\newline\\maudeKeyword{op-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purpose) << "}\\maudeSpace\\maudeLeftParen" <<
	      Token::latexIdentifier(op->id()) << "\\maudeSpace\\maudeHasSort\\maudeSpace";
	    const Vector<Sort*>& domainAndRange = op->getOpDeclarations()[0].getDomainAndRange();
	    int nrSorts = domainAndRange.length() - 1;
	    //
	    //	We don't use fancy printing for sorts in an op-hook since they
	    //	should always be printed as single tokens.
	    //
	    for (Index j = 0; j < nrSorts; ++j)
	      {
		Sort* sort = domainAndRange[j];
		if (sort->index() == Sort::KIND)
		  sort = sort->component()->sort(1);
		s << "\\maudeSort{" << Token::latexName(sort->id()) << "}\\maudeSpace";
	      }
	    Sort* sort = domainAndRange[nrSorts];
	    if (sort->index() == Sort::KIND)
	      sort = sort->component()->sort(1);
	    s << "\\maudePartialFunction\\maudeSpace\\maudeSort{" << Token::name(sort->id()) <<
	      "}\\maudeRightParen\n";
	  }
      }
      {
	//
	//	term-hooks
	//
	Term* term;
	for (int i = 0; getPolymorphTermAttachment(index, i, purpose, term); i++)
	  {
	    s << "\\newline\\maudeKeyword{term-hook}\\maudeSpace" << "\\maudeSymbolic{" <<
	      Token::latexName(purpose) << "}\\maudeSpace\\maudeLeftParen";
	    latexPrettyPrint(s, term, UNBOUNDED, UNBOUNDED, 0, UNBOUNDED, 0, false);
	    s << "\\maudeRightParen\n";
	  }
      }
      s << "\\maudeRightParen";
    }
}
