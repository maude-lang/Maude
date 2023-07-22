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
//      Class to generate Latex from Maude internal data structures.
//
#ifndef _maudeLatexBuffer_hh_
#define _maudeLatexBuffer_hh_

class MaudeLatexBuffer
{
  NO_COPYING(MaudeLatexBuffer);

public:
  MaudeLatexBuffer(const char* fileName);
  ~MaudeLatexBuffer();

  ostream& getStream();

  void generateBanner(const char* date, const char* time, time_t seconds);
  void generateDag(DagNode* dag);
  void generateModuleName(Module* module);

  void generateModifiers(Module* module, Int64 number = NONE, Int64 number2 = NONE);

  void generateCommand(const string command, DagNode* subject, Int64 number = NONE, Int64 number2 = NONE);

  void generateReduce(DagNode* subject);
  void generateContinue(Int64 limit);
  void generateRewrite(DagNode* subject, Int64 limit);
  void generateFrewrite(DagNode* subject, Int64 limit, Int64 gas);
  void generateErewrite(DagNode* subject, Int64 limit, Int64 gas);

  void generateResult(RewritingContext& context,
		      const Timer& timer,
		      bool showStats,
		      bool showTiming,
		      bool showBreakdown);

  void generateStats(RewritingContext& context,
		     const Timer& timer,
		     bool showTiming,
		     bool showBreakdown);
  
  void generateTiming(Int64 nrRewrites, Int64 cpu, Int64 real);
  void generateType(Sort* sort);

private:
  ofstream output;
};

inline ostream&
MaudeLatexBuffer::getStream()
{
  return output;
}

#endif
