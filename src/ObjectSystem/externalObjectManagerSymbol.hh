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
//	Abstract base class for external object mangager symbols.
//
#ifndef _ExternalObjectManagerSymbol_hh_
#define _ExternalObjectManagerSymbol_hh_
#include "freeSymbol.hh"

class ExternalObjectManagerSymbol : public FreeSymbol
{
  NO_COPYING(ExternalObjectManagerSymbol);

public:
  ExternalObjectManagerSymbol(int id);
  //
  //	This member function is used to handle messages sent to the
  //	manager object itself.
  //
  virtual bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context) = 0;
  //
  //	This member function is used to handle messages sent to the
  //	external objects created by the manager object.
  //
  virtual bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context) = 0;
  //
  //	This member function is used to quietly free up any resources
  //	allocated to an external object.
  //
  virtual void cleanUp(DagNode* objectId) = 0;

  void trivialReply(Symbol* replySymbol, 
		    FreeDagNode* originalMessage,
		    ObjectSystemRewritingContext& context);
};

#endif
