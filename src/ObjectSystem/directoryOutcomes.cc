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

void
DirectoryManagerSymbol::errorReply(const char* errorMessage,
				   FreeDagNode* originalMessage,
				   ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, directoryErrorMsg->makeDagNode(reply));
}

void
DirectoryManagerSymbol::openedDirectoryReply(int fd,
					     FreeDagNode* originalMessage,
					     ObjectSystemRewritingContext& context)
{
  
  Vector<DagNode*> reply(1, 3);
  reply[0] = succSymbol->makeNatDag(fd);
  DagNode* directoryName = directoryOidSymbol->makeDagNode(reply);
  context.addExternalObject(directoryName, this);
  reply.resize(3);
  reply[2] = directoryName;
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, openedDirectoryMsg->makeDagNode(reply));
}

void
DirectoryManagerSymbol::gotDirectoryEntryReply(const Rope& name,
					       DagNode* typeDag,
					       FreeDagNode* originalMessage,
					       ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(4);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, name);
  reply[3] = typeDag;
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, gotDirectoryEntryMsg->makeDagNode(reply));
}
