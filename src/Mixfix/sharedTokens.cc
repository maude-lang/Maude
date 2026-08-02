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
SharedTokens::skip(SkipType type, const Vector<Token>& tokens, Index start, Index beyondEnd)
{
  //
  //	For type == SORT_NAME we look for a legal sort name such as
  //	  S { Foo { Bar , X } { Y } , Baz }
  //	and return the index of the closing }, otherwise return NONE.
  //	If instantiation is true, we instead look for an instatiation such as
  //	  { Foo { Bar , X } { Y } , Baz }
  //	In both cases, we succeed if we encounter end-of-tokens or anything other
  //	than leftBrace while outside of {}s and in state EXPECT_COMMA_OR_BRACE
  //
  enum class State {
    EXPECT_LEFT_BRACE,
    EXPECT_SORT_NAME,
    EXPECT_COMMA_OR_BRACE
  };

  Index depth = 0;
  State state = (type == SORT_NAME) ? State::EXPECT_SORT_NAME : State::EXPECT_LEFT_BRACE ;

  for (Index i = start; i < beyondEnd; ++i)
    {
      int code = tokens[i].code();
      switch (state)
	{
	case State::EXPECT_COMMA_OR_BRACE:
	  {
	    if (depth == 0)
	      {
		if (code != leftBrace)
		  return i - 1;
		++depth;
		state = State::EXPECT_SORT_NAME;
		break;
	      }
	    if (code == rightBrace)
	      {
		--depth;
		break;
	      }
	    if (code == comma)
	      {
		state = State::EXPECT_SORT_NAME;
		break;
	      }
	    // fall thru
	  }
	case State::EXPECT_LEFT_BRACE:
	  {
	    if (code != leftBrace)
	      return NONE;
	    ++depth;
	    state = State::EXPECT_SORT_NAME;
	    break;
	  }
	case State::EXPECT_SORT_NAME:
	  {
	    if (Token::auxProperty(code) != Token::AUX_SORT)
	      return NONE;
	    state = State::EXPECT_COMMA_OR_BRACE;
	    break;
	  }
	}
    }
  //
  //	Ran out of tokens.
  //
  if (state == State::EXPECT_COMMA_OR_BRACE && depth == 0)
    return beyondEnd - 1;
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
      switch (state)
	{
	case State::EXPECT_LEFT_BRACKET:
	  {
	    if (tokens[i].code() != leftBracket)
	      return NONE;
	    state = State::EXPECT_SORT_NAME;
	    break;
	  }
	case State::EXPECT_SORT_NAME:
	  {
	    Index last = skip(SORT_NAME, tokens, i, beyondEnd);
	    if (last == NONE)
	      return NONE;
	    int singleTokenName = Token::bubbleToPrefixNameCode(tokens, i, last + 1);
	    DebugInfo("single token name " << Token::name(singleTokenName));
	    sortNames.push_back(singleTokenName);
	    state = State::EXPECT_COMMA_OR_RIGHT_BRACKET;
	    i = last;
	    break;
	  }
	case State::EXPECT_COMMA_OR_RIGHT_BRACKET:
	  {
	    int code = tokens[i].code();
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
