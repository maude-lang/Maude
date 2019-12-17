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

#define MAX_NR_ARGS	20

using namespace std;

struct Flags
{
  unsigned int sortIndex : 16;
  unsigned int evacuated : 1;
};

struct Node
{
  union
  {
    int symbol;
    Node* fwd;
  };
  Flags flags;
  Node* args[0];
};

union Link
{
  Link* l;
  int i;
  Node* n;
};

struct Context
{
  long long count;
  Link* safePtr;
  char* memNext;
  char* memEnd;
  int nrArgs;
  Node* args[MAX_NR_ARGS];
};

void collectGarbage();
int compare(Node*, Node*);
Node* eval(Node*);

extern Context g;
extern const char inFileName[];
extern const char outFileName[];
