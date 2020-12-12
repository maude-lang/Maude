/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//      Remote metaInterpreters: child side
//
#define EOT '\004'

Rope
InterpreterManagerSymbol::receiveMessage(int socketId)
{
  static char buffer[READ_BUFFER_SIZE];
  static ssize_t nextValidIndex = 0;
  static ssize_t n = 0;

  Rope message;
  //
  //	Left over characters from last read?
  //
  if (nextValidIndex != 0)
    {
      //
      //	May be many messages terminated by EOTs.
      //
      for (int i = nextValidIndex; i < n; ++i)
	{
	  if (buffer[i] == EOT)
	    {
	      //
	      //	Saw end of a message.
	      //
	      message = Rope(buffer + nextValidIndex, i - nextValidIndex);
	      if (i + 1 < n)
		{
		  //
		  //	More characters beyond message so save them.
		  //
		  nextValidIndex = i + 1;
		}
	      else
		{
		  //
		  //	No more characters.
		  //
		  nextValidIndex = 0;
		}
	      return message;
	    }
	} 
      message = Rope(buffer + nextValidIndex, n - nextValidIndex);
    }

  for (;;)
    {
      //
      //	Get whatever characters are available, restarting
      //	interrupted calls.
      //
      do
	n = read(socketId, buffer, READ_BUFFER_SIZE);
      while (n == -1 && errno == EINTR);
      Assert(n != -1, "unexpected read failure: " << strerror(errno));
      if (n == 0)
	{
	  //
	  //	EOF means parent must have closed socket so we
	  //	need to exit.
	  //
	  exit(UserLevelRewritingContext::SOCKET_CLOSED);
	}
      for (ssize_t i = 0; i < n; ++i)
	{
	  if (buffer[i] == EOT)
	    {
	      //
	      //	Saw end of a message.
	      //
	      message += Rope(buffer, i);
	      if (i + 1 < n)
		{
		  //
		  //	More characters beyond message so save them.
		  //
		  nextValidIndex = i + 1;
		}
	      else
		{
		  //
		  //	No more characters.
		  //
		  nextValidIndex = 0;
		}
	      return message;
	    }
	}
      //
      //	Add all characters.
      //
      message += Rope(buffer, n);
    }
  return message;  // dummy
}

void
InterpreterManagerSymbol::sendMessage(int socketId, const Rope& message)
{
  Rope::size_type length = message.length();
  char* charArray = message.makeZeroTerminatedString();
  charArray[length] = EOT;  // change NUL to EOT
  
  ssize_t nrUnsent = length + 1;
  char* p = charArray;
  do
    {
      ssize_t n;
      do
	n = write(socketId, p, nrUnsent);
      while (n == -1 && errno == EINTR);
      p += n;
      nrUnsent -= n;
    }
  while (nrUnsent > 0);
  delete [] charArray;
}

void
InterpreterManagerSymbol::becomeRemoteInterpreter(int socketId,
						  MixfixModule* m,
						  ObjectSystemRewritingContext& context)
{
  Interpreter* interpreter = new Interpreter;

  for (;;)
    {
      Rope request = receiveMessage(socketId);
      //
      //	We only support message symbols in the free theory.
      //
      FreeDagNode* requestDag = safeCast(FreeDagNode*, m->deserialize(request));
      DagNode* replyDag = handleMessage(requestDag, context, interpreter);
      Rope reply = m->serialize(replyDag);
      //cerr << "reply = " << reply << endl;
      sendMessage(socketId, reply);
    }
}

