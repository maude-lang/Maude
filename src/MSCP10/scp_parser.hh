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
 ******* scp_parser.hh
 *******/

#ifndef _scp_parser_hh_
#define _scp_parser_hh_

#include "macros.hh"
#include "vector.hh"
#include "string.h"

#include "scp_kernel.hh"

class ScpParser
{
  class ScpBaseNT*	membasent;
  int			lenbasent;
  int			maxbasent;
  class ScpAbslNT*	memabslnt;
  int			lenabslnt;
  int			maxabslnt;
  int*			ldgrabslnt;
  int*			rdgrabslnt;
  int                   maxnontm;
  class ScpProdtn*	memprodtn;
  int			lenprodtn;
  int			curprodtn;
  int*			ldgrprodtn;
  int*			rdgrprodtn;
  int*			memrhs;
  int			lenrhs;
  int			currhs;
  int			maxtermn;
  int*			ldgrtermn;
  int*			rdgrtermn;
  class ScpBubble*	membubble;
  int			lenbubble;
  int			curbubble;
  int*			memexcept;
  int			lenexcept;
  int			curexcept;
  int			compiled;
  int*			addrtermn;
  int*                  addrnontm;
  int*			covtbl;
  int*                  covnttb;
  int*                  covtmtb;
  int*                  ldertmtb; /* same as covtmtb */
  int*			ldernttb;
  int*                  rdernttb;
  int*                  invrdernttb;
  char*                 adjtb;
  char*                 bubexcept;
  class ScpCov*		memcov;
  class ScpCov*         bubcov;

  int*                  nodeterm;
  int*                  eventterm;
  int*                  inputterm;
  int                   maxlenterm;
  int                   lengthterm;
  int			rootterm;
  int                   ambnodes;
  int                   rootnode;

  class ScpEvent*	memevent;
  int			lenevent;
  int			curevent;

  class ScpNode*	memnode;
  int			lennode;
  int			curnode;

  class ScpAnal*        memanal;
  int                   lenanal;
  int                   curanal;
  int                   prevnextanal;
  
  int*                  memdetect;
  int*                  typedetect;
  int                   curdetect;
  int                   lendetect;
  int                   lasttokendetect;
  int                   prevlasttokendetect;
  int*                  errorlist;
  int*                  errorlevel;
  int                   curerrorlist;
  char*                 memalter;
  int                   lenalter;
  int                   curalter;

  int*                  memprodruntoken;
  int*                  memprodrunnode;
 
  int*                  memambnode;
  int                   prevambnode;

  int                   parseErrorRecover;

  /* MEMRHS must be at least 2 */
  /* MEMPRODTN must be at least 2 */
  enum { MEMPRODTN =   1000, ADDPRODTN =  1000,
	 MEMBASENT =    200, ADDBASENT =   200,
	 MEMABSLNT =    400, ADDABSLNT =   400,
	 MEMRHS    =   5000, ADDRHS    =  5000, 
	 MEMBUBBLE =     10, ADDBUBBLE =    10,
	 MEMBUBCOV =    100, ADDBUBCOV =   100,
	 MEMEXCEPT =     50, ADDEXCEPT =    50,
	 MEMTERMN  =    500, ADDTERMN  =   500,
         MEMLENTERM=    100, ADDLENTERM=   100,
         MEMEVENT  =    500, ADDEVENT  =   500,
         MEMNODE   =    500, ADDNODE   =   500,
         MEMANAL   =    500, ADDANAL   =   500,
         MEMDETECT =     50, ADDDETECT =    50,
                             ADDALTER  =    10}; 
  // insertProd
  int insertBasePrecNT(int,int);
  int getBaseNT(int);
  int getPrecNT(int);

  // compileParser
  void compileDerivationTables();
  void compileLDerGraph();
  void compileLDerTable();
  void insertLDerNT(int);
  void compileRDerGraph();
  void compileRDerTable();
  void insertRDerNT(int);
  void compileCoverageTables();
  void compileAdjacencyTables();
  void insertAdjTM(int,int);
  void insertAdjNT(int,int);

  // parseSentence
  void tryEvent(int,int,int,int,int,int);
  void runToken(int);
  void runTokenBubble(int); 
  void tryNode(int,int,int,int,int);
  void runNode(int);
  void runBubble(int,int,int,int,int,int=0);
  void initAnalysis();
  int  skipAnalysis(int);
  int  setAmbNode(int);
  void printErrorDetect();

  // Error Detection and Recovery
  void errorDetect(int,int,int = 0,int = 0);
  void errorRecovery(int = 0);

  enum { STANDARD_ERROR = 0,
	 EXCEPTION_ERROR = 1,
	 CLOSEPAR_ERROR = 2,
         MAXERRORLEVEL = 10};

  // printSentence
  void printAnalNodes(int);
  char* printSymbol(int);
  

public:

  //	ScpParser
  //	   creates a new parser
  ScpParser();

  //	~ScpParser
  //	   deletes a previously created parser
  ~ScpParser();

  //	insertProd	(rule version)
  //	   inserts a new production in a previously created parser
  void insertProd (int xlhs, const Vector<int>& xrhs,
		   int xprec,const Vector<int>& xgather);
  void compileParser();

