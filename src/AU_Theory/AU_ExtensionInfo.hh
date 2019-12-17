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
//	Class to record what part of an AU theory dag node was matched.
//
#ifndef _AU_ExtensionInfo_hh_
#define _AU_ExtensionInfo_hh_
#include "extensionInfo.hh"

class AU_ExtensionInfo : public ExtensionInfo
{
public:
  AU_ExtensionInfo(AU_DagNode* subject);
  DagNode* buildMatchedPortion() const;
  ExtensionInfo* makeClone() const;
  void copy(ExtensionInfo* extensionInfo);

  void setFirstMatched(int firstMatched);
  void setLastMatched(int lastMatched);
  void setExtraIdentity(bool flag);
  int firstMatched() const;
  int lastMatched() const;
  bool bigEnough() const;

private:
  AU_DagNode* subject;
  int first;
  int last;
  bool extraIdentityFlag;  // portion matched contains an identity not present in subject.
};

inline
AU_ExtensionInfo::AU_ExtensionInfo(AU_DagNode* subject)
  : subject(subject)
{
}

inline void
AU_ExtensionInfo::setFirstMatched(int firstMatched)
{
  first = firstMatched;
}

inline void
AU_ExtensionInfo::setLastMatched(int lastMatched)
{
  last = lastMatched;
  setMatchedWhole(first == 0 && last == subject->argArray.length() - 1);
}

inline void
AU_ExtensionInfo::setExtraIdentity(bool flag)
{
  extraIdentityFlag =  flag;
}

inline int
AU_ExtensionInfo::firstMatched() const
{
  return first;
}

inline int
AU_ExtensionInfo::lastMatched() const
{
  return last;
}

inline bool
AU_ExtensionInfo::bigEnough() const
{
  return last - first + 1 + extraIdentityFlag >= 2;
}

#endif
