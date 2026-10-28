/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2014 SRI International, Menlo Park, CA 94025, USA.

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
//      Serves to bind SMT notions to Maude sorts and operators.
//
#ifndef _SMT_Info_hh_
#define _SMT_Info_hh_
#include <map>

class SMT_Info
{
public:
  enum SMT_Type
    {
      NOT_SMT = -1,
      BOOLEAN = 0,
      INTEGER = 1,
      REAL = 2,
      SET = 3
    };

  SMT_Info();

  void setType(const Sort* sort, SMT_Type type);
  SMT_Type getType(const Sort* sort) const;

  void setConjunctionOperator(Symbol* symbol);
  Symbol* getConjunctionOperator() const;
  void setTrueSymbol(Symbol* symbol);
  Symbol* getTrueSymbol() const;
  void setEqualityOperator(Symbol* symbol);
  Symbol* getEqualityOperator(DagNode* lhs, DagNode* rhs) const;
  void setSetElementSort(const Sort* setSort, const Sort* elementSort);
  const Sort* getSetElementSort(const Sort* setSort) const;
  
 private:
  typedef map<int, SMT_Type> SortIndexToSMT_TypeMap;
  typedef map<int, Symbol*> EqualityOperatorMap;
  typedef map<int, const Sort*> SetElementSortMap;

  SortIndexToSMT_TypeMap typeMap;
  EqualityOperatorMap equalityOperatorMap;
  SetElementSortMap setElementSortMap;
  Symbol* conjunctionOperator;
  Symbol* trueSymbol;
};

inline
SMT_Info::SMT_Info()
{
  conjunctionOperator = 0;
  trueSymbol = 0;
}

inline void
SMT_Info::setType(const Sort* sort, SMT_Type type)
{
  typeMap.insert(SortIndexToSMT_TypeMap::value_type(sort->getIndexWithinModule(), type));
}

inline SMT_Info::SMT_Type
SMT_Info::getType(const Sort* sort) const
{
  SortIndexToSMT_TypeMap::const_iterator i = typeMap.find(sort->getIndexWithinModule());
  return (i == typeMap.end()) ? NOT_SMT : i->second;
}

inline void
SMT_Info::setConjunctionOperator(Symbol* symbol)
{
  conjunctionOperator = symbol;
}

inline Symbol*
SMT_Info::getConjunctionOperator() const
{
  return conjunctionOperator;
}

inline void
SMT_Info::setTrueSymbol(Symbol* symbol)
{
  trueSymbol = symbol;
}

inline Symbol*
SMT_Info::getTrueSymbol() const
{
  return trueSymbol;
}

inline void
SMT_Info::setSetElementSort(const Sort* setSort, const Sort* elementSort)
{
  setElementSortMap.insert(SetElementSortMap::value_type(setSort->getIndexWithinModule(), elementSort));
}

inline const Sort*
SMT_Info::getSetElementSort(const Sort* setSort) const
{
  SetElementSortMap::const_iterator i = setElementSortMap.find(setSort->getIndexWithinModule());
  return (i == setElementSortMap.end()) ? 0 : i->second;
}

#endif
