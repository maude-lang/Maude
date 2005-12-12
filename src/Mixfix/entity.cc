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
//      Implementation for class Entity.
//

//      utility stuff
#include "macros.hh"
#include "vector.hh"

//      forward declarations
#include "interface.hh"
#include "core.hh"
#include "mixfix.hh"

//	front end class definitions
#include "importModule.hh"
#include "preModule.hh"
#include "view.hh"
#include "entity.hh"

#ifndef NO_ASSERT
ostream&
operator<<(ostream& s, const Entity* e)
{
  if (const ImportModule* m = dynamic_cast<const ImportModule*>(e))
    s << "module " << m;
  else if (const View* v = dynamic_cast<const View*>(e))
    s << "view " << v;
  else
    s << "unknown entity";
}

ostream&
operator<<(ostream& s, const Entity::User* u)
{
  if (const PreModule* p = dynamic_cast<const PreModule*>(u))
    s << "premodule " << p;
  else if (const ImportModule* m = dynamic_cast<const ImportModule*>(u))
    s << "module " << m;
  else if (const View* v = dynamic_cast<const View*>(u))
    s << "view " << v;
  else
    s << "unknown user";
}
#endif
 
void
Entity::addUser(User* user)
{
  if (users.insert(user).second)
    {
      //DebugAdvisory("added " <<  user << " to user set for " << this);
    }
  else
    {
      //DebugAdvisory(user << " is already in user set for " << this);
    }
}

void
Entity::removeUser(User* user)
{
  if (users.erase(user) == 1)
    {
      //DebugAdvisory("removed " << user << " from user set for " << this);
    }
  else
    {
      //DebugAdvisory("missing " << user << " in user set for " << this);
    }
}

void
Entity::informUsers()
{
  //DebugAdvisory(this << " informs users");
  //
  //	We need to be careful since informing a user will often cause the user
  //	and/or other users to be removed, invalidating iterators.
  //
  const UserSet::const_iterator e = users.end();
  User* last = 0;
  for (;;)
    {
      UserSet::iterator i = users.begin();
      if (i == e)
	break;
      User* user = *i;
      if (user == last)
	users.erase(i);
      else
	{
	  user->regretToInform(this);  // invalidates i
	  last = user;
	}
    }
}