DagNode*
InterpreterManagerSymbol::handleMessage(FreeDagNode* message,
					ObjectSystemRewritingContext& context,
					Interpreter* interpreter)
{
  DebugInfo("remote interpreter handling " << message);
  //
  //	We need to protect message from garbage collector.
  DagRoot messagerProtection(message);
  Symbol* s = message->symbol();
  //
  //	Modules and views.
  //
  if (s == insertModuleMsg)
    return insertModule(message, context, interpreter);
  else if (s == showModuleMsg)
    return showModule(message, context, interpreter);
  else if (s == insertViewMsg)
    return insertView(message, context, interpreter);
  else if (s == showViewMsg)
    return showView(message, context, interpreter);
  //
  //	Reduction/rewriting/search.
  //
  else if (s == reduceTermMsg)
    return reduceTerm(message, context, interpreter);
  else if (s == rewriteTermMsg)
    return rewriteTerm(message, context, interpreter);
  else if (s == frewriteTermMsg)
    return frewriteTerm(message, context, interpreter);
  else if (s == erewriteTermMsg)
    return erewriteTerm(message, context, interpreter);
  else if (s == srewriteTermMsg)
    return srewriteTerm(message, context, interpreter);
  else if (s == getSearchResultMsg || s == getSearchResultAndPathMsg)
    return getSearchResult(message, context, interpreter);
  //
  //	Unification.
  //
  else if (s == getUnifierMsg)
    return getUnifier(message, context, false, false, interpreter);
  else if (s == getDisjointUnifierMsg)
    return getUnifier(message, context, true, false, interpreter);
  else if (s == getIrredundantUnifierMsg)
    return getUnifier(message, context, false, true, interpreter);
  else if (s == getIrredundantDisjointUnifierMsg)
    return getUnifier(message, context, true, true, interpreter);
  //
  //	Variant generation and variant unification/matching.
  //
  else if (s == getVariantMsg)
    return getVariant(message, context, interpreter);
  else if (s == getVariantUnifierMsg)
    return getVariantUnifier(message, context, false, interpreter);
  else if (s == getDisjointVariantUnifierMsg)
    return getVariantUnifier(message, context, true, interpreter);
  else if (s == getVariantMatcherMsg)
    return getVariantMatcher(message, context, interpreter);
  //
  //	Printing and parsing.
  //
  else if (s == printTermMsg)
    return printTerm(message, context, interpreter);
  else if (s == parseTermMsg)
    return parseTerm(message, context, interpreter);
  //
  //	Matching.
  //
  else if (s == getMatchMsg)
    return getMatch(message, context, interpreter);
  else if (s == getXmatchMsg)
    return getXmatch(message, context, interpreter);
  //
  //	Rule application.
  //
  else if (s == applyRuleMsg)
    return applyRule(message, context, true, interpreter);
  else if (s == applyRule2Msg)
    return applyRule(message, context, false, interpreter);
  //
  //	Narrowing with variant unification.
  //
  else if (s == getOneStepNarrowingMsg)
    return getOneStepNarrowing(message, context, interpreter);
  else if (s == getNarrowingSearchResultMsg)
    return getNarrowingSearchResult(message, context, false, interpreter);
  else if (s == getNarrowingSearchResultAndPathMsg)
    return getNarrowingSearchResult(message, context, true, interpreter);
  //
  //	Sort computations.
  //
  else if (s == getLesserSortsMsg)
    return getLesserSorts(message, context, interpreter);
  else if (s == getMaximalSortsMsg)
    return getMaximalSorts(message, context, interpreter);
  else if (s == getMinimalSortsMsg)
    return getMinimalSorts(message, context, interpreter);
  else if (s == compareTypesMsg)
    return compareTypes(message, context, interpreter);
  else if (s == getKindMsg)
    return getKind(message, context, interpreter);
  else if (s == getKindsMsg)
    return getKinds(message, context, interpreter);
  else if (s == getGlbTypesMsg)
    return getGlbTypes(message, context, interpreter);
  else if (s == getMaximalAritySetMsg)
    return getMaximalAritySet(message, context, interpreter);
  else if (s == normalizeTermMsg)
    return normalizeTerm(message, context, interpreter);

  return makeErrorReply("Unsupported message.", message);
}
