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
 ******* scp_compiler.cc
 *******/

#include "scp_parser.hh"

static int auxntm;
static int auxpbase;

void ScpParser::insertProd(int xlhs,  const Vector<int>& xrhs,
                           int xprec, const Vector<int>& xgather)
{
  register int regrhs;
  register int reggather;
  register int regcounter;
  curprodtn ++;

  Assert(!compiled,
	 "SCP-ERROR: call to insertProd after parseSentence" << endl);

  Assert(xlhs < 0, 
	 "SCP-ERROR: Bad LHS (" << xlhs << ") " <<
	 "in production " << curprodtn-1 << endl);
  Assert(xrhs.length() > 0, 
	 "SCP-ERROR: RHS empty " <<
	 "in production " << curprodtn-1 << endl);

  if (curprodtn >= lenprodtn) {
    ScpProdtn* xmemprodtn = new ScpProdtn[lenprodtn+ADDPRODTN];
    memcpy(xmemprodtn,memprodtn,lenprodtn*sizeof(ScpProdtn));
    memset(xmemprodtn+lenprodtn,0,ADDPRODTN*sizeof(ScpProdtn));
    lenprodtn += ADDPRODTN;
    delete [] memprodtn;
    memprodtn = xmemprodtn;
  }

  ScpProdtn* prodtn = &memprodtn[curprodtn];

  prodtn->lhs     = insertBasePrecNT(xlhs,xprec);
  memabslnt[-prodtn->lhs].lhsdefs ++;

  regrhs       	= xrhs.length();
  prodtn->lrhs    = regrhs;
  if (currhs+regrhs > lenrhs) {
    int* xmemrhs = new int[lenrhs+regrhs+ADDRHS];
    memcpy(xmemrhs,memrhs,lenrhs*sizeof(int));
    lenrhs += regrhs+ADDRHS;
    delete [] memrhs;
    memrhs = xmemrhs;
  }
  prodtn->rhs = currhs;
  reggather       = 0;
  for (regcounter=0; regrhs; regrhs--) {
    if (xrhs[regcounter] < 0) {
      Assert(reggather < xgather.length(),
	     "SCP-ERROR: Insufficient gather values " <<
	     "in production " << curprodtn-1 << endl);
      memrhs[currhs++] = insertBasePrecNT(xrhs[regcounter++],
					  xgather[reggather++]);
    } else {
      if (xrhs[regcounter] > maxtermn)
	maxtermn = xrhs[regcounter];
      memrhs[currhs++] = xrhs[regcounter++];
    }
  }
  prodtn->lgather = reggather;
  prodtn->bubble = 0;
}

