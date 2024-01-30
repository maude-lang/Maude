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
//      Implementation for class DirectoryManager.
//

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

#include "directoryManager.hh"

bool
DirectoryManager::checkAccess(const string& fullName, int mode)
{
  const char* name = fullName.c_str();
  if (access(name, mode) == 0)
    {
      //
      //	We can access it, but is it a regular file?
      //
      struct stat statBuffer;
      if (stat(name, &statBuffer) == 0 && (statBuffer.st_mode & S_IFMT) == S_IFREG)
	return true;
    }
  return false;
}

bool
DirectoryManager::checkAccess(const string& directory,
			      string& fileName,
			      int mode,
			      char const* const ext[])
{
  //
  //	See if directory/fileName is accessible, and if not
  //	try added extensions to see if the fixes the problem.
  //
  string full(directory + '/' + fileName);
  if (checkAccess(full, mode))
    return true;
  if (ext != 0)
    {
      string::size_type d = fileName.rfind('.');
      if (d != string::npos)
	{
	  for (char const* const* p = ext; *p; p++)
	    {
	      if (fileName.compare(d, string::npos, *p) == 0)
		return false;  // already ends in one of our extensions
	    }
	}
      
      for (char const* const* p = ext; *p; p++)
	{
	  if (checkAccess(full + *p, mode))
	    {
	      fileName += *p;
	      return true;
	    }
	}
    }
  return false;
}

bool
DirectoryManager::searchPath(const char* pathVar,
			     string& directory,
			     string& fileName,
			     int mode,
			     char const* const ext[])
{
  if (char* p = getenv(pathVar))
    {
      string path(p);
      string::size_type len = path.length();
      for (string::size_type start = 0; start < len;)
	{
	  string::size_type c = path.find(':', start);
	  if (c == string::npos)
	    c = len;
	  if (string::size_type partLen = c - start)
	    {
	      realPath(path.substr(start, partLen), directory);
	      if (checkAccess(directory, fileName, mode, ext))
		return true;
	    }
	  start = c + 1;
	}
    }
  return false;
}

void
DirectoryManager::realPath(const string& path, string& resolvedPath)
{
  string::size_type length = path.length();
  if (length == 0)
    {
      resolvedPath = getCwd();
      return;
    }
  //  cout << "in " << path << '\n';
  resolvedPath.erase();
  string::size_type p = 0;  // p will index the first path component for standard processing
  switch (path[0])
    {
    case '/':  // absolute path name
      {
	p = 1;
	break;
      }
    case '~':  // need to expand user home directory
      {
	const char* dirPath = 0;
	string::size_type e = path.find('/');
	if (e == string::npos)
	  e = length;
	if (e == 1)
	  {
	    //
	    //	Get users home directory.
	    //
	    dirPath = getenv("HOME");
	    if (dirPath == 0)
	      {
		if (passwd* pw = getpwuid(getuid()))
		  dirPath = pw->pw_dir;
	      }
	  }
	else
	  {
	    //
	    //	Get somebody elses home directory.
	    //
	    if (passwd* pw = getpwnam(path.substr(1, e - 1).c_str()))
	      dirPath = pw->pw_dir;
	  }
	if (dirPath != 0)
	  {
	    resolvedPath = dirPath;
	    p = e + 1;
	    break;
	  }
      }
      // fall thru
    default:  // relative path name
      {
	resolvedPath = getCwd();
	break;
      }
    }
  //
  //	Just in case a home directory or cwd ended in '/'.
  //
  string::size_type resLen = resolvedPath.length();
  if (resLen > 0 && resolvedPath[resLen - 1] == '/')
    resolvedPath.erase(resLen - 1);
  //
  //	Deal with each path component in turn.
  //
  while (p < length)
    {
      string::size_type pos = path.find('/', p);
      if (pos == string::npos)
	pos = length;
      string::size_type cLen = pos - p;
      //   cout << "cLen " << cLen << '\n';
      if (cLen == 0 || (cLen == 1 && path[p] == '.'))
	;  // ignore component
      else if (cLen == 2 && path[p] == '.' && path[p + 1] == '.')
	{
	  string::size_type backup = resolvedPath.rfind('/');
	  if (backup != string::npos)
	    resolvedPath.erase(backup);
	}
      else
	{
	  resolvedPath += '/';
	  resolvedPath += path.substr(p, cLen);
	}
      p = pos + 1;
    }
  if (resolvedPath.empty())
    resolvedPath = '/';
  //  cout << "out " << resolvedPath << '\n';
}