  //	insertProd	(bubble version)
  //	   indicates to the parser that a symbol is of type bubble,
  //		and specifies its properties
  //	   Description:
  //		nonTerminal: must be a negative number,
  //			it is not necessary de define previously nonTerminal
  //		lowBound: a natural number (0 to INT_MAX)
  //			0 means that the bubble may be empty
  //		upperBound: an integer number 
  //			if lowBound == upperBound then the Bubble has
  //				a fixed length and is not checked with the grammar
  //			to define a bubble without a fixed maximum length,
  //				use upperBound = -1
  //		leftParenToken: 
  //			0-N : the token number associated with the
  //				left parenthesis
  //			-1 : the bubble doesn't use parenthesis
  //		rightParenToken: similar to leftParenToken
  //			
  //
  void insertProd(int nonTerminal, 
		  int lowBound, 
		  int upperBound,
		  int leftParenToken,
		  int rightParenToken,
		  const Vector<int>& exceptionTokenList );

  // 	parseSentence
  //	    returns the number of analysis:
  //		0 => ungrammatical input (parsing failure)
  //		1 => grammatical and non-ambiguous input (1)
  //		N>1 => grammatical and ambiguous input
  // double parseSentence(Vector<int>& term,int root);
  int parseSentence(Vector<int>& term,int root,int errorRecover=0);

  //	getRootNode
  //	    returns the index of the root node
  //		-1 => there isn't a root node (ungrammatical input)
  //		N>=0 => the index of the root node
  int getRootNode();

  //	getProductionNumber(int node)
  //	    returns the production index which created the node
  //		-1 => if the node is a token, or the node index
  //				is incorrect
  //		N>=0 => the production index
  int getProductionNumber(int node);

  //	getFirstPosition(int node)
  //	    returns the position of the left-most token
  //	    covered by node
  //		-1 => no active parser or incorrect node index
  //		N>=0 => token position
  int getFirstPosition(int node);

  //	getLastPosition(int node)
  //	    returns the position of the right-most token
  //	    covered by node
  //		-1 => no active parser or incorrect node index
  //		N>=0 => token position
  int getLastPosition(int node);

  //	getNumberOfChildren(int node)
  //	    returns the number of Non-Terminal children of node
  //	    that is, the length of the gather of the
  //	    production taht created the node
  //		-1 => no active parser, or incorrect node
  //		N>=0 => number of NT children (maybe = 0)
  int getNumberOfChildren(int node);

  //	getChild(int node,int childnumber)
  //	    returns the index of the node corresponding to
  //	   	 the position indicated by childnumber
  //	    This function only considers NonTerminal children
  //	    The numeration of children begins with 0
  //	    That is, the set of possible children goes 
  //	    from 0 to getNumberOfNTChildren - 1
  //		-1 => something wrong
  //		N>=0 => node index
  int getChild(int node,int childnumber);

  //	nextAnalysis()
  //	    changes the configuration of the parser to the
  //	    next parser
  //	       -1 => there is no active parser
  //		0 => there isn't more analysis
  //		1 => the parser has changed the configuration
  int nextAnalysis();

  //	freeCurrentParse()
  //	    frees the memory used by the current parse
  void freeCurrentParse();


  //    getNumberOfErrors()
  //        when parseSentence() returns 0, getNumberOfErrors()
  //        obtains the total number of errors that the error
  //        detection and error recovery have been able to detect
  //        Errors are numbered beginning with 1
  int getNumberOfErrors();

  //    getErrorPosition(int errornumber)
  //        for each value errorn in 1 - getNumberOfErrors(), the function
  //        getErrorPosition(errorn) obtains the position where the error
  //        has appeared. Positions are numbered beginning with 0. So,
  //        for an input term of length L, the usual positions of errors
  //        are 0 - (L-1). Nevertheless, getErrorPosition() may return L,
  //        which means that the error has appeared at the end of the
  //        input: premature end.
  //        The function will return -1 is the argument (errornumber)
  //        isn't in the interval 1 - getNumberOfErrors()
  //
  int getErrorPosition(int);

  // The error detection strategy includes three kinds of errors.
  //  1.- Standard Errors
  //  2.- Exception Errors (inside Bubbles)
  //  3.- CloseParenthesis Errors (inside Bubbles)
  // As it is possible that more than just one type of error appears
  // at one position, the system distinguishes three functions:
  
  // getAlternativesStandardError(int errornumber)
  //      returns the set of alternatives (tokens) that may appear
  //      at the position of the error
  Vector<int> getAlternativesStandardError(int errornumber);
  
  // getBubblesExceptionError(int errornumber)
  //      returns the set of bubbles (non-terminals) that have failed
  //      at that position due to an exception error
  Vector<int> getBubblesExceptionError(int errornumber);

  // getBubblesCloseParenthesisError(int errornumber)
  //      returns the set of bubbles (non-terminals) that have failed
  //      at that position due to an unbalanced close parenthesis
  Vector<int> getBubblesCloseParenthesisError(int errornumber);

  // Notes: 
  //   1.- If the three functions return 0-length vectors, this means
  //       that the input must end at the position of the error

  // Auxiliar Functions
  void printGrammar();
  void printSentence();
  int  getLHSProduction(int);

  void printCurrentParse();
  void printNode(int);
};

typedef ScpParser Parser;

#endif
