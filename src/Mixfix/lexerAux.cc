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
//	Auxiliary functions and data needed by lexical analyzer.
//
#define MAX_IN_DEPTH	100

int inStackPtr = 0;
YY_BUFFER_STATE inStack[MAX_IN_DEPTH];
int dirMarkerStack[MAX_IN_DEPTH];
Vector<char*> pendingFiles;
int nrPendingRead = 0;
bool rootInteractive = false;
bool fakeNewline = false;  // fake \n for files that don't end with \n
bool fakeNewlineStack[MAX_IN_DEPTH];

void
getInput(char* buf, yy_size_t& result, yy_size_t max_size)
{
  result = YY_NULL;  // if we don't update result, YY_NULL will indicate an EOF condition
  if (UserLevelRewritingContext::interrupted())
    fakeNewline = false;
  else
    {
      int n = ioManager.getInput(buf, max_size, yyin);
      if (UserLevelRewritingContext::interrupted())
	fakeNewline = false;
      else
	{
	  if (n > 0)
	    {
	      result = n;
	      //
	      //	If the last character we saw wasn't a newline then if we
	      //	hit EOF next getInput() call we will need to fake one.
	      //
	      fakeNewline = (buf[n - 1] != '\n');
	    }
	  else
	    {
	      //
	      //	Saw EOF so fake a newline if needed.
	      //
	      if (fakeNewline && max_size > 0)
		{
		  buf[0] = '\n';
		  result = 1;
		  fakeNewline = false;
		}
	    }
	}
    }
}

void
cleanUpLexer()
{
  if (inStackPtr > 0)
    {
      IssueAdvisory("closing open files.");
      while (inStackPtr > 0)
	{
	  fclose(yyin);
	  yy_delete_buffer(YY_CURRENT_BUFFER);
	  --inStackPtr;
	  yy_switch_to_buffer(inStack[inStackPtr]);
	}
      directoryManager.popd(dirMarkerStack[0]);
      fakeNewline = false;
      UserLevelRewritingContext::setInteractive(rootInteractive);
    }
  fileTable.abortEverything(lineNumber);
  nrPendingRead = pendingFiles.length();  // avoid any further reading of pending files
  BEGIN(INITIAL);
}

void
lexerIdMode()
{
  BEGIN(ID_MODE);
}

void
lexBubble(int termination, int minLen)
{
  terminationSet = termination;
  parenCount = 0;
  minLength = minLen;
  lexerBubble.clear();
  yy_push_state(BUBBLE_MODE);
}

void
lexBubble(const Token& first, int termination, int minLen, int pCount)
{
  terminationSet = termination;
  parenCount = pCount;
  minLength = minLen;
  lexerBubble.clear();
  lexerBubble.append(first);
  yy_push_state(BUBBLE_MODE);
}

void
lexContinueBubble(const Token& next, int termination, int minLen, int pCount)
{
  terminationSet = termination;
  parenCount = pCount;
  lexerBubble.append(next);
  minLength = minLen + lexerBubble.length();
  yy_push_state(BUBBLE_MODE);
}

void
lexSave(const Token& first)
{
  lexerBubble.clear();
  lexerBubble.append(first);
}

void
lexContinueSave(const Token& next)
{
  lexerBubble.append(next);
}

void
lexerCmdMode()
{
  BEGIN(CMD_MODE);
}

void
lexerInitialMode()
{
  BEGIN(INITIAL);
}

void
lexerFileNameMode()
{
  accumulator.erase();
  yy_push_state(FILE_NAME_MODE);
}

void
lexerStringMode()
{
  yy_push_state(STRING_MODE);
}

void
lexerLatexMode()
{
  braceCount = 0;
  parenCount = 1;
  accumulator.erase();
  yy_push_state(LATEX_MODE);
}

void
createRootBuffer(FILE* fp, bool forceInteractive)
{
  yy_switch_to_buffer(yy_create_buffer(fp, YY_BUF_SIZE));
  rootInteractive = forceInteractive || (isatty(fileno(fp)) > 0);
  UserLevelRewritingContext::setInteractive(rootInteractive);
}

