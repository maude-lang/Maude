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
//	Code for handling importation.
//

void
PreModule::processImports()
{
  /*
  const ModuleDatabase::NameSet::const_iterator e = autoImports.end();
  for (ModuleDatabase::NameSet::const_iterator i = autoImports.begin(); i != e; ++i)
    importModule(*i, *this);
  */
  int nrAutoImports = autoImports.cardinality();
  for (int i = 0; i < nrAutoImports; i++)
    importModule(autoImports.index2Int(i), *this);
  int nrImports = imports.length();
  for (int i = 0; i < nrImports; i++)
    {
      Vector<Token>& import = imports[i];
      if (import.length() != 2)
	{
	  IssueWarning(LineNumber(import[0].lineNumber()) <<
		       ": bad import: " << QUOTE(import));
	}
      else
	{
	  WarningCheck(import[0].code() != Token::encode("us") &&
		       import[0].code() != Token::encode("using"), 
		       LineNumber(import[0].lineNumber()) <<
		       ": importation mode " << QUOTE("using") <<
		       " not supported - treating it like " <<
		       QUOTE("including") << '.');
	  importModule(import[1].code(), import[1].lineNumber());
	}
    }
}

void
PreModule::importModule(int name, const LineNumber& lineNumber)
{
  if (PreModule* m = interpreter.getModule(name))
    {
      if (VisibleModule* fm = m->getFlatSignature())
	{
	  fm->economize();  // HACK
	  if (fm->isBad())
	    {
	      IssueWarning(lineNumber << ": unable to import module " <<
			   QUOTE(m) << " due to unpatchable errors.");
	    }
	  else
	    flatModule->addImport(fm);
	}
      else
	{
	  IssueWarning(lineNumber << ": mutually recursive import of module " <<
		       QUOTE(m) << " ignored.");
	}
    }
  else
    {
      IssueWarning(*this << ": module " << QUOTE(Token::name(name)) <<
		   " does not exist.");
    }
}
