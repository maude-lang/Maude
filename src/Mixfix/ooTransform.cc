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
//	Code for processing object-oriented transformations on statements.
//

StatementTransformer::Outcome
SyntacticPreModule::transformSortConstraint(SortConstraint* sortConstraint)
{
  StatementInfo si;
  gatherObjects(sortConstraint, si);
  if (si.ignore)
    return Outcome::NOT_TRANSFORMED;
  if (si.objectMap.empty())
    return Outcome::NOT_APPLICABLE;
  if (si.checkVariables())
    {
      Verbose("Considering object completion on:\n  " << sortConstraint);
      if (doTransformation(si))
	{
	  Verbose("Transformed membership axiom:\n  " << sortConstraint << "\n");
	  return Outcome::TRANSFORMED;
	}
      else
	{
	  Verbose("Object completion not needed.\n");
	  return Outcome::NOT_NEEDED;
	}
    }
  return Outcome::NOT_TRANSFORMED;
}

StatementTransformer::Outcome
SyntacticPreModule::transformEquation(Equation* equation)
{
  StatementInfo si;
  gatherObjects(equation, si);
  gatherObjects(GatherMode::SUBJECT, equation->getRhs(), si);
  if (si.ignore)
    return Outcome::NOT_TRANSFORMED;
  if (si.objectMap.empty())
    return Outcome::NOT_APPLICABLE;
  if (si.checkVariables())
    {
      Verbose("Considering object completion on:\n  " << equation);
      if (doTransformation(si))
	{
	  Verbose("Transformed equation:\n  " << equation << "\n");
	  return Outcome::TRANSFORMED;
	}
      else
	{
	  Verbose("Object completion not needed.\n");
	  return Outcome::NOT_NEEDED;
	}
    }
  return Outcome::NOT_TRANSFORMED;
}

StatementTransformer::Outcome
SyntacticPreModule::transformRule(Rule* rule)
{
  StatementInfo si;
  gatherObjects(rule, si);
  gatherObjects(GatherMode::SUBJECT, rule->getRhs(), si);
  if (si.ignore)
    return Outcome::NOT_TRANSFORMED;
  if (si.objectMap.empty())
    return Outcome::NOT_APPLICABLE;
  if (si.checkVariables())
    {
      Verbose("Considering object completion on:\n  " << rule);
      if (doTransformation(si))
	{
	  Verbose("Transformed rule:\n  " << rule << "\n");
	  return Outcome::TRANSFORMED;
	}
      else
	{
	  Verbose("Object completion not needed.\n");
	  return Outcome::NOT_NEEDED;
	}
    }
  return Outcome::NOT_TRANSFORMED;
}

int
SyntacticPreModule::StatementInfo::chooseFreshVariableName(const char* base)
{
  int code = Token::encode(base);
  for (Index index = 2; forbiddenNames.find(code) != forbiddenNames.end(); ++index)
    {
      string name(base);
      name += int64ToString(index);
      code = Token::encode(name.c_str());
    }
  forbiddenNames.insert(code);
  return code;
}

Sort*
SyntacticPreModule::findCorrespondingClassSort(const Symbol* s) const
{
  //
  //	Check if a symbol is a class symbol; if yes, return the corresponding class sort, otherwise return 0.
  //
  //	Must be a constant.
  //
  if (s->arity() == 0)
    {
      //
      //	Must have a single declaration.
      //
      const Vector<OpDeclaration>& opDecls = s->getOpDeclarations();
      if (opDecls.size() == 1)
	{
	  //
	  //	Declaration must be a ctor.
	  //
	  const OpDeclaration& opDecl = opDecls[0];
	  if (opDecl.isConstructor())
	    {
	      //
	      //	Must put the constant in a class sort.
	      //
	      const Vector<Sort*>& domainAndRange = opDecls[0].getDomainAndRange();
	      Sort* sort = domainAndRange[0];
	      if (isClassSort(sort))
		return sort;
	    }
	}
    }
  return 0;
}

