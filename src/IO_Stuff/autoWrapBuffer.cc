/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class AutoWrapBuffer.
//

//      utility stuff
#include "macros.hh"

//	system class definitions
#include "autoWrapBuffer.hh"

AutoWrapBuffer::AutoWrapBuffer(streambuf* outputBuffer,
			       int lineWidth,
			       bool respectFlush,
			       WaitFunction waitFunction)
  : outputBuffer(outputBuffer),
    lineWidth(lineWidth),
    respectFlush(respectFlush),
    waitFunction(waitFunction)
{
  pendingWidth = UNDEFINED;
  cursorPosition = 0;
  seenBackQuote = false;
  seenBackSlash = false;
  inString = false;
  inEscape = false;
}

int
AutoWrapBuffer::sync()
{
  if (waitFunction)
    (*waitFunction)();
  Assert(pptr() - pbase() == 0, "not supposed to have a buffer");
  if (respectFlush)
    decideOnBreak();
  //
  //	Call the ANSI sync function.
  //
  return outputBuffer->pubsync();
}

void
AutoWrapBuffer::dumpBuffer()
{
  if (!(pendingBuffer.empty()))
    {
      outputBuffer->sputn(pendingBuffer.data(), pendingBuffer.size());
      pendingBuffer.erase();
    }
}

void
AutoWrapBuffer::handleEscapeSequenceChar(int ch)
{
  if (pendingWidth == UNDEFINED)
    outputBuffer->sputc(ch);  // buffering disabled
  else
    pendingBuffer += ch;
}

void
AutoWrapBuffer::handleChar(int ch)
{
  if (pendingWidth == UNDEFINED)
    outputBuffer->sputc(ch);  // buffering disabled
  else
    {
      pendingBuffer += ch;
      ++pendingWidth;
      if (pendingWidth > lineWidth - RIGHT_MARGIN - LEFT_MARGIN)
	{
	  //
	  //	Even inserting a \n before the characters in pendingBuffer
	  //	would not avoid a hard-wrapped token, so we don't bother.
	  //
	  dumpBuffer();
	  pendingWidth = UNDEFINED;  // disable buffering
	}
    }
}

void
AutoWrapBuffer::decideOnBreak()
{
  if (pendingWidth != UNDEFINED)
    {
      //
      //	We're buffering because we have a place before the
      //	characters in the buffer that it's ok to insert a \n
      //
      //	We've now come to another place where we can insert \n
      //	We need to decide on whether to insert \n before the buffer
      //	or just dump the buffer and stop buffering.
      //
      if (cursorPosition > lineWidth - RIGHT_MARGIN)
	{
	  //
	  //	Dumping the buffer without inserting a \n would
	  //	cause us to enter the right margin.
	  //
	  outputBuffer->sputc('\n');
	  //
	  //	Create left wrapped line margin.
	  //
	  for (int i = 0; i < LEFT_MARGIN; i++)
	    outputBuffer->sputc(' ');
	  //
	  //	Now we do a modified dump of the buffer that is smart
	  //	about the first character in the buffer being a space
	  //	a tab, and which takes the left margin into account.
	  //
	  int nrPending = pendingBuffer.size();
	  cursorPosition = LEFT_MARGIN;
	  if (nrPending > 0)
	    {
	      int t = (pendingBuffer[0] == ' ');  // need to skip leading space
	      if (nrPending - t > 0)
		{
		  outputBuffer->sputn(pendingBuffer.data() + t, nrPending - t);
		  if (pendingBuffer[0] == '\t')
		    cursorPosition = nextTabPosition(cursorPosition) + pendingWidth;
		  else
		    cursorPosition += pendingWidth - t;
		}
	      pendingBuffer.erase();
	    }
	}
      else
	dumpBuffer();
      //
      //	The buffer has now been dumped. We stop buffering
      //	for the moment, though is most but not all cases we will
      //	restart it with legalPositionToBreak().
      //
      pendingWidth = UNDEFINED;
    }
}

void
AutoWrapBuffer::legalPositionToBreak()
{
  //
  //	We just ecountered a legal position to insert a \n so we place
  //	subsequent characters in pendingBuffer and make sure cursor
  //	position is in range.
  //
  pendingWidth = 0;
  cursorPosition %= lineWidth;  // where is the cursor on the current line
}

int
AutoWrapBuffer::overflow(int ch)
{
  if (waitFunction)
    (*waitFunction)();
  Assert(pptr() - pbase() == 0, "not supposed to have a buffer");
  if (ch == EOF)
    return EOF;
  if (lineWidth == NONE)
    {
      //
      //	We're not wrapping lines.
      //
      outputBuffer->sputc(ch);
      return 0;
    }
  if (inEscape)
    {
      handleEscapeSequenceChar(ch);
      if (ch == 'm')
	inEscape = false;
      return 0;
    }
  if (!isprint(ch))
    inString = false;
  switch (ch)
    {
    case '"':
      {
	if (!seenBackSlash)
	  inString = !inString;
	goto normal;
      }
    case '\033':
      {
	inEscape = true;
	handleEscapeSequenceChar(ch);
	break;
      }
    case '\n':
      {
	decideOnBreak();
	outputBuffer->sputc(ch);
	cursorPosition = 0;
	break;
      }
    case '\t':
      {
	decideOnBreak();
	legalPositionToBreak();
	handleEscapeSequenceChar(ch);
	cursorPosition = nextTabPosition(cursorPosition);
	break;
      }
    case ' ':
      {
	if (!inString)
	  {
	    decideOnBreak();
	    legalPositionToBreak();
	  }
	goto normal;
      }
#ifdef WRAP_BEFORE_CLOSING
    case ')':
    case ']':
    case '}':
      {
	if (!inString && !seenBackQuote)
	  {
	    //
	    //	ok to add new line before this character
	    //
	    decideOnBreak();
	    legalPositionToBreak();
	  }
	goto normal;
      }
#endif
    case ',':
    case '(':
    case '[':
    case '{':
      {
	if (!inString && !seenBackQuote)
	  {
	    //
	    //	ok to add new line after this character
	    //
	    handleChar(ch);
	    ++cursorPosition;
	    decideOnBreak();
	    legalPositionToBreak();
	    break;
	  }
	//
	//	If we have just seen a backquote or we are in a string,
	//	,([{ lose their special properties and we fall thru
	//	into default case.
	//
      }
    default:
      {
    normal:
	handleChar(ch);
	++cursorPosition;
	break;
      }
    }
  seenBackQuote = (ch == '`');
  seenBackSlash = inString && !seenBackSlash && (ch == '\\');
  return 0;
}
