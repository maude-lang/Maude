/*

    This file is part of the Maude 3 interpreter.

    Copyright 2019 SRI International, Menlo Park, CA 94025, USA.

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
//      Code to handle the instantiation of views with bound parameters.
//

View*
View::instantiateBoundParameters(const Vector<Argument*>& arguments)
{
  DebugEnter("instantiating bound parameters of view " << this);
  //
  //	We are a view with bound parameters. We go back to our baseView which has free parameters
  //	and construct a new instantiation that is just "like us" except our bound parameters
  //	are instantiated to new arguments.
  //
  int nrFreeParameters = baseView->getNrParameters();
  Vector<Argument*> newArgs(nrFreeParameters);
  Vector<Argument*> extraArgs;
  for (int i = 0; i < nrFreeParameters; ++i)
    {
      //
      //	See what the free parameter was originally instantiated by.
      //
      Argument* a = savedArguments[i];
      if (Parameter* p = dynamic_cast<Parameter*>(a))
	{
	  int boundParameterName = p->id();
	  DebugInfo("argument " << i << " of base view is parameter " << Token::name(boundParameterName));
	  int index = findParameterIndex(boundParameterName);
	  Assert(index != NONE,  "didn't find bound parameter");
	  Argument* newArgument = arguments[index];
	  newArgs[i] = newArgument;
	  //
	  //	Need to check for passing a theory-view.
	  //
	  if (View* passedView = dynamic_cast<View*>(newArgument))
	    {
	      if (passedView->getToModule()->isTheory())
		{
		  //
		  //	Instantiating a bound parameter by a theory-view allows it
		  //	to escape. This must be recorded so that it can be rebound
		  //	in a subsequent instantiation.
		  //
		  DebugInfo("bound parameter " << Token::name(boundParameterName) <<
			    " escaped to become free parameter " <<
			    Token::name(baseView->getParameterName(i)) <<
			    " that will need rebinding");
		  extraArgs.append(p);
		}
	    }
	}
      else
	{
	  View* v = safeCast(View*, a);
	  DebugInfo("argument " << i << " of base view is view " << v);
	  //
	  //	The free parameter was originally instantiated by a view
	  //	so this instantiation does not change except we may have to instantiate
	  //	bound parameters in the view.
	  //
	  Assert(!v->getToModule()->isTheory(), "unexpected theory-view " << v);
	  int nrViewParameters = v->getNrParameters();
	  if (nrViewParameters > 0)
	    {
	      DebugInfo("view has " <<  nrViewParameters << " bound parameters");
	      Vector<Argument*> argumentsForView(nrViewParameters);
	      for (int j = 0; j < nrViewParameters; ++j)
		{
		  int parameterName = v->getParameterName(j);
		  int indexInUs = findParameterIndex(parameterName);
		  Assert(indexInUs != NONE, "failed to find parameter " << Token::name(parameterName) <<
			 " in view " << this << " to satisfy view " << v);
		  argumentsForView[j] = arguments[indexInUs];
		}
	      v = v->instantiateBoundParameters(argumentsForView);  // recursive call to construct an instantiation of this view
	      //
	      //	We're not sure whether instantiating a legal view with bound
	      //	parameters can ever fail; and this could change in the future.
	      //	For robustness we assert that it can't so we notice it during
	      //	debugging and then handle in anyway.
	      //
	      Assert(v != 0, "failed to instantiate a view argument for an instantiation of " << this);
	      if (v == 0)
		return 0;
	    }
	  newArgs[i] = v;
	}
    }
  if (View* instance = owner->makeViewInstantiation(baseView, newArgs))
    {
      if (!extraArgs.empty())
	instance = owner->makeViewInstantiation(instance, extraArgs);
      DebugExit("made instantiated view " << instance);
      return instance;
    }
  DebugExit("failed");
  return 0;
}
