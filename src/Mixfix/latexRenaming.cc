/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023-2024 SRI International, Menlo Park, CA 94025, USA.

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
//      Code for generating LaTeX for renamings.
//

string
Renaming::latexRenamingType(const set<int>& sorts, const Module* module)
{
  string result = "\\maudeLeftBracket";
  const char* sep = "";
  for (int i : sorts)
   {
     result += sep;
     result += MixfixModule::latexSort(i, module);
     sep = "\\maudeComma";
    }
  result += "\\maudeRightBracket";
  return result;
}

string
Renaming::latexRenaming(const char* sep,
			const char* sep2,
			const Module* fromModule,
			const Module* toModule,
			bool showProcessed) const
{
  //
  //	Problem situations that pretty op printing needs to handle.
  //
  const int fromOpProblems = Token::EXPOSED_COLON | Token::EXPOSED_TO; // colons and "to" in from operators names are always a problem
  int toOpProblems = (typeid(*this) == typeid(Renaming)) ?
    (Token::EXPOSED_COMMA | Token::EXPOSED_LEFT_BRACKET) :  // commas and left brackets in target operator names are a problem for renamings
    (Token::EXPOSED_TERM | Token::EXPOSED_DOT);  // "term" and dots in target operator names are a problem for views
  //
  //	If we are printing the unprocessing renaming, include class mappings.
  //
  string result;
  if (!showProcessed)
    {
      for (const auto& c : classMap)
	{
	  result += sep;
	  result += "\\maudeKeyword{class}\\maudeSpace";
	  result += MixfixModule::latexSort(c.fromClass.code(), fromModule);
	  result += "\\maudeSpace\\maudeKeyword{to}\\maudeSpace";
	  result += MixfixModule::latexSort(c.toClass.code(), toModule);
	  sep = sep2;
	}
      for (const auto& a : attrMap)
	{
	  result += sep;
	  result += "\\maudeKeyword{attr}\\maudeSpace";
	  result += Token::latexIdentifier(a.fromAttr.code());
	  if (!(a.type.empty()))
	    {
	      result += "\\maudeHasSort";
	      result += latexRenamingType(a.type, fromModule);
	    }
	  result += "\\maudeSpace\\maudeKeyword{to}\\maudeSpace";
	  result += Token::latexIdentifier(a.toAttr.code());
	  sep = sep2;
	}
    }
  //
  //	If we are printing the processed renaming, include generated sort mappings.
  //
  Index nrSortMappings = (nrUserSortMappings == NONE || showProcessed) ? sortMapIndex.size() : nrUserSortMappings;
  for (Index i = 0; i < nrSortMappings; ++i)
    {
      auto sm = sortMapIndex[i];
      result += sep;
      result += "\\maudeKeyword{sort}\\maudeSpace";
      result += MixfixModule::latexSort(sm->first, fromModule);
      result += "\\maudeSpace\\maudeKeyword{to}\\maudeSpace";
      result += MixfixModule::latexSort(sm->second, toModule);
      sep = sep2;
    }
  //
  //	If we are printing the processed renaming, include generated op mappings.
  //
  Index nrOpMappings = (nrUserOpMappings == NONE || showProcessed) ? opMapIndex.size() : nrUserOpMappings;
  for (Index i = 0; i < nrOpMappings; ++i)
    {
      auto om = opMapIndex[i];
      result += sep;
      result += "\\maudeKeyword{";
      result += (!showProcessed && om->second.mappingType == MappingType::MSG) ? "msg" : "op";
      result += "}\\maudeSpace";
      int id = om->first;
      if (om->second.types.size() == 1)
	result += MixfixModule::latexConstant(id, fromModule, fromOpProblems);  // could be parameterized
      else
	result += MixfixModule::latexPrettyOpName(id, fromOpProblems);
      if (!om->second.types.empty())
	{
	  result += "\\maudeHasSort";
	  Index nrDomainTypes = om->second.types.size() - 1;
	  for (Index i = 0; i < nrDomainTypes; ++i)
	    {
	      if (i != 0)
		result += "\\maudeSpace";
	      result += latexRenamingType(om->second.types[i], fromModule);
	    }
	  result += "\\maudeFunction";
	  result += latexRenamingType(om->second.types[nrDomainTypes], fromModule);
	}
      result += "\\maudeSpace\\maudeKeyword{to}\\maudeSpace";
      int target = om->second.name;
     if (om->second.types.size() == 1)
       result += MixfixModule::latexConstant(target, toModule, toOpProblems);
     else
       result += MixfixModule::latexPrettyOpName(target, toOpProblems);
     //
     //	If we have any attributes in target, print them inside []s.
     //
     if (om->second.prec >= MixfixModule::MIN_PREC ||
	 !om->second.gather.empty() ||
	 !om->second.format.empty() ||
	 om->second.latexMacro != NONE ||
	 om->second.rpo != NONE)
       {
	 sep = "\\maudeSpace\\maudeLeftBracket";
	 if (om->second.prec >= MixfixModule::MIN_PREC)
	   {
	     result += sep;
	     result += "\\maudeKeyword{prec}\\maudeSpace\\maudeNumber{";
	     result += to_string(om->second.prec);
	     result += "}";
	     sep = "\\maudeSpace";
	   }
	 const Vector<int>& gather = om->second.gather;
	 if (!gather.empty())
	   {
	     result += sep;
	     result += "\\maudeKeyword{gather}\\maudeSpace\\maudeLeftParen";
	     Index gatherLength = gather.size();
	     for (Index i = 0; i < gatherLength; ++i)
	       {
		 if (i != 0)
		   result += "\\maudeSpace";
		 switch (gather[i])
		   {
		   case MixfixModule::GATHER_e:
		     result += "\\maudeNormal{e}";
		     break;
		   case MixfixModule::GATHER_E:
		     result += "\\maudeNormal{E}";
		     break;
		   case MixfixModule::GATHER_AMP:
		     result += "\\maudeNormal{\\&}";
		     break;
		   }
	       }
	     result += "\\maudeRightParen";
	     sep = "\\maudeSpace";
	   }
	 const Vector<int>& format = om->second.format;
	 if (!format.empty())
	   {
	     result += sep;
	     result += "\\maudeKeyword{format}\\maudeSpace\\maudeLeftParen";
	     Index formatLength = format.size();
	     for (Index i = 0; i < formatLength; ++i)
	       {
		 if (i != 0)
		   result += "\\maudeSpace";
		 result += "\\maudeNormal{";
		 result += Token::name(format[i]);
		 result += "}";
	       }
	     result += "\\maudeRightParen";
	     sep = "\\maudeSpace";
	   }
	 if (om->second.latexMacro != NONE)
	    {
	      result += sep;
	      result += "\\maudeKeyword{latex}\\maudeSpace\\maudeLeftParen\\maudeSymbolic{";
	      result += Token::latexName(om->second.latexMacro);
	      result += "}\\maudeRightParen";
	      sep = "\\maudeSpace";
	    }
	 if (om->second.rpo != NONE)
	    {
	      result += sep;
	      result += "\\maudeKeyword{rpo}\\maudeSpace\\maudeNumber{";
	      result += to_string(om->second.rpo);
	      result += "}";
	      sep = "\\maudeSpace";
	    }
	 result += "\\maudeRightBracket";
       }
     sep = sep2;
    }
  for (auto lm : labelMapIndex)
    {
      result += sep;
      result += "\\maudeKeyword{label}\\maudeSpace\\maudeLabel{";
      result += Token::latexName(lm->first);
      result += "}\\maudeSpace\\maudeKeyword{to}\\maudeSpace\\maudeLabel{";
      result += Token::latexName(lm->second);
      result += "}";
      sep = sep2;
    }
  for (auto sm : stratMapIndex)
    {
      result += sep;
      result += "\\maudeKeyword{strat}\\maudeSpace";
      result += Token::latexIdentifier(sm->first);
      if (!sm->second.types.empty())
	{
	  result += "\\maudeHasSort";
	  for (const IdSet& t : sm->second.types)
	    {
	      result += latexRenamingType(t, fromModule);
	      result += "\\maudeSpace";
	    }
	}
      else
	result += "\\maudeSpace";
      Assert(sm->second.name != NONE && sm->second.expr == 0, "renamings with strat->expr mappings are not printable");
      result += "\\maudeKeyword{to}\\maudeSpace";
      result += Token::latexIdentifier(sm->second.name);
      sep = sep2;
    }
  return result;
}
