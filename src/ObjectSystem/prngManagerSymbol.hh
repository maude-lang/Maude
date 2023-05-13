/*

    This file is part of the Maude 3 interpreter.

    Copyright 2023 SRI International, Menlo Park, CA 94025, USA.

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
//      Class to handle pseudorandom number generator manager symbols.
//
#ifndef _prngManagerSymbol_hh_
#define _prngManagerSymbol_hh_
#include <random>
#include "externalObjectManagerSymbol.hh"

class PrngManagerSymbol : public ExternalObjectManagerSymbol
{
  NO_COPYING(PrngManagerSymbol);

public:
  PrngManagerSymbol(int id);

  bool attachData(const Vector<Sort*>& opDeclaration,
		  const char* purpose,
		  const Vector<const char*>& data);
  bool attachSymbol(const char* purpose, Symbol* symbol);

  void copyAttachments(Symbol* original, SymbolMap* map);

  void getDataAttachments(const Vector<Sort*>& opDeclaration,
			  Vector<const char*>& purposes,
			  Vector<Vector<const char*> >& data);
  void getSymbolAttachments(Vector<const char*>& purposes, Vector<Symbol*>& symbols);
  //
  //	Overridden methods from ExternalObjectManagerSymbol.
  //
  bool handleManagerMessage(DagNode* message, ObjectSystemRewritingContext& context);
  bool handleMessage(DagNode* message, ObjectSystemRewritingContext& context);
  void cleanUp(DagNode* objectId);

private:
  enum class PrngAlgorithm
    {
     FREE,
     MT32,
     MT64
    };

  struct Prng
  {
    PrngAlgorithm algorithm;
    
    union
    {
      mt19937* mt32;
      mt19937_64* mt64;
    };
  };

  typedef Vector<Prng> PrngMap;

  int getPrng(DagNode* prngArg) const;
  bool getSeed32(DagNode* seedArg, uint32_t& value) const;
  bool getSeed64(DagNode* seedArg, uint64_t& value) const;
  int findUnusedId();

  void createPrng(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void setSeed(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void getNext(FreeDagNode* message, ObjectSystemRewritingContext& context);
  void deletePrng(FreeDagNode* message, ObjectSystemRewritingContext& context);

  void errorReply(const char* errorMessage,
		  FreeDagNode* originalMessage,
		  ObjectSystemRewritingContext& context);

#define MACRO(SymbolName, SymbolClass, NrArgs) \
  SymbolClass* SymbolName;
#include "prngSignature.cc"
#undef MACRO

  PrngMap prngs;
};

#endif
