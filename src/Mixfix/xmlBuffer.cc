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
//      Implementation for class XmlBuffer.
//

//      utility stuff
#include "macros.hh"

#include "xmlBuffer.hh"

XmlBuffer::XmlBuffer(ostream& output, int flushLevel)
  : output(output),
    flushLevel(flushLevel)
{
  output << "<?xml version=\"1.0\" encoding=\"US-ASCII\" standalone=\"yes\"?>\n";
  indentLevel = 0;
  startTagIncomplete = false;
}

XmlBuffer::~XmlBuffer()
{
  while (!(elements.empty()))
    endElement();
}

void
XmlBuffer::characterData(const string& charData)
{
  if (startTagIncomplete)
    {
      output << ">\n";
      startTagIncomplete = false;
    }
  output << charData;
}
void
XmlBuffer::comment(const string& text)
{
  if (startTagIncomplete)
    {
      output << ">\n";
      startTagIncomplete = false;
    }
  indent();
  output << "<!-- " << text << " -->\n";
}

void
XmlBuffer::beginElement(const string& name)
{
  if (startTagIncomplete)
    output << ">\n";
  indent();
  output << '<' << name;
  startTagIncomplete = true;
  ++indentLevel;
  elements.push(name);
}

void
XmlBuffer::endElement()
{
  Assert(!(elements.empty()), "empty element stack");

  --indentLevel;
  if (startTagIncomplete)
    {
      output << "/>\n";
      startTagIncomplete = false;
    }
  else
    {
      indent();
      output << "</" << elements.top() << ">\n";
    }
  if (indentLevel <= flushLevel)
    output.flush();
  elements.pop();
}

void
XmlBuffer::attributePair(const string& name, const string& value)
{
  output << ' ' << name << "=\"";
  translate(value);
  output << '"';
}

void
XmlBuffer::attributePair(const string& name, const string& value, int index)
{
  output << ' ' << name << "=\"";
  translate(value);
  output << index << '"';
}

void
XmlBuffer::indent()
{
  for (int i = indentLevel; i > 0 ; i--)
    output << ' ';
}

void
XmlBuffer::translate(const string& value)
{
  const string::const_iterator e = value.end();
  for (string::const_iterator i = value.begin(); i != e; ++i)
    {
      switch (*i)
	{
	case '<':
	  output << "&lt;";
	  break;
	case '&':
	  output << "&amp;";
	  break;
	case '"':
	  output << "&quot;";
	  break;
	default:
	  output << *i;
	}
    }
}
