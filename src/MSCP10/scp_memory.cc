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

/************************************************
 ** SCP Parsing Algorithm                      **
 **  	Maude Implementation                   **
 **		Jose F. Quesada                **
 **		CSL - SRI Int.  1998           **
 ************************************************/

/*******
 ******* scp_memory.cc
 *******/

#include "scp_parser.hh"

ScpParser::ScpParser()
{
  compiled  = 0;

  lenprodtn = MEMPRODTN;
  memprodtn = new ScpProdtn[lenprodtn];
  curprodtn = 0;
  ldgrprodtn = 0;

  lenrhs    = MEMRHS;
  memrhs    = new int[lenrhs];
  currhs    = 0;

  lenbasent = MEMBASENT;
  membasent = new ScpBaseNT[lenbasent];
  memset(membasent,-1,lenbasent*sizeof(ScpBaseNT));
  maxbasent = 0;

  lenabslnt = MEMABSLNT;
  memabslnt = new ScpAbslNT[lenabslnt];
  memset(memabslnt,0,lenabslnt*sizeof(ScpAbslNT));
  maxabslnt = 0;

  lenbubble = MEMBUBBLE;
  membubble = new ScpBubble[lenbubble];
  curbubble = 0;

  lenexcept = MEMEXCEPT;
  memexcept = new int[lenexcept];
  curexcept = 0;

  maxtermn  = -1;

  addrtermn = 0;

  covtbl = 0;
  adjtb = 0;
  bubexcept = 0;

  memcov = 0;
  bubcov = 0;

  maxlenterm = MEMLENTERM;
  nodeterm  = new int[maxlenterm*5];
  eventterm = nodeterm+maxlenterm;
  inputterm = eventterm+maxlenterm;
  errorlist = inputterm+maxlenterm;
  errorlevel = errorlist+maxlenterm;

  lenevent  = MEMEVENT;
  memevent  = new ScpEvent[lenevent];

  lennode   = MEMNODE;
  memnode   = new ScpNode[lennode];
  memambnode = new int[lennode];
  prevambnode = 1; /* because memambnode hasn't been initialized */

  lenanal   = MEMANAL;
  memanal   = new ScpAnal[lenanal];

  lendetect = MEMDETECT;
  memdetect = new int[lendetect];
  typedetect = new int[lendetect];

  memalter  = 0;
  lenalter  = 0;

  memprodruntoken = 0;  // includes memprodrunnode
}

ScpParser::~ScpParser()
{
  delete [] memprodtn;
  delete [] ldgrprodtn;
  delete [] memrhs;
  delete [] membasent;
  delete [] memabslnt;
  delete [] membubble;
  delete [] memexcept;
  delete [] addrtermn;
  delete [] covtbl;
  delete [] adjtb;
  delete [] bubexcept;
  delete [] memcov;
  delete [] bubcov;
  delete [] nodeterm;
  delete [] memevent;
  delete [] memnode;
  delete [] memambnode;
  delete [] memanal;
  delete [] memprodruntoken;
  delete [] memdetect;
  delete [] typedetect;
  delete [] memalter;
}
