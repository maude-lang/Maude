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
//	Code for erewrite command.
//

void
Interpreter::eRewrite(const Vector<Token>& subject, Int64 limit, Int64 gas, bool debug)
{
  if (DagNode* d = makeDag(subject))
    {
      if (getFlag(SHOW_COMMAND))
	{
	  UserLevelRewritingContext::beginCommand();
	  cout << "erewrite ";
	  printModifiers(limit, gas);
	  cout << d << " ." << endl;
	  if (xmlBuffer != 0)
	    xmlBuffer->generateErewrite(d, limit, gas);
	}
      CacheableRewritingContext* context = new CacheableRewritingContext(d);
      context->setObjectMode(ObjectSystemRewritingContext::EXTERNAL);
      VisibleModule* fm = currentModule->getFlatModule();

      startUsingModule(fm);
      if (getFlag(AUTO_CLEAR_RULES))
	fm->resetRules();
      beginRewriting(debug);
      Timer timer(getFlag(SHOW_TIMING));
      context->fairStart(limit, (gas == NONE) ? 1 : gas);
      context->externalRewrite();
      endRewriting(timer, context, fm, &Interpreter::eRewriteCont);
    }
}

void
Interpreter::eRewriteCont(Int64 limit, bool debug)
{
  CacheableRewritingContext* context = safeCast(CacheableRewritingContext*, savedState);
  VisibleModule* fm = savedModule;
  savedState = 0;
  savedModule = 0;
  continueFunc = 0;
  if (xmlBuffer != 0 && getFlag(SHOW_COMMAND))
    xmlBuffer->generateContinue("erewrite", fm, limit);
  context->clearCount();
  beginRewriting(debug);
  Timer timer(getFlag(SHOW_TIMING));
  context->fairRestart(limit);
  context->externalRewrite();
  endRewriting(timer, context, fm, &Interpreter::eRewriteCont);
}
