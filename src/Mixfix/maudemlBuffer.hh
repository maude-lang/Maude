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
//      Class to generate MaudeML from Maude internal data structures.
//
#ifndef _maudemlBuffer_hh_
#define _maudemlBuffer_hh_
#include "xmlBuffer.hh"

class MaudemlBuffer : public XmlBuffer
{
  NO_COPYING(MaudemlBuffer);

public:
  MaudemlBuffer(ostream& output);

  void generateShowSearchPath(int stateNr);
  void generateShowSearchGraph();
  void generateReduce(DagNode* subject);
  void generateRewrite(DagNode* subject, Int64 limit);
  void generateFrewrite(DagNode* subject, Int64 limit, Int64 gas);
  void generateErewrite(DagNode* subject, Int64 limit, Int64 gas);
  void generateSearch(DagNode* subject,
		      PreEquation* pattern,
		      const string& searchType,
		      Int64 limit,
		      Int64 depth);
  void generateContinue(const string& command, Module* module, Int64 limit);
  void generateSearchResult(Int64 number,
			    RewriteSequenceSearch* state,
			    const Timer& timer,
			    bool showStats,
			    bool showTiming,
			    bool showBreakdown);
  void generateSubstitution(const Substitution* substitution,
			    const VariableInfo* varInfo);
  void generateAssignment(Term* variable, DagNode* value);
  // void generateShow(const string& item, Module* module = 0);

  void generate(Term* term);
  void generate(const ConditionFragment* c);
  void generate(Rule* rule);
  void generate(DagNode* dagNode, PointerSet* visited = 0);
  void generateSearchGraph(RewriteSequenceSearch* graph);
  void generateSearchPath(RewriteSequenceSearch* graph, int stateNr);
  void generateCondition(const Vector<ConditionFragment*>& condition);
  void generateResult(RewritingContext& context,
		      const Timer& timer,
		      bool showStats,
		      bool showTiming,
		      bool showBreakdown);

private:
  static string sortString(Sort* sort);
  static string kindString(ConnectedComponent* kind);

  void generateStats(RewritingContext& context,
		     const Timer& timer,
		     bool showTiming,
		     bool showBreakdown);
};

#endif