void
SyntacticPreModule::gatherObjects(PreEquation* pe, StatementInfo& si) const
{
  gatherObjects(GatherMode::PATTERN, pe->getLhs(), si);
  for (ConditionFragment* cf : pe->getCondition())
    {
      if (SortTestConditionFragment* st = dynamic_cast<SortTestConditionFragment*>(cf))
	gatherObjects(GatherMode::CONDITION_SUBJECT, st->getLhs(), si);
      else if (EqualityConditionFragment* eq = dynamic_cast<EqualityConditionFragment*>(cf))
	{
	  gatherObjects(GatherMode::CONDITION_SUBJECT, eq->getLhs(), si);
	  gatherObjects(GatherMode::CONDITION_SUBJECT, eq->getRhs(), si);
	}
      else if (AssignmentConditionFragment* as = dynamic_cast<AssignmentConditionFragment*>(cf))
	{
	  gatherObjects(GatherMode::CONDITION_PATTERN, as->getLhs(), si);
	  gatherObjects(GatherMode::CONDITION_SUBJECT, as->getRhs(), si);
	}
      else if (RewriteConditionFragment* rw = dynamic_cast<RewriteConditionFragment*>(cf))
	{
	  gatherObjects(GatherMode::CONDITION_SUBJECT, rw->getLhs(), si);
	  gatherObjects(GatherMode::CONDITION_PATTERN, rw->getRhs(), si);
	}
    }
}

void
SyntacticPreModule::gatherObjects(GatherMode mode, Term* term, StatementInfo& si) const
{
  Symbol* s = term->symbol();
  SymbolType st = flatModule->getSymbolType(s);
  if (st.getBasicType() == SymbolType::OBJECT_CONSTRUCTOR_SYMBOL)
    {
      if (flatModule->getObjectSymbols().contains(s->getIndexWithinModule()))
	{
	  //
	  //	Consider this to be an object occurrence.
	  //
	  if (mode == GatherMode::CONDITION_PATTERN)
	    {
	      IssueAdvisory(*term << ": object " << QUOTE(term) << " appearing in condition fragment pattern disables object completion.");
	      si.ignore = true;
	    }
	  else
	    {
	      ArgumentIterator i(*term);
	      Assert(i.valid(), "bad first arg");
	      //
	      //	We make a copy of the object name before we normalize it so we don't disturb the original.
	      //
	      Term* objectName = i.argument()->deepCopy()->normalize(true);
	      //
	      //	Have we seen an object with this name before?
	      //
	      auto oi = si.objectMap.find(objectName);
	      if (oi == si.objectMap.end())
		{
		  //
		  //	We have not seen an object occurrence with this name before.
		  //
		  if (mode == GatherMode::PATTERN)
		    {
		      //
		      //	We're processing the lhs - make a entry in the object map, using our normalized
		      //	copy of the object name.
		      //
		      auto p = si.objectMap.insert({objectName, {}});
		      Assert(p.second, "failed to insert object");
		      p.first->second.patternOccurrence.objectTerm = term;
		      //
		      //	Now look at the class argument.
		      //
		      i.next();
		      Assert(i.valid(), "bad second arg");
		      Term* classArgument = i.argument();
		      if (!recordClassArgument(classArgument, p.first->second))
			{
			  //
			  //	Lhs class argument is not a class constant or a variable of class sort.
			  //
			  IssueAdvisory(*classArgument << ": bad class argument " << QUOTE(classArgument) <<
					" in left-hand side object " << QUOTE(term) << " disables object completion.");
			  si.ignore = true;
			}
		      //
		      //	Now look at the attribute set argument.
		      //
		      i.next();
		      Assert(i.valid(), "bad third arg");
		      if (!analyzeAttributeSetArgument(i.argument(), p.first->second.classSort, p.first->second.patternOccurrence))
			si.ignore = true;
		    }
		  else
		    {
		      //
		      //	We have already processed the lhs so this object cannot have a lhs occurrence.
		      //	We treat is as a new object that we will quietly ignore.
		      //
		      objectName->deepSelfDestruct();
		    }
		}
	      else
		{
		  //
		  //	Because it is not used as the key for the entry in the object map, we destruct the normalized object name.
		  //
		  objectName->deepSelfDestruct();
		  //
		  //	We have seen an object occurrence with this name before.
		  //
		  if (mode == GatherMode::PATTERN)
		    {
		      //
		      //	We're processing the lhs so we have a duplicate occurrence.
		      //
		      IssueAdvisory(*term << ": objects " << QUOTE(oi->second.patternOccurrence.objectTerm) << " and " <<
				    QUOTE(term) <<
				    " occurring in the left-hand side of a statement have same name which disables object completion.");
		      si.ignore = true;
		    }
		  else
		    {
		      //
		      //	Rhs/condition case - add this occurrence to the entry in the object map.
		      //
		      //oi->second.subjectOccurrences.push_back({term});
		      //oi->second.subjectOccurrences.push_back({term, 0, {}});
		      oi->second.subjectOccurrences.push_back(ObjectOccurrence());
		      oi->second.subjectOccurrences.back().objectTerm = term;
		      //
		      //	Now look at the class argument.
		      //
		      i.next();
		      Assert(i.valid(), "bad second arg");
		      Term* classArgument = i.argument();
		      if (oi->second.classArgument != 0 && !classArgument->equal(oi->second.classArgument))
			{
			  //
			  //	For a given object, we require that all of its occurrences have the same class argument,
			  //	which must either be a class constant or a variable of class sort, thus no theory normalization
			  //	is needed before we compare.
			  //
			  IssueAdvisory(*classArgument << ": class argument " << QUOTE(classArgument) << " in object instance " <<
					QUOTE(term) << " differing from class argument " << QUOTE(oi->second.classArgument) <<
					" in left-hand side object instance " << QUOTE(oi->second.patternOccurrence.objectTerm) <<
					" disables object completion.");
			  si.ignore = true;
			}
		      //
		      //	Now look at the attribute set argument.
		      //
		      i.next();
		      Assert(i.valid(), "bad third arg");
		      if (!analyzeAttributeSetArgument(i.argument(), oi->second.classSort, oi->second.subjectOccurrences.back()))
			si.ignore = true;
		    }
		}
	    }
	}
      //
      //	Fall into general case to keep scanning for objects and variables.
      //
    }
  else if (st.getBasicType() == SymbolType::VARIABLE)
    {
      //
      //	We keep track of used variable names, and the number of occurrences of each variable with a
      //	given (base name, sort name) pair.
      //
      int name = safeCastNonNull<VariableTerm*>(term)->id();
      si.forbiddenNames.insert(name);
      Sort* sort = safeCastNonNull<VariableSymbol*>(s)->getSort();
      int sortName = sort->id();
      auto p = si.varCountMap.insert({{name, sortName}, 1});
      if (!p.second)
	++(p.first->second);
      return;
    }
  //
  //	General case - recursive call for each argument.
  //
  for (ArgumentIterator i(*term); i.valid(); i.next())
    gatherObjects(mode, i.argument(), si);
}

