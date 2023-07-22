/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
//	The descent functions supported by MetaLevelOpSymbol.
//

bool
MetaLevelOpSymbol::metaSortLeq(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downType(subject->getArgument(1), m, s1) &&
	  metaLevel->downType(subject->getArgument(2), m, s2))
	{
	  return context.builtInReplace(subject,
					metaLevel->upBool(s1->component() == s2->component()
							  && leq(s1, s2)));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaWellFormedModule(FreeDagNode* subject, RewritingContext& context)
{
  MixfixModule* m = metaLevel->downModule(subject->getArgument(0));
  return context.builtInReplace(subject, metaLevel->upBool(m != 0));
}

bool
MetaLevelOpSymbol::metaWellFormedTerm(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      bool result = false;
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  result = true;
	  t->deepSelfDestruct();
	}
      return context.builtInReplace(subject, metaLevel->upBool(result));
    }
  return false;
}

bool
MetaLevelOpSymbol::metaWellFormedSubstitution(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      bool result = false;
      Vector<Term*> variables;
      Vector<Term*> values;
      if (metaLevel->downSubstitution(subject->getArgument(1), m, variables, values))
	{
	  Vector<DagRoot*> dags;
	  m->protect();
	  if (MetaLevel::dagifySubstitution(variables, values, dags, context))
	    {
	      result = true;
	      for (int i = variables.length() - 1; i >= 0; i--)
		delete dags[i];
	    }
	  (void) m->unprotect();
	  for (int i = variables.length() - 1; i >= 0; i--)
	    {
	      variables[i]->deepSelfDestruct();
	      values[i]->deepSelfDestruct();
	    }
	}
      return context.builtInReplace(subject, metaLevel->upBool(result));
    }
  return false;
}

RewritingContext*
MetaLevelOpSymbol::term2RewritingContext(Term* term, RewritingContext& context)
{
  term = term->normalize(false);
  DagNode* d = term->term2DagEagerLazyAware();
  term->deepSelfDestruct();
  return context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
}

