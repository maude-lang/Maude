/*

    This file is part of the Maude 3 interpreter.

    Copyright 2021 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to handle directory manager symbols.
//
#ifndef _directoryManagerSymbol_hh_
#define _directoryManagerSymbol_hh_
#include <sys/types.h>
#include <dirent.h>
#include <map>
#include "externalObjectManagerSymbol.hh"

class DirectoryManagerSymbol : public ExternalObjectManagerSymbol
{
  NO_COPYING(DirectoryManagerSymbol);

public:
  DirectoryManagerSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);
  
  static void setAllowDir(bool flag);

private:
  struct OpenDirectory
  {
    Rope path;
    DIR* dir;
  };

  typedef map<int, OpenDirectory> DirectoryMap;
  //
  //	Shared flag to disable the functionality of this class.
  //
  static bool allowDir;

  void getOpenDirectory(DagNode* directoryArg, int& fd, OpenDirectory*& ofp);
  void handleSymbolicLink(Rope path,
			  Rope name,
			  FreeDagNode* message,
			  ObjectSystemRewritingContext& context);

  void openDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void makeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void removeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void getDirectoryEntry(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void closeDirectory(FreeDagNode* message, ObjectSystemRewritingContext& context);

  void errorReply(const char* errorMessage,
		  FreeDagNode* originalMessage,
		  ObjectSystemRewritingContext& context);
  void openedDirectoryReply(int fd,
			    FreeDagNode* originalMessage,
			    ObjectSystemRewritingContext& context);
  void gotDirectoryEntryReply(const Rope& name,
			      DagNode* typeDag,
			      FreeDagNode* originalMessage,
			      ObjectSystemRewritingContext& context);

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "directorySignature.cc"
#undef MACRO

  DirectoryMap openDirectories;
};

inline void
DirectoryManagerSymbol::setAllowDir(bool flag)
{
  allowDir = flag;
}

#endif