int ScpParser::insertBasePrecNT(int baseNT,int precNT)
{
  if (baseNT < maxbasent) {
    maxbasent = baseNT;
    if (-baseNT >= lenbasent) {
      ScpBaseNT* xmembasent = new ScpBaseNT[-baseNT+ADDBASENT];
      memcpy(xmembasent,membasent,lenbasent*sizeof(ScpBaseNT));
      memset(xmembasent+lenbasent,-1,
	     (-baseNT+ADDBASENT-lenbasent)*sizeof(ScpBaseNT));
      lenbasent = -baseNT+ADDBASENT;
      delete [] membasent;
      membasent = xmembasent;
    }
  }

  if (membasent[-baseNT].smallestPrec == -1) {
    membasent[-baseNT].smallestPrec = precNT;
    membasent[-baseNT].largestPrec  = precNT;
    maxabslnt--;
    if (-maxabslnt >= lenabslnt) {
      ScpAbslNT* xmemabslnt = new ScpAbslNT[lenabslnt+ADDABSLNT];
      memcpy(xmemabslnt,memabslnt,lenabslnt*sizeof(ScpAbslNT));
      memset(xmemabslnt+lenabslnt,0,ADDABSLNT*sizeof(ScpAbslNT));
      lenabslnt += ADDABSLNT;
      delete [] memabslnt;
      memabslnt = xmemabslnt;
    }
    memabslnt[-maxabslnt].baseNT = baseNT;
    memabslnt[-maxabslnt].precNT = precNT;
    memabslnt[-maxabslnt].nextNT = 0;
    membasent[-baseNT].firstAbs  = maxabslnt;
    membasent[-baseNT].lastAbs   = maxabslnt;
    return maxabslnt;
  } else if (precNT < membasent[-baseNT].smallestPrec) {
    membasent[-baseNT].smallestPrec = precNT;
    maxabslnt--;
    if (-maxabslnt >= lenabslnt) {
      ScpAbslNT* xmemabslnt = new ScpAbslNT[lenabslnt+ADDABSLNT];
      memcpy(xmemabslnt,memabslnt,lenabslnt*sizeof(ScpAbslNT));
      memset(xmemabslnt+lenabslnt,0,ADDABSLNT*sizeof(ScpAbslNT));
      lenabslnt += ADDABSLNT;
      delete [] memabslnt;
      memabslnt = xmemabslnt;
    }
    memabslnt[-maxabslnt].baseNT = baseNT;
    memabslnt[-maxabslnt].precNT = precNT;
    memabslnt[-maxabslnt].nextNT = membasent[-baseNT].firstAbs;
    membasent[-baseNT].firstAbs  = maxabslnt;
    return maxabslnt;
  } else if (precNT > membasent[-baseNT].largestPrec) {
    membasent[-baseNT].largestPrec = precNT;
    maxabslnt--;
    if (-maxabslnt >= lenabslnt) {
      ScpAbslNT* xmemabslnt = new ScpAbslNT[lenabslnt+ADDABSLNT];
      memcpy(xmemabslnt,memabslnt,lenabslnt*sizeof(ScpAbslNT));
      memset(xmemabslnt+lenabslnt,0,ADDABSLNT*sizeof(ScpAbslNT));
      lenabslnt += ADDABSLNT;
      delete [] memabslnt;
      memabslnt = xmemabslnt;
    }
    memabslnt[-maxabslnt].baseNT = baseNT;
    memabslnt[-maxabslnt].precNT = precNT;
    memabslnt[-maxabslnt].nextNT = 0;
    memabslnt[-membasent[-baseNT].lastAbs].nextNT = maxabslnt;
    membasent[-baseNT].lastAbs = maxabslnt;
    return maxabslnt;
  } else if (precNT == membasent[-baseNT].smallestPrec) {
    return membasent[-baseNT].firstAbs;
  } else if (precNT == membasent[-baseNT].largestPrec) {
    return membasent[-baseNT].lastAbs;
  } else {
    int curAbs = membasent[-baseNT].firstAbs;
    int nexAbs = memabslnt[-curAbs].nextNT;
    while (precNT >= memabslnt[-nexAbs].precNT) {
      if (precNT == memabslnt[-nexAbs].precNT) {
	return nexAbs;
      }
      curAbs = nexAbs;
      nexAbs = memabslnt[-curAbs].nextNT;
    }
    maxabslnt--;
    if (-maxabslnt >= lenabslnt) {
      ScpAbslNT* xmemabslnt = new ScpAbslNT[lenabslnt+ADDABSLNT];
      memcpy(xmemabslnt,memabslnt,lenabslnt*sizeof(ScpAbslNT));
      memset(xmemabslnt+lenabslnt,0,ADDABSLNT*sizeof(ScpAbslNT));
      lenabslnt += ADDABSLNT;
      delete [] memabslnt;
      memabslnt = xmemabslnt;
    }
    memabslnt[-maxabslnt].baseNT = baseNT;
    memabslnt[-maxabslnt].precNT = precNT;
    memabslnt[-maxabslnt].nextNT = nexAbs;
    memabslnt[-curAbs].nextNT = maxabslnt;
    return maxabslnt;
  }
}

int ScpParser::getBaseNT(int absl)
{
  return memabslnt[-absl].baseNT;
}

int ScpParser::getPrecNT(int absl)
{
  return memabslnt[-absl].precNT;
}


void ScpParser::insertProd( int nonTerminal,int lowBound,int upperBound,
			    int leftParenToken,int rightParenToken,
			    const Vector<int>& exceptionTokenList )
{
  register int reglength;
  register int regcounter;
  curprodtn ++;

  Assert(!compiled,
	 "SCP-ERROR: call to insertProd after parseSentence" << endl);

  Assert(nonTerminal < 0,
	 "SCP-ERROR: Bad nonTerminal(bubble) (" << 
	 nonTerminal << ") " <<
	 "in production " << curprodtn << endl);
  if (curprodtn >= lenprodtn) {  // changed from > to fix bug 3/8/02 SME
    ScpProdtn* xmemprodtn = new ScpProdtn[lenprodtn+ADDPRODTN];
    memcpy(xmemprodtn,memprodtn,lenprodtn*sizeof(ScpProdtn));
    memset(xmemprodtn+lenprodtn,0,ADDPRODTN*sizeof(ScpProdtn));
    lenprodtn += ADDPRODTN;
    delete [] memprodtn;
    memprodtn = xmemprodtn;
  }

  ScpProdtn* prodtn = &memprodtn[curprodtn];

  // Store the bubble in membubble
  curbubble ++;
  prodtn->bubble    = curbubble;
  prodtn->lgather   = 0;

  if (curbubble > lenbubble) {
    ScpBubble* xmembubble = new ScpBubble[lenbubble+ADDBUBBLE];
    memcpy(xmembubble,membubble,lenbubble*sizeof(ScpBubble));
    lenbubble += ADDBUBBLE;
    delete [] membubble;
    membubble = xmembubble;
  }
  ScpBubble* bubble = &membubble[curbubble];
  bubble->abslnt  = insertBasePrecNT(nonTerminal,0);
  memabslnt[-bubble->abslnt].bubbletermn = curbubble;
  bubble->lbound  = lowBound;
  bubble->ubound  = upperBound;
  if (leftParenToken > maxtermn)
    maxtermn = leftParenToken;
  bubble->lparen  = leftParenToken;
  if (rightParenToken > maxtermn)
    maxtermn = rightParenToken;
  bubble->rparen  = rightParenToken;
  bubble->prodtn  = curprodtn;

  reglength	= exceptionTokenList.length();
  bubble->lexcept = reglength;
  if (curexcept+reglength > lenexcept) {
    int* xmemexcept = new int[lenexcept+ADDEXCEPT];
    memcpy(xmemexcept,memexcept,lenexcept*sizeof(int));
    lenexcept += ADDEXCEPT;
    delete [] memexcept;
    memexcept = xmemexcept;
  }
  bubble->except = curexcept;
  for (regcounter=0; reglength; reglength--) {
    if (exceptionTokenList[regcounter] > maxtermn)
      maxtermn = exceptionTokenList[regcounter];
    memexcept[curexcept++] = exceptionTokenList[regcounter++];
  }
}