void
DirectoryManager::initialize()
{
  char buffer[MAXPATHLEN];
  const char* cwd = getenv("PWD");
  if (cwd == 0)
    {
      cwd = getcwd(buffer, MAXPATHLEN);  // really want to emulate GNU xgetcwd
      if (cwd == 0)
	cwd = "/";
    }
  directoryStack.append(directoryNames.encode(cwd));
}

bool
DirectoryManager::cd(const string& directory)
{
  if (chdir(directory.c_str()) != 0)
    return false;
  directoryStack[directoryStack.length() - 1] = directoryNames.encode(directory.c_str());
  return true;
}

int
DirectoryManager::pushd(const string& directory)
{
  int oldLength = directoryStack.length();
  if (directory.compare(".") == 0)
    {
      //
      //	If we didn't use a temporary we would have a really subtle
      //	memory problem since directoryStack[] returns a 
      //	reference which might be invalidated by the append() call
      //	before it is dereferenced.
      //
      int cwd = directoryStack[oldLength - 1];
      directoryStack.append(cwd);
    }
  else
    {
      if (chdir(directory.c_str()) != 0)
	oldLength = UNDEFINED;
      else
	directoryStack.append(directoryNames.encode(directory.c_str()));
    }
  return oldLength;
}

const char*
DirectoryManager::popd(int indexOfDirectoryToPop)
{
  //
  //	We pop the directory indexed by indexOfDirectoryToPop; or the directory
  //	currently on the top of the stack, if indexOfDirectoryToPop = UNDEFINED
  //	
  int top = directoryStack.length() - 1;
  //
  //	Check that indexOfDirectoryToPop isn't greater than index of the
  //	directory currently on the top of the stack.
  //
  if (indexOfDirectoryToPop > top)
    return 0;
  //
  //	The usual case is indexOfDirectoryToPop == UNDEFINED which is take to mean
  //	indexOfDirectoryToPop = "index of current top directory".
  //
  if (indexOfDirectoryToPop == UNDEFINED)
    indexOfDirectoryToPop = top;
  //
  //	If we have something on the stack we can popd to.
  //
  if (indexOfDirectoryToPop > 0)
    {
      int code = directoryStack[indexOfDirectoryToPop - 1];  // previous directory
      const char* dirName = directoryNames.name(code);
      if (code != directoryStack[top])  // only change directory if actually different
	{
	  if (chdir(dirName) != 0)  // failed
	    return 0;
	}
      directoryStack.contractTo(indexOfDirectoryToPop);
      return dirName;
    }
  return 0;
}

const char*
DirectoryManager::getCwd()
{
  return directoryNames.name(directoryStack[directoryStack.length() - 1]);
}

void
DirectoryManager::visitFile(const string& fileName)
{
  //
  //	Record a visit to a file, with the file's modification time.
  //
  struct stat buf;
  if (stat(fileName.c_str(), &buf) == 0)
    {
      pair<int, ino_t> id(directoryStack[directoryStack.length() - 1], buf.st_ino);
      visitedMap[id] = buf.st_mtime;
    }
}

bool
DirectoryManager::alreadySeen(const string& directory, const string& fileName)
{
  //
  //	Check if we previous visited a file, and the file is unchanged.
  //
  string full(directory + '/' + fileName);
  struct stat buf;
  if (stat(full.c_str(), &buf) == 0)
    {
      pair<int, ino_t> id(directoryNames.encode(directory.c_str()), buf.st_ino);
      VisitedMap::const_iterator i = visitedMap.find(id);
      if (i != visitedMap.end() && i->second == buf.st_mtime)
	{
	  DebugAdvisory("already seen " << full);
	  return true;
	}
    }
  return false;
}
