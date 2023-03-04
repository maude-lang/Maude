/*

    This file is part of the Maude 3 interpreter.

    Copyright 2018-2023 SRI International, Menlo Park, CA 94025, USA.

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

MetaModule*
MetaLevel::downSignature(DagNode* metaModule, Interpreter* owner)
{
  //
  //	This functionality is intended for use by the meta-interpreter rather
  //	than the functional metalevel.
  //	In particular, we:
  //	(1) Don't use the metalevel cache, since will it hold modules created by the functional
  //	metalevel which can have the same meta-syntax but different semantics as modules
  //	pulled down in a meta-interpreter because of different imports with the same name.
  //	(2) Don't do statement imports and module compilation, since this work may
  //	not be needed if a module is being inserted but not being rewritten in.
  //
  Symbol* ms = metaModule->symbol();

  MixfixModule::ModuleType mt;
  if (ms == fmodSymbol)
    mt = MixfixModule::FUNCTIONAL_MODULE;
  else if (ms == fthSymbol)
    mt = MixfixModule::FUNCTIONAL_THEORY;
  else if (ms == modSymbol)
    mt = MixfixModule::SYSTEM_MODULE;
  else if (ms == thSymbol)
    mt = MixfixModule::SYSTEM_THEORY;
  else if (ms == smodSymbol)
    mt = MixfixModule::STRATEGY_MODULE;
  else if (ms == sthSymbol)
    mt = MixfixModule::STRATEGY_THEORY;
  else
    return 0;

  FreeDagNode* f = safeCast(FreeDagNode*, metaModule);
  int id;
  DagNode* metaParameterDeclList;
  if (downHeader(f->getArgument(0), id, metaParameterDeclList))
    {
      MetaModule* m = new MetaModule(id, mt, owner);
      if (downParameterDeclList(metaParameterDeclList, m) &&
	  downImports(f->getArgument(1), m))
	{
	  m->importSorts();
	  if (downSorts(f->getArgument(2), m) &&  downSubsorts(f->getArgument(3), m))
	    {
	      m->closeSortSet();
	      if (!(m->isBad()))
		{
		  m->importOps();
		  if (downOpDecls(f->getArgument(4), m))
		    {
		      m->closeSignature();
		      m->importStrategies();
		      m->importRuleLabels();
		      if (!m->isStrategic() ||
			  downStratDecls(f->getArgument(8), m))
			{
			  m->fixUpImportedOps();
			  if (downFixUps(m) && !(m->isBad()))
			    {
			      m->closeFixUps();
			      if (downMembAxs(f->getArgument(5), m) &&
				  downEquations(f->getArgument(6), m) &&
				  (mt == MixfixModule::FUNCTIONAL_MODULE ||
				  mt == MixfixModule::FUNCTIONAL_THEORY ||
				  downRules(f->getArgument(7), m)) &&
				  (!m->isStrategic() ||
				  downStratDefs(f->getArgument(9), m)))
				{
				  m->registerRuleLabels();
				  m->localStatementsComplete();
				  m->resetImports();
				  return m;
				}
			    }
			}
		    }
		}
	    }
	}
      //
      //	Put the import status flags of any modules that the
      //	metamodule (transitively) depended on in a good state.
      //
      m->resetImports();
      //
      //	Deep self destruction ensures that pointers to the doomed
      //	metamodule are removed from modules it depends on.
      //
      m->deepSelfDestruct();
      //
      //	Pulling down module expressions may have resulted in
      //	the creation of cached modules that no longer have
      //	dependents now that we failed to build the metamodule.
      //	Thus we now need to tidy the module and view caches.
      //	
      owner->cleanCaches();
    }
  return 0;
}

bool
MetaLevel::downImports2(DagNode* metaImports, MetaPreModule* pm)
{
  Symbol* mi = metaImports->symbol();
  if (mi == importListSymbol)
    {
      for (DagArgumentIterator i(metaImports); i.valid(); i.next())
	{
	  if (!downImport2(i.argument(), pm))
	    return false;
	}
    }
  else if (mi != nilImportListSymbol)
    return downImport2(metaImports, pm);
  return true;
}

bool
MetaLevel::downImport2(DagNode* metaImport, MetaPreModule* pm)
{
  Symbol* mi = metaImport->symbol();
  ImportModule::ImportMode mode;
  if (mi == protectingSymbol)
    mode = ImportModule::PROTECTING;
  else if (mi == extendingSymbol)
    mode = ImportModule::EXTENDING;
  else if (mi == includingSymbol)
    mode = ImportModule::INCLUDING;
  else if (mi == generatedBySymbol)
    mode = ImportModule::GENERATED_BY;
  else
    return false;
  
  FreeDagNode* f = safeCast(FreeDagNode*, metaImport);
  if (ModuleExpression* modExpr = downModuleExpression(f->getArgument(0)))
    {
      pm->addImport(LineNumber(FileTable::META_LEVEL_CREATED), mode, modExpr);
      return true;
    }
  return false;
}

bool
MetaLevel::downParameterDeclList2(DagNode* metaParameterDeclList, MetaPreModule* pm)
{
  if (metaParameterDeclList == 0)
    return true;
  //if (m->isTheory())
  //  return false;
  if (metaParameterDeclList->symbol() == parameterDeclListSymbol)
    {
      for (DagArgumentIterator i(metaParameterDeclList); i.valid(); i.next())
	{
	  if (!downParameterDecl2(i.argument(), pm))
	    return false;
	}
      return true;
    }
  return downParameterDecl2(metaParameterDeclList, pm);
}

bool
MetaLevel::downParameterDecl2(DagNode* metaParameterDecl, MetaPreModule* pm)
{
  //
  //	We assume the parameterization is safe - we just want to get
  //	an object level copy in the MetaPreModule.
  //
  if (metaParameterDecl->symbol() == parameterDeclSymbol)
    {
      FreeDagNode* f = safeCast(FreeDagNode*, metaParameterDecl);
      int name;
      if (downQid(f->getArgument(0), name))
	{
	  if (ModuleExpression* theory = downModuleExpression(f->getArgument(1)))
	    {
	      Token t;
	      t.tokenize(name, FileTable::META_LEVEL_CREATED);
	      pm->addParameter(t, theory);
	      return true;
	    }
	}
    }
  return false;  // don't expect to reach this
}
