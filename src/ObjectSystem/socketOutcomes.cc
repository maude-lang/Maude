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

void
SocketManagerSymbol::errorReply(const char* errorMessage,
				FreeDagNode* originalMessage,
				ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, socketErrorMsg->makeDagNode(reply));
}


void
SocketManagerSymbol::createdSocketReply(int fd,
					FreeDagNode* originalMessage,
					ObjectSystemRewritingContext& context)
{
  activeSockets[fd].state = NOMINAL;
  Vector<DagNode*> reply(1, 3);
  reply[0] = succSymbol->makeNatDag(fd);
  DagNode* socketName = socketOidSymbol->makeDagNode(reply);
  context.addExternalObject(socketName, this);
  reply.resize(3);
  reply[2] = socketName;
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, createdSocketMsg->makeDagNode(reply));
}

void
SocketManagerSymbol::acceptedClientReply(const char* addr,
					 int fd,
					 FreeDagNode* originalMessage,
					 ObjectSystemRewritingContext& context)
{
  activeSockets[fd].state = NOMINAL;
  Vector<DagNode*> reply(1, 4);
  reply[0] = succSymbol->makeNatDag(fd);
  DagNode* socketName = socketOidSymbol->makeDagNode(reply);
  context.addExternalObject(socketName, this);
  reply.resize(4);
  reply[3] = socketName;
  reply[2] = new StringDagNode(stringSymbol, addr);
  reply[1] = originalMessage->getArgument(0);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, acceptedClientMsg->makeDagNode(reply));

}

void
SocketManagerSymbol::sentMsgReply(FreeDagNode* originalMessage,
				  ObjectSystemRewritingContext& context)
{
  Vector<DagNode*> reply(2);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  reply[1] = originalMessage->getArgument(0);
  context.bufferMessage(target, sentMsg->makeDagNode(reply));
}

void
SocketManagerSymbol::receivedMsgReply(char buffer[],
				      ssize_t length,
				      FreeDagNode* originalMessage,
				      ObjectSystemRewritingContext& context)
{
  crope text(buffer, length);
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, text);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, receivedMsg->makeDagNode(reply));
}

void
SocketManagerSymbol::closedSocketReply(int socketId,
				       const char* errorMessage,
				       FreeDagNode* originalMessage,
				       ObjectSystemRewritingContext& context)
{
  close(socketId);
  DagNode* socketName = originalMessage->getArgument(0);
  context.deleteExternalObject(socketName);
  activeSockets.erase(socketId);
  Vector<DagNode*> reply(3);
  reply[1] = socketName;
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, closedSocketMsg->makeDagNode(reply));
}