void ScpParser::compileParser()
{
  if (compiled)
    return;

  compiled = 1;
  maxtermn ++;	// 0-maxtermn => maxtermn+1
  maxnontm = -maxabslnt+1;

  /* Prod 0 */
  memprodtn[0].lrhs = 2;
  memprodtn[0].rhs = currhs;
  memprodtn[0].lgather = 1;
  /*
    We seem to have MEMORY CORRUPTION when currhs == lenrhs or
    currhs+1 == lenrhs because the memrhs is not being reallocated.

  cerr << memprodtn[0].rhs << '\t' << currhs << '\t' << lenrhs << endl;

    We add the following lines to fix this problem. 3/20/02 SME
  */
  const int extrarhs = 2;
  if (currhs+extrarhs > lenrhs) {
    int* xmemrhs = new int[lenrhs+extrarhs+ADDRHS];
    memcpy(xmemrhs,memrhs,lenrhs*sizeof(int));
    lenrhs += extrarhs+ADDRHS;
    delete [] memrhs;
    memrhs = xmemrhs;
  }
  /*
    End of added code.
  */
  memrhs[memprodtn[0].rhs+1] = maxtermn + 10;  // MEMORY CORRUPTION
  memprodtn[0].bubble = 0;

  /*            covnttb    ldertmtb   ldernttb   rdernttb   invrdernttb */
  /*                       covtmtb                                      */
  int lentbl = (maxnontm + maxtermn + maxnontm + maxnontm + 1) * (maxnontm);
  covtbl = new int[lentbl];
  memset(covtbl,0,lentbl*sizeof(int));
  covnttb  = covtbl;
  ldertmtb = covtmtb = covnttb + (maxnontm*maxnontm);
  ldernttb = ldertmtb + (maxtermn*maxnontm);
  rdernttb = ldernttb + (maxnontm*maxnontm);
  invrdernttb = rdernttb + (maxnontm*maxnontm);

  /*        rdernttb   adjtb */
  lentbl = (maxnontm + maxtermn) * (maxnontm);
  adjtb = new char[lentbl];
  memset(adjtb,0,lentbl*sizeof(char));

  if (curbubble) {
    lentbl = curbubble * maxtermn;
    bubexcept = new char[lentbl];
    memset(bubexcept,0,lentbl*sizeof(char));
  }

  compileDerivationTables();
  compileCoverageTables();
  compileAdjacencyTables();
  
  memprodruntoken = new int[(curprodtn+1)*2];
  memprodrunnode = memprodruntoken+(curprodtn+1);
}

void ScpParser::compileDerivationTables()
{
  register int tm;
  register int addrbase = 0;

  addrtermn = new int[maxtermn+maxnontm];
  addrnontm = addrtermn+maxtermn;
  for (tm = 0; tm < maxtermn; tm++,addrbase+=maxnontm)
    addrtermn[tm] = addrbase;

  register int ntm;
  addrbase = maxnontm;
  for (ntm = 1; ntm < maxnontm; ntm++,addrbase+=maxnontm) 
    addrnontm[ntm] = addrbase;

  /*            ldgrprodtn      ldgrabslnt ldgrtermn */
  /*            rdgrprodtn      rdgrabslnt rdgrtermn */
  int lentbl = ((curprodtn+1) + maxnontm + maxtermn) * 2;

  ldgrprodtn = new int[lentbl];

  memset(ldgrprodtn,0,(lentbl)*sizeof(int));
  ldgrabslnt = ldgrprodtn + (curprodtn+1);
  ldgrtermn  = ldgrabslnt + maxnontm;
  rdgrprodtn = ldgrtermn  + maxtermn;
  rdgrabslnt = rdgrprodtn + (curprodtn+1);
  rdgrtermn  = rdgrabslnt + maxnontm;

  compileLDerGraph();
  compileLDerTable();
  compileRDerGraph();
  compileRDerTable();
}

