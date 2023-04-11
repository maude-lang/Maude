/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2023 SRI International, Menlo Park, CA 94025, USA.

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
#include <vector>
#include "syntaxContainer.hh"
#include "rope.hh"
#include "symbolType.hh"
#include "strategyLanguage.hh"
#include "mixfixModule.hh"

// Forward declaration
class RewriteStrategy;

class Renaming : public SyntaxContainer
{
public:
  ~Renaming() override;

  void addSortMapping(Token from, Token to);
  void addLabelMapping(Token from, Token to);
  void addOpMapping(const Vector<Token>& tokens);
  void addStratMapping(Token from);
  void addParameter2(Token name, ModuleExpression* theory) override;
  void addVarDecl(Token varName) override;
  void addType(bool kind, const Vector<Token>& tokens) override;
  void addOpTarget(const Vector<Token>& tokens);
  void addStratTarget(Token to);
  void setPrec(Token precTok);
  void setGather(const Vector<Token>& gather);
  void setFormat(const Vector<Token>& format);
  void setLatexMacro(const string& latexMacro);

  void addClassMapping(const Token& fromClass, const Token& toClass);
  void addAttrMapping(const Token& token);
  void addAttrType(const Vector<Token>& tokens);
  void addAttrTarget(const Token& token);
  void markAsMsg();
  //
  //	These versions are intended for internally generated renamings
  //	rather than those produced by parsing.
  //
  void addSortMapping(int from, int to);
  void addLabelMapping(int from, int to);
  void addOpMapping(int code);
  void addStratMapping(int code);
  void addStratMappingVarIndices(const Vector<int>& indexMap);
  void addType(const ConnectedComponent* component);
  void addOpTarget(int code);
  void addOpTargetTerm(Term* fromTerm, Term* targetTerm, bool gcTargetTerm);
  void addStratTarget(int code);
  void addStratTargetExpr(CallStrategy* fromCall, StrategyExpression* term);

  int getNrSortMappings() const;
  int getSortFrom(int index) const;
  int getSortTo(int index) const;
  int getNrLabelMappings() const;
  int getLabelFrom(int index) const;
  int getLabelTo(int index) const;
  int getNrOpMappings() const;
  int getOpFrom(int index) const;
  int getOpTo(int index) const;
  Term* getFromTerm(int index) const;
  Term* getOpTargetTerm(int index) const;
  int getNrTypes(int index) const;
  int getPrec(int index) const;
  const Vector<int>& getGather(int index) const;
  const Vector<int>& getFormat(int index) const;
  const set<int>& getTypeSorts(int index, int typeNr) const;

  int getNrStratMappings() const;
  int getStratFrom(int index) const;
  int getStratTo(int index) const;
  int getNrStratTypes(int index) const;
  CallStrategy* getStratFromCall(int index) const;
  StrategyExpression* getStratTargetExpr(int index) const;
  const Vector<int>& getStratVarIndices(int index) const;
  const set<int>& getStratTypeSorts(int index, int typeNr) const;
  void discardStratMappings();

  int getNrClassMappings() const;
  Token getFromClass(int index) const;
  Token getToClass(int index) const;
  int getNrAttrMappings() const;
  Token getFromAttr(int index) const;
  const set<int>& getAttrTypeSorts(int index) const;
  Token getToAttr(int index) const;

  void convertClassMappings(ImportModule* module, Renaming* canonical) const;
  void convertAttrMappings(const ImportModule* module, Renaming* canonical) const;
  void pruneSortMappings(ImportModule* module, Renaming* canonical) const;
  void pruneLabelMappings(ImportModule* module, Renaming* canonical) const;
  void canonicalizeOpMappings(ImportModule* module, Renaming* canonical) const;
  void canonicalizeStrategyMappings(ImportModule* module, Renaming* canonical) const;
  Renaming* makeCanonicalVersion(ImportModule* module) const;
  Rope makeCanonicalName() const;

