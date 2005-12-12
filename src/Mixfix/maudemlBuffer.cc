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
//      Implementation for class MaudemlBuffer.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"
#include "pointerSet.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "variable.hh"
#include "NA_Theory.hh"
#include "S_Theory.hh"
#include "higher.hh"
#include "mixfix.hh"

//      interface class definitions
#include "symbol.hh"
#include "dagNode.hh"
#include "term.hh"

//      core class definitions
#include "argumentIterator.hh"
#include "dagArgumentIterator.hh"
#include "sort.hh"
#include "connectedComponent.hh"
#include "equation.hh"
#include "rule.hh"
#include "sortConstraint.hh"
#include "conditionFragment.hh"
#include "module.hh"

//	variable class definitions
#include "variableSymbol.hh"
#include "variableTerm.hh"
#include "variableDagNode.hh"

// S theory class definitions
#include "S_Symbol.hh"
#include "S_Term.hh"
#include "S_DagNode.hh"

//	builtin class definitions
//#include "sortTestSymbol.hh"
#include "floatSymbol.hh"
#include "floatTerm.hh"
#include "floatDagNode.hh"
#include "stringSymbol.hh"
#include "stringTerm.hh"
#include "stringDagNode.hh"

//	higher class definitions
#include "equalityConditionFragment.hh"
#include "sortTestConditionFragment.hh"
#include "assignmentConditionFragment.hh"
#include "rewriteConditionFragment.hh"
#include "rewriteSequenceSearch.hh"
#include "pattern.hh"

//	front end class definitions
#include "token.hh"
#include "timer.hh"
#include "quotedIdentifierSymbol.hh"
#include "quotedIdentifierTerm.hh"
#include "quotedIdentifierDagNode.hh"
#include "maudemlBuffer.hh"

MaudemlBuffer::MaudemlBuffer(ostream& output)
  : XmlBuffer(output, 1)
{
  beginElement("maudeml");
}

void
MaudemlBuffer::generateShowSearchPath(int stateNr)
{
  beginElement("show");
  attributePair("item", "search path");
  attributePair("state-number", int64ToString(stateNr));
  endElement();
}
void
MaudemlBuffer::generateShowSearchGraph()
{
  beginElement("show");
  attributePair("item", "search graph");
  endElement();
}

void
MaudemlBuffer::generateReduce(DagNode* subject)
{
  beginElement("reduce");
  attributePair("module", Token::name(subject->symbol()->getModule()->id()));
  generate(subject);
  endElement();
}

void
MaudemlBuffer::generateRewrite(DagNode* subject, Int64 limit)
{
  beginElement("rewrite");
  attributePair("module", Token::name(subject->symbol()->getModule()->id()));
  if (limit != NONE)
    attributePair("limit", int64ToString(limit));
  generate(subject);
  endElement();
}

void
MaudemlBuffer::generateFrewrite(DagNode* subject, Int64 limit, Int64 gas)
{
  beginElement("frewrite");
  attributePair("module", Token::name(subject->symbol()->getModule()->id()));
  if (limit != NONE)
    attributePair("limit", int64ToString(limit));
  if (limit != NONE)
    attributePair("gas", int64ToString(gas));
  generate(subject);
  endElement();
}

void
MaudemlBuffer::generateErewrite(DagNode* subject, Int64 limit, Int64 gas)
{
  beginElement("erewrite");
  attributePair("module", Token::name(subject->symbol()->getModule()->id()));
  if (limit != NONE)
    attributePair("limit", int64ToString(limit));
  if (limit != NONE)
    attributePair("gas", int64ToString(gas));
  generate(subject);
  endElement();
}

void
MaudemlBuffer::generateSearch(DagNode* subject,
			      PreEquation* pattern,
			      const string& searchType,
			      Int64 limit)
{
  beginElement("search");
  attributePair("module", Token::name(subject->symbol()->getModule()->id()));
  attributePair("search-type", searchType);
  if (limit != NONE)
    attributePair("limit", int64ToString(limit));
  generate(subject);
  generate(pattern->getLhs());
  if (pattern->hasCondition())
    generateCondition(pattern->getCondition());
  endElement();
}