bool
MetaLevelOpSymbol::metaSameKind(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downType(subject->getArgument(1), m, s1) &&
	  metaLevel->downType(subject->getArgument(2), m, s2))
	{
	  bool result = s1->component() == s2->component();
	  return context.builtInReplace(subject, metaLevel->upBool(result));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaLeastSort(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  t = t->normalize(false);
	  DagNode* d = t->term2DagEagerLazyAware();
	  t->deepSelfDestruct();
	  RewritingContext* objectContext =
	    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	  m->protect();
	  d->computeTrueSort(*objectContext);
	  context.addInCount(*objectContext);
	  PointerMap qidMap;
	  d = metaLevel->upType(d->getSort(), qidMap);
	  delete objectContext;
	  (void) m->unprotect();
	  return context.builtInReplace(subject, d);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaMaximalAritySet(FreeDagNode* subject, RewritingContext& context)
{
  //
  //  maximalAritySet : Module Qid TypeList Sort ~> TypeListSet
  //
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      int id;
      Vector<Sort*> arity;
      Sort* target;
      if (metaLevel->downOpName(subject->getArgument(1), id) &&
	  metaLevel->downTypeList(subject->getArgument(2), m, arity) &&
	  metaLevel->downSimpleSort(subject->getArgument(3), m, target))
	{
	  int nrArgs = arity.size();
	  Vector<ConnectedComponent*> domain(nrArgs);
	  for (int i = 0; i < nrArgs; i++)
	    domain[i] = arity[i]->component();
	  if (Symbol* s = m->findSymbol(id, domain, target->component()))
	    {
	      PointerMap qidMap;
	      return context.builtInReplace(subject,
					    metaLevel->upTypeListSet(s->getOpDeclarations(),
								     s->getMaximalOpDeclSet(target),
								     qidMap));
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaCompleteName(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downType(subject->getArgument(1), m, s))
	{
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upType(s, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaLesserSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downType(subject->getArgument(1), m, s))
	{
	  ConnectedComponent* component = s->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> lesserSorts;
	  for (int i = s->index() + 1; i < nrSorts; i++)
	    {
	      Sort* s2 = component->sort(i);
	      if (leq(s2, s))
		lesserSorts.append(s2);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(lesserSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaMaximalSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* k;
      if (metaLevel->downType(subject->getArgument(1), m, k) &&
	  k->index() == Sort::KIND)
	{
	  ConnectedComponent* component = k->component();
	  int nrMaximalSorts = component->nrMaximalSorts();
	  Vector<Sort*> maximalSorts(nrMaximalSorts);
	  for (int i = 1; i <= nrMaximalSorts; i++)
	    maximalSorts[i - 1] = component->sort(i);
	  return context.builtInReplace(subject, metaLevel->upSortSet(maximalSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaMinimalSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* k;
      if (metaLevel->downType(subject->getArgument(1), m, k) &&
	  k->index() == Sort::KIND)
	{
	  ConnectedComponent* component = k->component();
	  int nrSorts = component->nrSorts();
	  Vector<Sort*> minimalSorts;
	  for (int i = 1; i < nrSorts; i++)
	    {
	      Sort* s = component->sort(i);
	      if (s->getSubsorts().empty())
		minimalSorts.append(s);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(minimalSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGlbSorts(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s1;
      Sort* s2;
      if (metaLevel->downType(subject->getArgument(1), m, s1) &&
	  metaLevel->downType(subject->getArgument(2), m, s2))
	{
	  Vector<Sort*> glbSorts;
	  ConnectedComponent* component = s1->component();
	  if (s2->component() == component)
	    {
	      NatSet leqSorts(s1->getLeqSorts());
	      leqSorts.intersect(s2->getLeqSorts());
	      component->findMaximalSorts(leqSorts, glbSorts);
	    }
	  return context.builtInReplace(subject, metaLevel->upSortSet(glbSorts));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetKind(FreeDagNode* subject, RewritingContext& context)
{
  if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Sort* s;
      if (metaLevel->downType(subject->getArgument(1), m, s))
	{
	  Sort* kind = s->component()->sort(Sort::ERROR_SORT);
	  PointerMap qidMap;
	  return context.builtInReplace(subject, metaLevel->upType(kind, qidMap));
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetKinds(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = dynamic_cast<ImportModule*>(metaLevel->downModule(subject->getArgument(0))))
    return context.builtInReplace(subject,
				  metaLevel->upKindSet(m->getConnectedComponents(),
						       m->getNrUserComponents()));
  return false;
}

bool
MetaLevelOpSymbol::metaReduce(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  t = t->normalize(false);
	  DagNode* d = t->term2DagEagerLazyAware();
	  t->deepSelfDestruct();
	  RewritingContext* objectContext =
	    context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	  m->protect();
	  objectContext->reduce();
	  context.addInCount(*objectContext);
	  d = metaLevel->upResultPair(objectContext->root(), m);
	  delete objectContext;
	  (void) m->unprotect();
	  return context.builtInReplace(subject, d);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaNormalize(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  t = t->normalize(true);
	  t->symbol()->fillInSortInfo(t);
	  DagNode* d = metaLevel->upResultPair(t, m);
	  t->deepSelfDestruct();
	  return context.builtInReplace(subject, d);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaRewrite(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 limit;
      if (metaLevel->downBound64(subject->getArgument(2), limit) && limit != 0)
	{
	  if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	    {
	      t = t->normalize(false);
	      DagNode* d = t->term2DagEagerLazyAware();
	      t->deepSelfDestruct();
	      RewritingContext* objectContext =
		  context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	      m->protect();
	      m->resetRules();
	      objectContext->ruleRewrite(limit);
	      context.addInCount(*objectContext);
	      d = metaLevel->upResultPair(objectContext->root(), m);
	      delete objectContext;
	      (void) m->unprotect();
	      return context.builtInReplace(subject, d);
	    }
	  else
	    {
	      DebugAdvisory("bad metaterm " << QUOTE(subject->getArgument(1)));
	    }
	}
      else
	{
	  DebugAdvisory("bad bound " << QUOTE(subject->getArgument(2)));
	}
    }
  else
    {
      DebugAdvisory("bad metamodule " << QUOTE(subject->getArgument(0)));
    }
  return false;
}

bool
MetaLevelOpSymbol::metaFrewrite(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 limit;
      Int64 gas;
      if (metaLevel->downBound64(subject->getArgument(2), limit) && limit != 0 &&
	  metaLevel->downSaturate64(subject->getArgument(3), gas) && gas != 0)
	{
	  if (Term* t = metaLevel->downTerm(subject->getArgument(1), m))
	    {
	      t = t->normalize(false);
	      DagNode* d = t->term2DagEagerLazyAware();
	      t->deepSelfDestruct();
	      RewritingContext* objectContext =
		context.makeSubcontext(d, UserLevelRewritingContext::META_EVAL);
	      safeCast(ObjectSystemRewritingContext*, objectContext)->
		setObjectMode(ObjectSystemRewritingContext::FAIR);
	      m->protect();
	      m->resetRules();
	      objectContext->fairRewrite(limit, gas);
	      objectContext->root()->computeTrueSort(*objectContext);
	      context.addInCount(*objectContext);
	      d = metaLevel->upResultPair(objectContext->root(), m);
	      delete objectContext;
	      (void) m->unprotect();
	      return context.builtInReplace(subject, d);
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaParse(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	  op metaParse : Module VariableSet QidList Type? ~> ResultPair?
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      ConnectedComponent* component;
      Vector<int> metaTokens;
      if (metaLevel->downComponent(subject->getArgument(3), m, component) &&
	  metaLevel->downQidList(subject->getArgument(2), metaTokens))
	{
	  AliasMapParserPair* pair;
	  CacheableState* cachedPair;
	  Int64 dummy;
	  if (m->remove(subject, cachedPair, dummy, 2 /* ignore last two arguments */))
	    {
	      DebugInfo("alias-map/parser cache hit for " << subject);
	      pair = safeCast(AliasMapParserPair*, cachedPair);
	    }
	  else
	    {
	      //
	      //	We don't have a cached (AliasMap, MixfixParser) pair
	      //	for (Module, VariableSet) so we need to make one.
	      //
	      //	We don't have special treatment for the empty AliasMap.
	      //	MixfixParsers are only made by makeGrammar() in response
	      //	to a parse request so we don't expect to have a default
	      //	parser that we could use.
	      //
	      pair = new AliasMapParserPair();  // empty map, null parser
	      if (!(metaLevel->downVariableDeclSet(subject->getArgument(1), pair->aliasMap, m)))
		{
		  delete pair;
		  return false;
		}
	    }
	  //
	  //	Swap our (AliasMap, MixfixParser) pair with the AliasMap
	  //	and Parser in m.
	  //	
	  m->swapVariableAliasMap(pair->aliasMap, pair->parser);
	  //
	  //	Do the parse.
	  //
	  int nrTokens = metaTokens.length();
	  Vector<Token> tokens(nrTokens);
	  for (int i = 0; i < nrTokens; i++)
	    tokens[i].tokenize(metaTokens[i], FileTable::META_LEVEL_CREATED);
	  Term* parse1;
	  Term* parse2;
	  int firstBad;
	  int result = m->parseTerm2(tokens, component, parse1, parse2, firstBad);
	  //
	  //	Restore AliasMaps and Parsers.
	  //
	  m->swapVariableAliasMap(pair->aliasMap, pair->parser);
	  //
	  //	Cache the AliasMap and Parser we just used.
	  //
	  m->insert(subject, pair, 0 /* dummy value */);
	  //
	  //	Handle the result from the parser.
	  //
	  switch(result)
	    {
	    case -1: 	// bad token
	    case 0:	// no parse
	      {
		return context.builtInReplace(subject, metaLevel->upNoParse(firstBad));
	      }
	    case 1:	// 1 parse
	      {
		parse1->symbol()->fillInSortInfo(parse1);
		DagNode* d = metaLevel->upResultPair(parse1, m);
		parse1->deepSelfDestruct();
		return context.builtInReplace(subject, d);
	      }
	    default:	// more than 1 parse
	      {
		parse1->symbol()->fillInSortInfo(parse1);
		parse2->symbol()->fillInSortInfo(parse2);
		DagNode* d = metaLevel->upAmbiguity(parse1, parse2, m);
		parse1->deepSelfDestruct();
		parse2->deepSelfDestruct();
		return context.builtInReplace(subject, d);
	      }
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaPrettyPrint(FreeDagNode* subject, RewritingContext& context)
{
  int printFlags;
  if (metaLevel->downPrintOptionSet(subject->getArgument(3), printFlags))
    {
      if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
	{
	  MixfixModule::AliasMap aliasMap;
	  if (metaLevel->downVariableDeclSet(subject->getArgument(1), aliasMap, m))
	    {
	      if (Term* t = metaLevel->downTerm(subject->getArgument(2), m))
		{
		  //
		  //	Swap in our alias map and a null parser.
		  //
		  MixfixParser* parser = 0;
		  m->swapVariableAliasMap(aliasMap, parser);
		  //
		  //	Do the pretty print.
		  //
		  Vector<int> buffer;
		  m->bufferPrint(buffer, t, printFlags);
		  //
		  //	Restore original alias map and parser.
		  //
		  m->swapVariableAliasMap(aliasMap, parser);
		  
		  t->deepSelfDestruct();
		  return context.builtInReplace(subject, metaLevel->upQidList(buffer));
		}
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaUpTerm(FreeDagNode* subject, RewritingContext& context)
{
  MixfixModule* m = safeCast(MixfixModule*, getModule());
  PointerMap qidMap;
  PointerMap dagNodeMap;
  DagNode* d = metaLevel->upDagNode(subject->getArgument(0), m, qidMap, dagNodeMap);
  return context.builtInReplace(subject, d);
}

bool
MetaLevelOpSymbol::metaDownTerm(FreeDagNode* subject, RewritingContext& context)
{
  MixfixModule* m = safeCast(MixfixModule*, getModule());
  DagNode* mt = subject->getArgument(0);
  DagNode* d = subject->getArgument(1);
  if (Term* t = metaLevel->downTerm(mt, m))
    {
      ConnectedComponent* tc = t->symbol()->rangeComponent();
      ConnectedComponent* wc = d->symbol()->rangeComponent();
      if (tc == wc)
	{
	  t = t->normalize(false);
	  d = t->term2DagEagerLazyAware();
	}
      else
	{
	  IssueAdvisory("attempt to reflect down meta-term " << mt << " yielded term " << t << " in kind " << tc <<
			" whereas a term in kind " << wc << " was expected.");
	}
      t->deepSelfDestruct();
    }
  return context.builtInReplace(subject, d);
}

bool
MetaLevelOpSymbol::metaParseStrategy(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	  op metaParseStrategy : Module VariableSet QidList Type? ~> Strategy?
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Vector<int> metaTokens;
      if (metaLevel->downQidList(subject->getArgument(2), metaTokens))
	{
	  AliasMapParserPair* pair;
	  CacheableState* cachedPair;
	  Int64 dummy;
	  if (m->remove(subject, cachedPair, dummy, 1 /* ignore last argument */))
	    {
	      DebugInfo("cache hit for " << subject);
	      pair = safeCast(AliasMapParserPair*, cachedPair);
	    }
	  else
	    {
	      //
	      //	We don't have a cached (AliasMap, MixfixParser) pair
	      //	for (Module, VariableSet) so we need to make one.
	      //
	      //	We don't have special treatment for the empty AliasMap.
	      //	MixfixParsers are only made by makeGrammar() in response
	      //	to a parse request so we don't expect to have a default
	      //	parser that we could use.
	      //
	      pair = new AliasMapParserPair();  // empty map, null parser
	      if (!(metaLevel->downVariableDeclSet(subject->getArgument(1), pair->aliasMap, m)))
		{
		  delete pair;
		  return false;
		}
	    }
	  //
	  //	Swap our (AliasMap, MixfixParser) pair with the AliasMap
	  //	and Parser in m.
	  //
	  m->swapVariableAliasMap(pair->aliasMap, pair->parser);

	  int nrTokens = metaTokens.length();
	  Vector<Token> tokens(nrTokens);
	  for (int i = 0; i < nrTokens; i++)
	    tokens[i].tokenize(metaTokens[i], FileTable::META_LEVEL_CREATED);

	  StrategyExpression* parse1;
	  StrategyExpression* parse2;
	  int firstBad;
	  int ok = m->parseStrategyExpr2(tokens, parse1, parse2, firstBad);
	  //
	  //	Restore AliasMaps and Parsers.
	  //
	  m->swapVariableAliasMap(pair->aliasMap, pair->parser);
	  //
	  //	Cache the AliasMap and Parser we just used.
	  //
	  m->insert(subject, pair, 0 /* dummy value */);
	  //
	  //	Handle the result from the parser.
	  //
	  switch(ok)
	    {
	    case -1: 	// bad token
	    case 0:	// no parse
	      {
		return context.builtInReplace(subject, metaLevel->upNoParse(firstBad, true));
	      }
	    case 1:	// 1 parse
	      {
		// Strategies are not checked here

		DagNode* d = metaLevel->upStratExpr(parse1, m);
		delete parse1;
		return context.builtInReplace(subject, d);
	      }
	    default:	// more than 1 parse
	      {
		DagNode* d = metaLevel->upAmbiguity(parse1, parse2, m);
		delete parse1;
		delete parse2;
		return context.builtInReplace(subject, d);
	      }
	    }
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaPrettyPrintStrategy(FreeDagNode* subject, RewritingContext& context)
{
  int printFlags;
  if (metaLevel->downPrintOptionSet(subject->getArgument(3), printFlags))
    {
      if (MixfixModule* m = metaLevel->downModule(subject->getArgument(0)))
	{
	  MixfixModule::AliasMap aliasMap;
	  if (metaLevel->downVariableDeclSet(subject->getArgument(1), aliasMap, m))
	    {
	      if (StrategyExpression* e = metaLevel->downStratExpr(subject->getArgument(2), m))
		{
		  //
		  //	Swap in our alias map and a null parser.
		  //
		  MixfixParser* parser = 0;
		  m->swapVariableAliasMap(aliasMap, parser);
		  //
		  //	Do the pretty print.
		  //
		  Vector<int> buffer;
		  m->bufferPrint(buffer, e, printFlags);
		  //
		  //	Restore original alias map and parser.
		  //
		  m->swapVariableAliasMap(aliasMap, parser);
		  delete e;
		  return context.builtInReplace(subject, metaLevel->upQidList(buffer));
		}
	    }
	}
    }
  return false;
}

#include "SMT_Info.hh"
#include "variableGenerator.hh"

bool
MetaLevelOpSymbol::metaCheck(FreeDagNode* subject, RewritingContext& context)
{
  if (ImportModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      if (Term* term = metaLevel->downTerm(subject->getArgument(1), m))
	{
	  m->protect();
	  term = term->normalize(false);
	  DagNode* d = term->term2Dag();

	  const SMT_Info& smtInfo = m->getSMT_Info();
	  VariableGenerator vg(smtInfo);
	  VariableGenerator::Result result = vg.checkDag(d);
	  switch (result)
	    {
	    case VariableGenerator::BAD_DAG:
	      {
		IssueAdvisory("term " << QUOTE(term) << " is not a valid SMT Boolean expression.");
		break;
	      }
	    case VariableGenerator::SAT_UNKNOWN:
	      {
		IssueAdvisory("sat solver could not determined satisfiability of " << QUOTE(term) << ".");
		break;
	      }
	    case VariableGenerator::UNSAT:
	    case VariableGenerator::SAT:
	      {
		DagNode* r = metaLevel->upBool(result == VariableGenerator::SAT);
		term->deepSelfDestruct();
		(void) m->unprotect();
		return context.builtInReplace(subject, r);
	      }
	    }

	  term->deepSelfDestruct();
	  (void) m->unprotect();
	}
    }
  return false;
}
