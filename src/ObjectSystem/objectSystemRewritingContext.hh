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
//      Class for object system rewriting contexts.
//
#ifndef _objectSystemRewritingContext_hh_
#define _objectSystemRewritingContext_hh_
#include "rewritingContext.hh"

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

  void setObjectMode(Mode m);
  Mode getObjectMode() const;

private:
  Mode mode;
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
