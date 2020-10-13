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
//      Class for object system rewriting contexts.
//
#ifndef _objectSystemRewritingContext_hh_
#define _objectSystemRewritingContext_hh_
#include <map>
#include <list>
#include "rewritingContext.hh"
#include "simpleRootContainer.hh"
#include "objectSystem.hh"

class ObjectSystemRewritingContext : public RewritingContext
{
  NO_COPYING(ObjectSystemRewritingContext);

public:
  enum Mode
  {
    STANDARD,	// use standard rewriting
    FAIR,	// use object-message fair rewriting
    EXTERNAL	// as above, but allow interactions with external objects
  };

  ObjectSystemRewritingContext(DagNode* root);
  ~ObjectSystemRewritingContext();

  void addExternalObject(DagNode* name, ExternalObjectManagerSymbol* manager);
  void deleteExternalObject(DagNode* name);
  void bufferMessage(DagNode* target, DagNode* message);

  bool getExternalMessages(DagNode* target, list<DagNode*>& messages);
  bool offerMessageExternally(DagNode* target, DagNode* message);

  void setObjectMode(Mode m);
  Mode getObjectMode() const;

  void externalRewrite();

protected:
  void markReachableNodes();

private:
  
  struct dagNodeLt
  {
    bool operator()(const DagNode* d1, const DagNode* d2) const
    {
      return d1->compare(d2) < 0;
    }
  };

  typedef map<DagNode*, ExternalObjectManagerSymbol*, dagNodeLt> ObjectMap;
  typedef map<DagNode*, list<DagNode*>, dagNodeLt> MessageMap;

  bool interleave();

  Mode mode;
  ObjectMap externalObjects;
  MessageMap incomingMessages;
};

inline
ObjectSystemRewritingContext::ObjectSystemRewritingContext(DagNode* root)
  : RewritingContext(root)
{
  mode = STANDARD;
}

inline void
ObjectSystemRewritingContext::setObjectMode(Mode m)
{
  mode = m;
}

inline ObjectSystemRewritingContext::Mode
ObjectSystemRewritingContext::getObjectMode() const
{
  return mode;
}

#endif
