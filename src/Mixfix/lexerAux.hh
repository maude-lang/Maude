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
//	Declarations for auxiliary functions needed by lexical analyzer.
//

extern int inStackPtr;
extern YY_BUFFER_STATE inStack[];

void getInput(char* buf, int& result, int max_size);
void lexerIdMode();
void lexerCmdMode();
void lexerOpMode();
void lexerInitialMode();
void lexerFileNameMode();
void lexerStringMode();
bool includeFile(const string& directory, const string& fileName, bool silent, int lineNr);
bool handleEof();
void eatComment(bool firstNonWhite);
void cleanUpLexer();
