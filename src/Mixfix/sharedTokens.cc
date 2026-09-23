/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2026 SRI International, Menlo Park, CA 94025, USA.

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
//      Implementation for class SharedTokens.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//	front end class definitions
#include "token.hh"
#include "sharedTokens.hh"

#define MACRO(Name, String) \
   int SharedTokens::Name;
#include "specialTokens.cc"
#undef MACRO

SharedTokens::SharedTokens()
{
  if (leftParen == 0)
    {
#define MACRO(Name, String) \
      Name = Token::encode(String);
#include "specialTokens.cc"
#undef MACRO
    }
}

Index
SharedTokens::skipBracePair(const Vector<Token>& tokens, Index start, Index beyondEnd)
{
  //
  //	Skip over a single top level {...} part of a structured sort.
  //	Returns index of closing } if valid {...} seen or NONE otherwise.
  //
  enum class State {
    EXPECT_LEFT_BRACE,
    EXPECT_SORT_COMPONENT,
    EXPECT_COMMA_OR_BRACE
  };
  
  Index depth = 0;
  State state = State::EXPECT_LEFT_BRACE;

  for (Index i = start; i < beyondEnd; ++i)
    {
      int code = tokens[i].code();
      switch (state)
	{
	case State::EXPECT_COMMA_OR_BRACE:
	  {
	    if (code == rightBrace)
	      {
		--depth;
		if (depth == 0)
		  return i;
		break;
	      }
	    if (code == comma)
	      {
		state = State::EXPECT_SORT_COMPONENT;
		break;
	      }
	    // fall thru
	  }
	case State::EXPECT_LEFT_BRACE:
	  {
	    if (code != leftBrace)
	      return NONE;
	    ++depth;
	    state = State::EXPECT_SORT_COMPONENT;
	    break;
	  }
	case State::EXPECT_SORT_COMPONENT:
	  {
	    if (!Token::isValidSortComponent(code))
	      return NONE;
	    state = State::EXPECT_COMMA_OR_BRACE;
	    break;
	  }
	}
    }
  //
  //	Ran out of tokens.
  //
  return NONE;
}

Index
SharedTokens::skipKindName(const Vector<Token>& tokens,
			   Index start,
			   Index beyondEnd,
			   Vector<int>& sortNames)
{
  enum class State {
    EXPECT_LEFT_BRACKET,
    EXPECT_SORT_NAME,
    EXPECT_COMMA_OR_RIGHT_BRACKET
  };

  State state = State::EXPECT_LEFT_BRACKET;

  for (Index i = start; i < beyondEnd; ++i)
    {
      DebugInfo("token " << tokens[i]);
      int code = tokens[i].code();
      switch (state)
	{
	case State::EXPECT_LEFT_BRACKET:
	  {
	    if (code != leftBracket)
	      return NONE;
	    state = State::EXPECT_SORT_NAME;
	    break;
	  }
	case State::EXPECT_SORT_NAME:
	  {
	    int singleTokenName = code;
	    int auxProp = Token::auxProperty(code);
	    if (auxProp != Token::AUX_STRUCTURED_SORT)
	      {
		if (auxProp != Token::AUX_SORT)
		  return NONE;
		//
		//	Check for structured sort as separate tokens.
		//
		Index end = i;
		for (;;)
		  {
		    Index last = skipBracePair(tokens, end + 1, beyondEnd);
		    if (last == NONE)
		      break;
		    end = last;
		  }
		if (end != i)
		  {
		    singleTokenName = Token::bubbleToPrefixNameCode(tokens, i, end + 1);
		    i = end;
		  }
	      }
	    sortNames.push_back(singleTokenName);
	    state = State::EXPECT_COMMA_OR_RIGHT_BRACKET;
	    break;
	  }
	case State::EXPECT_COMMA_OR_RIGHT_BRACKET:
	  {
	    if (code == rightBracket)
	      return i;
	    if (code != comma)
	      return NONE;
	    state = State::EXPECT_SORT_NAME;
	    break;
	  }
	}
    }
  return NONE;
}
