/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019-2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Code for legacy variant narrowing descent functions.
//

bool
MetaLevelOpSymbol::legacyMetaGetVariant2(FreeDagNode* subject,
					 RewritingContext& context,
					 bool irredundant)
{
  DebugAdvisory(Tty(Tty::CYAN) << "meta variant call: " << Tty(Tty::GREEN) <<
		(DagNode*) subject << Tty(Tty::RESET));
  //
  //	We handle both metaGenerateVariant() and metaGenerateIrredundantVariant().
  //
  if (MetaModule* m = metaLevel->downModule(subject->getArgument(0)))
    {
      DagNode* metaVarIndex = subject->getArgument(3);
      Int64 solutionNr;
      if (metaLevel->isNat(metaVarIndex) &&
	  metaLevel->downSaturate64(subject->getArgument(4), solutionNr) && solutionNr >= 0)
	{
	  const mpz_class& varIndex = metaLevel->getNat(metaVarIndex);
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
		      for (Term* t : blockerTerms)
			{
			  //
			  //	We don't really need to normalize but we do need to set hash values.
			  //
			  t = t->normalize(true);
			  blockerDags.append(t->term2Dag());
			  t->deepSelfDestruct();
			}
		      vs = new VariantSearch(startContext,
					     blockerDags,
					     new FreshVariableSource(m, varIndex),
					     VariantSearch::DELETE_FRESH_VARIABLE_GENERATOR |
					     VariantSearch::CHECK_VARIABLE_NAMES |
					     (irredundant ? VariantSearch::IRREDUNDANT_MODE : 0));
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
	  while (lastSolutionNr < solutionNr)
	    {
	      
	      if (!(vs->findNextVariant()))
		{
		  bool incomplete = vs->isIncomplete();
		  delete vs;
		  result = metaLevel->upNoVariant(incomplete);
		  goto fail;
		}
	      context.transferCountFrom(*(vs->getContext()));
	      ++lastSolutionNr;
	    }
	  m->insert(subject, vs, solutionNr);
	  {
	    int nrFreeVariables;
	    int resultVariableFamily;
	    int parentIndex;
	    bool moreInLayer;
	    const Vector<DagNode*>& variant = vs->getCurrentVariant(nrFreeVariables,
								    resultVariableFamily,
								    &parentIndex,
								    &moreInLayer);

	    mpz_class lastVarIndex = varIndex + nrFreeVariables;
	    mpz_class parentIndexBig(parentIndex);
	    result = metaLevel->upVariant(variant,
					  vs->getVariableInfo(),
					  lastVarIndex,
					  parentIndexBig,
					  moreInLayer,
					  m);
	  }
	fail:
	  (void) m->unprotect();
	  return context.builtInReplace(subject, result);
	}
    }
  return false;
}

bool
MetaLevelOpSymbol::legacyMetaGetVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return legacyMetaGetVariant2(subject, context, false);
}

bool
MetaLevelOpSymbol::legacyMetaGetIrredundantVariant(FreeDagNode* subject, RewritingContext& context)
{
  //
  //	op metaGenerateIrredundantVariant : Module Term TermList Nat Nat ~> Variant? .
  //
  return legacyMetaGetVariant2(subject, context, true);
}
