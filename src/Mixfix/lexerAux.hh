/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2021 SRI International, Menlo Park, CA 94025, USA.

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
//	Declarations for auxiliary functions needed by lexical analyzer.
//
void lexerIdMode();
void lexerTokenTreeMode(int terminatingTokens);
void lexerCmdMode();
void lexerOpMode();
void lexerInitialMode();
void lexerFileNameMode();
void lexerStringMode();
void lexerLatexMode();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
bool handleEof();
void eatComment(bool firstNonWhite);
void cleanUpLexer();
void checkForPending();

void setDebugMode(bool polarity);
bool generateImpliedStep();


void lexBubble(int termination, int minLen);
void lexBubble(const Token& first, int termination, int minLen, int pCount = 0);
void lexContinueBubble(const Token& next, int termination, int minLen, int pCount = 0);
void lexSave(const Token& first);
void lexContinueSave(const Token& next);

void bubbleEofError();
bool startOfStatement(int code);
const char* missingToken();
bool analyzeEofError();

enum TERMINATION_TOKENS
  {
    BAR_COLON = 0x1,
    BAR_COMMA = 0x2,
    BAR_LEFT_BRACKET = 0x4,
    BAR_EQUALS = 0x8,
    BAR_ARROW2 = 0x10,
    BAR_TO = 0x20,
    BAR_IF = 0x40,
    BAR_RIGHT_PAREN = 0x80,
    BAR_OP_ATTRIBUTE = 0x100,
    BAR_RIGHT_BRACKET = 0x200,

    BAR_ASSIGN = 0x400,

    END_STATEMENT = 0x40000000,
    END_COMMAND = 0x80000000
  };