void ScpParser::compileLDerGraph()
{
  register int ip;
  register int rhs0;
  register int lhs;
  register int ldergraph;
  register int prevldergraph;
  register int addedldergraph;
  ScpProdtn* prodtn;

  for (ip = 1; ip <= curprodtn; ip++) {
    prodtn = &memprodtn[ip];
    if (!prodtn->bubble) {
      rhs0 = memrhs[prodtn->rhs];
      if (rhs0 < 0) {
	// lhs  = memrhs[prodtn->lhs];
	lhs = prodtn->lhs;
	ldergraph = ldgrabslnt[-rhs0]; 
	if (!ldergraph) 
	  ldgrabslnt[-rhs0] = ip;
	else {
	  if (lhs == memprodtn[ldergraph].lhs) { 
	    ldgrprodtn[ip] = ldergraph;
	    ldgrabslnt[-rhs0]  = ip; 
	  }  else if (lhs > memprodtn[ldergraph].lhs) {
	    ldgrprodtn[ip] = ldergraph;
	    ldgrabslnt[-rhs0] = ip;
	  } else {
	    prevldergraph = ldergraph;
	    addedldergraph = 0;
	    ldergraph = ldgrprodtn[ldergraph];
	    while (!addedldergraph && ldergraph) {
	      if (lhs == memprodtn[ldergraph].lhs) { 
		ldgrprodtn[ip]  = ldergraph;
		ldgrprodtn[prevldergraph] = ip;
		addedldergraph = 1;
	      } else if (lhs > memprodtn[ldergraph].lhs) {
		ldgrprodtn[ip] = ldergraph;
		ldgrprodtn[prevldergraph] = ip;
		addedldergraph = 1;
	      } else {
		prevldergraph = ldergraph;
		ldergraph = ldgrprodtn[ldergraph];
	      }
	    }
	    if (!addedldergraph)
	      ldgrprodtn[prevldergraph] = ip;
	  }
	}
      } else {
	// lhs  = memrhs[prodtn->lhs];
	lhs = prodtn->lhs;
	ldergraph = ldgrtermn[rhs0]; 
	if (!ldergraph) {
	  ldgrtermn[rhs0] = ip;
	} else {
	  if (lhs == memprodtn[ldergraph].lhs) { 
	    ldgrprodtn[ip] = ldergraph;
	    ldgrtermn[rhs0] =  ip;
	  } else if (lhs > memprodtn[ldergraph].lhs) {
	    ldgrprodtn[ip] = ldergraph;
	    ldgrtermn[rhs0] = ip;
	  } else {
	    prevldergraph = ldergraph;
	    addedldergraph = 0;
	    ldergraph = ldgrprodtn[ldergraph];
	    while (!addedldergraph && ldergraph) {
	      if (lhs == memprodtn[ldergraph].lhs) {
		ldgrprodtn[ip] = ldergraph;
		ldgrprodtn[prevldergraph] = ip;
		addedldergraph = 1;
	      } else if (lhs > memprodtn[ldergraph].lhs) {
		ldgrprodtn[ip] = ldergraph;
		ldgrprodtn[prevldergraph] = ip;
		addedldergraph = 1;
	      } else {
		prevldergraph = ldergraph;
		ldergraph = ldgrprodtn[ldergraph];
	      }
	    }
	    if (!addedldergraph)
	      ldgrprodtn[prevldergraph] = ip;
	  }
	}
      }
    }
  }
}

void ScpParser::compileLDerTable()
{
  for (auxntm = 1,auxpbase = maxnontm; auxntm < maxnontm; auxntm++,auxpbase+=maxnontm) 
    insertLDerNT(auxntm);
}

void ScpParser::insertLDerNT(int lder)
{
  int curnt = lder;
  while (curnt) {
    if (!ldernttb[auxpbase+curnt]) {
      ldernttb[auxpbase+curnt] = ldernttb[auxpbase]+1;
      ldernttb[auxpbase] = curnt;
      ldernttb[auxntm]++;
      int ldergraph = ldgrabslnt[curnt];
      while (ldergraph) {
	if (!ldernttb[auxpbase-memprodtn[ldergraph].lhs])
	  insertLDerNT(-memprodtn[ldergraph].lhs);
	ldergraph = ldgrprodtn[ldergraph];
      }
    }
    curnt = -memabslnt[curnt].nextNT;
  }
}