bool
SyntacticPreModule::recordClassArgument(Term* classArgument, ObjectInfo& oi) const
{
  //
  //	If we have a valid class argument we fill out the classSort and classArgument.
  //	Also if it is a variable we fill out class classVariableName.
  //
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(classArgument))
    {
      Sort* sort = vt->getSort();
      if (!isClassSort(sort))
	return false;
      oi.classSort = sort;
      oi.classVariableName = vt->id();
    }
  else
    {
      oi.classSort = findCorrespondingClassSort(classArgument->symbol());
      if (!oi.classSort)
	return false;
    }
  oi.classArgument = classArgument;
  return true;
}

bool
SyntacticPreModule::analyzeAttributeSetArgument(Term* attributeSetArgument,
						Sort* classSort,
						ObjectOccurrence& oo) const
{
  ObjectConstructorSymbol* os = safeCastNonNull<ObjectConstructorSymbol*>(oo.objectTerm->symbol());
  ACU_Symbol* attributeSetSymbol = os->getAttributeSetSymbol();
  Symbol* asas = attributeSetArgument->symbol();
  if (asas == attributeSetSymbol)
    {
      for (ArgumentIterator i(*attributeSetArgument); i.valid(); i.next())
	{
	  if (!analyzeAttributeSetArgument(i.argument(), classSort, oo))
	    return false;
	}
      return true;
    }
  if (attributeSymbols.find(asas) != attributeSymbols.end())
    {
      if (classSort != 0)
	{
	  //
	  //	Valid class.
	  //
	  auto pureClass = localClasses.find(classSort->id());
	  if (pureClass != localClasses.end())
	    {
	      //
	      //	Dealing with a pure class for which we know all the declared and inherited attributes.
	      //
	      if (pureClass->second.find(asas) == pureClass->second.end())
		{
		  //
		  //	Attribute isn't one that was declared or inherited.
		  //	It doesn't affect our completion algorithm, and we only do this check for pure
		  //	classes anyway so we don't treat it as an error.
		  //
		  IssueAdvisory(*attributeSetArgument << ": object occurrence " << QUOTE(oo.objectTerm) <<
				" contains an attribute " << QUOTE(ATTRIBUTE(asas)) <<
				" that was neither declared for nor inherited by class " << QUOTE(classSort) << ".");
		}			
	    }
	}
      auto p = oo.attributeTerms.insert({asas, attributeSetArgument});
      if (!p.second)
	{
	  IssueAdvisory(*attributeSetArgument << ": duplicate attribute arguments " << QUOTE(p.first->second) <<
			" and " << QUOTE(attributeSetArgument) << " disable object completion.");
	  return false;
	}
      return true;
    }
  if (VariableTerm* vt = dynamic_cast<VariableTerm*>(attributeSetArgument))
    {
      Sort* sort = vt->getSort();
      Sort* attributeSetSort = attributeSetSymbol->getRangeSort();
      if (sort == attributeSetSort)  // FIXME: might have multiple range sorts
	{
	  if (oo.variableTerm)
	    {
	      IssueAdvisory(*vt << ": multiple variables " << QUOTE(static_cast<Term*>(oo.variableTerm)) <<
			    " and " << QUOTE(attributeSetArgument) << " in attribute set disables object completion.");
	      return false;
	    }
	  oo.variableTerm = vt;
	  return true;
	}
      IssueAdvisory(*vt << ": variable " << QUOTE(attributeSetArgument) << " of sort other than " << QUOTE(attributeSetSort) <<
		    " in attribute set disables object completion.");
      return false;
    }
  if (attributeSetArgument->equal(attributeSetSymbol->getIdentity()))  // FIXME: might need to normalize
    return true;
  IssueAdvisory(*attributeSetArgument << ": unrecognized term " << QUOTE(attributeSetArgument) <<
		" in attribute set disables object completion.");
  return false;
}


