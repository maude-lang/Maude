/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2020 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for socket manager symbols.
//
#ifndef _socketManagerSymbol_hh_
#define _socketManagerSymbol_hh_
#include <map>
#include "externalObjectManagerSymbol.hh"
#include "pseudoThread.hh"
#include "dagRoot.hh"
#include "rope.hh"

class SocketManagerSymbol
  : public ExternalObjectManagerSymbol,
    public PseudoThread
{
  NO_COPYING(SocketManagerSymbol);

public:
  SocketManagerSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);
  void copyAttachments(Symbol* original, SymbolMap* map);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes,
			    Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);
  //
  //	Overridden methods from PseudoThread.
  //
  void doRead(int fd);
  void doWrite(int fd);
  void doError(int fd);
  void doHungUp(int fd);
  //
  //	For other socket creators (our clients).
  //
  DagNode* manageSocket(int fd,
			bool disallowClose,
			bool readOnly,
			ObjectSystemRewritingContext& context);

private:
  enum Values
    {
      MAX_PORT_NUMBER = 0xFFFF
    };
  
  enum Sizes
    {
      READ_BUFFER_SIZE = 208 * 1024  // based on Linux default
    };
 
  enum SocketState
    {
      NOMINAL = 0,
      WAITING_TO_CONNECT= 1,
      WAITING_TO_READ = 2,
      WAITING_TO_WRITE = 4,
      LISTENING = 8,
      WAITING_TO_ACCEPT = 16
    };

  struct ActiveSocket
  {
    ActiveSocket();
    ~ActiveSocket();

    int state;
    bool disallowClose;
    bool readOnly;
    bool seenEOF;
    //
    //	If we are waiting to read, we keep a pointer to the message
    //	that prompted that wait, so that we can generate a response.
    //
    DagRoot lastReadMessage;  // must be protected from GC
    //
    //	If we are waiting to write, we keep a pointer to the message
    //	that prompted that wait so we can generate a response.
    //
    DagRoot lastWriteMessage;  // must be protected from GC
    //
    //	If we are waiting to read and/or write, we keep a pointer to
    //	the context in which the socket is represented as an external
    //	object so we can inject messages from the outside.
    //
    ObjectSystemRewritingContext* objectContext;
    //
    //	Outgoing text.
    //
    char* textArray;
    const char* unsent;
    Rope::size_type nrUnsent;
  };

  typedef map<int, ActiveSocket> SocketMap;

  bool getPort(DagNode* protocolArg, int& protocol);
  bool getActiveSocket(DagNode* socketArg, int& socketId, ActiveSocket*& asp);
  bool getText(DagNode* textArg, Rope& text);
  bool setNonblockingFlag(int fd, FreeDagNode* message, ObjectSystemRewritingContext& context);

  void errorReply(const char* errorMessage,
		  FreeDagNode* orignalMessage,
		  ObjectSystemRewritingContext& context);
  void createdSocketReply(int fd,
			  FreeDagNode* originalMessage,
			  ObjectSystemRewritingContext& context);
  void acceptedClientReply(const char* addr,
			   int fd,
			   FreeDagNode* originalMessage,
			   ObjectSystemRewritingContext& context);
  void sentMsgReply(FreeDagNode* originalMessage,
		    ObjectSystemRewritingContext& context);
  void receivedMsgReply(char buffer[],
			ssize_t length,
			FreeDagNode* originalMessage,
			ObjectSystemRewritingContext& context);
  void closedSocketReply(int socketId,
			 const char* errorMessage,
			 FreeDagNode* originalMessage,
			 ObjectSystemRewritingContext& context);

  void handleException(int fd, const char* errorText);

  bool createClientTcpSocket(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool createServerTcpSocket(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool acceptClient(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool send(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool receive(FreeDagNode* message, ObjectSystemRewritingContext& context);
  bool closeSocket(FreeDagNode* message, ObjectSystemRewritingContext& context);
  //
  //	Socket subsystem signature (generated by macro expansion).
  //
#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "socketSignature.cc"
#undef MACRO

  SocketMap activeSockets;
};

#endif
