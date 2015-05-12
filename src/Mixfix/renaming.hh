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
//      Class for renamings.
//
#ifndef _renaming_hh_
#define _renaming_hh_
#include <set>
#include <map>
#include "syntaxContainer.hh"
#include "ropeStuff.hh"
#include "symbolType.hh"

class Renaming : public SyntaxContainer
{
public:
  virtual ~Renaming() {}

  void addSortMapping(Token from, Token to);
  void addLabelMapping(Token from, Token to);
  void addOpMapping(const Vector<Token>& tokens);
  void addVarDecl(Token varName);
  void addType(bool kind, const Vector<Token>& tokens);
  void addOpTarget(const Vector<Token>& tokens);
  void setPrec(Token precTok);
  void setGather(const Vector<Token>& gather);
  void setFormat(const Vector<Token>& format);
  void setLatexMacro(const string& latexMacro);
  //
  //	These versions are intended for internally generated renamings
  //	rather than those produced by parsing.
  //
  void addSortMapping(int from, int to);
  void addLabelMapping(int from, int to);
  void addOpMapping(int code);
  void addType(const ConnectedComponent* component);
  void addOpTarget(int code);
  void addOpTargetTerm(Term* term);

  int getNrSortMappings() const;
  int getSortFrom(int index) const;
  int getSortTo(int index) const;
  int getNrLabelMappings() const;
  int getLabelFrom(int index) const;
  int getLabelTo(int index) const;
  int getNrOpMappings() const;
  int getOpFrom(int index) const;
  int getOpTo(int index) const;
  Term* getOpTargetTerm(int index) const;
  int getNrTypes(int index) const;
  int getPrec(int index) const;
  const Vector<int>& getGather(int index) const;
  const Vector<int>& getFormat(int index) const;
  const set<int>& getTypeSorts(int index, int typeNr) const;

  Renaming* makeCanonicalVersion(ImportModule* module) const;
  crope makeCanonicalName() const;

  void addSortAndLabelMappings(const Renaming* original);
  void addOpMappingPartialCopy(const Renaming* original, int index);

  int renameSort(int oldId) const;
  int renameLabel(int oldId) const;
  int renameOp(Symbol* oldSymbol) const;
  int renameOp(int id, const Vector<int>& sortNames) const;
  int renamePolymorph(int oldId) const;
  void printRenaming(ostream& s, const char* sep, const char* sep2) const;

private:
  typedef set<int> IdSet;

  struct OpMapping
  {
    Vector<IdSet> types;	// rename operator with specific arity only
    int name;			// new name
    Term* term;			// for op->term mappings
    //
    //	Can change syntactic attributes only.
    //
    int prec;			// < MixfixModule::MIN_PREC if not set
    Vector<int> gather;		// empty if not set
    Vector<int> format;		// empty if not set
    string latexMacro;		// empty if not set
    int index;
  };

  typedef map<int, int> IdMap;
  typedef multimap<int, OpMapping> OpMap;

  static bool typeMatch(const set<int>& type, const ConnectedComponent* component);
  static bool typeMatch(const Vector<set<int> >& types, Symbol* oldSymbol);
  static bool typeMatch(const Vector<set<int> >& types, const Vector<int>& sortNames);
  static void setType(set<int>& type, const ConnectedComponent* component);
  static crope makeTypeName(const set<int>& type);

  void printRenamingType(ostream& s, int opMappingNr, int typeNr) const;

  IdMap sortMap;
  Vector<IdMap::const_iterator> sortMapIndex;
  IdMap labelMap;
  Vector<IdMap::const_iterator> labelMapIndex;
  OpMap opMap;
  Vector<OpMap::const_iterator> opMapIndex;
  OpMap::iterator lastOpMapping;
};

inline int
Renaming::getNrSortMappings() const
{
  return sortMapIndex.length();
}

inline int
Renaming::getSortFrom(int index) const
{
  return sortMapIndex[index]->first;
}

inline int
Renaming::getSortTo(int index) const
{
  return sortMapIndex[index]->second;
}

inline int
Renaming::getNrLabelMappings() const
{
  return labelMapIndex.length();
}

inline int
Renaming::getLabelFrom(int index) const
{
  return labelMapIndex[index]->first;
}

inline int
Renaming::getLabelTo(int index) const
{
  return labelMapIndex[index]->second;
}

inline int
Renaming::getNrOpMappings() const
{
  return opMapIndex.length();
}

inline int
Renaming::getOpFrom(int index) const
{
  return opMapIndex[index]->first;
}

inline int
Renaming::getOpTo(int index) const
{
  return opMapIndex[index]->second.name;
}

inline Term*
Renaming::getOpTargetTerm(int index) const
{
  return opMapIndex[index]->second.term;
}

inline int
Renaming::getNrTypes(int index) const
{
  return opMapIndex[index]->second.types.length();
}

inline const set<int>&
Renaming::getTypeSorts(int index, int typeNr) const
{
  return opMapIndex[index]->second.types[typeNr];
}

inline int
Renaming::getPrec(int index) const
{
  return opMapIndex[index]->second.prec;
}

inline const Vector<int>&
Renaming::getGather(int index) const
{
  return opMapIndex[index]->second.gather;
}

inline const Vector<int>&
Renaming::getFormat(int index) const
{
  return opMapIndex[index]->second.format;
}

ostream& operator<<(ostream& s, const Renaming* renaming);

#endif
