/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021-2024 SRI International, Menlo Park, CA 94025, USA.

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
//	Main directory manipulation code.
//

void
DirectoryManagerSymbol::getOpenDirectory(DagNode* directoryArg, int& fd, OpenDirectory*& ofp)
{
  //
  //	In order for message to have been delivered there must have been
  //	an open directory object so any failure to find it is an internal error.
  //
  Assert(directoryArg->symbol() == directoryOidSymbol, "bad directory symbol " << directoryArg);
  DagNode* idArg = safeCast(FreeDagNode*, directoryArg)->getArgument(0);
  DebugSave(ok, succSymbol->getSignedInt(idArg, fd));
  Assert(ok, "bad directory number " << directoryArg);
  DirectoryMap::iterator i = openDirectories.find(fd);
  Assert(i != openDirectories.end(), "didn't find open directory " << directoryArg);
  ofp = &(i->second);
}

void
DirectoryManagerSymbol::openDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  if (allowDir)
    {
      DagNode* pathArg = message->getArgument(2);
      if (pathArg->symbol() == stringSymbol)
	{
	  const Rope& path = safeCast(StringDagNode*, pathArg)->getValue();
	  char* pathStr = path.makeZeroTerminatedString();

	  DIR* dp = opendir(pathStr);
	  delete [] pathStr;
	  if (dp)
	    {
	      int fd = dirfd(dp);
	      openedDirectoryReply(fd, message, context);
	      OpenDirectory& od = openDirectories[fd];
	      od.path = path;
	      if (path[path.length() - 1] != '/')
		od.path += '/';
	      od.dir = dp;
	    }
	  else
	    {
	      const char* errText = strerror(errno);
	      DebugAdvisory("unexpected opendir() error: " << errText);
	      errorReply(errText, message, context);
	    }
	}
      else
	errorReply("Bad directory name.", message, context);
    }
  else
    {
      IssueAdvisory("operations on directories disabled.");
      errorReply("Directory operations disabled.", message, context);
    }
}

void
DirectoryManagerSymbol::makeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  if (allowDir)
    {
      DagNode* pathArg = message->getArgument(2);
      if (pathArg->symbol() == stringSymbol)
	{
	  const Rope& path = safeCast(StringDagNode*, pathArg)->getValue();
	  char* pathStr = path.makeZeroTerminatedString();
	  int result = mkdir(pathStr, 0777);
	  delete [] pathStr;
	  if (result == 0)
	    trivialReply(madeDirectoryMsg, message, context);
	  else
	    errorReply(strerror(errno), message, context);
	}
      else
	errorReply("Bad directory name.", message, context);
    }
  else
    {
      IssueAdvisory("operations on directories disabled.");
      errorReply("Directory operations disabled.", message, context);
    }
}

void
DirectoryManagerSymbol::removeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  Assert(message->getArgument(0)->symbol() == this, "misdirected message");
  if (allowDir)
    {
      DagNode* pathArg = message->getArgument(2);
      if (pathArg->symbol() == stringSymbol)
	{
	  const Rope& path = safeCast(StringDagNode*, pathArg)->getValue();
	  char* pathStr = path.makeZeroTerminatedString();
	  int result = rmdir(pathStr);
	  delete [] pathStr;
	  if (result == 0)
	    trivialReply(removedDirectoryMsg, message, context);
	  else
	    errorReply(strerror(errno), message, context);
	}
      else
	errorReply("Bad directory name.", message, context);
    }
  else
    {
      IssueAdvisory("operations on directories disabled.");
      errorReply("Directory operations disabled.", message, context);
    }
}

void
DirectoryManagerSymbol::handleSymbolicLink(Rope path,
					   Rope name,
					   FreeDagNode* message,
					   ObjectSystemRewritingContext& context)
{
  char* pathStr = path.makeZeroTerminatedString();
  char buffer[PATH_MAX + 1];
  ssize_t nrBytes = readlink(pathStr, buffer, PATH_MAX);
  delete [] pathStr;
  if (nrBytes == -1)
    {
      const char* errText = strerror(errno);
      DebugAdvisory("unexpected readlink() error: " << errText);
      errorReply(errText, message, context);
      return;
    }
  buffer[nrBytes] = '\0';
  Vector<DagNode*> arg(1);
  arg[0] = new StringDagNode(stringSymbol, buffer);
  DagNode* d = symbolicLinkEntrySymbol->makeDagNode(arg);
  gotDirectoryEntryReply(name, d, message, context);
}

