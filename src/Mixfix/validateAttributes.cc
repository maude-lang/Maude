/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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

void
MixfixModule::validateAttributes(Token prefixName,
				 const Vector<Sort*>& domainAndRange,
				 SymbolType& symbolType)
{
  //
  //	Do sanity checks on flags in symbolType and clear any that
  //	aren't satisfied by the domainAndRange.
  //
  int lineNr = prefixName.lineNumber();
  int nrArgs = domainAndRange.length() - 1;
  //
  //	iter operators must be unary with domain and range sorts in the same
  //	connected component.
  //
  if (symbolType.hasFlag(SymbolType::ITER))
    {
      if (nrArgs != 1)
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": iter operator " << QUOTE(prefixName) <<
		       " has " << QUOTE(nrArgs) << " domain sorts rather than 1.");
	  symbolType.clearFlags(SymbolType::ITER);
	}
      else
	{
	  //
	  //	May be a polymorph with null sorts.
	  //
	  Sort* domainSort = domainAndRange[0];
	  Sort* rangeSort = domainAndRange[1];
	  if (domainSort == 0)
	    {
	      if (rangeSort != 0)
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": iter operator " << QUOTE(prefixName) <<
			       " has a polymorphic domain sort " <<
			       " but a range sort " << QUOTE(rangeSort) << '.');
		  symbolType.clearFlags(SymbolType::ITER);
		}
	    }
	  else
	    {
	      if (rangeSort == 0)
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": iter operator " << QUOTE(prefixName) <<
			       " has a domain sort " << QUOTE(domainSort) <<
			       " but a polymorphic range sort.");
		  symbolType.clearFlags(SymbolType::ITER);
		}
	      else
		{
		  if (domainSort->component() != rangeSort->component())
		    {
		      IssueWarning(LineNumber(lineNr) <<
				   ": iter operator " << QUOTE(prefixName) <<
				   " has a domain sort " << QUOTE(domainSort) <<
				   " in a different connected component from its range sort " <<
				   QUOTE(rangeSort) << '.');
		      symbolType.clearFlags(SymbolType::ITER);
		    }
		}
	    }
	}
    }
  //
  //	Associative symbols must have two arguments and have both domain
  //	sorts and range sort in the same connected component.
  //
  if (symbolType.hasFlag(SymbolType::ASSOC))
    {
      if (nrArgs != 2)
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": assoc operator " << QUOTE(prefixName) <<
		       " has " << nrArgs << " rather than 2 domain sorts.");
	  symbolType.clearFlags(SymbolType::ASSOC);
	}
      else
	{
	  Sort* domainSort1 = domainAndRange[0];
	  Sort* domainSort2 = domainAndRange[1];
	  Sort* rangeSort = domainAndRange[2];
	  if (domainSort1 == 0 && domainSort2 == 0 && rangeSort == 0)
	    ;  // ok
	  else if (domainSort1 != 0 && domainSort2 != 0 && rangeSort != 0)
	    {
	      bool leftOK = domainSort1->component() == rangeSort->component();
	      if (leftOK && domainSort1->component() == rangeSort->component())
		;  // ok
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": assoc operator " << QUOTE(prefixName) <<
			       " has a domain sort " <<
			       QUOTE((leftOK ? domainSort2 : domainSort1)) <<
			       " in a different connected component from its range sort " <<
			       QUOTE(rangeSort) << '.');
		  symbolType.clearFlags(SymbolType::ASSOC);
		}
	    }
	  else
	    {
	      IssueWarning(LineNumber(lineNr) <<
			   ": assoc operator " << QUOTE(prefixName) <<
			   " must either have all sorts polymorphic or none.");
	      symbolType.clearFlags(SymbolType::ASSOC);
	    }
	}
    }
  //
  //	Commutative symbols must have two arguments and have both domain
  //	sorts in the same connected component.
  //
  if (symbolType.hasFlag(SymbolType::COMM))
    {
      if (nrArgs != 2)
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": comm operator " << QUOTE(prefixName) <<
		       " has " << nrArgs << " rather than 2 domain sorts.");
	  symbolType.clearFlags(SymbolType::COMM);
	}
      else
	{
	  Sort* domainSort1 = domainAndRange[0];
	  Sort* domainSort2 = domainAndRange[1];
	  if (domainSort1 == 0 && domainSort2 == 0)
	    ;  // ok
	  else if (domainSort1 != 0 && domainSort2 != 0)
	    {
	      if (domainSort1->component() != domainSort2->component())
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": comm operator " << QUOTE(prefixName) <<
			       " has domain sort " << QUOTE(domainSort1) << 
			       " in a different connected component from domain sort " <<
			       QUOTE(domainSort2) << '.');
		  symbolType.clearFlags(SymbolType::COMM);
		}
	    }
	  else
	    {
	      IssueWarning(LineNumber(lineNr) <<
			   ": comm operator " << QUOTE(prefixName) <<
			   " must either have both domain sorts polymorhic or neither.");
	      symbolType.clearFlags(SymbolType::COMM);
	    }
	}
    }
  //
  //	Symbols with identity must have two arguments.
  //
  if (symbolType.hasAtLeastOneFlag(SymbolType::LEFT_ID | SymbolType::RIGHT_ID))
    {
      if (nrArgs != 2)
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": id operator " << QUOTE(prefixName) <<
		       " has " << nrArgs << " rather than 2 domain sorts.");
	  symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
	}
      else
	{
	  //
	  //	When we encounter an issue we clear both identity flags for
	  //	safety and to avoid a repeated warning, rather than just the
	  //	offending identity flag.
	  //
	  Sort* domainSort1 = domainAndRange[0];
	  Sort* domainSort2 = domainAndRange[1];
	  Sort* rangeSort = domainAndRange[2];
	  //
	  //	Explicit or implicit left identity: right domain must agree with range,
	  //	left domain cannot be polymorphic since we can't parse polymorphic
	  //	terms and we're not going to delay parsing identities until
	  //	the polymorph is instantiated.
	  //
	  if (symbolType.hasFlag(SymbolType::LEFT_ID) ||
	      (symbolType.hasAllFlags(SymbolType::RIGHT_ID | SymbolType::COMM)))
	    {
	      if (domainSort1 == 0)
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": operator " << QUOTE(prefixName) <<
			       " with a left identity cannot have left argument polymorphic.");
		  symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		}
	      else if (domainSort2 == 0 && rangeSort == 0)
		;  // ok
	      else if (domainSort2 != 0 && rangeSort != 0)
		{
		  if (domainSort2->component() == rangeSort->component())
		    ;  // ok
		  else
		    {
		      IssueWarning(LineNumber(lineNr) <<
				   ": id operator " << QUOTE(prefixName) <<
				   " has a domain sort " << QUOTE(domainSort2) <<
				   " in a different connected component from its range sort " <<
				   QUOTE(rangeSort) << '.');
		      symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		    }
		}
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": id operator " << QUOTE(prefixName) <<
			       " must have right domain sort and range sort polymophic.");
		  symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		}
	    }
	  //
	  //	Explicit or implicit right identity: left domain must agree with range,
	  //	right domain cannot be polymorphic since we can't parse polymorphic
	  //	terms and we're not going to delay parsing identities until
	  //	the polymorph is instantiated.
	  //
	  if (symbolType.hasFlag(SymbolType::RIGHT_ID) ||
	      (symbolType.hasAllFlags(SymbolType::LEFT_ID | SymbolType::COMM)))
	    {
	      if (domainSort2 == 0)
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": operator " << QUOTE(prefixName) <<
			       " with a right identity cannot have right argument polymorphic.");
		  symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		}
	      else if (domainSort1 == 0 && rangeSort == 0)
		;  // ok
	      else if (domainSort1 != 0 && rangeSort != 0)
		{
		  if (domainSort1->component() == rangeSort->component())
		    ;  // ok
		  else
		    {
		      IssueWarning(LineNumber(lineNr) <<
				   ": id operator " << QUOTE(prefixName) <<
				   " has a domain sort " << QUOTE(domainSort1) <<
				   " in a different connected component from its range sort " <<
				   QUOTE(rangeSort) << '.');
		      symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		    }
		}
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": id operator " << QUOTE(prefixName) <<
			       " must have left domain sort and range sort polymophic.");
		  symbolType.clearFlags(SymbolType::LEFT_ID | SymbolType::RIGHT_ID);
		}
	    }
	}
    }
  //
  //	Idempotent symbols must have two arguments and have both domain
  //	sorts and range sort in the same connected component.
  //
  if (symbolType.hasFlag(SymbolType::IDEM))
    {
      if (nrArgs != 2)
	{
	  IssueWarning(LineNumber(lineNr) <<
		       ": idem operator " << QUOTE(prefixName) <<
		       " has " << nrArgs << " rather than 2 domain sorts.");
	  symbolType.clearFlags(SymbolType::IDEM);
	}
      else
	{
	  Sort* domainSort1 = domainAndRange[0];
	  Sort* domainSort2 = domainAndRange[1];
	  Sort* rangeSort = domainAndRange[2];
	  if (domainSort1 == 0 && domainSort2 == 0 && rangeSort == 0)
	    ;  // ok
	  else if (domainSort1 != 0 && domainSort2 != 0 && rangeSort != 0)
	    {
	      bool leftOK = domainSort1->component() == rangeSort->component();
	      if (leftOK && domainSort1->component() == rangeSort->component())
		;  // ok
	      else
		{
		  IssueWarning(LineNumber(lineNr) <<
			       ": idem operator " << QUOTE(prefixName) <<
			       " has a domain sort " <<
			       QUOTE((leftOK ? domainSort2 : domainSort1)) <<
			       " in a different connected component from its range sort " <<
			       QUOTE(rangeSort) << '.');
		  symbolType.clearFlags(SymbolType::IDEM);
		}
	    }
	  else
	    {
	      IssueWarning(LineNumber(lineNr) <<
			   ": idem operator " << QUOTE(prefixName) <<
			   " must either have all sorts polymorphic or none.");
	      symbolType.clearFlags(SymbolType::IDEM);
	    }
	}
    }
  //
  //	Parameter constants must be constants.
  //
  if (symbolType.hasFlag(SymbolType::PCONST))
    {
      if (nrArgs != 0)
	{
	  IssueWarning(LineNumber(lineNr) << ": pconst attribute is only available for constants while operator " <<
		       QUOTE(prefixName) <<" has " << nrArgs << " domain sort" << pluralize(nrArgs) << "s.");
	  symbolType.clearFlags(SymbolType::PCONST);
	}
      if (symbolType.hasFlag(SymbolType::POLY))
	{
	  IssueWarning(LineNumber(lineNr) << ": pconst attribute is not allowed for polymorphic constant " << QUOTE(prefixName) << ".");
	  symbolType.clearFlags(SymbolType::PCONST);
	}
    }
}
