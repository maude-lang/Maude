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
//      Class to handle XML syntax and formatting.
//
#ifndef _xmlBuffer_hh_
#define _xmlBuffer_hh_
#include <string>
#include <stack>

class XmlBuffer
{
  NO_COPYING(XmlBuffer);

public:
  XmlBuffer(ostream& output, int flushLevel = NONE);
  ~XmlBuffer();

  void beginElement(const string& name);
  void endElement();
  void attributePair(const string& name, const string& value);
  void attributePair(const string& name, const string& value, int index);
  void characterData(const string& charData);
  void comment(const string& text);

private:
  void indent();
  void translate(const string& value);

  ostream& output;
  const int flushLevel;
  stack<string> elements;
  int indentLevel;
  bool startTagIncomplete;
};

#endif