void
MaudemlBuffer::generateContinue(const string& command, Module* module, Int64 limit)
{
  beginElement("continue");
  attributePair("command", command);
  attributePair("module", Token::name(module->id()));
  if (limit != NONE)
    attributePair("limit", int64ToString(limit));
  endElement();
}

void
MaudemlBuffer::generateSearchResult(Int64 number,
				    RewriteSequenceSearch* state,
				    const Timer& timer,
				    bool showStats,
				    bool showTiming,
				    bool showBreakdown)
{
  beginElement("search-result");
  if (number == NONE)
    attributePair("solution-number", "NONE");
  else
    {
      attributePair("solution-number", int64ToString(number));
      attributePair("state-number", int64ToString(state->getStateNr()));
    }
  if (showStats)
    {
      attributePair("total-states", int64ToString(state->getNrStates()));
      generateStats(*(state->getContext()), timer, showTiming, showBreakdown);
    }
  if (number != NONE)
  generateSubstitution(state->getSubstitution(), state->getGoal());
  endElement();
}

void
MaudemlBuffer::generateSubstitution(const Substitution* substitution,
				    const VariableInfo* varInfo)
{
  beginElement("substitution");
  int nrVars = varInfo->getNrRealVariables();
  for (int i = 0; i < nrVars; i++)
    generateAssignment(varInfo->index2Variable(i), substitution->value(i));
  endElement();
}

void
MaudemlBuffer::generateAssignment(Term* variable, DagNode* value)
{
  beginElement("assignment");
  generate(variable);
  if (value == 0)
    {
      beginElement("unbound");
      endElement();
    }
  else
    generate(value);
  endElement();
}

void
MaudemlBuffer::generate(Term* term)
{
  beginElement("term");
  Symbol* symbol = term->symbol();
  if (VariableTerm* v = dynamic_cast<VariableTerm*>(term))
    {
      string opName(Token::name(v->id()));
      opName += ":";
      opName += sortString(safeCast(VariableSymbol*, symbol)->getSort());
      attributePair("op", opName);
    }
  else if (FloatTerm* m = dynamic_cast<FloatTerm*>(term))
      attributePair("op", doubleToString(m->getValue()));
  else if (StringTerm* s = dynamic_cast<StringTerm*>(term))
    {
      string strValue;
      Token::ropeToString(s->getValue(), strValue);
      attributePair("op", strValue);
    }
  else if (QuotedIdentifierTerm* q = dynamic_cast<QuotedIdentifierTerm*>(term))
    {
      string opName("'");
      opName += Token::name(q->getIdIndex());
      attributePair("op", opName);
    }
  else
    {
      attributePair("op", Token::name(symbol->id()));
      if (S_Term* s = dynamic_cast<S_Term*>(term))
	{
	  char* numberStr = mpz_get_str(0, 10, s->getNumber().get_mpz_t());
	  attributePair("number", numberStr);
	  free(numberStr);
	}
    }
  int sortIndex = term->getSortIndex();
  if (sortIndex == Sort::SORT_UNKNOWN)
    attributePair("kind", kindString(symbol->rangeComponent()));
  else
    attributePair("syntactic-sort", sortString(symbol->rangeComponent()->sort(sortIndex)));

  ArgumentIterator a(*term);
  while (a.valid())
    {
      generate(a.argument());
      a.next();
    }
  endElement();
}

