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
 **      Maude Implementation                  **
 **        Jose F. Quesada                     **
 **        CSL - SRI Int.  1998                **
 ************************************************/

/*******
 ******* scp_parser.cc
 *******/

#include "scp_parser.hh"


int ScpParser::parseSentence(Vector<int>& term,int root,int errorRecover)
{
  // Control: compile grammar
  if (!compiled)
    compileParser();

  // Init EventTerm
  lengthterm = term.length()+1;
  if (lengthterm > maxlenterm) {
    maxlenterm = lengthterm + ADDLENTERM;
    delete [] nodeterm;
    nodeterm  = new int[maxlenterm*5];
    eventterm = nodeterm+maxlenterm;
    inputterm = eventterm+maxlenterm;
    errorlist = inputterm+maxlenterm;
    errorlevel = errorlist+maxlenterm;
  }
  memset(eventterm,0,(lengthterm+1)*sizeof(int));
  memset(nodeterm,0,(lengthterm+1)*sizeof(int));

  // ErrorRecover
  parseErrorRecover = errorRecover;
  if (errorRecover) {
    curdetect = 0;
    lasttokendetect = 0;
    prevlasttokendetect = -1;
  }

  // Init Counters and Flags
  rootterm = membasent[-root].lastAbs;

  register int tk;
  // Copy term(Vector<int>) to inputterm(int*)
  for (tk = 0; tk < lengthterm-1; tk++) 
    inputterm[tk] = term[tk];
  // End Token
  inputterm[tk] = maxtermn + 10;

  // Prod 0
  memprodtn[0].lhs = rootterm;
  memrhs[memprodtn[0].rhs] = rootterm;

  // Initial Flags
  curevent = 0;  /* event 0 is not used */
  curnode  = 0;  /* node 0 is not used */
  curanal  = 0;  /* anal 0 is not used */
  rootnode = 0;
  ambnodes = 0;
  prevnextanal = 0;

  #ifdef SCPTRACE
  cout << "==============================================" << endl;
  cout << "parse: ";
  for (tk = 0; tk < lengthterm-1; tk++)
    cout << inputterm[tk] << " " ;
  cout << endl;
  #endif
    

  tryEvent(0,0,0,0,0,0);

  if (curbubble) {
    for (tk = 0; tk < lengthterm; tk++)
      if (eventterm[tk])
	runTokenBubble(tk);
  } else {
    for (tk = 0; tk < lengthterm-1; tk++)
      if ((inputterm[tk] < maxtermn) &&
	  (eventterm[tk]))
	runToken(tk);
  }

  #ifdef SCPTRACE
  cout << "parse: ";
  for (tk = 0; tk < lengthterm-1; tk++)
    cout << inputterm[tk] << " " ;
  cout << endl;

  cout << "Length: " << lengthterm-1 << " RootNode: " << rootnode << endl;
  cout << "  Nodes: " << curnode << " Events: " << curevent 
       << " Anals: " << curanal << endl;
  #endif

  if (!errorRecover)
    curerrorlist = 0;

  if (!rootnode) {
    if (errorRecover) {
      errorRecovery();
    } else {
      memset(errorlist,0,(lengthterm+1)*sizeof(int));
      memset(errorlevel,0,(lengthterm+1)*sizeof(int));
      parseSentence(term,root,1);
    }
    return 0;
  } else if (rootnode && !ambnodes)
    return 1;
  else {
    if (prevambnode) 
      memset(memambnode,0,(curnode+1)*sizeof(int));
    prevambnode = 1;
    return setAmbNode(rootnode);
  }
}