bool
SyntacticPreModule::StatementInfo::checkVariables() const
{
  //
  //	Look at each object.
  //
  for (auto& oi : objectMap)
    {
      if (oi.second.classVariableName != NONE)
	{
	  //
	  //	Object is using a variable as its class name. Check that variable doesn't occur elsewhere.
	  //
	  int nrObjectOccurrences = 1 +  oi.second.subjectOccurrences.size();
	  auto vc = varCountMap.find({oi.second.classVariableName, oi.second.classSort->id()});
	  Assert(vc != varCountMap.end(), "missing count for variable " << oi.second.classArgument);
	  if (vc->second > nrObjectOccurrences)
	    {
	      IssueAdvisory(*(oi.second.classArgument) << ": using class variable " <<  QUOTE(oi.second.classArgument) << " from object instance " << 
			    QUOTE(oi.second.patternOccurrence.objectTerm) <<
			    " for other purposes disables object completion.");
	      return false;
	    }
	  Assert(vc->second == nrObjectOccurrences, "missed occurrences of " << oi.second.classArgument);
	}
      VariableTerm* attributeSetVariable = oi.second.patternOccurrence.variableTerm;
      if (attributeSetVariable == 0)
	{
	  //
	  //	No attribute set variable in the pattern occurrence so make sure there is none in the rhs/condition occurrences.
	  //
	  for (const ObjectOccurrence& oo : oi.second.subjectOccurrences)
	    {
	      if (oo.variableTerm != 0)
		{
		  IssueAdvisory(*oo.objectTerm << ": object instance " << QUOTE(oo.objectTerm) << " has a variable " <<
				QUOTE(static_cast<Term*>(oo.variableTerm)) <<
				" in its attribute set, which does not appear in the left-hand side occurrence of this object, " <<
				QUOTE(oi.second.patternOccurrence.objectTerm) << ". This disables object completion.");
		  return false;
		}
	    }
	}
      else
	{
	  //
	  //	Attribute set variable in the pattern occurrence so make sure all rhs/condition occurrences have the same variable.
	  //
	  Term* asv = attributeSetVariable;  // for output overload disambiguation
	  for (const ObjectOccurrence& oo : oi.second.subjectOccurrences)
	    {
	      if (oo.variableTerm == 0)
		{
		  IssueAdvisory(*oo.objectTerm << ": left-hand side object instance " <<
				QUOTE(oi.second.patternOccurrence.objectTerm) <<
				" had a variable " << QUOTE(asv) <<
				" in its attribute set while object instance " <<
				QUOTE(oo.objectTerm) << " does not. This disables object completion.");
		  return false;
		}
	      if (!attributeSetVariable->equal(oo.variableTerm))
		{
		  IssueAdvisory(*oo.objectTerm << ": object instance " << QUOTE(oo.objectTerm) << " has a variable " <<
				QUOTE(static_cast<Term*>(oo.variableTerm)) <<
				" in its attribute set, while left-hand side object instance " <<
				QUOTE(oi.second.patternOccurrence.objectTerm) << " has a variable " <<
				QUOTE(asv) << ". This disables object completion.");
		  return false;
		}
	    }
	  int nrObjectOccurrences = 1 +  oi.second.subjectOccurrences.size();
	  auto vc = varCountMap.find({attributeSetVariable->id(), attributeSetVariable->getSort()->id()});
	  Assert(vc != varCountMap.end(), "missing count for variable " << asv);
	  if (vc->second > nrObjectOccurrences)
	    {
	      IssueAdvisory(*(oi.second.classArgument) << ": using attribute set variable " <<  QUOTE(asv) << " from object instance " << 
			    QUOTE(oi.second.patternOccurrence.objectTerm) <<
			    " for other purposes disables object completion.");
	      return false;
	    }
	  Assert(vc->second == nrObjectOccurrences, "missed occurrences of " << oi.second.classArgument);
	}
    }
  return true;
}

