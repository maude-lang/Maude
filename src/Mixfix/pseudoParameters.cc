/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for computing psuedo-parameters - identifiers that look like parameters but are not.
//	They could be view names or random junk in sorts and constants with Token::AUX_STRUCTURED_SORT names.
//	We need to be aware of them in case they are captured by real parameters and cause chaos during module operations.
//

bool
ImportModule::accumulatePseudoParameters(int id)
{
  //
  //	Accumulate identifers that look like parameters.
  //	Return true if id had the Token::AUX_STRUCTURED_SORT property and false otherwise.
  //
  if (Token::auxProperty(id) == Token::AUX_STRUCTURED_SORT)
    {
      //
      //	Identifier looks like
      //	  header{p1,p2,...,pn}
      //	where each pi could itself have the Token::AUX_STRUCTURED_SORT property.
      //
      int header;
      Vector<int> parameters;
      Token::splitParameterList(id, header, parameters);
      for (int p : parameters)
	{
	  if (!accumulatePseudoParameters(p))
	    pseudoParameters.insert(p);
	}
      return true;
    }
  return false;
}

const EnclosingObject::ParameterSet&
ImportModule::getPseudoParameters()
{
  if (!pseudoParametersComputed)
    {
      pseudoParametersComputed = true;
      //
      //	Look at each sort.
      //
      for (Sort* s : getSorts())
	accumulatePseudoParameters(s->id());
      //
      //	Look at each constant.
      //
      for (Symbol* s : getSymbols())
	{
	  if (s->arity() == 0)
	    accumulatePseudoParameters(s->id());
	}
      //
      //	Remove actual parameters.
      //
      for (int p : parameterNames)
	pseudoParameters.erase(p);
    }
  return pseudoParameters;
}
