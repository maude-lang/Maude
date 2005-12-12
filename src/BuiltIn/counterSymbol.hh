/*

    This file is part of the Maude 2 interpreter.

    Copyright 2004 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for symbols for built in operations on numbers.
//
#ifndef _counterSymbol_hh_
#define _counterSymbol_hh_
#include <list>
#include <stack>
#include "numberOpSymbol.hh"

class CounterSymbol : public NumberOpSymbol
{
public:
  CounterSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void resetRules();
  void saveHiddenState();
  void restoreHiddenState();
  bool eqRewrite(DagNode* subject, RewritingContext& context);
  DagNode* ruleRewrite(DagNode* subject, RewritingContext& context);

private:
  typedef stack<mpz_class, list<mpz_class> > MpzStack;  // optimize for the empty case

  mpz_class currentValue;
  MpzStack currentValueStack;
};

#endif
