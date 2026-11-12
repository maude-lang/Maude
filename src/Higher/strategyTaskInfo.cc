/*

    This file is part of the Maude 3 strategy-aware model checker.

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
//	Definition of the model checking information attached to tasks.
//


/**
 * Auxiliary information attached to each task.
 */
struct TaskInfo
{
  using Key = pair<int, StrategyStackManager::StackId>;
  using SeenMap = map<Key, StrategyTransitionGraph::Substate*>;
  using CallMap = map<VariableBindingsManager::ContextId, TaskInfo*>;

  // Variable context indentifier
  TaskInfo* parent;
  StrategicTask* rootTask;
  VariableBindingsManager::ContextId varContext;

  // Table to prevent cycling and repetitive work
  SeenMap seenMap;

  // Table to share TaskInfo between final recursive calls
  CallMap callMap;

  // Additional information to manage its lifetime
  size_t usersCount;

  void link(Key localKey, StrategyTransitionGraph::Substate* substate);
  void unlink(SeenMap::const_iterator entry);
  void unlink(Key localKey);

  // Frees the TaskInfo if the reference count arrive to zero.
  // This function must only be called if the TaskInfo was created by new.
  bool free();

  TaskInfo(StrategicTask* task);
  TaskInfo(TaskInfo* parent, VariableBindingsManager::ContextId context);
};

inline
TaskInfo::TaskInfo(StrategicTask* task)
 : parent(this),
   rootTask(task),
   varContext(VariableBindingsManager::EMPTY_CONTEXT),
   usersCount(1)
{
  //
  // We can share ourselves with the CallTask with empty context.
  // The caller task can also be shared when the context is equal
  // but not necessarily empty and this has advantages.
  //
  callMap[VariableBindingsManager::EMPTY_CONTEXT] = this;
}

TaskInfo::TaskInfo(TaskInfo* parent,
		   VariableBindingsManager::ContextId context)
 : parent(parent),
   rootTask(0),
   varContext(context),
   usersCount(1)
{
   // Call map is not used in this case
}

inline void
TaskInfo::link(Key key, StrategyTransitionGraph::Substate* substate)
{
  seenMap[key] = substate;
  substate->referenceCount++;
}

inline void
TaskInfo::unlink(Key key)
{
  SeenMap::iterator it = seenMap.find(key);
  if (it != seenMap.end())
    unlink(it);
}

inline bool
TaskInfo::free()
{
  if (--usersCount == 0)
    {
      // Unlinks from the call map of the owner task
      if (varContext != VariableBindingsManager::EMPTY_CONTEXT)
	parent->callMap.erase(varContext);

      // The callMap should only contain the empty context when it is deleted.
      // However, child tasks are not removed before their parents in a
      // StrategyExecution::finish chain, but inmediatly after.
      //
      // We set our children's varContext to EMPTY_CONTEXT to prevent them
      // from accessing this parent, which is about to be freed.
      {
	const CallMap::const_iterator end = callMap.end();
	for (CallMap::const_iterator it = callMap.begin(); it != end; it++)
	  if (it->first != VariableBindingsManager::EMPTY_CONTEXT)
	    it->second->varContext = VariableBindingsManager::EMPTY_CONTEXT;

      }

      // Unlink the substates in its seen table
      {
	const SeenMap::const_iterator end = seenMap.end();
	for (SeenMap::const_iterator it = seenMap.begin(); it != end; ++it)
	  unlink(it);
      }

      delete this;
      return true;
    }

  return false;
}

inline void
TaskInfo::unlink(SeenMap::const_iterator entry)
{
  entry->second->free();
}
