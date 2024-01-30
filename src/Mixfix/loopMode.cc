/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for loop mode.
//

void
Interpreter::loop(const Vector<Token>& subject)
{
  if (DagNode* d = makeDag(subject))
    {
      savedLoopSubject = subject;  // deep copy
      VisibleModule* fm = currentModule->getFlatModule();
      startUsingModule(fm);
      CacheableRewritingContext* context = new CacheableRewritingContext(d);
      if (getFlag(EREWRITE_LOOP_MODE))
	context->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
      if (latexBuffer)
	latexBuffer->generateCommand(getFlag(SHOW_COMMAND), "loop", d);
      doLoop(context, fm);
    }
}

bool
Interpreter::contLoop2(const Vector<Token>& input)
{
  //
  //	We have no idea what if anything is left in savedState - could be
  //	from a previous non loop command.
  //
  CacheableRewritingContext* savedContext = dynamic_cast<CacheableRewritingContext*>(savedState);
  if (savedContext != 0)
    {
      DagNode* d = savedContext->root();
      if (LoopSymbol* l = dynamic_cast<LoopSymbol*>(d->symbol()))
	{
	  VisibleModule* fm = savedModule;
	  savedState = 0;
	  savedModule = 0;
	  continueFunc = 0;
	  l->injectInput(d, input);
	  if (latexBuffer)
	    latexBuffer->generateLoopTokens(getFlag(SHOW_COMMAND), input);
	  doLoop(savedContext, fm);
	  return true;
	}
      else
	IssueWarning("bad loop state.");
    }
  else
    IssueWarning("no loop state.");
  //
  //	Clean up anything left in savedState that was unsuitable.
  //
  clearContinueInfo();
  return false;
}

void
Interpreter::contLoop(const Vector<Token>& input)
{
  if (!contLoop2(input) && !savedLoopSubject.empty())
    {
      Vector<Token> savedInput(input);  // in case input gets overwritten, say in debugger
      IssueAdvisory("attempting to reinitialize loop.");
      if (latexBuffer)
	latexBuffer->generateAdvisory("attempting to reinitialize loop.");
      if (DagNode* d = makeDag(savedLoopSubject))
	{
	  VisibleModule* fm = currentModule->getFlatModule();
	  startUsingModule(fm);
	  CacheableRewritingContext* context = new CacheableRewritingContext(d);
	  if (getFlag(EREWRITE_LOOP_MODE))
	    context->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
	  doLoop(context, fm);
	  if (contLoop2(savedInput))
	    return;
	}
      IssueAdvisory("unable to reinitialize loop.");
      if (latexBuffer)
	latexBuffer->generateAdvisory("unable to reinitialize loop.");
    }
}

void
Interpreter::doLoop(CacheableRewritingContext* context, VisibleModule* module)
{
  if (getFlag(AUTO_CLEAR_RULES))
    module->resetRules();
#ifdef QUANTIFY_REWRITING
  quantify_start_recording_data();
#endif
  Timer timer(getFlag(SHOW_LOOP_TIMING));
  //
  //	Use chosen rewriting algorithm
  //
  if (getFlag(EREWRITE_LOOP_MODE))
    {
      context->fairStart(NONE, 1);
      context->externalRewrite();
    }
  else
    context->ruleRewrite(NONE);
  
  timer.stop();
#ifdef QUANTIFY_REWRITING
  quantify_stop_recording_data();
#endif
  clearContinueInfo();
  if (UserLevelRewritingContext::aborted())
    {
      delete context;
      (void) module->unprotect();
    }
  else
    {
      if (getFlag(SHOW_LOOP_STATS))
	printStats(timer, *context, getFlag(SHOW_LOOP_TIMING));
      DagNode* r = context->root();
      if (LoopSymbol* l = dynamic_cast<LoopSymbol*>(r->symbol()))
	{
	  Vector<int> bubble;
	  l->extractOutput(r, bubble);
	  printBubble(cout, bubble);
	  if (latexBuffer)
	    latexBuffer->generateBubbleResult(bubble);
	}
      else
	{
	  cout << "non-loop result " << r->getSort() << ": " << r << '\n';
	  if (latexBuffer)
	    latexBuffer->generateResult("non-loop result", r);
	}
      cout.flush();

      savedState = context;
      savedModule = module;
      continueFunc = getFlag(EREWRITE_LOOP_MODE) ?
	&Interpreter::eRewriteCont :
	&Interpreter::rewriteCont;
    }
  if (latexBuffer)
    latexBuffer->cleanUp();
  UserLevelRewritingContext::clearDebug();  // even if we didn't start in debug mode
}

void
Interpreter::printBubble(ostream& s, const Vector<int>& bubble)
{
  bool ansiActive = false;
  bool needSpace = false;
  int nrTokens = bubble.length();
  for (int i = 0; i < nrTokens; i++)
    {
      const char* n = Token::name(bubble[i]);
      //
      //	Single character case.
      //
      if (n[0] != 0 && n[1] == 0)
	{
	  switch (n[0])
	    {
	    case '(':	case ')':	case '[':	case ']':
	    case '{':	case '}':	case ',':
	      needSpace = false;
	      break;
	    default:
	      if (needSpace)
		s << ' ';
	      needSpace = true;
	    }
	  s << n[0];
	  continue;
	}
      //
      //	First character backslash case.
      //
      if (n[0] == '\\')
	{
	  if (n[2] == 0)
	    {
	      switch (n[1])
		{
		case 'n':
		  {
		    s << '\n';
		    needSpace = false;
		    continue;
		  }
		case 't':
		  {
		    s << '\t';
		    needSpace = false;
		    continue;
		  }
		case 's':
		  {
		    s << ' ';
		    needSpace = false;
		    continue;
		  }
		case '\\':
		  {
		    if (needSpace)
		      s << ' ';
		    s << '\\';
		    needSpace = true;
		    continue;
		  }
		  //
		  //	ANSI stuff.
		  //
#define MACRO(m, t) \
case m: { s << Tty(Tty::t); ansiActive = true; continue; }
#include "ansiEscapeSequences.cc"
#undef MACRO
		case 'o':
		  {
		    s << Tty(Tty::RESET);
		    ansiActive = false;
		    continue;
		  }
		}
	    }
	  else if (n[1] == '`' &&  n[3] == 0)
	    {
	      switch (n[2])
		{
		case '(':
		case ')':
		case '[':
		case ']':
		case '{':
		case '}':
		case ',':
		  if (needSpace)
		    s << ' ';
		  s << n[2];
		  needSpace = true;
		  continue;
		}
	    }
	}
      //
      //	Normal case.
      //
      if (needSpace)
	s << ' ';
      s << n;
      needSpace = true;
    }
  if (ansiActive)
    s << Tty(Tty::RESET);
  s << '\n';
}