void ScpParser::compileRDerGraph()
{
  register int ip;
  register int rhsN;
  register int lhs;
  register int rdergraph;
  register int prevrdergraph;
  register int addedrdergraph;
  ScpProdtn* prodtn;

  for (ip = 1; ip <= curprodtn; ip++) {
    prodtn = &memprodtn[ip];
    if (!prodtn->bubble) {
      rhsN = memrhs[prodtn->rhs+prodtn->lrhs-1];
      if (rhsN < 0) {
	//	lhs  = memrhs[prodtn->lhs];
	lhs = prodtn->lhs;
	rdergraph = rdgrabslnt[-rhsN]; 
	if (!rdergraph) 
	  rdgrabslnt[-rhsN] = ip;
	else {
	  if (lhs == memprodtn[rdergraph].lhs)
	    ;
	  else if (lhs > memprodtn[rdergraph].lhs) {
	    rdgrprodtn[ip] = rdergraph;
	    rdgrabslnt[-rhsN] = ip;
	  } else {
	    prevrdergraph = rdergraph;
	    addedrdergraph = 0;
	    rdergraph = rdgrprodtn[rdergraph];
	    while (!addedrdergraph && rdergraph) {
	      if (lhs == memprodtn[rdergraph].lhs)
		addedrdergraph = 1;
	      else if (lhs > memprodtn[rdergraph].lhs) {
		rdgrprodtn[ip] = rdergraph;
		rdgrprodtn[prevrdergraph] = ip;
		addedrdergraph = 1;
	      } else {
		prevrdergraph = rdergraph;
		rdergraph = rdgrprodtn[rdergraph];
	      }
	    }
	    if (!addedrdergraph)
	      rdgrprodtn[prevrdergraph] = ip;
	  }
	}
      } else {
	//	lhs  = memrhs[prodtn->lhs];
	lhs = prodtn->lhs;
	rdergraph = rdgrtermn[rhsN]; 
	if (!rdergraph) 
	  rdgrtermn[rhsN] = ip;
	else {
	  if (lhs == memprodtn[rdergraph].lhs)
	    ;
	  else if (lhs > memprodtn[rdergraph].lhs) {
	    rdgrprodtn[ip] = rdergraph;
	    rdgrtermn[rhsN] = ip;
	  } else {
	    prevrdergraph = rdergraph;
	    addedrdergraph = 0;
	    rdergraph = rdgrprodtn[rdergraph];
	    while (!addedrdergraph && rdergraph) {
	      if (lhs == memprodtn[rdergraph].lhs)
		addedrdergraph = 1;
	      else if (lhs > memprodtn[rdergraph].lhs) {
		rdgrprodtn[ip] = rdergraph;
		rdgrprodtn[prevrdergraph] = ip;
		addedrdergraph = 1;
	      } else {
		prevrdergraph = rdergraph;
		rdergraph = rdgrprodtn[rdergraph];
	      }
	    }
	    if (!addedrdergraph)
	      rdgrprodtn[prevrdergraph] = ip;
	  }
	}
      }
    }
  }
}

void ScpParser::compileRDerTable()
{
  for (auxntm = 1,auxpbase = maxnontm; auxntm < maxnontm; auxntm++,auxpbase+=maxnontm) 
    insertRDerNT(auxntm);
}

void ScpParser::insertRDerNT(int rder)
{
  int curnt = rder;
  while (curnt) {
    if (!rdernttb[auxpbase+curnt]) {
      rdernttb[auxpbase+curnt] = rdernttb[curnt]+1;
      rdernttb[curnt] = auxntm;
      int rdergraph = rdgrabslnt[curnt];
      while (rdergraph) {
	if (!rdernttb[auxpbase-memprodtn[rdergraph].lhs])
	  insertRDerNT(-memprodtn[rdergraph].lhs);
	rdergraph = rdgrprodtn[rdergraph];
      }
    }
    curnt = -memabslnt[curnt].nextNT;
  }
}

