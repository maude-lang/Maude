/*

    This file is part of the Maude 3 interpreter.

    Copyright 2020 SRI International, Menlo Park, CA 94025, USA.

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
//	Signal handling code.
//

Vector<PseudoThread::ChildRequest> PseudoThread::childRequests;
bool PseudoThread::installedSigchldHandler = false;
bool PseudoThread::exitedFlag = false;

void
PseudoThread::requestChildExitCallback(pid_t childPid)
{
  //
  //	We need to ensure that we're not interrupted by the
  //	signal handler while we're changing childVec.
  //
  sigset_t oldset;
  sigset_t newset;
  sigemptyset(&newset);
  sigaddset(&newset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &newset, &oldset);
  childRequests.append(ChildRequest(this, childPid));
  sigprocmask(SIG_SETMASK, &oldset, 0);

  if (!installedSigchldHandler)
    {
      static struct sigaction sigchldAction;
      //
      //	We use a 3 argument SIGINFO style handler because we
      //	need the extra information about the child.
      //
      sigchldAction.sa_sigaction = sigchldHandler;
      sigchldAction.sa_flags = SA_SIGINFO;
#ifdef SA_INTERRUPT
      //
      //	Avoid old BSD semantics which automatically restarts
      //	interrupted system calls.
      //
      //	It is important that ppoll() or pselect() exit in order
      //	not to block indefinitely.
      //
      sigchldAction.sa_flags |= SA_INTERRUPT;
#endif
      sigaction(SIGCHLD, &sigchldAction, 0);
      installedSigchldHandler = true;
    }
}

void
PseudoThread::cancelChildExitCallback(pid_t childPid)
{
  //
  //	We need to ensure that we're not interrupted by the
  //	signal handler while we're changing childVec.
  //
  sigset_t oldset;
  sigset_t newset;
  sigemptyset(&newset);
  sigaddset(&newset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &newset, &oldset);

  int nrRequests = childRequests.size();
  for (int i = 0; i < nrRequests; ++i)
    {
      if (childRequests[i].pid == childPid)
	{
	  --nrRequests;
	  if (i < nrRequests)
	    childRequests[i] = childRequests[nrRequests];
	  childRequests.contractTo(nrRequests);
	  break;
	}
    }
  
  sigprocmask(SIG_SETMASK, &oldset, 0);
}

void
PseudoThread::sigchldHandler(int /* signalNr */, siginfo_t* info, void* /* context */)
{
  if (info->si_code == CLD_EXITED ||
      info->si_code == CLD_KILLED ||
      info->si_code == CLD_DUMPED)
    {
      //
      //	Currently we only worry about the child exiting normally (CLD_EXITED)
      //	being killed by a signal (CLD_KILLED) or exiting abnoramlly (CLD_DUMPED).
      //	We ignore other changes of state.
      //	We're in a signal handler so we don't want to call library functions
      //	that might access global stuff such as memory allocation.
      //	We know that Vector::iterator does not allocate memory.
      //
      pid_t childPid = info->si_pid;
      for (ChildRequest& c : childRequests)
	{
	  if (c.pid == childPid)
	    {
	      c.exited = true;
	      exitedFlag = true;
	      break;
	    }
	}
    }
}

bool
PseudoThread::dispatchChildRequests()
{
  if (!exitedFlag)
    return false;

  DebugInfo("child exits to process");
  
  sigset_t oldset;
  sigset_t newset;
  sigemptyset(&newset);
  sigaddset(&newset, SIGCHLD);
  sigprocmask(SIG_BLOCK, &newset, &oldset);

  bool didCallback = false;
  int nrRequests = childRequests.size();
  DebugInfo("nrRequests = " << nrRequests);
  for (int i = 0; i < nrRequests;)
    {
      if (childRequests[i].exited)
	{
	  childRequests[i].client->doChildExit(childRequests[i].pid);
	  didCallback = true;
	  //
	  //	Reduce number of pending requests and fill in the hole
	  //	if we're not the final request.
	  //
	  --nrRequests;
	  if (i < nrRequests)
	    childRequests[i] = childRequests[nrRequests];
	  childRequests.contractTo(nrRequests);
	}
      else
	++i;
    }

  exitedFlag = false;
  sigprocmask(SIG_SETMASK, &oldset, 0);
  return didCallback;
}

void
PseudoThread::doChildExit(pid_t childPid)
{
  CantHappen("failed to do child exit on " << childPid);
}
