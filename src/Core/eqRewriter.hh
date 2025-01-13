/*

    This file is part of the Maude 3 interpreter.

    Copyright 2025 SRI International, Menlo Park, CA 94025, USA.

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
//	Class for providing a replaceable eqRewrite() function.
//
#ifndef _eqRewriter_hh_
#define _eqRewriter_hh_

class EqRewriter
{
public:
  typedef bool (*EqRewriteFunctionPtr)(Symbol*, DagNode*, RewritingContext&);

  bool eqRewrite2(Symbol* symbol, DagNode* subject, RewritingContext& context);
  void setEqRewrite(EqRewriteFunctionPtr functionPtr);

private:
  static bool doNothing(Symbol* symbol, DagNode* subject, RewritingContext& context);

  EqRewriteFunctionPtr eqRewriteFunctionPtr = &doNothing;
};

inline bool
EqRewriter::eqRewrite2(Symbol* symbol, DagNode* subject, RewritingContext& context)
{
  return (*eqRewriteFunctionPtr)(symbol, subject, context);
}

inline void
EqRewriter::setEqRewrite(EqRewriteFunctionPtr functionPtr)
{
  eqRewriteFunctionPtr = functionPtr;
}

inline bool  // always called via a pointer; declared inline for header file
EqRewriter::doNothing(Symbol* /* symbol */, DagNode* /* subject */, RewritingContext& /* context */)
{
  return false;
}

#endif