void
MaudemlBuffer::generate(const ConditionFragment* c)
{
  if (const EqualityConditionFragment* e =
      dynamic_cast<const EqualityConditionFragment*>(c))
    {
      beginElement("equality-test");
      generate(e->getLhs());
      generate(e->getRhs());
      endElement();
    }
  else if (const SortTestConditionFragment* t =
	   dynamic_cast<const SortTestConditionFragment*>(c))
    {
      beginElement("sort-test");
      attributePair("sort", sortString(t->getSort()));
      generate(t->getLhs());
      endElement();
    }
  else if(const AssignmentConditionFragment* a =
	  dynamic_cast<const AssignmentConditionFragment*>(c))
    {
      beginElement("assignment-test");
      generate(a->getLhs());
      generate(a->getRhs());
      endElement();
    }
  else if(const RewriteConditionFragment* r =
	  dynamic_cast<const RewriteConditionFragment*>(c))
    {
      beginElement("rewrite-test");
      generate(r->getLhs());
      generate(r->getRhs());
      endElement();
    }
}

void
MaudemlBuffer::generateCondition(const Vector<ConditionFragment*>& condition)
{
  beginElement("condition");
  int nrFragments = condition.size();
  for (int i = 0; i < nrFragments; i++)
    generate(condition[i]);
  endElement();
}

void
MaudemlBuffer::generate(Rule* rule)
{
  beginElement("rule");
  int l = rule->getLabel().id();
  if (l != NONE)
    attributePair("label", Token::name(l));
  generate(rule->getLhs());
  generate(rule->getRhs());
  if (rule->hasCondition())
    generateCondition(rule->getCondition());
  endElement();
}

void
MaudemlBuffer::generate(DagNode* dagNode, PointerSet* visited)
{
  int index = NONE;
  if (visited != 0)
    {
      index = visited->pointer2Index(dagNode);
      if (index == NONE)
	index = visited->insert(dagNode);
      else
	{
	  beginElement("backref");
	  attributePair("ref", "t", index);
	  endElement();
	  return;
	}
    }

  beginElement("term");
  if (index != NONE)
    attributePair("id", "t", index);

  Symbol* symbol = dagNode->symbol();
  if (VariableDagNode* v = dynamic_cast<VariableDagNode*>(dagNode))
    {
      string opName(Token::name(v->id()));
      opName += ":";
      opName += sortString(safeCast(VariableSymbol*, symbol)->getSort());
      attributePair("op", opName);
    }
  else if (FloatDagNode* m = dynamic_cast<FloatDagNode*>(dagNode))
      attributePair("op", doubleToString(m->getValue()));
  else if (StringDagNode* s = dynamic_cast<StringDagNode*>(dagNode))
    {
      string strValue;
      Token::ropeToString(s->getValue(), strValue);
      attributePair("op", strValue);
    }
  else if (QuotedIdentifierDagNode* q = dynamic_cast<QuotedIdentifierDagNode*>(dagNode))
    {
      string opName("'");
      opName += Token::name(q->getIdIndex());
      attributePair("op", opName);
    }
  else
    {
      attributePair("op", Token::name(symbol->id()));
      if (S_DagNode* s = dynamic_cast<S_DagNode*>(dagNode))
	{
	  char* numberStr = mpz_get_str(0, 10, s->getNumber().get_mpz_t());
	  attributePair("number", numberStr);
	  free(numberStr);
	}
    }
  int sortIndex = dagNode->getSortIndex();
  if (sortIndex == Sort::SORT_UNKNOWN)
    attributePair("kind", kindString(symbol->rangeComponent()));
  else
    attributePair("sort", sortString(symbol->rangeComponent()->sort(sortIndex)));

  DagArgumentIterator a(*dagNode);
  while (a.valid())
    {
      generate(a.argument(), visited);
      a.next();
    }
  endElement();
}

string
MaudemlBuffer::sortString(Sort* sort)
{
  if (sort->index() == Sort::KIND)
    return kindString(sort->component());
  else
    return Token::name(sort->id());
}