bool
SyntacticPreModule::doTransformation(StatementInfo& si)
{
  //
  //	We've done all the sanity checks and are committed to transforming the statement if needed.
  //
  bool didTransform = false;
  for (auto& oi : si.objectMap)
    {
      DebugAdvisory("transforming " << oi.first);
      //
      //	We know all occurrences of this object have the same class argument; if is is
      //	a constant we need to replace it with a variable.
      //
      if (oi.second.classVariableName == NONE)
	{
	  //
	  //	Not a variable - must be a constant.
	  //
	  VariableSymbol* vs = safeCastNonNull<VariableSymbol*>(flatModule->instantiateVariable(oi.second.classSort));
	  int varName = si.chooseFreshVariableName("V");
	  transformClassArgument(oi.second.patternOccurrence, vs, varName);
	  for (ObjectOccurrence& oo : oi.second.subjectOccurrences)
	    transformClassArgument(oo, vs, varName);
	  didTransform = true;
	}
      //
      //	Fix pattern.
      //
      if (transformPatternAttributes(oi.second, si))
	didTransform = true;
      for (ObjectOccurrence& oo : oi.second.subjectOccurrences)
	{
	  if (transformSubjectAttributes(oo, oi.second.patternOccurrence))
	    didTransform = true;
	}
    }
  return didTransform;
}

void
SyntacticPreModule::transformClassArgument(ObjectOccurrence& oo, VariableSymbol* vs, int varName)
{
  FreeTerm* freeTerm = safeCastNonNull<FreeTerm*>(oo.objectTerm);
  Term* vt = new VariableTerm(vs, varName);
  DebugAdvisory("made variable " << vt);
  Term* oldArgument = freeTerm->replaceArgument(1, vt);
  oldArgument->deepSelfDestruct();
}

bool
SyntacticPreModule::transformPatternAttributes(ObjectInfo& oi, StatementInfo& si)
{
  bool changed = false;
  ObjectOccurrence& po = oi.patternOccurrence;
  ObjectConstructorSymbol* os = safeCastNonNull<ObjectConstructorSymbol*>(po.objectTerm->symbol());
  ACU_Symbol* attributeSetSymbol = os->getAttributeSetSymbol();
  Sort* attributeSetSort = attributeSetSymbol->getRangeSort();  // FIXME: might have multiple declarations

  if (po.variableTerm == 0)
    {
      //
      //	Need to make attribute set variable.
      //
      VariableSymbol* vs = safeCastNonNull<VariableSymbol*>(flatModule->instantiateVariable(attributeSetSort));
      int varName = si.chooseFreshVariableName("Atts");
      po.variableTerm = new VariableTerm(vs, varName);
      changed = true;
    }
  //
  //	Now are there any attributes in the subject occurrences that don't appear in the pattern occurrence?
  //
  AttributeMap& patternAttributes = po.attributeTerms;
  for (ObjectOccurrence& oo : oi.subjectOccurrences)
    {
      for (auto& at : oo.attributeTerms)
	{
	  if (patternAttributes.find(at.first) == patternAttributes.end())
	    {
	      //
	      //	Found an attribute that occurs in oo but not in patternOccurrence.
	      //	We make an attribute term that has a kind variable under the attribute symbol.
	      //
	      Sort* kind = at.first->domainComponent(0)->sort(Sort::KIND);
	      VariableSymbol* vs =  safeCastNonNull<VariableSymbol*>(flatModule->instantiateVariable(kind));
	      int varName = si.chooseFreshVariableName("A");

	      Vector<Term*> kindVariable(1);
	      kindVariable[0] = new VariableTerm(vs, varName);
	      Term* newAttributeArgument = at.first->makeTerm(kindVariable);
	      patternAttributes.insert({at.first, newAttributeArgument});
	      changed = true;
	    }
	}
    }
  if (!changed)
    return false;
  //
  //	We created an attribute set variable and/or added an attribute to the  pattern attribute set.
  //	We need to replace the attribute set argument in the pattern occurrence.
  //
  Term* newAttributeSet = po.variableTerm;
  if (!patternAttributes.empty())
    {
      Vector<Term*> arguments;
      for (auto& at : patternAttributes)
	arguments.append(at.second);
      arguments.append(po.variableTerm);
      newAttributeSet = attributeSetSymbol->makeTerm(arguments);
    }
  //
  //	Surgically replace old attribute set argument.
  //
  DebugInfo("doing surgery on " << po.objectTerm);
  FreeTerm* freeTerm = safeCastNonNull<FreeTerm*>(po.objectTerm);
  DebugInfo("newAttributeSet = " << newAttributeSet);
  Term* oldAttributeSet = freeTerm->replaceArgument(2, newAttributeSet);
  DebugInfo("oldAttributeSet = " << oldAttributeSet);
  garbageCollectAttributeSet(oldAttributeSet, attributeSetSymbol);
  return true;
}