  void addSortConstantAndLabelMappings(const Renaming* original);
  void addOpMappingPartialCopy(const Renaming* original, int index, int newFromName, int newToName);
  void addStratMappingPartialCopy(const Renaming* original, int index);

  int renameSort(int oldId) const;
  int renameLabel(int oldId) const;
  int renameOp(Symbol* oldSymbol) const;
  int renameOp(int id, const Vector<int>& sortNames) const;
  int renamePolymorph(int oldId) const;
  int renameStrat(RewriteStrategy* oldStrategy) const;
  int renameStrat(int label, const Vector<int>& sortNames) const;
  void printRenaming(ostream& s, const char* sep, const char* sep2, bool showProcessed = false) const;

protected:
  //
  //	This returns true even if its is the identity mapping.
  //	This is needed for processing OO mappings in views.
  //
  bool hasSortMapping(int oldId) const;
  //
  //	Needed for procssing OO mappings in views.
  //
  bool isMsgMapping(int index) const;
  static bool typeMatch(const set<int>& type, const ConnectedComponent* component);
  //
  //	Ugly hack to add and purge generated sort/op mappings for OO mappings in views.
  //
  void recordUserMappings();
  void purgeGeneratedMappings();

private:
  //
  //	We use this instead of Type because we don't care about the kind flag and
  //	we want the set of identifiers sorted in a canonical order.
  //
  typedef set<int> IdSet;
  //
  //	Attribute and message mappings are stored as op mappings in surface renaming and
  //	transformed away when a canonical renaming is computed with respect to some module.
  //
  enum class MappingType : char
    {
     OP,	// normal op to op or op to term mapping
     MSG	// msg mapping
    };

  struct OpMapping
  {
    Vector<IdSet> types;	// rename operator with specific arity only
    int name;			// new name
    MappingType mappingType = MappingType::OP;
    //
    //	Renamings only ever borrow terms from a view and hence they never
    //	delete them. The object using the renaming becomes stale as soon
    //	as the view the term came from becomes stale, so dangling pointers
    //	are never dereferenced.
    //
    bool gcToTerm = false;	// whether we are responsible for garbage collected toTerm
    Term* fromTerm = 0;		// not used for renaming but useful for view instantiation and debugging
    Term* toTerm = 0;		// for op->term mappings
    //
    //	Can change syntactic attributes only.
    //
    Vector<int> gather;		// empty if not set
    Vector<int> format;		// empty if not set
    string latexMacro;		// empty if not set
    int prec = MixfixModule::MIN_PREC - 1;	// < MixfixModule::MIN_PREC if not set
    int index;
  };

  struct StratMapping
  {
    Vector<IdSet> types;	// rename operator with specific arity only
    int name;			// new name
    CallStrategy* fromCall;	// not used for renaming but useful for view instantiation and debugging
    StrategyExpression* expr;	// for strat->expr mappings
    Vector<int> varsMap;	// lhs indices of the rhs variables
    int index;
  };

  struct ClassMapping
  {
    Token fromClass;
    Token toClass;
  };

  struct AttrMapping
  {
    Token fromAttr;
    IdSet type;
    Token toAttr;
  };

  typedef map<int, int> IdMap;
  typedef multimap<int, OpMapping> OpMap;
  typedef multimap<int, StratMapping> StratMap;
  typedef Vector<ClassMapping> ClassMap;
  typedef Vector<AttrMapping> AttrMap;  // Vector now supports move semantics and back()

  static bool equal(const Vector<int>& left, const Vector<int>& right);
  static bool isIdentityOpMapping(const ImportModule* module, const OpMapping& om, const Symbol* symbol);
  static bool isIdentityOpMapping(const ImportModule* module, const OpMapping& om, int index);
  static bool typeMatch(const Vector<set<int> >& types, Symbol* oldSymbol);
  static bool typeMatch(const Vector<set<int> >& types, RewriteStrategy* oldStrat);
  static bool typeMatch(const Vector<set<int> >& types, const Vector<int>& sortNames);
  static void setType(set<int>& type, const ConnectedComponent* component);
  static Rope makeTypeName(const set<int>& type);

