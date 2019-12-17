/*

    This file is part of the Maude 3 interpreter.

    Copyright 1997-2012 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for variant narrowing descent functions.
//

bool
MetaLevelOpSymbol::metaGetVariant2(FreeDagNode* subject, RewritingContext& context, bool irredundant)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant call: " << Tty(Tty::GREEN) << (DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaGenerateVariant() and metaGenerateIrredundantVariant().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      Int64 solutionNr;
      int variableFamilyName;
      int variableFamily;
      if (metaLevel->downQid(subject->getArgument(3), variableFamilyName) &&
	  (variableFamily = FreshVariableSource::getFamily(variableFamilyName)) != NONE &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
	{
	  VariantSearch* vs;
	  Int64 lastSolutionNr;
	  if (m->getCachedStateObject(subject, context, solutionNr, vs, lastSolutionNr))
	    m->protect();  // Use cached state
	  else
	    {
	      if (Term* start = metaLevel->downTerm(subject->getArgument(1),  m))
		{
		  Vector<Term*> blockerTerms;
		  if (metaLevel->downTermList(subject->getArgument(2), m, blockerTerms))
		    {
		      m->protect();
		      RewritingContext* startContext = term2RewritingContext(start, context);

		      Vector<DagNode*> blockerDags; 
		      FOR_EACH_CONST(i, Vector<Term*>, blockerTerms)
			{
			  Term* t = *i;
			  t = t->normalize(true);  // we don't really need to normalize but we do need to set hash values
			  blockerDags.append(t->term2Dag());
			  t->deepSelfDestruct();
			}
		      vs = new VariantSearch(startContext,
					     blockerDags,
					     new FreshVariableSource(m),
					     false,	// not unification mode
					     irredundant,
					     true,	// delete fresh variable generator on destruction
					     variableFamily,
					     true);	// check variable names
		      lastSolutionNr = -1;
		    }
		  else
		    {
		      delete start;
		      return false;
		    }
		}
	      else
		return false;
	    }

	  DagNode* result;
	  const Vector<DagNode*>* variant = 0;  // just to avoid compiler warning
	  int nrFreeVariables;
	  int parentIndex = -1;  // dummy initialization to prevent compiler warning
	  bool moreInLayer = false;  // dummy initialization to prevent compiler warning
	  int resultVariableFamily;
	  if (lastSolutionNr == solutionNr)
	    {
	      //
	      //	So the user can ask for the same variant over and over again without
	      //	a horrible loss of performance.
	      //
	      variant = vs->getLastReturnedVariant(nrFreeVariables, resultVariableFamily, parentIndex, moreInLayer);
	    }
	  else
	    {
	      while (lastSolutionNr < solutionNr)
		{
		  variant = vs->getNextVariant(nrFreeVariables, resultVariableFamily, parentIndex, moreInLayer);
		  if (variant == 0)
		    {
		      bool incomplete = vs->isIncomplete();
		      delete vs;
		      result = metaLevel->upNoVariant(incomplete);
		      goto fail;
		    }
		  context.transferCountFrom(*(vs->getContext()));
		  ++lastSolutionNr;
		}
	    }
	  {
	    m->insert(subject, vs, solutionNr);
	    int variableNameId = FreshVariableSource::getBaseName(resultVariableFamily);
	    mpz_class parentIndexBig(parentIndex);
	    result = metaLevel->upVariant(*variant, vs->getVariableInfo(), variableNameId, parentIndexBig, moreInLayer, m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::metaGetVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateVariant : Module Term TermList Qid Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, false);
}

bool
MetaLevelOpSymbol::metaGetIrredundantVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateIrredundantVariant : Module Term TermList Qid Nat ~> Variant? .
  //
  return metaGetVariant2(subject, context, true);
}
