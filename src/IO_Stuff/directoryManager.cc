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
//      Implementation for class DirectoryManager.
//

#include <sys/param.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>

//      utility stuff
#include "macros.hh"
#include "vector.hh"

#include "directoryManager.hh"

bool
DirectoryManager::checkAccess(const string& directory,
			      string& fileName,
			      int mode,
			      char const* const ext[])
{
  string full(directory + '/' + fileName);
  if (access(full.c_str(), mode) == 0)
    return true;
  if (ext != 0)
    {
      string::size_type d = fileName.rfind('.');
      if (d != string::npos)
	{
	  for (char const* const* p = ext; *p; p++)
	    {
	      //	      if (fileName.compare(d, string::npos, *p) == 0)
	      if (fileName.substr(d).compare(*p) == 0)  // HACK
		return false;  // already ends in one of our extensions
	    }
	}
      
      for (char const* const* p = ext; *p; p++)
	{
	  if (access((full + *p).c_str(), mode) == 0)
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
  string::size_type p = 0;
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
      //	memory problem since both directoryStack[] returns a 
      //	references which are might be invalidated by the append() call
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
DirectoryManager::popd(int prevLength)
{
  int top = directoryStack.length() - 1;
  if (prevLength > top)  // HACK for safety
    return 0;
  if (prevLength == UNDEFINED)
    prevLength = top;
  if (prevLength > 0)
    {
      int code = directoryStack[prevLength - 1];
      const char* dirName = directoryNames.name(code);
      if (code != directoryStack[top])
	chdir(dirName);
      directoryStack.contractTo(prevLength);
      return dirName;
    }
  return 0;
}

const char*
DirectoryManager::getCwd()
{
  return directoryNames.name(directoryStack[directoryStack.length() - 1]);
}