void
DirectoryManagerSymbol::getDirectoryEntry(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* directoryName = message->getArgument(0);
  int fd;
  OpenDirectory* odp;
  getOpenDirectory(directoryName, fd, odp);

  errno = 0;
  struct dirent* entry = readdir(odp->dir);
  if (entry == 0)
    {
      Assert(errno == 0, "readdir() returned " << strerror(errno));
      gotDirectoryEntryReply("", endOfDirectorySymbol->makeDagNode(), message, context);
    }
  else
    {
      Symbol* typeSymbol = 0;
      switch (entry->d_type)
	{
	case DT_BLK:
	  {
	    typeSymbol = blockDeviceEntrySymbol;
	    break;
	  }
	case DT_CHR:
	  {
	    typeSymbol = charDeviceEntrySymbol;
	    break;
	  }
	case DT_DIR:
	  {
	    typeSymbol = directoryEntrySymbol;
	    break;
	  }
	case DT_FIFO:
	  {
	    typeSymbol = pipeEntrySymbol;
	    break;
	  }
	case DT_LNK:
	  {
	    handleSymbolicLink(odp->path + entry->d_name, entry->d_name, message, context);
	    return;
	  }
	case DT_REG:
	  {
	    typeSymbol = fileEntrySymbol;
	    break;
	  }
	case DT_SOCK:
	  {
	    typeSymbol = directoryEntrySymbol;
	    break;
	  }
	case DT_UNKNOWN:
	  {
	    //
	    //	readdir() does not guarantee the file type will be obtained; it depends on the file system.
	    //	Here we fall back to trying to get the answer from from an lstat() system call on the file.
	    //	We use lstat() rather than stat() just in case the file is a symbolic link because we want
	    //	to report that information.
	    //
	    Rope path(odp->path + entry->d_name);
	    char* pathStr = path.makeZeroTerminatedString();
	    struct stat statBuffer;
	    int result = lstat(pathStr, &statBuffer);
	    delete [] pathStr;
	    if (result == -1)
	      {
		//
		//	Maybe file disappeared in the meantime.
		//
		const char* errText = strerror(errno);
		DebugAdvisory("unexpected lstat() error: " << errText);
		errorReply(errText, message, context);
	        return;
	      }
	    switch (statBuffer.st_mode & S_IFMT)
	      {
	      case S_IFIFO:
		{
		  typeSymbol = pipeEntrySymbol;
		  break;
		}
	      case S_IFCHR:
		{
		  typeSymbol = charDeviceEntrySymbol;
		  break;
		}
	      case S_IFDIR:
		{
		  typeSymbol = directoryEntrySymbol;
		  break;
		}
	      case S_IFBLK:
		{
		  typeSymbol = blockDeviceEntrySymbol;
		  break;
		}
	      case S_IFREG:
		{
		  typeSymbol = fileEntrySymbol;
		  break;
		}
	      case S_IFSOCK:
		{
		  typeSymbol = socketEntrySymbol;
		  break;
		}
	      case S_IFLNK:
		{
		  handleSymbolicLink(path, entry->d_name, message, context);
		  return;
		}
	      default:
		{
		  CantHappen("bad st_mode " << statBuffer.st_mode);
		}
	      }
	  }
	}
      gotDirectoryEntryReply(entry->d_name, typeSymbol->makeDagNode(), message, context);
    }
}

void
DirectoryManagerSymbol::closeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* directoryName = message->getArgument(0);
  int fd;
  OpenDirectory* odp;
  getOpenDirectory(directoryName, fd, odp);
  closedir(odp->dir);
  openDirectories.erase(fd);
  context.deleteExternalObject(directoryName);
  trivialReply(closedDirectoryMsg, message, context);
}

void
DirectoryManagerSymbol::cleanUp(DagNode* objectId)
{
  int fd;
  OpenDirectory* odp;
  getOpenDirectory(objectId, fd, odp);
  DebugAdvisory("cleaning up " << objectId);
  closedir(odp->dir);
  openDirectories.erase(fd);
}