void ScpParser::tryEvent(int prod,int idot,int fpos,int lpos, 
			 int prevnode = 0,int prevevent = 0)
{
  #ifdef SCPTRACE
  cout << "SCPtrace$$   tryEvent (" 
      << " prod:"   << prod-1 
       << " symbol:" << printSymbol(memprodtn[prod].lhs)
       << " idot:"   << idot 
       << " fpos:"   << fpos
       << " lpos:"   << lpos
       << " )" << endl;
  #endif
  int pos = lpos;
  int dot = idot;
  int rhs = memprodtn[prod].rhs+dot;
  while ((dot < memprodtn[prod].lrhs) && 
	 (memrhs[rhs] >= 0) &&
	 (pos < lengthterm)) {
    if (memrhs[rhs] == inputterm[pos]) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryEvent:skipTerm (" 
	   << " pos:"   << pos 
	   << " token:" << inputterm[pos] 
	   << " )" << endl;
      #endif
      rhs ++;
      pos ++;
      dot ++;
    } else {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      FAIL:tryEvent:skipTerm (" 
	   << " pos:" << pos
	   << " inputToken:" << inputterm[pos] 
	   << " neededToken:" << memrhs[rhs] 
	   << " )" << endl;
      #endif
      if (parseErrorRecover) {
	if (pos >= lasttokendetect) {
	  errorDetect(pos,memrhs[rhs]);

	  curevent ++;
	  if (curevent >= lenevent) {
	    ScpEvent* xmemevent = new ScpEvent[lenevent+ADDEVENT];
	    memcpy(xmemevent,memevent,lenevent*sizeof(ScpEvent));
	    lenevent += ADDEVENT;
	    delete [] memevent;
	    memevent = xmemevent;
	  }
	  ScpEvent* event = &memevent[curevent];
	  event->prodtn = prod;
	  event->dot = dot;
	  event->fpos = fpos;
	  event->nextsymbol = memrhs[rhs];
	  if (event->nextsymbol >= 0) {
	    if (event->nextsymbol < maxtermn)
	      event->addrnextsymbol = addrtermn[event->nextsymbol];
	    else
	      event->addrnextsymbol =  0;
	  }
	  else
	    event->addrnextsymbol = addrnontm[-event->nextsymbol];
	  event->subsumed = 1; 
	  event->prevnode = prevnode;
	  event->prevevent = prevevent;
          #ifdef SCPTRACE 
	  cout << "SCPtrace$$      tryEvent:newEvent[" << curevent << "] ( "
	       << " prod:"   << prod-1 
	       << " symbol:"    << printSymbol(memprodtn[prod].lhs)
	       << " dot:"   << dot 
	       << " nextsymbol: " << memrhs[rhs]
	       << " fpos:" << fpos 
	       << " lpos:" << pos 
	       << endl;
	  cout << "SCPtrace$$      tryEvent:addEvent[" << pos << "] ADD " << curevent << endl;
          #endif
	  if (!eventterm[pos]) {
	    event->nextevent = 0;
	    eventterm[pos] = curevent;
	  } else {
	    if (curbubble && (lpos == fpos)) {
	      int xevents = eventterm[pos];
	      while (memevent[xevents].nextevent)
		xevents = memevent[xevents].nextevent;
	      memevent[xevents].nextevent = curevent;
	      event->nextevent = 0;
	    } else {
	      event->nextevent = eventterm[pos];
	      eventterm[pos] = curevent;
	    }
	  }
	}
      }
      return;
    }
  }

  if (dot < memprodtn[prod].lrhs) {
    if ((memabslnt[-memrhs[rhs]].bubblecov) ||
	 ((inputterm[pos] < maxtermn) &&
	  (ldertmtb[addrtermn[inputterm[pos]]-memrhs[rhs]]))) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryEvent:LDerControl (" 
	   << " symbol:" << printSymbol(memrhs[rhs]) 
	   << " token:" << inputterm[pos] 
	   << " ) => 1" << endl;
      #endif
      curevent ++;
      if (curevent >= lenevent) {
	ScpEvent* xmemevent = new ScpEvent[lenevent+ADDEVENT];
	memcpy(xmemevent,memevent,lenevent*sizeof(ScpEvent));
	lenevent += ADDEVENT;
	delete [] memevent;
	memevent = xmemevent;
      }
      ScpEvent* event = &memevent[curevent];
      event->prodtn = prod;
      event->dot = dot;
      event->fpos = fpos;
      event->nextsymbol = memrhs[rhs];
      event->addrnextsymbol = addrnontm[-event->nextsymbol];
      event->subsumed = 0;
      event->prevnode = prevnode;
      event->prevevent = prevevent;
      #ifdef SCPTRACE 
      cout << "SCPtrace$$      tryEvent:newEvent[" << curevent << "] ( "
	   << " prod:"   << prod-1 
	   << " symbol:"    << printSymbol(memprodtn[prod].lhs)
	   << " dot:"   << dot 
	   << " nextsymbol: " << printSymbol(memrhs[rhs])
	   << " fpos:" << fpos 
	   << " lpos:" << pos 
	   << endl;
	// << "SCPtrace$$                                           "
	// << " prevnode:" << prevnode
	// << " prevevent:" << prevevent
	// << " )" << endl;
      cout << "SCPtrace$$      tryEvent:addEvent[" << pos << "] ADD " << curevent << endl;
      #endif
      if (!eventterm[pos]) {
	event->nextevent = 0;
	eventterm[pos] = curevent;
      } else {
	int xevents = eventterm[pos];
	int sub = 0;
	while (!sub && xevents) {
	  if (memevent[xevents].nextsymbol < 0) {
	    if (ldernttb[event->addrnextsymbol - memevent[xevents].nextsymbol]) {
  	      #ifdef SCPTRACE
	      cout << "SCPtrace$$      tryEvent:subsume event " << xevents 
		   << " to " << curevent << endl;
	      #endif
	      event->subsumed = 1;
	      sub = 1;
	    } else if (ldernttb[memevent[xevents].addrnextsymbol - event->nextsymbol]){
	      #ifdef SCPTRACE
	      cout << "SCPtrace$$      tryEvent:subsume event " << curevent 
		   << " to " << xevents << endl;
	      #endif
	      memevent[xevents].subsumed = 1;
	    }
	  }
	  xevents = memevent[xevents].nextevent;
	}
	if (curbubble && (lpos == fpos)) {
	  xevents = eventterm[pos];
	  while (memevent[xevents].nextevent)
	    xevents = memevent[xevents].nextevent;
	  memevent[xevents].nextevent = curevent;
	  event->nextevent = 0;
	} else {
	  event->nextevent = eventterm[pos];
	  eventterm[pos] = curevent;
	}
      }
    } else {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      FAIL:tryEvent:LDerControl (" 
	   << " symbol:" << printSymbol(memrhs[rhs])
	   << " token:" << inputterm[pos] 
	   << " ) => 0" << endl;
      #endif
      if (parseErrorRecover) {
	if (pos >= lasttokendetect) {
	  errorDetect(pos,memrhs[rhs]);
	  if ((inputterm[pos] < maxtermn) &&
	      (!ldertmtb[addrtermn[inputterm[pos]]-memrhs[rhs]])) {
            #ifdef SCPTRACE
	    cout << "SCPtrace$$      ErrorRecoveryPhase: cancelling FAIL" << endl;
            #endif
	    curevent ++;
	    if (curevent >= lenevent) {
	      ScpEvent* xmemevent = new ScpEvent[lenevent+ADDEVENT];
	      memcpy(xmemevent,memevent,lenevent*sizeof(ScpEvent));
	      lenevent += ADDEVENT;
	      delete [] memevent;
	      memevent = xmemevent;
	    }
	    ScpEvent* event = &memevent[curevent];
	    event->prodtn = prod;
	    event->dot = dot;
	    event->fpos = fpos;
	    event->nextsymbol = memrhs[rhs];
	    event->addrnextsymbol = addrnontm[-event->nextsymbol];
	    event->subsumed = 1;
	    event->prevnode = prevnode;
	    event->prevevent = prevevent;
            #ifdef SCPTRACE 
	    cout << "SCPtrace$$      tryEvent:newEvent[" << curevent << "] ( "
		 << " prod:"   << prod-1 
		 << " symbol:"    << printSymbol(memprodtn[prod].lhs)
		 << " dot:"   << dot 
		 << " nextsymbol: " << printSymbol(memrhs[rhs])
		 << " fpos:" << fpos 
		 << " lpos:" << pos 
		 << endl;
	    cout << "SCPtrace$$      tryEvent:addEvent[" << pos << "] ADD " << curevent << endl;
            #endif
	    if (!eventterm[pos]) {
	      event->nextevent = 0;
	      eventterm[pos] = curevent;
	    } else {
	      int xevents = eventterm[pos];
	      int sub = 0;
	      while (!sub && xevents) {
		if (memevent[xevents].nextsymbol < 0) {
		  if (ldernttb[event->addrnextsymbol - memevent[xevents].nextsymbol]) {
    	            #ifdef SCPTRACE
		    cout << "SCPtrace$$      tryEvent:subsume event " << xevents 
			 << " to " << curevent << endl;
                    #endif
		    event->subsumed = 1;
		    sub = 1;
		  } else if (ldernttb[memevent[xevents].addrnextsymbol - event->nextsymbol]){
	            #ifdef SCPTRACE
		    cout << "SCPtrace$$      tryEvent:subsume event " << curevent 
			 << " to " << xevents << endl;
	            #endif
		    memevent[xevents].subsumed = 1;
		  }
		}
		xevents = memevent[xevents].nextevent;
	      }
	      if (curbubble && (lpos == fpos)) {
		xevents = eventterm[pos];
		while (memevent[xevents].nextevent)
		  xevents = memevent[xevents].nextevent;
		memevent[xevents].nextevent = curevent;
		event->nextevent = 0;
	      } else {
		event->nextevent = eventterm[pos];
		eventterm[pos] = curevent;
	      }
	    }
	  }
	}
      }
    }
  } else {
    if (pos == lengthterm) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$   AssigningRootNode: " << prevnode << endl;
      #endif
      rootnode = prevnode;
    } else
      tryNode(prod,fpos,pos,prevnode,prevevent);
  }
}

void ScpParser::runToken(int token)
{
  #ifdef SCPTRACE
  cout << "SCPtrace$$   runToken (" 
       << " token:" << inputterm[token] 
       << " post:" << token 
       << " )" << endl;
  #endif
  int prememprodruntoken;
  int curmemprodruntoken = 0;
  int event = eventterm[token];
  int addrtoken = addrtermn[inputterm[token]];
  while (event) {
    if (!memevent[event].subsumed) {
      int cov = covtmtb[addrtoken-memevent[event].nextsymbol];
      prememprodruntoken = curmemprodruntoken-1;
      while (cov) {
	int pro = memcov[cov].prodtn;
	int run = 0;
	for (int rt = prememprodruntoken; (!run) && (rt >= 0); rt--)
	  if (memprodruntoken[rt] == pro) 
	    run = 1;
	if (run) {
          #ifdef SCPTRACE
	  cout << "SCPtrace$$      runToken:triggerCoverage-Run:" 
	       << memcov[cov].prodtn - 1 
	       << "   runToken (" 
	       << " token:" << inputterm[token] 
	       << " post:" << token 
	       << " )" << endl;
          #endif
	} else {
          #ifdef SCPTRACE
	  cout << "SCPtrace$$      runToken:triggerCoverage:" << memcov[cov].prodtn -1
	       << "   runToken (" 
	       << " token:" << inputterm[token] 
	       << " post:" << token 
	       << " )" << endl;
          #endif
	  memprodruntoken[curmemprodruntoken++] = pro;
	  tryEvent(pro,0,token,token,0,0);
	}
	cov = memcov[cov].next;
      }
    }
    event = memevent[event].nextevent;
  }
}


void ScpParser::runTokenBubble(int token)
{
  #ifdef SCPTRACE
  cout << "SCPtrace$$   runToken (" 
       << " token:" << inputterm[token] 
       << " post:" << token 
       << " )" << endl;
  #endif
  int prememprodruntoken;
  int curmemprodruntoken = 0;
  int event = eventterm[token];
  int  addrtoken;
  if (inputterm[token] < maxtermn)
    addrtoken = addrtermn[inputterm[token]];
  int bcov;
  int ecov;
  while (event) {
    if ((inputterm[token] < maxtermn) && (!memevent[event].subsumed)) {
      int cov = covtmtb[addrtoken-memevent[event].nextsymbol];
      prememprodruntoken = curmemprodruntoken-1;
      while (cov) {
	int pro = memcov[cov].prodtn;
	int run = 0;
	for (int rt = prememprodruntoken; (!run) && (rt >= 0); rt--)
	  if (memprodruntoken[rt] == pro) 
	    run = 1;
	if (run) {
          #ifdef SCPTRACE
	  cout << "SCPtrace$$      runToken:triggerCoverage-Run:" 
	       << memcov[cov].prodtn -1
	       << "   runToken (" 
	       << " token:" << inputterm[token] 
	       << " post:" << token 
	       << " )" << endl;
          #endif
	} else {
          #ifdef SCPTRACE
	  cout << "SCPtrace$$      runToken:triggerCoverage:" << memcov[cov].prodtn -1
	       << "   runToken (" 
	       << " token:" << inputterm[token] 
	       << " post:" << token 
	       << " )" << endl;
          #endif
	  memprodruntoken[curmemprodruntoken++] = pro;
	  tryEvent(pro,0,token,token,0,0);
	}
	cov = memcov[cov].next;
      }
    }
    if (!memevent[event].subsumed) {
      if (ecov = memabslnt[-memevent[event].nextsymbol].bubbleev) {
	while (ecov) {
	  if (bcov = memabslnt[-memrhs[memprodtn[bubcov[ecov].prodtn].rhs]].bubblecov) {
	    while (bcov) {
	      runBubble(bubcov[ecov].prodtn,0,
			token,memcov[bcov].prodtn,token,0);
	      bcov = memcov[bcov].next;
	    }
	  }
	  ecov = bubcov[ecov].next;
	}
      }
    }

    if (memevent[event].nextsymbol < 0) {
      if (memabslnt[-memevent[event].nextsymbol].bubbletermn) {
	if (bcov = memabslnt[-memevent[event].nextsymbol].bubblecov) {
	  while (bcov) {
	    runBubble(memevent[event].prodtn,memevent[event].dot,
		      memevent[event].fpos,memcov[bcov].prodtn,token,event);
	    bcov = memcov[bcov].next;
	  }
	}
      }
    }
    event = memevent[event].nextevent;
  }
}

void ScpParser::tryNode(int prod,int fpos,int lpos,int prevnode,int prevevent)
{
  #ifdef SCPTRACE
  cout << "SCPtrace$$   tryNode (" 
       << " prod:" << prod -1;
  if (memprodtn[prod].bubble)
    cout << " symbol:" << printSymbol(membubble[memprodtn[prod].bubble].abslnt);
  else
    cout << " symbol:" << printSymbol(memprodtn[prod].lhs);
  cout << " fpos:" << fpos 
       << " lpos:" << lpos 
       << " )" << endl;
  #endif
  if (lpos >= lengthterm) 
    return;

  // Adjacency Control
  int nextterm = inputterm[lpos];
  int lhs;
  if (memprodtn[prod].bubble)
    lhs = membubble[memprodtn[prod].bubble].abslnt;
  else
    lhs = memprodtn[prod].lhs;
  if (lpos < (lengthterm-1)) {
    if (nextterm >= maxtermn) {
      ;
    } else if (adjtb[addrtermn[nextterm]-lhs]) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryNode:AdjControl (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " token:" << nextterm 
	   << " ) => 1" << endl;
      #endif
    } else if (memabslnt[-lhs].adjbubble) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryNode:AdjControl (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " ) => (adjBubble) 1" << endl;
      #endif
    } else {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      FAIL:tryNode:AdjControl (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " token:" << nextterm 
	   << " ) => 0" << endl;
      #endif
      if (!parseErrorRecover)
	return;
      else {
        #ifdef SCPTRACE
	cout << "SCPtrace$$      ErrorRecoveryPhase: cancelling FAIL" << endl;
        #endif
      }
    }
  } else {
    if (rdernttb[addrnontm[-lhs]-rootterm]) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryNode:AdjControl:RMS (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " ) => 1" << endl;
      #endif
    } else if (memabslnt[-lhs].adjbubble) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryNode:AdjControl:RMS (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " ) => (adjBubble) 1" << endl;
      #endif
    } else {
      #ifdef SCPTRACE
      cout << "SCPtrace$$      FAIL:tryNode:AdjControl:RMS (" 
	   << " symbol:" << printSymbol(lhs) 
	   << " ) => 0" << endl;
      #endif
      if (!parseErrorRecover)
	return;
      else {
        #ifdef SCPTRACE
	cout << "SCPtrace$$      ErrorRecoveryPhase: cancelling FAIL" << endl;
        #endif
      }
    }
  }

  int symbol = lhs;

  // Local Ambiguity control 
  int xnode = nodeterm[fpos];
  while (xnode) {
    if ((memnode[xnode].lpos == lpos) && (memnode[xnode].symbol == symbol)) {
      if (memprodtn[prod].bubble || parseErrorRecover)
	return;
      curanal++;
      if (curanal >= lenanal) {
	ScpAnal* xmemanal = new ScpAnal[lenanal+ADDANAL];
	memcpy(xmemanal,memanal,lenanal*sizeof(ScpAnal));
	lenanal += ADDANAL;
	delete [] memanal;
	memanal = xmemanal;
      }
      ScpAnal* anal = &memanal[curanal];
      anal->prodtn = prod;
      anal->prevnode = prevnode;
      anal->prevevent = prevevent;
      anal->nextanal = memnode[xnode].initanal;
      memnode[xnode].initanal = curanal;
      ambnodes = 1;
      #ifdef SCPTRACE
      cout << "SCPtrace$$      tryNode:LocalAmbiguity with node " << xnode << endl;
      cout << "SCPtrace$$      tryNode:newAnal[" << curanal << "] ("
	   << " prod:" << prod -1
	   << " prevnode:" << prevnode 
	   << " prevevent:" << prevevent
	   << " nextanal:" << anal->nextanal
	   << ")" << endl;
      #endif
      return;
    }
    xnode = memnode[xnode].nextnode;
  }
	
  // Node Creation
  curnode ++;
  if (curnode >= lennode) {
    ScpNode* xmemnode = new ScpNode[lennode+ADDNODE];
    memcpy(xmemnode,memnode,lennode*sizeof(ScpNode));
    int* xmemambnode = new int[lennode+ADDNODE];
    lennode += ADDNODE;
    delete [] memnode;
    memnode = xmemnode;
    delete [] memambnode;
    memambnode = xmemambnode;
    prevambnode = 1;
  }
  curanal++;
  if (curanal >= lenanal) {
    ScpAnal* xmemanal = new ScpAnal[lenanal+ADDANAL];
    memcpy(xmemanal,memanal,lenanal*sizeof(ScpAnal));
    lenanal += ADDANAL;
    delete [] memanal;
    memanal = xmemanal;
  }
  ScpNode* node = &memnode[curnode];
  ScpAnal* anal = &memanal[curanal];
  node->fpos = fpos;
  node->lpos = lpos;
  node->symbol = symbol;
  node->nextnode = nodeterm[fpos];
  nodeterm[fpos] = curnode;
  node->initanal = curanal;
  anal->prodtn = prod;
  anal->prevnode = prevnode;
  anal->prevevent = prevevent;
  anal->nextanal = 0;

  #ifdef SCPTRACE 
  cout << "SCPtrace$$      tryNode:newNode[" << curnode << "] (" 
       << " symbol:" << printSymbol(node->symbol)
       << " fpos:" << fpos 
       << " lpos:" << lpos 
       << " initanal: " << curanal
       << ")" << endl;
  cout << "SCPtrace$$      tryNode:newAnal[" << curanal << "] ("
       << " prod:" << prod-1
       << " prevnode:" << prevnode 
       << " prevevent:" << prevevent
       << " nextanal:" << 0
       << ")" << endl;
  #endif


  // Run Node
  int event;
  int inode = curnode;
  ScpEvent* pevent;
  int prememprodrunnode;
  int curmemprodrunnode = 0;
  while (symbol) {
    int addrlhs = addrnontm[-symbol];
    event = eventterm[fpos];
    while (event) {
      if (!memevent[event].subsumed) {
	int cov = covnttb[addrlhs-memevent[event].nextsymbol];
	prememprodrunnode = curmemprodrunnode-1;
	while (cov) {
	  int pro = memcov[cov].prodtn;
	  int run = 0;
	  for (int rt = prememprodrunnode; (!run) && (rt >= 0); rt--)
	    if (memprodrunnode[rt] == pro) 
	      run = 1;
	  if (run) {
            #ifdef SCPTRACE
	    cout << "SCPtrace$$   runNode:triggerCoverage-Run:" << memcov[cov].prodtn -1
		 << "   runNode (" 
		 << " node:" << inode 
		 << " as symbol:" << printSymbol(symbol) 
  		 << " )" << endl; 
            #endif
	  } else {
            #ifdef SCPTRACE
	    cout << "SCPtrace$$   runNode:triggerCoverage:" << memcov[cov].prodtn -1
		 << "   runNode (" 
		 << " node:" << inode 
		 << " as symbol:" << printSymbol(symbol) 
  		 << " )" << endl; 
            #endif
	    tryEvent(memcov[cov].prodtn,1,fpos,lpos,inode);
	    memprodrunnode[curmemprodrunnode++] = pro;
	  }
	  cov = memcov[cov].next;
	}
      }
      event = memevent[event].nextevent;
    }
    event = eventterm[fpos];
    while (event) {
      pevent = &memevent[event];
      if (pevent->nextsymbol == symbol) {
        #ifdef SCPTRACE
	cout << "SCPtrace$$   runNode:linkEvent:" << event 
	     << "   runNode (" 
	     << " node:" << inode 
	     << " as symbol:" << printSymbol(symbol)
	     << " )" << endl; 
        #endif
	  tryEvent(pevent->prodtn,pevent->dot+1,pevent->fpos,lpos,inode,event);
      }
      event = memevent[event].nextevent;
    }
    symbol = memabslnt[-symbol].nextNT;
  }
}

void ScpParser::runBubble(int evprod,int dot,int evfpos,
			  int bprod,int fpos,int event)
{
  #ifdef SCPTRACE
  cout << "SCPtrace$$   runBubble("
       << " evprodtn:" << evprod-1
       << " evdot:" << dot
       << " evfpos:" << evfpos
       << " bprod:" << bprod-1
       << " lpos:" << fpos
       << " event:" << event
       << " )" << endl;
  #endif

  ScpBubble* bubble = &membubble[memprodtn[bprod].bubble];
  int lpos = fpos;
  int lbnd = bubble->lbound;
  int ubnd = bubble->ubound;
  int lpar = bubble->lparen;
  int rpar = bubble->rparen;
  int pars = 0;

  // LBound Tokens
  #ifdef SCPTRACE
  cout << "SCPtrace$$      lbound:" << bubble->lbound << endl;
  cout << "SCPtrace$$      ubound:" << bubble->ubound << endl;
  cout << "SCPtrace$$      lparen:" << bubble->lparen << endl;
  cout << "SCPtrace$$      rparen:" << bubble->rparen << endl;
  #endif
  while ((lbnd>0 || pars) && ubnd && (lpos < lengthterm-1)) {
    #ifdef SCPTRACE
    cout << "SCPtrace$$      skipLBound: " << inputterm[lpos] << endl;
    #endif
    if ((inputterm[lpos] < maxtermn) && 
	(bubexcept[bubble->addrbubexcept + inputterm[lpos]])) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$         exceptionError: " << inputterm[lpos] << endl;
      #endif
      if (parseErrorRecover && (lpos > lasttokendetect)) {
	errorDetect(lpos,inputterm[lpos],EXCEPTION_ERROR,bubble->abslnt);
	errorRecovery(1);
      } else
	return;
    } else if (inputterm[lpos] == rpar) {
      #ifdef SCPTRACE
      cout << "SCPtrace$$         closeParenthesis: pars=" << pars-1 << endl;
      #endif
      if (pars <= 0) {
	#ifdef SCPTRACE
	cout << "SCPtrace$$         ERROR-No Open Parenthesis" << endl;
	#endif
	if (parseErrorRecover && (lpos > lasttokendetect)) {
	  errorDetect(lpos,inputterm[lpos],CLOSEPAR_ERROR,bubble->abslnt);
	  errorRecovery(1);
	  pars++;
	} else
	  return; /* Error: over-closed parenthesis */
      }
      pars--;
    } else if (inputterm[lpos] == lpar) {
      pars++;
      #ifdef SCPTRACE
      cout << "SCPtrace$$         openParenthesis: pars=" << pars << endl;
      #endif
    } 
    lbnd --;
    ubnd --;
    lpos ++;
  }

  if (lbnd > 0)
    return;

  dot ++;
  int lrhs   = memprodtn[evprod].lrhs;
  int rhs    = memprodtn[evprod].rhs;
  int bubbleend;
  int bubbletermn;
  int bubblenexttermn;
  int bubblenexttermnlen;
  int bubblenext;
  if (dot < lrhs) {
    bubbleend = 0;
    if (memrhs[rhs+dot] >= 0) {
      bubbletermn = 1;
      bubblenexttermn = memrhs[rhs+dot];
      bubblenexttermnlen = 1;
      int xdot = dot + 1;
      bubblenext = 0;
      while (!bubblenext && (xdot < lrhs)) {
	if (memrhs[rhs+xdot] < 0) {
	  bubblenext = memrhs[rhs+xdot];
	} else {
	  xdot++;
	  bubblenexttermnlen ++;
	}
      }
      if (!bubblenext) {
	bubbleend = 1;
	bubblenext = memprodtn[evprod].lhs;
      }
    } else {
      bubbletermn = 0;
      bubblenext = memrhs[memprodtn[evprod].rhs+dot];
    }
  } else {
    bubbletermn = 0;
    bubbleend = 1;
    bubblenext = memprodtn[evprod].lhs;
  }

  #ifdef SCPTRACE
  cout << "SCPtrace$$      bubbleend  :" << bubbleend << endl;
  cout << "SCPtrace$$      bubbletermn:" << bubbletermn << endl;
  cout << "SCPtrace$$      bubblenext :" << printSymbol(bubblenext) << endl;
  if (bubbletermn)
    cout << "SCPtrace$$      bubblenextt:" << bubblenexttermn << endl;
  #endif

  int termerror;
  int xlpos = lpos;
  int poslpos;
  if (!pars)
    poslpos = lpos;
  else
    poslpos = 0;
  int found = 0;
  while (!found && (lpos <= lengthterm-1)) {
    #ifdef SCPTRACE
    cout << "SCPtrace$$      seekFirstCandidate: "
	 << " lpos=" << lpos 
	 << " token=" << inputterm[lpos]
	 << endl;
    #endif

    xlpos = lpos;
    termerror = 0;
    if (bubbletermn && !pars) {
      int xdot = dot;
      int blen = 1;
      for (blen = 1; !termerror && (blen <= bubblenexttermnlen); blen++) {
        #ifdef SCPTRACE
	cout << "SCPtrace$$      skipNextTerminal: "
	     << " xlpos=" << xlpos 
	     << " xdot=" << xdot
	     << " rhs=" << memrhs[rhs+xdot]
	     << " token=" << inputterm[xlpos]
	     << endl;
        #endif
	if (xlpos > lengthterm -1)
	  termerror = 1;
	else if (inputterm[xlpos] != memrhs[rhs+xdot])
	  termerror = 1;
	else {
	  xlpos++;
	  xdot++;
	} 
      }
      if (blen < bubblenexttermnlen)
	termerror = 1;
    }

    if (!pars && !termerror) {
      if (xlpos <= lengthterm) {
	if (!pars)
	  poslpos = xlpos;
	if (xlpos == lengthterm) {
	  if (rdernttb[addrnontm[-bubble->abslnt]-rootterm])
	    found = 1;
	} else if (xlpos == lengthterm-1) {
	  if (bubbleend)
	    found = 1;
	  else if (!bubbleend && memabslnt[-bubblenext].bubblecov)
	    found = 1;
	} else if (inputterm[xlpos] >= maxtermn) {
	  if (bubbleend) {
	    if (memabslnt[-bubblenext].adjbubble)
	      found = 1;
	    else if (memabslnt[-bubble->abslnt].adjbubble)
	      found = 1;
	  } else {
	    if (memabslnt[-bubblenext].bubblecov)
	      found = 1;
	  }
	} else if (bubbleend) {
	  if (adjtb[addrtermn[inputterm[xlpos]]-bubblenext])
	    found = 1;
	  else if (adjtb[addrtermn[inputterm[xlpos]]-bubble->abslnt])
	    found = 1;
          /* Incorporated 2/13/99 to solve p13 problem */
	  else if (memabslnt[-bubblenext].bubblecov)
	    found = 1;
	} else {
	  if (memabslnt[-bubblenext].bubblecov)
	    found = 1;
	  else if (covtmtb[addrtermn[inputterm[xlpos]]-bubblenext]) 
	    found = 1;
	}
      } 
    } else {
      if (parseErrorRecover) {
	if (!pars)
	  poslpos = xlpos;
      }
    }
    if (!found && (ubnd == 0))
      break;
    else if (!found) {
      if ((inputterm[lpos] < maxtermn) && 
	  (bubexcept[bubble->addrbubexcept + inputterm[lpos]])) {
        #ifdef SCPTRACE
	cout << "SCPtrace$$         exceptionError: " << inputterm[lpos] << endl;
        #endif
	if (parseErrorRecover && (lpos >= lasttokendetect)) {
          #ifdef SCPTRACE
	  cout << "SCPtrace$$      ErrorRecoveryPhase: cancelling FAIL" << endl;
          #endif
	  // errorDetect(lpos,inputterm[lpos],EXCEPTION_ERROR,bubble->abslnt);
	  break;
	} else
	  return;
      } else if (inputterm[lpos] == rpar) {
        #ifdef SCPTRACE
	cout << "SCPtrace$$         closeParenthesis: pars=" << pars-1 << endl;
        #endif
	if (pars <= 0) {
	  #ifdef SCPTRACE
	  cout << "SCPtrace$$         ERROR-No Open Parenthesis" << endl;
	  #endif
	  if (parseErrorRecover && (lpos >= lasttokendetect)) {
            #ifdef SCPTRACE
	    cout << "SCPtrace$$      ErrorRecoveryPhase: cancelling FAIL" << endl;
            #endif
	    break;
	    // errorDetect(lpos,inputterm[lpos],CLOSEPAR_ERROR,bubble->abslnt);
	  } else
	    return;
	}
	pars--;
      } else if (inputterm[lpos] == lpar) {
	pars++;
        #ifdef SCPTRACE
	cout << "SCPtrace$$         openParenthesis: pars=" << pars << endl;
        #endif
      }
      lpos++;
      ubnd--;
    }
  }

  if (found)
    tryNode(bprod,fpos,lpos,0,0);
  else if (parseErrorRecover && poslpos && (poslpos >= lasttokendetect))
    tryNode(bprod,fpos,poslpos,0,0);
}

void ScpParser::errorDetect(int pos,int symbol,int type,int bubbleabslnt)
{
  if (pos > lasttokendetect) {
    if (!lendetect) {
      lendetect = MEMDETECT;
      memdetect = new int[lendetect];
      typedetect = new int[lendetect];
    }
    #ifdef SCPTRACE
    cout << "SCPtrace$$  ErrorDetect: "
	 << " position: " << pos
	 << " type: " << type 
	 << " #detect: " << 0
	 << " symbolNeeded: " << symbol;
    if (type == EXCEPTION_ERROR)
      cout << " type: ExceptionError, bubble: " << bubbleabslnt;
    else if (type == CLOSEPAR_ERROR)
      cout << " type: CloseParenthesisError, bubble: " << bubbleabslnt;
    cout << endl;
    #endif
    if (type == STANDARD_ERROR)
      memdetect[0] = symbol;
    else 
      memdetect[0] = bubbleabslnt;
    typedetect[0] = type;
    curdetect = 1;
    lasttokendetect = pos;
  } else if (pos == lasttokendetect) {
    if (curdetect >= lendetect) {
      int* xmemdetect = new int[lendetect+ADDDETECT];
      memcpy(xmemdetect,memdetect,lendetect*sizeof(int));
      int* xtypedetect = new int[lendetect+ADDDETECT];
      memcpy(xtypedetect,typedetect,lendetect*sizeof(int));
      lendetect += ADDDETECT;
      delete [] memdetect;
      memdetect = xmemdetect;
      delete [] typedetect;
      typedetect = xtypedetect;
    }
    #ifdef SCPTRACE
    cout << "SCPtrace$$  ErrorDetect: "
	 << " position: " << pos 
	 << " type: " << type 
	 << " #detect: " << curdetect
	 << " symbolNeeded: " << symbol;
    if (type == EXCEPTION_ERROR)
      cout << " type: ExceptionError";
    else if (type == CLOSEPAR_ERROR)
      cout << " type: CloseParenthesisError";
    cout << endl;
    #endif
    if (type == STANDARD_ERROR)
      memdetect[curdetect] = symbol;
    else 
      memdetect[curdetect] = bubbleabslnt;
    typedetect[curdetect] = type;
    curdetect ++;
  }
}

void ScpParser::errorRecovery(int bubblerecovery)
{
  int erlevel;

  if (lasttokendetect > prevlasttokendetect) {
    prevlasttokendetect = lasttokendetect;

    curerrorlist++;

    if (curerrorlist > lenalter) {
      char* xmemalter = new char[(maxtermn+(maxnontm*2))*(lenalter+ADDALTER)];
      memcpy(xmemalter,memalter,(maxtermn+(maxnontm*2))*lenalter*sizeof(char));
      lenalter += ADDALTER;
      delete [] memalter;
      memalter = xmemalter;
    }
    
    int lasttokenerror = lasttokendetect;
    int addralter = (curerrorlist-1)*(maxtermn+(maxnontm*2));
    memset(memalter+addralter,0,maxtermn+(maxnontm*2));
    
    errorlist[curerrorlist] = lasttokenerror+1;
    errorlevel[curerrorlist] = 1;
    erlevel = 2;
    
    for (int i=0; i<curdetect; i++) {
      if (typedetect[i] == STANDARD_ERROR) {
	if (memdetect[i] >= 0) {
	  if (memdetect[i] < maxtermn)
	    memalter[addralter+memdetect[i]] = 1;
	} else {
	  for (int tm = 0; tm < maxtermn; tm++) 
	    if (covtmtb[addrtermn[tm]-memdetect[i]])
	      memalter[addralter+tm] = 1;
	}
      } else if (typedetect[i] == EXCEPTION_ERROR)
	memalter[addralter+maxtermn-memdetect[i]] = 1;
      else if (typedetect[i] == CLOSEPAR_ERROR)
	memalter[addralter+maxtermn+maxnontm-memdetect[i]] = 1;;
    }
  } else {
    errorlevel[curerrorlist] = errorlevel[curerrorlist]+1;
    erlevel = errorlevel[curerrorlist];
  }

  if (erlevel > MAXERRORLEVEL)
    return;

  if (bubblerecovery)
    return;

  #ifdef SCPTRACE
  cout << "SCPtrace$$  ErroRecoveryStrategy" << endl;
  #endif

  int success = 0;

  // InputSkip Strategy
  #ifdef  SCPTRACE
  cout << "SCPtrace$$   InputSkipStrategy" << endl;
  #endif

  int lasttokenerror = lasttokendetect;
  int event = eventterm[lasttokenerror];
  while (event) {
    #ifdef SCPTRACE
    cout << "SCPtrace$$      InputSkipEvent: " << event 
	 << " nextsymbol: " << memevent[event].nextsymbol 
	 << endl;
    #endif
    int foundskip = 0;
    int pos = lasttokenerror;
    int nextsymbol = memevent[event].nextsymbol;
    if (nextsymbol >= 0) {
      int xerlevel = 0;
      while (!foundskip && (xerlevel++ <= erlevel) && (pos < lengthterm-1)) {
	if (inputterm[pos] == nextsymbol)
	  foundskip = 1;
	else 
	  pos ++;
      }
    } else {
      int xerlevel = 1;
      while (!foundskip && (pos < lengthterm-1)) {
	if ((inputterm[pos] < maxtermn) && (ldertmtb[addrtermn[inputterm[pos]]-nextsymbol]))
	  foundskip = 1;
	else
	  pos ++;
      }
    }
    if (foundskip) {
      success = 1;
      tryEvent(memevent[event].prodtn,memevent[event].dot,
	       memevent[event].fpos,pos,
	       memevent[event].prevnode,memevent[event].prevevent);
    }
    event = memevent[event].nextevent;
  }


  // RuleSkip Strategy
  #ifdef  SCPTRACE
  cout << "SCPtrace$$   RuleSkipStrategy" << endl;
  #endif

  event = eventterm[lasttokenerror];
  while (event) {
    #ifdef SCPTRACE
    cout << "SCPtrace$$      RuleSkipEvent: " << event 
	 << " nextsymbol: " << memevent[event].nextsymbol 
	 << endl;
    #endif
    int foundskip = 0;
    int dot = memevent[event].dot;
    int pro = memevent[event].prodtn;
    int lrhs = memprodtn[pro].lrhs;
    int xerlevel = 1;
    while (!foundskip && (xerlevel++ <= erlevel) && (dot < lrhs)) {
      int nextsymbol = memrhs[memprodtn[pro].rhs+dot];
      if (nextsymbol >= 0) {
	if (inputterm[lasttokenerror] == nextsymbol)
	  foundskip = 1;
	else
	  dot ++;
      } else {
	if ((inputterm[lasttokenerror] < maxtermn) && (ldertmtb[addrtermn[inputterm[lasttokenerror]]-nextsymbol]))
	  foundskip = 1;
	else
	  dot ++;
      }
    }
    if (dot == lrhs)
      foundskip = 1;
    if (foundskip) {
      success = 1;
      tryEvent(memevent[event].prodtn,dot,
	       memevent[event].fpos,lasttokenerror,
	       memevent[event].prevnode,memevent[event].prevevent);
    }
    event = memevent[event].nextevent;
  }


  // InputRuleSkip Strategy
  #ifdef  SCPTRACE
  cout << "SCPtrace$$   InputRuleSkipStrategy" << endl;
  #endif

  event = eventterm[lasttokenerror];
  while (event) {
    #ifdef SCPTRACE
    cout << "SCPtrace$$      InputRuleSkipEvent: " << event 
	 << " nextsymbol: " << memevent[event].nextsymbol 
	 << endl;
    #endif
    int foundskip = 0;
    int dot = memevent[event].dot;
    int pos = lasttokenerror;
    int pro = memevent[event].prodtn;
    int lrhs = memprodtn[pro].lrhs;
    int xerlevel = 1;
    while (!foundskip && (xerlevel++ <= erlevel) && (dot < lrhs) && (pos < lengthterm-1)) {
      int nextsymbol = memrhs[memprodtn[pro].rhs+dot];
      if (nextsymbol >= 0) {
	if (inputterm[pos] == nextsymbol)
	  foundskip = 1;
	else {
	  dot ++;
	  pos ++;
	}
      } else {
	if ((inputterm[pos] < maxtermn) && (ldertmtb[addrtermn[inputterm[pos]]-nextsymbol]))
	  foundskip = 1;
	else {
	  dot ++;
	  pos ++;
	}
      }
    }
    if (dot == lrhs)
      foundskip = 1;

    if (foundskip) {
      success = 1;
      tryEvent(memevent[event].prodtn,dot,
	       memevent[event].fpos,pos,
	       memevent[event].prevnode,memevent[event].prevevent);
    }
    event = memevent[event].nextevent;
  }


  if (!success) {
    errorRecovery(bubblerecovery);
    return;
  }

  // Parsing Continuation
  int tk;
  if (curbubble) {
    for (tk = lasttokenerror; tk < lengthterm; tk++)
      if (eventterm[tk])
	runTokenBubble(tk);
  } else {
    for (tk = lasttokenerror; tk < lengthterm-1; tk++)
      if ((inputterm[tk] < maxtermn) &&
	  (eventterm[tk]))
	runToken(tk);
  }

  if (!rootnode)
    errorRecovery(bubblerecovery);
}

int ScpParser::getRootNode()
{
  return rootnode;
}

int ScpParser::getProductionNumber(int node)
{
  if (node > 0) 
    return memanal[memnode[node].initanal].prodtn-1;
  else
    return -1;
}

int ScpParser::getFirstPosition(int node)
{
  if (node > 0)
    return memnode[node].fpos;
  else
    return -1;
}

int ScpParser::getLastPosition(int node)
{
  if (node > 0)
    return memnode[node].lpos;
  else
    return -1;
}

int ScpParser::getNumberOfChildren(int node)
{
  if (node > 0)
    return memprodtn[memanal[memnode[node].initanal].prodtn].lgather;
  else
    return -1;
}

int ScpParser::getChild(int node,int number)
{
  int nchildren;
  if ((node > 0) && ((nchildren = getNumberOfChildren(node)) > number)) {
    int position = nchildren - number - 1;
    if (!position)
      return memanal[memnode[node].initanal].prevnode;
    else { 
      int prevevent = memanal[memnode[node].initanal].prevevent;
      while (prevevent && (--position))
	prevevent = memevent[prevevent].prevevent;
      if (prevevent)
	return memevent[prevevent].prevnode;
    }
  }
  return -1;
}

int ScpParser::getLHSProduction(int prod)
{
  if (prod < curprodtn) {
    if (memprodtn[prod+1].bubble) 
      return memabslnt[-membubble[memprodtn[prod+1].bubble].abslnt].baseNT;
    else 
      return memabslnt[-memprodtn[prod+1].lhs].baseNT;
  } else
    return 0;
}

int ScpParser::getNumberOfErrors()
{
  return curerrorlist;
}

int ScpParser::getErrorPosition(int errornumber)
{
  if ((errornumber > 0) && (errornumber <= curerrorlist))
    return errorlist[errornumber]-1;
  else
    return -1;
}

Vector<int> ScpParser::getAlternativesStandardError(int errornumber)
{
  Vector<int> alter (0);
  if ((errornumber > 0) && (errornumber <= curerrorlist)) {
    int addralter = (errornumber-1)*(maxtermn+(maxnontm*2));
    int index = 0;
    for (int tm=0; tm < maxtermn; tm++) {
      if (memalter[addralter+tm]) {
	alter.expandBy(1);
	alter[index++] = tm;
      }
    }
  }
  return alter;
}

Vector<int> ScpParser::getBubblesExceptionError(int errornumber)
{
  Vector<int> bexcep (0);
  if ((errornumber > 0) && (errornumber <= curerrorlist)) {
    int addrbexcep = ((errornumber-1)*(maxtermn+(maxnontm*2)))+maxtermn;
    int index = 0;
    for (int nt=1; nt<maxnontm; nt++) {
      if (memalter[addrbexcep+nt]) {
	bexcep.expandBy(1);
	bexcep[index++] = getBaseNT(-nt);
      }
    }
  }
  return bexcep;
}

Vector<int> ScpParser::getBubblesCloseParenthesisError(int errornumber)
{
  Vector<int> cparen (0);
  if ((errornumber > 0) && (errornumber <= curerrorlist)) {
    int addrcparen = ((errornumber-1)*(maxtermn+(maxnontm*2)))+maxtermn+maxnontm;
    int index = 0;
    for (int nt=1; nt<maxnontm; nt++) {
      if (memalter[addrcparen+nt]) {
	cparen.expandBy(1);
	cparen[index++] = getBaseNT(-nt);
      }
    }
  }
  return cparen;
}

int ScpParser::nextAnalysis()
{
  if (rootnode) {
    if (!prevnextanal) {
      initAnalysis();
      prevnextanal = 1;
    }
    return skipAnalysis(rootnode);
  }
  return 0;
}

void ScpParser::initAnalysis()
{
  for (int nd=1; nd <= curnode; nd++)
    memnode[nd].skipanal = memnode[nd].initanal;
}

int ScpParser::skipAnalysis(int node)
{
  if (node <= 0)
    return 0;
  for (int nch=0; nch<getNumberOfChildren(node); nch++) {
    int ch = getChild(node,nch);
    if (skipAnalysis(ch))
      return 1;
  }
  if (memanal[memnode[node].initanal].nextanal) {
    memnode[node].initanal = memanal[memnode[node].initanal].nextanal;
    return 1;
  } else {
    memnode[node].initanal = memnode[node].skipanal;
    return 0;
  }
}

/* Be careful with MAXINT */
int ScpParser::setAmbNode(int node)
{
  int pnode;
  int pevent;
  int ambnode = 0;
  int ambanal;
  int anal = memnode[node].initanal;
  while (anal) {
    ambanal = 1;
    if (pnode = memanal[anal].prevnode) {
      if (memambnode[pnode])
	ambanal *= memambnode[pnode];
      else
	ambanal *= setAmbNode(pnode);
    }
    pevent = memanal[anal].prevevent;
    while (pevent) {
      if (pnode = memevent[pevent].prevnode) {
	if (memambnode[pnode])
	  ambanal *= memambnode[pnode];
	else
	  ambanal *= setAmbNode(pnode);
      }
      pevent = memevent[pevent].prevevent;
    }
    anal = memanal[anal].nextanal;
    ambnode += ambanal;
  }
  memambnode[node] = ambnode;
  return ambnode;
}

void ScpParser::freeCurrentParse()
{
  maxlenterm = 0;
  delete [] nodeterm;
  nodeterm = 0;

  lenevent = 0;
  delete [] memevent;
  memevent = 0;

  lennode = 0;
  delete [] memnode;
  memnode = 0;
  delete [] memambnode;
  memambnode = 0;

  lenanal = 0;
  delete [] memanal;
  memanal = 0;

  lendetect = 0;
  delete [] memdetect;
  memdetect = 0;
  delete [] typedetect;
  typedetect = 0;

  lenalter = 0;
  delete [] memalter;
  memalter = 0;
}

void ScpParser::printCurrentParse()
{
  cout << " *** MSCP: printCurrentParse *** " << endl;
  int root = getRootNode();
  cout << "      getRootNode = " << root << endl;
  int nextana;
  do {
    cout << "#############################" << endl;
    printNode(root);
    nextana = nextAnalysis();
    cout << "# nextAnalysis = " << nextana << endl;
  } while (nextana);
}

void ScpParser::printNode(int node)
{
  cout << "*****************************" << endl;
  cout << "* Node: " << node << endl;
  int first = getFirstPosition(node);
  cout << "getFirstPosition = " << first << endl;
  int last = getLastPosition(node);
  cout << "getLastPosition = " << last << endl;
  int prod = getProductionNumber(node);
  cout << "getProductionNumber = " << prod << endl;
  int nch = getNumberOfChildren(node);
  cout << "getNumberOfChildren = " << nch << endl;
  int xch;
  for (int ch=0; ch < nch; ch++) {
    xch = getChild(node,ch);
    cout << "getChild (" << ch << ") = " << xch << endl;
  }
  cout << "*****************************" << endl;
  for (int ch=0; ch < nch; ch++) {
    xch = getChild(node,ch);
    printNode(xch);
  }
}


void ScpParser::printSentence()
{
  cout << "###############################" << endl;
  cout << "##      printSentence " << endl;

  for (int nd=1; nd<=curnode; nd++) {
    cout << "=====================" << endl;
    cout << " Node: " << nd << endl;
    cout << "   Symbol: " << printSymbol(memnode[nd].symbol) << endl;
    cout << "   First position: " << memnode[nd].fpos << endl;
    cout << "   Last position:  " << memnode[nd].lpos << endl;
    int anal = memnode[nd].initanal;
    while (anal) {
      cout << "     Anal: " << anal << endl;
      cout << "       Production: " << memanal[anal].prodtn-1 << endl;
      cout << "       Nodes: "; 
      printAnalNodes(memanal[anal].prevevent);
      if (memanal[anal].prevnode)
	cout << memanal[anal].prevnode << " ";
      anal = memanal[anal].nextanal;
      cout << endl;
    }
  }
}

void ScpParser::printAnalNodes(int event)
{
  if (event) {
    printAnalNodes(memevent[event].prevevent);
    if (memevent[event].prevnode)
      cout << memevent[event].prevnode << " ";
  }
}

char* ScpParser::printSymbol(int absl)
{
  char p[100];

  sprintf(p,"%d<%d/%d>",absl,getBaseNT(absl),getPrecNT(absl));
  return strdup(p);
}

void ScpParser::printErrorDetect()
{
  cout << " *** MSCP: printErrorDetection *** " << endl;

  for (int er=1; er <= curerrorlist; er++) {
    cout << "#############################" << endl;
    cout << "#  ErrorNumber: " << er << endl;
    cout << "#      Position: " << errorlist[er]-1 << endl;
    cout << "#      Alternatives: ";
    int linalt = 0;
    int addralter = (er-1)*maxtermn;
    for (int tm=0; tm < maxtermn; tm++) {
      if (memalter[addralter+tm]) {
	if (linalt > 8) {
	  cout << endl << "#                    ";
	  linalt++;
	}
	printf("%4d ",tm);
	linalt++;
      }
    }
    cout << endl;
  }
}




