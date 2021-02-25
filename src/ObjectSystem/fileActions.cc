/*

    This file is part of the Maude 3 interpreter.

    Copyright 2017-2021 SRI International, Menlo Park, CA 94025, USA.

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

#include "../IO_Stuff/IO_Manager.hh"  // HACK
extern IO_Manager ioManager;  // HACK
//
//	Main file manipulation code.
//
#include <errno.h>

void
FileManagerSymbol::getOpenFile(DagNode* fileArg, int& fd, OpenFile*& ofp)
{
  //
  //	In order for message to have been delivered there must have been
  //	an open file object so any failure to find it is an internal error.
  //
  Assert(fileArg->symbol() == fileOidSymbol, "bad file symbol " << fileArg);
  DagNode* idArg = safeCast(FreeDagNode*, fileArg)->getArgument(0);
  DebugSave(ok, succSymbol->getSignedInt(idArg, fd));
  Assert(ok, "bad file number " << fileArg);
  FileMap::iterator i = openFiles.find(fd);
  Assert(i != openFiles.end(), "didn't find open file " << fileArg);
  ofp = &(i->second);
}

bool
FileManagerSymbol::getText(DagNode* textArg, Rope& text)
{
  if (textArg->symbol() == stringSymbol)
    {
      text = safeCast(StringDagNode*, textArg)->getValue();
      return true;
    }
  return false;
}

bool
FileManagerSymbol::checkModeExt(const char* modeStr, bool& mainFlag, bool& otherFlag)
{
  //
  //	Comments are for the r case; w and a cases are symmetric.
  //
  mainFlag = true;
  int n = 2;
  char c = modeStr[1];
  if (c == 'b')
    {
      c = modeStr[2];
      n = 3;
    }
  if (c == '\0')
    {
      otherFlag = false;
      return true;  // r rb
    }
  else if (c == '+')
    {
      otherFlag = true;
      c = modeStr[n];
      if (c == '\0')
	return true;  // r+ rb+
      if (n == 2 && c == 'b' && modeStr[3] == '\0')
	return true;  // r+b
    }
  return false;
}

bool
FileManagerSymbol::getMode(DagNode* modeDag, char*& modeStr, bool& okToRead, bool& okToWrite)
{
  //
  //	We recognize 15 legal mode strings:
  //	  r rb r+ rb+ r+b
  //	  w wb w+ wb+ w+b
  //	  a ab a+ ab+ a+b
  //
  if (modeDag->symbol() == stringSymbol)
    {
      const Rope& mode = safeCast(StringDagNode*, modeDag)->getValue();
      modeStr = mode.makeZeroTerminatedString();
      char c = modeStr[0];
      if (c == 'r')
	{
	  if (checkModeExt(modeStr, okToRead, okToWrite))
	    return true;
	}
      else if (c == 'w' || c == 'a')
	{
	  if (checkModeExt(modeStr, okToWrite, okToRead))
	    return true;
	}
      delete [] modeStr;
    }
  return false;
}

void
FileManagerSymbol::removeFile(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");

  if (allowFiles)
    {
      DagNode* pathArg = message->getArgument(2);
      if (pathArg->symbol() == stringSymbol)
	{
	  const Rope& path = safeCast(StringDagNode*, pathArg)->getValue();
	  char* pathStr = path.makeZeroTerminatedString();
	  if (unlink(pathStr) == 0)
	    trivialReply(removedFileMsg, message, context);
	  else
	    errorReply(strerror(errno), message, context);
	  delete [] pathStr;
	}
      else
	errorReply("Bad file name.", message, context);
    }
  else
    {
      IssueAdvisory("operations on file system disabled.");
      errorReply("File operations disabled.", message, context);
    }
}

void
FileManagerSymbol::openFile(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");

  if (allowFiles)
    {
      DagNode* pathArg = message->getArgument(2);
      if (pathArg->symbol() == stringSymbol)
	{
	  DagNode* modeArg = message->getArgument(3);
	  char* modeStr;
	  bool okToRead;
	  bool okToWrite;
	  if (getMode(modeArg, modeStr, okToRead, okToWrite))
	    {
	      const Rope& path = safeCast(StringDagNode*, pathArg)->getValue();
	      char* pathStr = path.makeZeroTerminatedString();

	      FILE* fp = fopen(pathStr, modeStr);
	      delete [] modeStr;
	      delete [] pathStr;

	      if (fp)
		{
		  int fd = fileno(fp);
		  openedFileReply(fd, message, context);
		  OpenFile& of = openFiles[fd];
		  of.fp = fp;
		  of.okToRead = okToRead;
		  of.okToWrite = okToWrite;
		  of.lastOpWasWrite = false;
		}
	      else
		{
		  const char* errText = strerror(errno);
		  DebugAdvisory("unexpected fopen() error: " << errText);
		  errorReply(errText, message, context);
		}
	    }
	  else
	    errorReply("Bad mode.", message, context);
	}
      else
	errorReply("Bad file name.", message, context);
    }
  else
    {
      IssueAdvisory("operations on file system disabled.");
      errorReply("File operations disabled.", message, context);
    }
}

void
FileManagerSymbol::write(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  //
  //	We will decline the message if we're a read-only file descriptor
  //	or the text is not text or it is empty.
  //
  Rope text;
  if (ofp->okToWrite)
    {
      if (getText(message->getArgument(2), text) && !(text.empty()))
	{
	  char* textArray = text.makeZeroTerminatedString();
	  size_t charsToWrite = text.length();
	  size_t charsWritten = fwrite(textArray, 1, text.length(), ofp->fp);
	  ofp->lastOpWasWrite = true;
	  delete [] textArray;

	  if (charsWritten == charsToWrite)
	    trivialReply(wroteMsg, message, context);
	  else
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected fwrite() error " << charsToWrite <<
			    " vs " << charsWritten << " : " << errText);
	      errorReply(errText, message, context);
	    }
	}
      else
	errorReply("Bad characters.", message, context);
    }
  else
    errorReply("File not open for writing.", message, context);
}

void
FileManagerSymbol::flush(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  if (ofp->okToWrite)
    {
      fflush(ofp->fp);
      trivialReply(flushedMsg, message, context);
    }
  else
    errorReply("File not open for writing.", message, context);
}

void
FileManagerSymbol::getLine(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  if (ofp->okToRead)
    {
      if (ofp->lastOpWasWrite)
	{
	  fseek(ofp->fp, 0, SEEK_CUR);  // required by ANSI C for write followed by read
	  ofp->lastOpWasWrite = false;
	}
      char* line = 0;
      size_t n = 0;
      errno = 0;
      ssize_t nrRead = getline(&line, &n, ofp->fp);  // might included embedded null chars
      if (nrRead == -1)
	{
	  if (errno != 0)
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected getline() error : " << errText);
	      errorReply(errText, message, context);
	    }
	  else
	    {
	      //
	      //	Must be end-of-file; handle it by returning an empty line.
	      //
	      Rope empty;
	      gotLineReply(empty, message,context);
	    }
	}
      else
	{
	  Rope nonempty(line, nrRead);
	  gotLineReply(nonempty, message, context);
	}
      //
      //	Even in the EOF/error case, getline() may have realloc()'d memory.
      //
      if (line != 0)
	free(line);
    }
  else
    errorReply("File not open for reading.", message, context);
}

void
FileManagerSymbol::getChars(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);

  if (ofp->okToRead)
    {
      DagNode* nrArg = message->getArgument(2);
      Int64 nrCharsToRead;
      if (succSymbol->getSignedInt64(nrArg, nrCharsToRead)) // need to deal with unbounded
	{
	  if (ofp->lastOpWasWrite)
	    {
	      fseek(ofp->fp, 0, SEEK_CUR);  // required by ANSI C for write followed by read
	      ofp->lastOpWasWrite = false;
	    }
	  Rope text;
	  FILE* fp = ofp->fp;
	  do
	    {
	      char buffer[BUFFER_SIZE];
	      size_t nrWanted = (nrCharsToRead > BUFFER_SIZE) ? BUFFER_SIZE : nrCharsToRead;
	      size_t nrCharsRead = fread(buffer, 1, nrWanted, fp);
	      if (nrCharsRead > 0)
		{
		  Rope t(buffer, nrCharsRead);
		  text += t;
		}
	      if (nrCharsRead != nrWanted)
		break;  // maybe check feof() to see if was EOF or an error?
	      nrCharsToRead -= nrCharsRead;
	    }
	  while (nrCharsToRead > 0);
	  gotCharsReply(text, message, context);
	}
      else
	errorReply("Bad size.", message, context);
    }
  else
    errorReply("File not open for reading.", message, context);
}

void
FileManagerSymbol::getPosition(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  Int64 position = ftell(ofp->fp);
  if (position < 0)
    {
      const char* errText = strerror(errno);
      DebugAdvisory("unexpected ftell() error : " << errText);
      errorReply(errText, message, context);
    }
  else
    positionGotReply(position, message, context);
}

bool
FileManagerSymbol::getBase(DagNode* baseDag, int& base)
{
  Symbol* s = baseDag->symbol();
  if (s == startSymbol)
    base = SEEK_SET;
  else if (s == currentSymbol)
    base = SEEK_CUR;
  else if (s == endSymbol)
    base = SEEK_END;
  else
    return false;
  return true;
}

void
FileManagerSymbol::setPosition(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  int base;
  if (getBase(message->getArgument(3), base))
    {
      Int64 offset;
      if (minusSymbol->getSignedInt64(message->getArgument(2), offset))
	{
	  ofp->lastOpWasWrite = false;
	  int ok = fseek(ofp->fp, offset, base);
	  if (ok == 0)
	    trivialReply(positionSetMsg, message, context);
	  else
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected fseek() error : " << errText);
	      errorReply(errText, message, context);
	    }
	}
      else
	errorReply("Bad offset.", message, context);
    }
  else
    errorReply("Bad base.", message, context);
}

void
FileManagerSymbol::closeFile(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int fd;
  OpenFile* ofp;
  DagNode* fileName = message->getArgument(0);
  getOpenFile(fileName, fd, ofp);
  fclose(ofp->fp);
  openFiles.erase(fd);
  context.deleteExternalObject(fileName);
  trivialReply(closedFileMsg, message, context);
}

void
FileManagerSymbol::cleanUp(DagNode* objectId)
{
  int fd;
  OpenFile* ofp;
  getOpenFile(objectId, fd, ofp);
  DebugAdvisory("cleaning up " << objectId);
  fclose(ofp->fp);
  openFiles.erase(fd);
}
