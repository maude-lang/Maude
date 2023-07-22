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

int
PrngManagerSymbol::getPrng(DagNode* prngArg) const
{
  //
  //	We assume we have a good prng name because the message was delivered.
  //
  int id;
  DagNode* idArg = safeCastNonNull<FreeDagNode*>(prngArg)->getArgument(0);
  DebugSave(ok, succSymbol->getSignedInt(idArg, id));
  Assert(ok, "bad prng number " << prngArg);
  return id;
}

bool
PrngManagerSymbol::getSeed32(DagNode* seedArg, uint_fast32_t& value) const
{
  //
  //	We allow arbitrary large seeds but only return the low order 32-bits.
  //
  if (succSymbol->isNat(seedArg))
    {
      value = succSymbol->getNat(seedArg).get_ui();
      return true;
    }
  return false;
}

bool
PrngManagerSymbol::getSeed64(DagNode* seedArg, uint_fast64_t& value) const
{
  //
  //	We allow arbitrary large seeds but only return the low order 64-bits.
  //
  if (succSymbol->isNat(seedArg))
    {
      const mpz_class& n = succSymbol->getNat(seedArg);
      mpz_class u = n >> BITS_PER_UINT;
      value = u.get_ui();
      value <<= BITS_PER_UINT;
      value |= n.get_ui();
      return true;
    }
  return false;
}

int
PrngManagerSymbol::findUnusedId()
{
  Index nrIds = prngs.size();
  for (Index i = 0; i < nrIds; ++i)
    {
      if (prngs[i].algorithm == PrngAlgorithm::FREE)
	return i;
    }
  prngs.resize(nrIds + 1);
  return nrIds;
}

void
PrngManagerSymbol::createPrng(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DebugInfo("message = " << message);
  int id;
  DagNode* algArg = message->getArgument(2);
  
  if (algArg->symbol() == stringSymbol)
    {
      const Rope& algName = safeCastNonNull<StringDagNode*>(algArg)->getValue();
      if (algName == "MT32")
	{
	  id = findUnusedId();
	  prngs[id].mt32 = new std::mt19937();
	  prngs[id].algorithm = PrngAlgorithm::MT32;
	}
      else if (algName == "MT64")
	{
	  id = findUnusedId();
	  prngs[id].mt64 = new std::mt19937_64();
	  prngs[id].algorithm = PrngAlgorithm::MT64;
	}
      else
	{
	  errorReply("Unknown algorithm.", message, context);
	  return;
	}
    }
  else
    {
      errorReply("Bad algorithm argument.", message, context);
      return;
    }

  Vector<DagNode*> reply(3);
  reply.resize(1);
  reply[0] = succSymbol->makeNatDag(id);
  DagNode* prngName = prngOidSymbol->makeDagNode(reply);
  context.addExternalObject(prngName, this);
  DebugInfo("made " << prngName);

  reply.resize(3);
  reply[2] = prngName;
  reply[1] = message->getArgument(0);
  DagNode* target = message->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, createdPrngMsg->makeDagNode(reply));
}

void
PrngManagerSymbol::setSeed(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int id = getPrng(message->getArgument(0));
  DagNode* seedArg = message->getArgument(2);
  switch (prngs[id].algorithm)
    {
    case PrngAlgorithm::MT32:
      {
	uint_fast32_t value;
	if (getSeed32(seedArg, value))
	  prngs[id].mt32->seed(value);
	else
	  {
	    errorReply("Bad seed.", message, context);
	    return;
	  }
	break;
      }
    case PrngAlgorithm::MT64:
      {
	uint_fast64_t value;
	if (getSeed64(seedArg, value))
	  prngs[id].mt64->seed(value);
	else
	  {
	    errorReply("Bad seed.", message, context);
	    return;
	  }
	break;
      }
    case PrngAlgorithm::FREE:
      {
	CantHappen("algorithm is FREE");
	break;
      }
    }
  trivialReply(seedSetMsg, message, context);
}

void
PrngManagerSymbol::getNext(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  int id = getPrng(message->getArgument(0));
  uint_fast64_t value = 0;
  switch (prngs[id].algorithm)
    {
    case PrngAlgorithm::MT32:
      {
	value = (*(prngs[id].mt32))();
	break;
      }
    case PrngAlgorithm::MT64:
      {
	value = (*(prngs[id].mt64))();
	break;
      }
    case PrngAlgorithm::FREE:
      {
	CantHappen("algorithm is FREE");
	break;
      }
    }
  Vector<DagNode*> reply(3);
  reply[1] = message->getArgument(0);
  reply[2] = succSymbol->makeNatDag64(value);
  DagNode* target = message->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, gotNextMsg->makeDagNode(reply));
}

void
PrngManagerSymbol::deletePrng(FreeDagNode* message, ObjectSystemRewritingContext& context)
{
  DagNode* prngArg = message->getArgument(0);
  cleanUp(prngArg);
  context.deleteExternalObject(prngArg);
  trivialReply(deletedPrngMsg, message, context);
}

void
PrngManagerSymbol::errorReply(const char* errorMessage,
			      FreeDagNode* originalMessage,
			      ObjectSystemRewritingContext& context)
{
  DebugInfo("errorMessage = " << errorMessage << " originalMessage = " << originalMessage);
  Vector<DagNode*> reply(3);
  reply[1] = originalMessage->getArgument(0);
  reply[2] = new StringDagNode(stringSymbol, errorMessage);
  DagNode* target = originalMessage->getArgument(1);
  reply[0] = target;
  context.bufferMessage(target, prngErrorMsg->makeDagNode(reply));
}