  void printRenamingType(ostream& s, const set<int>& sorts) const;

  IdMap sortMap;
  Vector<IdMap::const_iterator> sortMapIndex;
  IdMap labelMap;
  Vector<IdMap::const_iterator> labelMapIndex;
  OpMap opMap;
  Vector<OpMap::const_iterator> opMapIndex;
  OpMap::iterator lastOpMapping;
  StratMap stratMap;
  Vector<StratMap::const_iterator> stratMapIndex;
  StratMap::iterator lastStratMapping;
  ClassMap classMap;
  AttrMap attrMap;
  //
  //	This is an ugly hack - we want to add sort and op mappings when we processs
  //	class and attr mappings in derived View but we want to restore the user's
  //	original renaming if the View gets reevaluated. The alternative is to keep
  //	the generated sort and op mappings in the View, but then they need to be
  //	made visible to module instantiation code that needs to deals with desugared
  //	mappings and the metalevel which also deals with desugared mapppings, which
  //	is probably equally ugly.
  //
  Index nrUserSortMappings = NONE;
  Index nrUserOpMappings = NONE;
  bool lastSeenWasStrategy;	// Used to discriminate where to addType
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
Renaming::getNrClassMappings() const
{
  return classMap.size();
}

inline Token
Renaming::getFromClass(int index) const
{
  return classMap[index].fromClass;
}

inline Token
Renaming::getToClass(int index) const
{
  return classMap[index].toClass;
}

inline int
Renaming::getNrAttrMappings() const
{
  return attrMap.size();
}

inline Token
Renaming::getFromAttr(int index) const
{
  return attrMap[index].fromAttr;
}

inline const set<int>&
Renaming::getAttrTypeSorts(int index) const
{
  return attrMap[index].type;
}

inline Token
Renaming::getToAttr(int index) const
{
  return attrMap[index].toAttr;
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

inline bool
Renaming::isMsgMapping(int index) const
{
  return opMapIndex[index]->second.mappingType == MappingType::MSG;
}

inline Term*
Renaming::getOpTargetTerm(int index) const
{
  return opMapIndex[index]->second.toTerm;
}

inline Term*
Renaming::getFromTerm(int index) const
{
  return opMapIndex[index]->second.fromTerm;
}

inline int
Renaming::getNrTypes(int index) const
{
  return opMapIndex[index]->second.types.length();
}

inline int
Renaming::getNrStratMappings() const
{
  return stratMapIndex.length();
}

inline int
Renaming::getStratFrom(int index) const
{
  return stratMapIndex[index]->first;
}

inline const Vector<int>&
Renaming::getStratVarIndices(int index) const
{
  return stratMapIndex[index]->second.varsMap;
}

inline const set<int>&
Renaming::getStratTypeSorts(int index, int typeNr) const
{
  return stratMapIndex[index]->second.types[typeNr];
}

inline int
Renaming::getStratTo(int index) const
{
  return stratMapIndex[index]->second.name;
}

inline int
Renaming::getNrStratTypes(int index) const
{
  return stratMapIndex[index]->second.types.length();
}

inline CallStrategy*
Renaming::getStratFromCall(int index) const
{
  return stratMapIndex[index]->second.fromCall;
}

inline StrategyExpression*
Renaming::getStratTargetExpr(int index) const
{
  return stratMapIndex[index]->second.expr;
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

inline void
Renaming::discardStratMappings()
{
  stratMapIndex.clear();
  stratMap.clear();
}

inline void
Renaming::markAsMsg()
{
  lastOpMapping->second.mappingType = MappingType::MSG;
}

inline bool
Renaming::hasSortMapping(int oldId) const
{
  return sortMap.find(oldId) != sortMap.end();
}

inline void
Renaming::recordUserMappings()
{
  nrUserSortMappings = sortMapIndex.size();
  nrUserOpMappings = opMapIndex.size();
}

ostream& operator<<(ostream& s, const Renaming* renaming);

#endif