string
MaudemlBuffer::kindString(ConnectedComponent* kind)
{
  string name("[");
  int n = kind->nrMaximalSorts();
  for (int i = 1;; i++)
    {
      name += sortString(kind->sort(i));
      if (i == n)
	break;
      name += ',';
    }
  name += ']';
  return name;
}

void
MaudemlBuffer::generateResult(RewritingContext& context,
			      const Timer& timer,
			      bool showStats,
			      bool showTiming,
			      bool showBreakdown)
{
  beginElement("result");
  if (showStats)
    generateStats(context, timer, showTiming, showBreakdown);
  generate(context.root());
  endElement();
}

void
MaudemlBuffer::generateStats(RewritingContext& context,
			     const Timer& timer,
			     bool showTiming,
			     bool showBreakdown)
{
  Int64 total = context.getTotalCount();
  attributePair("total-rewrites", int64ToString(total));
  if (showBreakdown)
    {
      attributePair("mb-rewrites", int64ToString(context.getMbCount()));
      attributePair("eq-rewrites", int64ToString(context.getEqCount()));
      attributePair("rl-rewrites", int64ToString(context.getRlCount()));
    }
  Int64 real;
  Int64 virt;
  Int64 prof;
  if (showTiming && timer.getTimes(real, virt, prof))
    {
      attributePair("real-time-ms", int64ToString(real / 1000));
      attributePair("cpu-time-ms", int64ToString(prof / 1000));
      if (prof > 0)
	attributePair("rewrites-per-second", int64ToString((1000000 * total) / prof));
    }
}

void
MaudemlBuffer::generateSearchGraph(RewriteSequenceSearch* graph)
{
  beginElement("graphml");
  beginElement("key");
  attributePair("id", "term");
  attributePair("for", "node");
  endElement();
  beginElement("key");
  attributePair("id", "rule");
  attributePair("for", "edge");
  endElement();
  beginElement("graph");
  //attributePair("id", "g1");
  attributePair("edgedefault", "directed");
  int nrStates = graph->getNrStates();
  for (int i = 0; i < nrStates; i++)
    {
      beginElement("node");
      attributePair("id", "n", i);
      beginElement("data");
      attributePair("key", "term");
      generate(graph->getStateDag(i));
      endElement();
      endElement();

      const RewriteSequenceSearch::ArcMap& fwdArcs =
	graph->getStateFwdArcs(i);
      for (RewriteSequenceSearch::ArcMap::const_iterator j = fwdArcs.begin();
	   j != fwdArcs.end(); ++j)
	{
	  beginElement("edge");
	  attributePair("source", "n", i);
	  attributePair("target", "n", (*j).first);
	  const set<Rule*>& r = (*j).second;
	  for (set<Rule*>::const_iterator k = r.begin(); k != r.end(); k++)
	    {
	      beginElement("data");
	      attributePair("key", "rule");
	      generate(*k);
	      endElement();
	    }
	  endElement();
	}
    }
  endElement();
  endElement();
}

void
MaudemlBuffer::generateSearchPath(RewriteSequenceSearch* graph, int stateNr)
{
  beginElement("graphml");
  beginElement("key");
  attributePair("id", "term");
  attributePair("for", "node");
  endElement();
  beginElement("key");
  attributePair("id", "rule");
  attributePair("for", "edge");
  endElement();
  beginElement("graph");
  //attributePair("id", "g1");
  attributePair("edgedefault", "directed");

  int parent;
  for (int i = stateNr;; i = parent)
    {
      beginElement("node");
      attributePair("id", "n", i);
      beginElement("data");
      attributePair("key", "term");
      generate(graph->getStateDag(i));
      endElement();
      endElement();
      parent = graph->getStateParent(i);
      if (parent == NONE)
	break;
      beginElement("edge");
      attributePair("source", "n", parent);
      attributePair("target", "n", i);
      beginElement("data");
      attributePair("key", "rule");
      generate(graph->getStateRule(i));
      endElement();
      endElement();
    }

  endElement();
  endElement();
}
