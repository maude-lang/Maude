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
//	Class to generate sequence partitions associated with A and AU matching.
//
#ifndef _sequencePartition_hh_
#define _sequencePartition_hh_

class SequencePartition
{
public:
  SequencePartition(int sequenceLength, int estNrParts = 0);

  void insertPart(int minLength, int maxLength);
  bool solve();
  int start(int partNr) const;
  int end(int partNr) const;
  int partCount() const;

private:
  struct Part
  {
    int minLength;
    int maxLength;
    int sumPrevMin;
    int sumPrevMax;
    int start;
  };

  bool mainSolve(bool findFirst);

  const int sequenceLength;
  Vector<Part> parts;
  int minSum;
  int maxSum;
  bool closed;
  bool failed;
};

inline int
SequencePartition::start(int partNr) const
{
  Assert(closed, "solve() not called");
  Assert(!failed, "non-existent solution");
  return parts[partNr].start;
}

inline int
SequencePartition::end(int partNr) const
{
  Assert(closed, "solve() not called");
  Assert(!failed, "non-existent solution");
  return (partNr == parts.length() - 1) ?
    sequenceLength - 1 : parts[partNr + 1].start - 1;
}

inline int
SequencePartition::partCount() const
{
  return parts.length();
}

#endif

