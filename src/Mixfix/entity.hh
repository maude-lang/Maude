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
//      Base class for entities that must inform their users just before they
//	are deleted or otherwise invalidated.
//
#ifndef _entity_hh_
#define _entity_hh_
#include <set>

class Entity
{
public:
  class User
  {
  public:
    virtual void regretToInform(Entity* doomedEntity) = 0;
  };

  void addUser(User* user);
  void removeUser(User* user);
  int getNrUsers() const;
  void informUsers();

#ifndef NO_ASSERT
  virtual void dummyToAllowDynamicCasts() {}
#endif

private:
  struct UserLt
  {
    bool operator()(const User* user1, const User* user2)
    {
      //
      //	Direct < comparison of unrelated pointers is undefined. But since the casting
      //	to a large enough integer is invertable, it must be injective and so we can
      //	get a total ordering by comparing the integers.
      //
      return reinterpret_cast<size_t>(user1) < reinterpret_cast<size_t>(user2);
    }
  };

  typedef set<User*, UserLt> UserSet;

  UserSet users;
};

inline int
Entity::getNrUsers() const
{
  return users.size();  // inefficient
}

#ifndef NO_ASSERT
ostream& operator<<(ostream& s, const Entity* e);
ostream& operator<<(ostream& s, const Entity::User* u);
#endif

#endif
