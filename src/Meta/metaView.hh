/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018 SRI International, Menlo Park, CA 94025, USA.

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
//      Class for views created at the metalevel.
//
#ifndef _metaView_hh_
#define _metaView_hh_
#include "view.hh"
#include "dagRoot.hh"

class MetaView : public View
{
  NO_COPYING(MetaView);

public:
  MetaView(Token name, DagNode* opMappings, DagNode* stratMappings, MetaLevel* metaLevel, Interpreter* owner);
  ~MetaView();

private:
  //
  //	Most of the work is done in View. We just handle op->term mappings.
  //
  bool handleTermAndExprMappings();

  DagRoot opMappings;
  DagRoot stratMappings;
  MetaLevel* const metaLevel;
};

#endif
