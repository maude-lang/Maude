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
//	Class to record what part of an S_DagNode was not matched.
//
#ifndef _S_ExtensionInfo_hh_
#define _S_ExtensionInfo_hh_
#include <gmpxx.h>
#include "extensionInfo.hh"

class S_ExtensionInfo : public ExtensionInfo
{
public:
  S_ExtensionInfo(S_DagNode* subject);
  //
  //	Member functions required by theory interface.
  //
  DagNode* buildMatchedPortion() const;
  ExtensionInfo* makeClone() const;
  void copy(ExtensionInfo* extensionInfo);
  //
  //	S_ExtensionInfo specific member functions.
  //
  void setUnmatched(const mpz_class& number);
  const mpz_class& getUnmatched() const;

private:
  S_DagNode* subject;
  mpz_class unmatched;
};

inline void
S_ExtensionInfo::setUnmatched(const mpz_class& number)
{
  unmatched = number;
}

inline const mpz_class&
S_ExtensionInfo::getUnmatched() const
{
  return unmatched;
}

#endif