void ScpParser::compileCoverageTables()
{
  register int ltcov = 1;
  register int ntm;
  register int addrbase = 0;

  for (ntm = 1; ntm < maxnontm; ntm++) 
    ltcov += memabslnt[ntm].lhsdefs * ldernttb[ntm];

  register int bub;
  for (bub = 1; bub <= curbubble; bub++) 
    ltcov += ldernttb[-membubble[bub].abslnt];

  memcov = new ScpCov[ltcov];

  register int last;
  ltcov = 0;

  register int xldgr;
  register int addrbaset;
  register int lhs;

  for (ntm = 1; ntm < maxnontm; ntm++) {
    if (ldgrabslnt[ntm]) {
      xldgr = ldgrabslnt[ntm];
      addrbaset = addrnontm[ntm];
      while (xldgr) {
	lhs = -memprodtn[xldgr].lhs;
	addrbase = addrnontm[lhs];
	last = ldernttb[addrbase];
	while (last) {
	  ltcov ++;
	  memcov[ltcov].prodtn = xldgr;
	  memcov[ltcov].next  = covnttb[addrbaset+last];
	  covnttb[addrbaset+last] = ltcov;
	  last = ldernttb[addrbase+last]-1;
	}
	xldgr = ldgrprodtn[xldgr];
      }
    }
  }

  register int tm;
  for (tm = 0; tm < maxtermn; tm++) {
    if (ldgrtermn[tm]) {
      xldgr  = ldgrtermn[tm];
      addrbaset = addrtermn[tm];
      while (xldgr) {
	lhs = -memprodtn[xldgr].lhs;
	addrbase = addrnontm[lhs];
	last = ldernttb[addrbase];
	while (last) {
	  ltcov ++;
	  memcov[ltcov].prodtn = xldgr;
	  if (covtmtb[addrbaset+last]) {
	    memcov[ltcov].ldernext = memcov[covtmtb[addrbaset+last]].ldernext;
	    memcov[ltcov].next  = covtmtb[addrbaset+last];
	  } else {
	    memcov[ltcov].ldernext = invrdernttb[last];
	    memcov[ltcov].next = 0;
	  }
	  invrdernttb[last] = tm + 1;
	  covtmtb[addrbaset+last] = ltcov;
	  last = ldernttb[addrbase+last]-1;
	}
	xldgr = ldgrprodtn[xldgr];
      }
    }
  }

  if (!curbubble)
    return;

  int xbubexcept = 0;
  for (bub = 1; bub <= curbubble; bub++) {
    int abslnt = -membubble[bub].abslnt;
    addrbase = addrnontm[abslnt];
    last = ldernttb[addrbase];
    while (last) {
      ltcov ++;
      memcov[ltcov].prodtn = membubble[bub].prodtn;
      memcov[ltcov].next  = memabslnt[last].bubblecov;
      memabslnt[last].bubblecov = ltcov;
      last = ldernttb[addrbase+last]-1;
    }
    membubble[bub].addrbubexcept = xbubexcept;
    int lexcept = membubble[bub].lexcept;
    int except = membubble[bub].except;
    for (int ex=0; ex < lexcept; ex++)
      bubexcept[xbubexcept+memexcept[except+ex]] = (char)1;
    xbubexcept += maxtermn;

    abslnt = -memabslnt[abslnt].nextNT;
    while (abslnt) {
      memabslnt[abslnt].bubbletermn = 1;
      abslnt = -memabslnt[abslnt].nextNT;
    }
  }

  int lenbubcov = MEMBUBCOV;
  bubcov = new ScpCov[lenbubcov];
  int curbubcov = 0;

  register int pro;
  for (pro = 1; pro <= curprodtn; pro++) {
    if (!memprodtn[pro].bubble &&
	((ntm = memrhs[memprodtn[pro].rhs]) < 0) && 
	(memabslnt[-ntm].bubblecov)) {
      ntm = -ntm;
      if (ldgrabslnt[ntm]) {
	int bcov = memabslnt[ntm].bubblecov;
	while (bcov) {
	  xldgr = ldgrabslnt[ntm];
	  addrbaset = addrnontm[ntm];
	  while (xldgr) {
	    lhs = -memprodtn[xldgr].lhs;
	    addrbase = addrnontm[lhs];
	    last = ldernttb[addrbase];
	    while (last) {
	      curbubcov++;
	      if (curbubcov >= lenbubcov) {
		ScpCov* xbubcov = new ScpCov[lenbubcov+ADDBUBCOV];
		memcpy(xbubcov,bubcov,lenbubcov*sizeof(ScpCov));
		lenbubcov += ADDBUBCOV;
		delete [] bubcov;
		bubcov = xbubcov;
	      }
	      int found = 0;
	      int blastcov = memabslnt[last].bubbleev;
	      while (!found && blastcov) {
		if (bubcov[blastcov].prodtn == pro)
		  found = 1;
		else
		  blastcov =  bubcov[blastcov].next;
	      }
	      if (!found) {
		bubcov[curbubcov].prodtn = pro;
		bubcov[curbubcov].next  = memabslnt[last].bubbleev;
		memabslnt[last].bubbleev = curbubcov;
	      }
	      last = ldernttb[addrbase+last]-1;
	    }
	    xldgr = ldgrprodtn[xldgr];
	  }
	  bcov = memcov[bcov].next;
	}
      }
    }
  }
}

void ScpParser::compileAdjacencyTables()
{
  register int ip;
  register int rhs;
  ScpProdtn* prodtn;
  register int rhsN0;
  register int rhsN1;
  register int addrbase;
  int *rhsN;

  for (ip = 1; ip <= curprodtn; ip++) {
    prodtn = &memprodtn[ip];
    if (!prodtn->bubble && (prodtn->lrhs > 1)) {
      rhsN = &memrhs[prodtn->rhs];
      for (rhs = 1; rhs < prodtn->lrhs; rhs++) {
	if (*rhsN < 0) {
	  rhsN0 = -(*rhsN);
	  rhsN1 = *(rhsN+1);
	  if (rhsN1 >= 0) {
	    addrbase = addrtermn[rhsN1];
	    if (!adjtb[addrbase+rhsN0]) {
	      //      adjtb[addrbase+rhsN0] = 1;
	      insertAdjTM(addrbase,rhsN0);
	    }
	  } else {
	    if (memabslnt[-rhsN1].bubblecov) { 
	      int xnext = -membasent[-memabslnt[rhsN0].baseNT].firstAbs;
	      while (xnext != rhsN0) {
		memabslnt[xnext].adjbubble = 1;
		xnext = -memabslnt[xnext].nextNT;
	      }
	      memabslnt[rhsN0].adjbubble = 1;
	    }
	    insertAdjNT(-rhsN1,rhsN0);
	  }
	}
	rhsN++;
      }
    }
  }
}

