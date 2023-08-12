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
//      Class for modules that can be printed out.
//
#ifndef _visibleModule_hh_
#define _visibleModule_hh_
#include "importModule.hh"
#include "view.hh"

class VisibleModule : public ImportModule
{
  NO_COPYING(VisibleModule);

public:
  VisibleModule(int name, ModuleType moduleType, Interpreter* owner);

  Interpreter* getOwner() const;

  void showSummary(ostream& s);
  void showKinds(ostream& s) const;
  void showSortsAndSubsorts(ostream& s) const;
  void showModule(ostream& s, bool all) const;
  void showPolymorphs(ostream& s, bool indent, bool all) const;
  void showOps(ostream& s, bool indent, bool all) const;
  void showVars(ostream& s, bool indent) const;
  void showMbs(ostream& s, bool indent, bool all) const;
  void showEqs(ostream& s, bool indent, bool all) const;
  void showRls(ostream& s, bool indent, bool all) const;
  void showStrats(ostream& s, bool indent, bool all) const;
  void showSds(ostream& s, bool indent, bool all) const;

  void latexShowSummary(ostream& s);
  void latexShowKinds(ostream& s) const;
  void latexShowSortsAndSubsorts(ostream& s) const;
  void latexShowModule(ostream& s, bool all);
  void latexShowPolymorphs(ostream& s, const char* indent, bool all);
  void latexShowOps(ostream& s, const char* indent, bool all);
  void latexShowVars(ostream& s, const char* indent) const;
  void latexShowMbs(ostream& s, const char* indent, bool all) const;
  void latexShowEqs(ostream& s, const char* indent, bool all) const;
  void latexShowRls(ostream& s, const char* indent, bool all) const;
  void latexShowStrats(ostream& s, const char* indent, bool all);
  void latexShowSds(ostream& s, const char* indent, bool all);

  void latexPrintRule(ostream& s, const char* indent, const Rule* rl) const;
  bool latexPrintStrategy(ostream& s, StrategyExpression* strategy, int requiredPrec = UNBOUNDED) const;
  
  static void latexPrintCondition(ostream& s, const Vector<ConditionFragment*>& condition);
  static void latexPrintModuleName(ostream& s, Module* module);

private:
  void showImports(ostream& s) const;
  void showSorts1(ostream& s, bool indent, bool all) const;
  void showSubsorts(ostream& s, bool indent, bool all) const;
  void showPolymorphDecl(ostream& s, bool indent, int index) const;
  void showPolymorphAttributes(ostream& s, int index) const;
  void showDecls(ostream& s, bool indent, int index, bool all) const;
  void showAttributes(ostream& s, Symbol* symbol, int opDeclIndex) const;

  void latexShowSorts(ostream& s, bool all) const;
  void latexShowSubsorts(ostream& s, bool all) const;
  void latexShowDecls(ostream& s, const char* indent, Index index, bool all);
  void latexShowPolymorphDecl(ostream& s, const char* indent, Index index);


  static void latexPrintAttributes(ostream& s,
			    const PreEquation* pe,
			    int metadata,
			    const PrintAttribute* printAttribute,
			    bool owise = false,
			    bool variant = false,
			    bool narrowing = false);

  void latexPrintMembershipAxiom(ostream& s, const char* indent, const SortConstraint* mb) const;
  void latexPrintEquation(ostream& s, const char* indent, const Equation* eq) const;
  void latexPrintStrategyDecl(ostream& s, const char* indent, const RewriteStrategy* rs);
  void latexPrintStrategyDefinition(ostream& s, const char* indent, const StrategyDefinition* e);

  void latexShowImports(ostream& s) const;
  void latexShowAttributes(ostream& s, Symbol* symbol, Index opDeclIndex);
  void latexShowPolymorphAttributes(ostream& s, int index);

  static void latexPrintConditionFragment(ostream& s, const ConditionFragment* c);
  static void latexPrintCondition(ostream& s, const PreEquation* pe);
  static void latexPrintViewName(ostream& s, View* view);
  
  Interpreter* const owner;
};

inline Interpreter*
VisibleModule::getOwner() const
{
  return owner;
}

inline void
VisibleModule::latexPrintModuleName(ostream& s, Module* module)
{
  s << "\\maudeModule{" << Token::latexName(module->id()) << "}";
}

inline void
VisibleModule::latexPrintViewName(ostream& s, View* view)
{
  s << "\\maudeView{" << Token::latexName(view->id()) << "}";
}

#endif
