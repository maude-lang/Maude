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
//      Class for modules with profiling information.
//
#ifndef _profileModule_hh_
#define _profileModule_hh_
#include "module.hh"

class ProfileModule : public Module
{
  NO_COPYING(ProfileModule);

public:
  ProfileModule(int name);

  void clearProfile();
  void profileMbRewrite(DagNode* redex, const SortConstraint* mb);
  void profileEqRewrite(DagNode* redex, const Equation* eq, int type);
  void profileRlRewrite(DagNode* redex, const Rule* rl);
  void profileMbConditionStart(const SortConstraint* mb);
  void profileEqConditionStart(const Equation* eq);
  void profileRlConditionStart(const Rule* rl);
  void profileFragment(const PreEquation* preEquation, int fragmentIndex, bool success);
  void showProfile(ostream& s) const;

private:
  struct SymbolProfile
  {
    SymbolProfile();
      
    Int64 nrBuiltinMbRewrites;
    Int64 nrBuiltinEqRewrites;
    Int64 nrBuiltinRlRewrites;
    Int64 nrMemoRewrites;
  };

  struct FragmentProfile
  {
    FragmentProfile();

    Int64 nrSuccesses;
    Int64 nrFailures;
  };

  struct StatementProfile
  {
    StatementProfile();
    void updateFragmentInfo(int index, bool success);

    Int64 nrRewrites;
    //
    //	Only maintained for conditional statements.
    //
    Int64 nrConditionStarts;
    Vector<FragmentProfile> fragmentInfo;
  };

  static void showSymbol(ostream& s, Symbol* op);
  static void showFragmentProfile(ostream& s,
				  const Vector<FragmentProfile>& fragmentInfo,
				  Int64 nrFirsts);

  Vector<SymbolProfile> symbolInfo;
  Vector<StatementProfile> mbInfo;
  Vector<StatementProfile> eqInfo;
  Vector<StatementProfile> rlInfo;
};

#endif