bool
includeFile(const string& directory, const string& fileName, bool silent, int lineNr)
{
  if (inStackPtr >= MAX_IN_DEPTH)
    {
      IssueWarning(LineNumber(lineNr) <<
		   ": ins nested too deeply - couldn't open file " <<
		   QUOTE(fileName));
      return false;
    }
  int dirMarker = directoryManager.pushd(directory);
  if (dirMarker == UNDEFINED)
    {
      IssueWarning(LineNumber(lineNr) << ": couldn't chdir to " <<
		   QUOTE(directory));
      return false;
    }
  FILE* fp = fopen(fileName.c_str(), "r");
  if (fp == 0)
    {
      IssueWarning(LineNumber(lineNr) << ": couldn't open file " <<
		   QUOTE(fileName));
      directoryManager.popd(dirMarker);
      return false;
    }
  dirMarkerStack[inStackPtr] = dirMarker;
  inStack[inStackPtr] = YY_CURRENT_BUFFER;
  fakeNewlineStack[inStackPtr] = fakeNewline;
  fakeNewline = false;
  ++inStackPtr;
  yyin = fp;
  fileTable.openFile(lineNumber, fileName.c_str(), silent);
  directoryManager.visitFile(fileName);
  yy_switch_to_buffer(yy_create_buffer(yyin, YY_BUF_SIZE));
  UserLevelRewritingContext::setInteractive(false);
  return true;
}

bool
handleEof()
{
  if (inStackPtr == 0)
    return false;
  --inStackPtr;
  fclose(yyin);
  fileTable.closeFile(lineNumber);
  yy_delete_buffer(YY_CURRENT_BUFFER);
  yy_switch_to_buffer(inStack[inStackPtr]);
  directoryManager.popd(dirMarkerStack[inStackPtr]);
  fakeNewline = fakeNewlineStack[inStackPtr];
  if (inStackPtr == 0)
    {
      if (nrPendingRead < pendingFiles.length())
	{
	  string directory;
	  string fileName;
	  if (findFile(pendingFiles[nrPendingRead++], directory, fileName, FileTable::COMMAND_LINE) &&
	      includeFile(directory, fileName, true, FileTable::COMMAND_LINE))
	    return true;
	  nrPendingRead = pendingFiles.length();  // avoid any futher reading of pending files
	}
      UserLevelRewritingContext::setInteractive(rootInteractive);
    }
  return true;
}

void
checkForPending()
{
  if (nrPendingRead < pendingFiles.length())
    {
      string directory;
      string fileName;
      if (!(findFile(pendingFiles[nrPendingRead++], directory, fileName, FileTable::COMMAND_LINE) &&
	    includeFile(directory, fileName, true, FileTable::COMMAND_LINE)))
	nrPendingRead = pendingFiles.length();  // avoid any futher reading of pending files
    }
}

void
eatComment(bool firstNonWhite)
{
  bool passThrough = !firstNonWhite && fileTable.outputLine();
  bool parenMode = false;
  int parenCount = 0;
  bool backquoteSeen = false;
  for (;;)
    {
      int c = yyinput();
      switch(c)
	{
	case ' ':
	case '\t':
	case '\r':
	  break;
	case '\n':
	case '\f':
	  {
            ++lineNumber;
	    if (!parenMode)
	      {
		if (passThrough)
		  cout << endl;
		return;
	      }
	    break;
	  }
	case '(':
	  {
	    if (firstNonWhite)
	      {
		firstNonWhite = false;
		parenMode = true;
		parenCount = 1;
	      }
	    else
	      {
		if (parenMode && !backquoteSeen)
		  ++parenCount;
	      }
	    break;
	  }
	case ')':
	  {
	    if (parenMode && !backquoteSeen)
	      {
		--parenCount;
		if (parenCount == 0)
		  return;
	      }
	    break;
	  }
	case 0:  // HACK to deal with flex returning NUL rather than EOF at end-of-file
	case EOF:
	  {
	    return;
	  }
	default:
	  {
	    firstNonWhite = false;
	  }
	}
      backquoteSeen = (c == '`');
      if (passThrough)
	cout << static_cast<char>(c);
    }
}