void ScpParser::insertAdjTM(int pbase,int adj)
{
  int last = rdernttb[adj];
  while (last) {
    adjtb[pbase+last] = 1;
    last = rdernttb[addrnontm[last]+adj]-1;
  }
}
  
void ScpParser::insertAdjNT(int base,int adj)
{
  int last = invrdernttb[base];
  while (last) {
    if (!adjtb[addrtermn[last-1]+adj]) 
      insertAdjTM(addrtermn[last-1],adj);
    last = memcov[ldertmtb[addrtermn[last-1]+base]].ldernext;
  }
}

void ScpParser::printGrammar()
{
  cout << "============================" << endl;
  cout << "Grammar" << endl;
  cout << "============================" << endl;
  cout << " Number of productions ..........: " << curprodtn << endl;
  cout << " Number of bubbles ..............: " << curbubble << endl;
  cout << " Max/Number base-nonterminal ....: " << maxbasent << endl;
  cout << " Max/Number absl-nonterminal ....: " << maxabslnt << endl;
  cout << " Max terminal ...................: " << maxtermn-1 << endl;
  cout << " Number of terminals ............: " << maxtermn << endl;
  cout << "                    curprodtn ...: " << curprodtn << endl;
  cout << "                    lenprodtn ...: " << lenprodtn << endl;
  cout << "                    currhs ......: " << currhs    << endl;
  cout << "                    lenrhs ......: " << lenrhs    << endl;
  cout << "                    curbubble ...: " << curbubble << endl;
  cout << "                    lenbubble ...: " << lenbubble << endl;
  cout << "                    curexcept ...: " << curexcept << endl;
  cout << "                    lenexcept ...: " << lenexcept << endl;

  cout << endl;
  cout << "============================" << endl;
  cout << "Base/Absl Translation Model" << endl;
  cout << "============================" << endl;
  for (int basent = -1; basent >= maxbasent; basent--) {
    cout << " BaseNT: " << basent << endl;
    cout << "     (smP:" << membasent[-basent].smallestPrec << "," <<
      "lgP:" << membasent[-basent].largestPrec << "," <<
      "ftA:" << membasent[-basent].firstAbs << "," <<
      "ltA:" << membasent[-basent].lastAbs << ")" << endl;
    int cA = membasent[-basent].firstAbs;
    while (cA) {
      cout << "      AbslNT: " << cA << " [" <<
	"baseNT:" << memabslnt[-cA].baseNT << "," <<
	"precNT:" << memabslnt[-cA].precNT << "," <<
	"nextNT:" << memabslnt[-cA].nextNT << "]" <<
	endl;
      cA = memabslnt[-cA].nextNT;
    }
    cout << endl;
  }

  cout << endl;
  cout << "============================" << endl;
  cout << "Productions" << endl;
  cout << "============================" << endl;
  for (int ip = 1; ip <= curprodtn; ip++) {
    if (!memprodtn[ip].bubble) {
      cout << "(" << ip-1 << ")  production: " << memprodtn[ip].lhs ;
      cout << "<" << getBaseNT(memprodtn[ip].lhs) << "/";
      cout << getPrecNT(memprodtn[ip].lhs) << ">";
      cout << " ::= " ;
      for (int rh = 0; rh < memprodtn[ip].lrhs; rh ++) {
	cout << memrhs[memprodtn[ip].rhs+rh];
	if (memrhs[memprodtn[ip].rhs+rh] < 0) {
	  cout << "<" << 
	    getBaseNT(memrhs[memprodtn[ip].rhs+rh]) << "/" <<
	    getPrecNT(memrhs[memprodtn[ip].rhs+rh]) << ">";
	}
	cout << " ";
      }
      cout << endl;
    } else {
      cout << "(" << ip-1 << ")  production: " ;
      ScpBubble* bubble = &membubble[memprodtn[ip].bubble];
      cout << bubble->abslnt;
      cout << "<" << getBaseNT(bubble->abslnt) << "/";
      cout << getPrecNT(bubble->abslnt) << ">";
      cout << ", ";
      cout << bubble->lbound << ", " ;
      cout << bubble->ubound << ", " ;
      cout << bubble->lparen << ", " ;
      cout << bubble->rparen << ", " ;
      for (int ex = 0; ex < bubble->lexcept; ex ++)
	cout << memexcept[bubble->except+ex] << " ";
      cout << " || ";
      for (int tm = 0; tm < maxtermn; tm++) 
	if (bubexcept[bubble->addrbubexcept + tm])
	  cout << tm << " ";
      cout << endl;
    }
  }

  cout << endl;
  cout << "============================" << endl;
  cout << "Non-Terminals " << endl;
  cout << "============================" << endl;
  for (int nt=1; nt <= -maxabslnt ; nt ++) {
    printf(" Non-Terminal: [%4d]\n",-nt);

    if (ldgrabslnt[nt]) {
      int linderg;
      cout << "   LDerGraph: " ;
      int lderg = ldgrabslnt[nt];
      linderg = 0;
      while (lderg) {
	if (linderg > 10) {
	  cout << endl << "              " ;
	  linderg = 0;
	}
	printf("(%4d) ",lderg);
	lderg = ldgrprodtn[lderg];
	linderg ++ ;
      }
      cout << endl;
    }

    cout << "   LDerTable: " ;
    int linderg = 0;
    for (int lder = 1; lder <= -maxabslnt; lder++) {
      if (ldernttb[addrnontm[nt]+lder]) {
	if (linderg > 7) {
	  cout << endl << "              " ;
	  linderg = 0;
	}
	printf("[%4d] ",-lder);
	linderg ++;
      }
    }
    cout << endl;

    if (rdgrabslnt[nt]) {
      int linderg;
      cout << "   RDerGraph: " ;
      int rderg = rdgrabslnt[nt];
      linderg = 0;
      while (rderg) {
	if (linderg > 10) {
	  cout << endl << "              " ;
	  linderg = 0;
	}
	printf("(%4d) ",rderg);
	rderg = rdgrprodtn[rderg];
	linderg ++ ;
      }
      cout << endl;
    }

    cout << "   RDerTable: " ;
    linderg = 0;
    for (int rder = 1; rder < maxnontm; rder++) {
      if (rdernttb[addrnontm[nt]+rder]) {
	if (linderg > 7) {
	  cout << endl << "              " ;
	  linderg = 0;
	}
	printf("[%4d] ",-rder);
	linderg ++;
      }
    }
    cout << endl;

    cout << "   TMCovTable: " << endl;
    for (int tm = 0; tm < maxtermn; tm++) {
      int tmaddrbase = addrtermn[tm];
      if (covtmtb[tmaddrbase+nt]) {
	printf("      [%4d]: ",tm);
	int lincov = 0;
	int cov = covtmtb[tmaddrbase+nt];
	while (cov) {
	  if (lincov > 8) {
	    cout << endl << "              " ;
	    lincov = 0;
	  }
	  lincov++;
	  printf("(%4d) ",memcov[cov].prodtn-1);
	  cov = memcov[cov].next;
	}
	cout << endl;
      }
    }

    cout << "   NTCovTable: " << endl;
    for (int ntm = 1; ntm <= -maxabslnt; ntm++) {
      int ntaddrbase = addrnontm[ntm];
      if (covnttb[ntaddrbase+nt]) {
	printf("      [%4d]: ",-ntm);
	int lincov = 0;
	int cov = covnttb[ntaddrbase+nt];
	while (cov) {
	  if (lincov > 8) {
	    cout << endl << "              " ;
	    lincov = 0;
	  }
	  lincov++;
	  printf("(%4d) ",memcov[cov].prodtn-1);
	  cov = memcov[cov].next;
	}
	cout << endl;
      }
    }
      
    if (memabslnt[nt].bubblecov) {
      cout << "   BubbleCov: " ;
      int bcov = memabslnt[nt].bubblecov;
      int lincov = 0;
      while (bcov) {
	if (lincov > 8) {
	  cout << endl << "              " ;
	  lincov = 0;
	}
	lincov++;
	printf("(%4d) ",memcov[bcov].prodtn-1);
	bcov = memcov[bcov].next;
      }
      cout << endl;
    }

    if (memabslnt[nt].bubbletermn)
      cout << "   BubbleTermn: 1" << endl;

    if (memabslnt[nt].bubbleev) {
      cout << "   BubbleEv: ";
      int bcov = memabslnt[nt].bubbleev;
      int lincov = 0;
      while (bcov) {
	if (lincov > 8) {
	  cout << endl << "              " ;
	  lincov = 0;
	}
	lincov++;
	printf("(%4d) ",bubcov[bcov].prodtn-1);
	bcov = bubcov[bcov].next;
      }
      cout << endl;
    }
    
    cout << "   AdjTable:  ";
    int linadj = 0;
    for (int tm = 0; tm < maxtermn; tm++) {
      if (adjtb[addrtermn[tm]+nt]) {
	if (linadj > 8) {
          cout << endl << "              " ;
          linadj = 0;
        }
	printf("[%4d] ",tm);
	linadj++;
      }
    }
    cout << endl;

    if (memabslnt[nt].adjbubble) 
      cout << "   AdjBubble: 1 " << endl;

    cout << " ----------------------------" << endl;
  }

  cout << endl;
  cout << "============================" << endl;
  cout << "Terminals" << endl;
  cout << "============================" << endl;
  for (int term = 0; term < maxtermn; term ++) {
    printf(" Terminal: [%4d]\n",term);

    cout << "    LDerTable: " ;
    int linderg = 0;
    for (int lder = 1; lder <= -maxabslnt; lder++) {
      if (ldertmtb[addrtermn[term]+lder]) {
	if (linderg > 7) {
	  cout << endl << "               " ;
	  linderg = 0;
	}
	printf("[%4d] ",-lder);
	linderg ++;
      } 
    }
    cout << endl;
  }
}

