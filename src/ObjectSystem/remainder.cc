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
//	Container for leftover args during object-message rewriting.
//

struct ConfigSymbol::Remainder : private SimpleRootContainer
{
  void dump(ostream& s, int indentLevel = 0);

  Vector<DagNode*> dagNodes;
  Vector<int> multiplicities;

private:
  void markReachableNodes();
};

void
ConfigSymbol::Remainder::markReachableNodes()
{
  FOR_EACH_CONST(i, Vector<DagNode*>, dagNodes)
    (*i)->mark();
}

void
ConfigSymbol::Remainder::dump(ostream& s, int indentLevel)
{
  s << Indent(indentLevel) << "begin{Remainder}\n";
  int nrRem = dagNodes.length();
  for (int i = 0; i < nrRem; ++i)
    {
      s << Indent(indentLevel + 1) << multiplicities[i] << '\t' <<
	dagNodes[i] << '\n';
    }
  s << Indent(indentLevel) << "end{Remainder}\n";
}
