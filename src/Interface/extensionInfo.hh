/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2010 SRI International, Menlo Park, CA 94025, USA.

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
//	Abstract base class for extension information.
//
#ifndef _extensionInfo_hh_
#define _extensionInfo_hh_

class ExtensionInfo
{
public:
  virtual ~ExtensionInfo() {}

  bool validAfterMatch() const;
  void setValidAfterMatch(bool status);

  bool matchedWhole() const;
  void setMatchedWhole(bool status);

  virtual DagNode* buildMatchedPortion() const = 0;
  virtual ExtensionInfo* makeClone() const = 0;		// for saving extension info
  virtual void copy(ExtensionInfo* extensionInfo) = 0;	// for restoring extension info

private:
  //
  //	The match phase records if extension info is valid after
  //	the match phase or do we need to wait until after
  //	the solve phase is successful.
  //
  bool validAfterMatchFlag;
  //
  //	Did we match the whole of the subject theory layer
  //	(extension is empty) or just part.
  //
  bool matchedWholeFlag;
};

inline bool
ExtensionInfo::validAfterMatch() const
{
  return validAfterMatchFlag;
}

inline void
ExtensionInfo::setValidAfterMatch(bool status)
{
  validAfterMatchFlag = status;
}

inline bool
ExtensionInfo::matchedWhole() const
{
   return matchedWholeFlag;
}

inline void
ExtensionInfo::setMatchedWhole(bool status)
{
  matchedWholeFlag = status;
}

#endif
