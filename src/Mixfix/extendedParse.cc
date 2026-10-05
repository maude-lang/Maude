/*

    This file is part of the Maude 3 interpreter.

    Copyright 2026 SRI International, Menlo Park, CA 94025, USA.

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

void
MixfixParser::makeOtfTranslations()
{
  otfTranslations.clear();
  //
  //	We scan through the original tokens, looking for things that
  //	might be on-the-fly variables of known sort, and make these
  //	into otf translations.
  //
  Index beyondEnd = currentOffset + sentence.size();
  for (Index i = currentOffset; i < beyondEnd; ++i)
    {
      int code = (*currentSentence)[i].code();
      int sp = Token::specialProperty(code);
      if (sp == Token::CONTAINS_COLON)
	{
	  //
	  //	Token looks like X:Bar so it may be an otf
	  //	variable of sort Bar or an otf variable of
	  //	sort Bar{...}...{...}
	  //
	  bool uncertain = (tokenSet.find(code) != NONE) ||
	    (otfTranslations.find(code) != otfTranslations.end());  // maybe X:Bar has some other meaning
	  int varName;
	  int sortName;
	  Token::split(code, varName, sortName);
	  //
	  //	We need to check
	  //	  Bar
	  //	  Bar{...}
	  //	  Bar{...}{...}
	  //	  ...
	  //	If more than one is a valid sort we are uncertain about all translations,
	  //	however, we don't need to flag the first one, because the uncertainty only
	  //	starts when the second one becomes active. In X:Bar{X} for example, in any
	  //	parse where the second X parses as the extended scope of X:Bar, if X:Bar
	  //	didn't have any other meaning then X:Bar must be an otf variable, or we
	  //	would have a parse error before reaching the second X.
	  //
	  if (Sort* sort = client.findSort(sortName))
	    {
	      //
	      //	Deal with Bar case.
	      //
	      makeOtfTranslation(varName, i, sort->getIndexWithinModule(), uncertain);
	      uncertain = true;  // any addition valid sort flagged as uncertain
	    }
	  //
	  //	Look for Bar {...}...{...}
	  //
	  Vector<Token> structuredSortName;
	  Index start = i + 1;
	  for (;;)
	    {
	      Index last = SharedTokens::skipBracePair(*currentSentence, start, beyondEnd);
	      if (last == NONE)
		break;
	      //
	      //	Saw legal syntax for {...} in parameterized sort.
	      //
	      structuredSortName.resize(last - i + 1);
	      structuredSortName[0].tokenize(sortName, (*currentSentence)[i].lineNumber());
	      for (Index j = start; j <= last; ++j)
		structuredSortName[j - i] = (*currentSentence)[j];
	      int structuredSortCode = Token::bubbleToPrefixNameCode(structuredSortName);

	      if (Sort* sort = client.findSort(structuredSortCode))
		{
		  makeOtfTranslation(varName, last, sort->getIndexWithinModule(), uncertain);
		  uncertain = true;  // any additional valid sort flagged as uncertain
		}
	      start = last + 1;
	    }
	}
      else if (sp == Token::ENDS_IN_COLON)
	{
	  //
	  //	Token looks like X: so it may be an otf variable of kind type.
	  //
	  int varName;
	  int sortName;
	  Token::split(code, varName, sortName);
	  Vector<int> sortNames;
	  Index last = SharedTokens::skipKindName(*currentSentence,
						  i + 1,
						  beyondEnd,
						  sortNames);
	  if (last != NONE)
	    {
	      if (ConnectedComponent* component = checkSortNames(sortNames))
		{
		  Sort* kind = component->sort(Sort::KIND);
		  bool uncertain = (tokenSet.find(code) != NONE) ||
		    (otfTranslations.find(code) != otfTranslations.end());
		  makeOtfTranslation(varName, last, kind->getIndexWithinModule(), uncertain);
		}
	    }
	}
    }
}

void
MixfixParser::makeOtfTranslation(int varName, int location, int sortIndex, bool uncertain)
{
  const MixfixModule::AliasMap& aliasMap = client.getVariableAliases();
  MixfixModule::AliasMap::const_iterator p = aliasMap.find(varName);
  if (p != aliasMap.end())
    {
      if (p->second->getIndexWithinModule() == sortIndex)
	{
	  //
	  //	Translation would duplicate a variable alias mapping and
	  //	produce false ambiguity.
	  //
	  return;
	}
    }
  
  Vector<OtfDef>& translations = otfTranslations[varName];
  for (const OtfDef& d : translations)
    {
      if (d.sortIndex == sortIndex)
	return;  // saw the same definition earlier
    }
  translations.push_back({location, sortIndex, uncertain});
}

ConnectedComponent* 
MixfixParser::checkSortNames(const Vector<int>& sortNames)
{
  //
  //	Check that sorts exist and are all in the same connected component.
  //
  ConnectedComponent* component = nullptr;
  for (int sortName : sortNames)
    {
      Sort* s = client.findSort(sortName);
      if (s == nullptr)
	return nullptr;
      ConnectedComponent* c = s->component();
      if (component == nullptr)
	component = c;
      else if (component != c)
	return nullptr;
    }
  return component;
}

int
MixfixParser::extendedParse(int root, int& firstBad, int nrTokens)
{
  //
  //	Parse using wildcards and extended scope for otf variables.
  //	We assume that all tokens < firstBad have their classic translation
  //	store in sentence.
  //
  //	Return value is:
  //	  -1 : bad token at position firstBad
  //	  0  : tokens good, but parse fails at or before position firstBad
  //	  1  : exactly one parse
  //	  2  : two or more parses - ambiguous
  //
  //	Translate tokens into terminals.
  //
  makeOtfTranslations();
  terminalLists.clear();
  //
  //	Wildcard variable names are fresh with respect to sentence.
  //
  usedNames.clear();
  //
  //	Keep track if we actually use extended translations.
  //
  bool usedExtendedTranslation = false;
  //
  //	Translate tokens into terminals.
  //
  Vector<int> translations;
  for (Index i = 0; i < nrTokens; ++i)
    {
      translations.clear();
      Index j = currentOffset + i;
      int code = (*currentSentence)[j].code();
      //
      //	First do standard translation.
      //
      if (j < firstBad)
	{
	  //
	  //	We already translated this token.
	  //
	  translations.push_back(sentence[i]);
	}
      else
	{
	  int terminal = tokenSet.find(code);
	  if (terminal == NONE)
	    terminal = translateSpecialToken(code);
	  if (terminal != NONE)
	    translations.push_back(terminal);
	}
      //
      //	Then check for otf variable extended scope translations.
      //
      auto t = otfTranslations.find(code);
      if (t != otfTranslations.end())
	{
	  for (const OtfDef& d : t->second)
	    {
	      if (d.location < i)
		{
		  int otfTerminal = bareOtfVariableTerminals[d.sortIndex];
		  translations.push_back(otfTerminal);
		  usedExtendedTranslation = true;
		  if (d.uncertain)
		    {
		      if (!inexact)
			{
			  inexact = true;
			  Verbose("Inexact parsing triggered by:");
			}
		      Verbose("  " << Token::name(code) << ":" << client.getSorts()[d.sortIndex]);
		    }
		}
	      //
	      //	Translations aren't guaranteed to be in order so
	      //	we need to check them all.
	      //
	    }
	}
      //
      //	Check if we have one or more translations.
      //
      Index nrTranslations = translations.size();
      if (nrTranslations == 1)
	sentence[i] = translations[0];
      else if (nrTranslations > 1)
	{
	  sentence[i] = Parser::flip(terminalLists.size());
	  terminalLists.push_back(std::move(translations));
	}
      else
	{
	  //
	  //	We can't use Token::specialProperty() for starting with '_' because
	  //	is could coincide with other special properties; e.g. _X:Foo
	  //	We only recognize a wildcard if there is no other translation.
	  //	We limit named wildcards to tokens that are valid view names to
	  //	rule out edge cases.
	  //
	  if (wildcardTerminal != NONE &&
	      Token::name(code)[0] == '_' &&
	      Token::isValidViewName(code))
	    {
	      sentence[i] = wildcardTerminal;
	      usedExtendedTranslation = true;
	    }
	  else
	    {
	      firstBad = j;
	      return -1;  // bad token
	    }
	}
      if (j == firstBad && !usedExtendedTranslation)
	{
	  //
	  //	Extended translation won't resolve the failure to parse
	  //	in the classic translation. Because we didn't encounter
	  //	a bad token at firstBad, we know the classic parse attempt
	  //	must have failed in the CFG parser and not the
	  //	token->terminal translation, so we can just return 0 parses,
	  //	leaving the firstBad location unchanged.
	  //
	  nrParses = 0;
	  return 0;
	}
    }

#if PARSER_DEBUG
  cout << "extended parse: ";
  for (int i = 0; i < sentence.length(); i++)
    cout << sentence[i] << ' ';
  cout << ", " << root << '\n';
#endif

  nrParses = parser.parseSentence(sentence, root, terminalLists);
  DebugAdvisoryCheck(nrParses == 1, "New parser returned " << nrParses << " parses");
  if (nrParses == 0)  // no parse
    {
      int newFirstBad = currentOffset + parser.getErrorPosition();
      if (newFirstBad > firstBad)
	{
	  //
	  //	We got further, but that maybe due to inexactness.
	  //
	  if (inexact)
	    {
	      Verbose("Reported error may not be the earliest due to inexact parsing.");
	      firstBad = newFirstBad;
	    }
	}
    }
  else if (inexact)
    {
      //cerr << Tty(Tty::BLUE) << "inexact parse" << Tty(Tty::RESET) << endl;
      //cerr << "nrParses = " << nrParses << endl;
      firstBad = -1;
      if (nrParses == 1)
	{
	  if (!checkParse(firstBad))
	    {
	      nrParses = 0;  // no consistent parses
	    }
	}
      else
	{
	  //
	  //	Ambiguous parse in bigger grammar.
	  //	Find first consistent parse.
	  //
	  while (!checkParse(firstBad))
	    {
	      //cerr << Tty(Tty::BLUE) << "parse failed check" << Tty(Tty::RESET) << endl;
	      if (!parser.extractNextParse())
		{
		  nrParses = 0;  // no consistent parses
		  return nrParses;
		}
	    }
	  //cerr << "found first consistent" << endl;
	  //
	  //	Save our consistent parse and see if there is another one for
	  //	ambiguity in the smaller grammar.
	  //
	  parser.saveParse();
	  while (parser.extractNextParse())
	    {
	      if (checkParse(firstBad))
		{
		  //
		  //	Found a second consistent parse.
		  //	Swap it with the first one.
		  //
		  parser.swapParse();
		  nrParses = 2;  // ambiguous
		  //cerr << "found second consistent" << endl;
		  return nrParses;
		}
	    }
	  //cerr << "no more consistent" << endl;
	  parser.swapParse();  // restore consistent parse
	  nrParses = 1;  // only found a single consistent parse.
	}
    }
  return nrParses;
}

bool
MixfixParser::checkParse(int& firstBad)
{
  seenSet.clear();
  if (!checkSubparse(ROOT_NODE, firstBad))
    return false;
  return true;
}

bool
MixfixParser::checkSubparse(int node, int& firstBad)
{
  Action& a = actions[parser.getProductionNumber(node)];
  switch (a.action)
    {
    case MAKE_OTF_VARIABLE_KNOWN_SORT:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	int baseName;
	int sortName;
	Token::split(varName, baseName, sortName);
	seenSet.insert({baseName, a.data});
	break;
      }
     case MAKE_OTF_VARIABLE:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	int baseName;
	int sortName;
	Token::split(varName, baseName, sortName);
	Sort* sort = client.findSort(sortName);
	Assert(sort != nullptr, "didn't find sort for " << Token::name(sortName));
	seenSet.insert({baseName, sort->getIndexWithinModule()});
	break;	
      }
    case MAKE_BARE_OTF_VARIABLE:
      {
	int pos = currentOffset + parser.getFirstPosition(node);
	int varName = (*currentSentence)[pos].code();
	if (seenSet.find({varName, a.data}) == seenSet.end())
	  {
	    if (pos > firstBad)
	      firstBad = pos;  // got further
	    return false;
	  }
	break;
      }
    default:
      {
	int nrChildren = parser.getNumberOfChildren(node);
	for (int i = 0; i < nrChildren; ++i)
	  {
	    if (!checkSubparse(parser.getChild(node, i), firstBad))
	      return false;
	  }
      }
    }
  return true;
}
