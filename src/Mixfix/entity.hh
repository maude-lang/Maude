/*

    This file is part of the Maude 3 interpreter.

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
//      Base class for entities that must inform their users just before they
//	are deleted or otherwise invalidated.
//
#ifndef _entity_hh_
#define _entity_hh_
#include <set>

class Entity
{
public:
#ifndef NO_ASSERT
  //
  //	We give ourself a virtual destructor so that debugging code can
  //	use dynamic_cast<>s to figure out what derived object we are.
  //
  virtual ~Entity() {}
#endif

  class User
  {
  public:
    virtual void regretToInform(Entity* doomedEntity) = 0;
  };

  void addUser(User* user);
  void removeUser(User* user);
  int getNrUsers() const;
  void informUsers();

private:
  typedef set<User*> UserSet;

  UserSet users;
};

inline int
Entity::getNrUsers() const
{
  return users.size();
}

#ifndef NO_ASSERT
ostream& operator<<(ostream& s, const Entity* e);
ostream& operator<<(ostream& s, const Entity::User* u);
#endif

#endif