bool
SyntacticPreModule::transformSubjectAttributes(ObjectOccurrence& so, ObjectOccurrence& po)
{
  //
  //	First we accumulate copies of any missing attributes.
  //
  Vector<Term*> arguments;
  for (auto& at : po.attributeTerms)
    {
      if (so.attributeTerms.find(at.first) == so.attributeTerms.end())
	  arguments.append(at.second->deepCopy());
    }
  //
  //	If there weren't any and we already have an attribute set variable, we're good.
  //
  if (arguments.empty() && so.variableTerm != 0)
    return false;
  //
  //	We going to transform the argument list; we will need attributeSetSymbol
  //	if only to find the unused parts of the old attribute set.
  //
  ObjectConstructorSymbol* os = safeCastNonNull<ObjectConstructorSymbol*>(so.objectTerm->symbol());
  ACU_Symbol* attributeSetSymbol = os->getAttributeSetSymbol();
  //
  //	Copy in any existing attributes.
  //
  for (auto& at :  so.attributeTerms)
    arguments.append(at.second);
  //
  //	Make a new variable, if needed. This could be our only item in the attribute set.
  //
  Term* newAttributeSet = (so.variableTerm == 0) ? po.variableTerm->deepCopy() : so.variableTerm;
  if (!arguments.empty())
    {
      //
      //	If we have attribute terms, we stick the variable on the end and make a set.
      //
      arguments.append(newAttributeSet);
      newAttributeSet = attributeSetSymbol->makeTerm(arguments);
    }
  //
  //	Surgically replace old attribute set argument.
  //
  DebugInfo("doing surgery on " << so.objectTerm);
  FreeTerm* freeTerm = safeCastNonNull<FreeTerm*>(so.objectTerm);
  DebugInfo("newAttributeSet = " << newAttributeSet);
  Term* oldAttributeSet = freeTerm->replaceArgument(2, newAttributeSet);
  DebugInfo("oldAttributeSet = " << oldAttributeSet);
  garbageCollectAttributeSet(oldAttributeSet, attributeSetSymbol);
  return true;
}

void
SyntacticPreModule::garbageCollectAttributeSet(Term* attributeSet, Symbol* attributeSetSymbol) const
{
  DebugEnter("attributeSet = " << attributeSet);
  //
  //	We want to delete the parts of the old attribute set that were not reused in the
  //	new attribute set. These should only be attributeSet constructors and identities.
  //
  Symbol* as = attributeSet->symbol();
  if (as == attributeSetSymbol)
    {
      for (ArgumentIterator i(*attributeSet); i.valid(); i.next())
	garbageCollectAttributeSet(i.argument(), attributeSetSymbol);
      //
      //	Now do a shallow delete.
      //
      delete attributeSet;
      return;
    }
  if (attributeSymbols.find(as) != attributeSymbols.end())
    return;  // assume we reused the attribute subterm
  if (dynamic_cast<VariableTerm*>(attributeSet))
   return;  // assume we reused the variable subterm
  //
  //	The only other legal subterm is the identity (which could be denormalized).
  //
  attributeSet->deepSelfDestruct();
}
