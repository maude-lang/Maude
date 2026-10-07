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
      
      sigchldAction.sa_handler = sigchldHandler;
      // don't set sigchldAction.sa_sigaction as it may be a union with the above
      sigemptyset(&sigchldAction.sa_mask);  // don't block any additional signals in the handler
      sigchldAction.sa_flags = 0;  // no flags
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
  Index nrRequests = childRequests.size();
  for (Index i = 0; i < nrRequests; ++i)
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
}

void
PseudoThread::sigchldHandler(int /* signalNr */)
{
  //
  //	Child status events are not queued so we just record that at least one
  //	happened, and leave it to non-signal handler code to figure out if one
  //	or more children exited.
  //
  exitedFlag = true;
}

bool
PseudoThread::dispatchChildRequests()
{
  DebugInfo("exitedFlag = " << exitedFlag);
  if (!exitedFlag)
    return false;
  //
  //	We block SIGCHLD so that we don't get a race between setting
  //	setting  exitedFlag to true in the signal handler and setting it
  //	to false here.
  //
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
      pid_t pid = childRequests[i].pid;
      int wstatus;
      if (waitpid(pid, &wstatus, WNOHANG) == pid)
	{
	  childRequests[i].client->doChildExit(childRequests[i].pid, wstatus);
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
  //
  //	Now we can unblock SIGCHLD without losing events.
  //
  sigprocmask(SIG_SETMASK, &oldset, 0);
  return didCallback;
}

void
PseudoThread::doChildExit(pid_t childPid, int /* status */)
{
  CantHappen("failed to do child exit on " << childPid);
}
