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
//	Misc process manipulation code.
//

void
ProcessManagerSymbol::cleanUp(DagNode* objectId)
{
  int processId;
  ChildProcess* cpp;
  if (getChildProcess(objectId, processId, cpp))
    {
      DebugInfo("cleaning up " << objectId);
      if (cpp->waitContext != 0)
	{
	  //
	  //	We are waiting for an exit callback on this child.
	  //	This can happen after a ^C and abort.
	  //	We need to cancel this request so it doesn't get
	  //	delivered in some future computation when waitContext
	  //	is stale.
	  //
	  cancelChildExitCallback(processId);
	}
      /*
      if (cpp->ioSocket != NONE)
	close(cpp->ioSocket);
      if (cpp->errSocket != NONE)
	close(cpp->errSocket);
      */
      //
      //	Many programs will exit when their stdio streams are closed
      //	but we can't be sure of that. So make sure.
      //	No point checking first since we would run into a race
      //	condition.
      //
      kill(processId, SIGKILL);  // may return failure code
      //
      //	Wait for it to die and remove the zombie.
      //	We want to be robust against early exit due to
      //	signals.
      //
      int r;
      do
	r = waitpid(processId, 0, 0);
      while (r == -1 && errno == EINTR);
      //
      //	Remove our record of the child.
      //
      childProcesses.erase(processId);
    }
}

void
ProcessManagerSymbol::doChildExit(pid_t childPid)
{
  DebugInfo("childPid = " << childPid);
  //
  //	First we wait for the child.
  //
  int wstatus;
  DebugSave(exitPid, waitpid(childPid, &wstatus, 0));
  Assert(exitPid == childPid, "unexpected return value " << exitPid << " from waidpid() for process " << childPid);
  //
  //	Recover original waitForProcessExit message and context.
  //
  ProcessMap::iterator i = childProcesses.find(childPid);
  Assert(i != childProcesses.end(), "missing child");
  ChildProcess& cp = i->second;
  FreeDagNode* message = safeCast(FreeDagNode*, cp.waitMessage.getNode());
  ObjectSystemRewritingContext& context = *(cp.waitContext);
  //
  //	Generate exited() reply.
  //
  //
  //	Child status changes; make sure it is an exit.
  //
  if (WIFEXITED(wstatus))
    {
      //
      //	Normal exit with exit code.
      //
      int exitCode = WEXITSTATUS(wstatus);
      exitedReply(childPid, exitCode, message, context);
    }
  else if (WIFSIGNALED(wstatus))
    {
      //
      //	Terminated by signal.
      //
      int terminatingSignal = WTERMSIG(wstatus);
      exitedReply(childPid, ~terminatingSignal, message, context);
    }
  else
    CantHappen("neither WIFEXITED nor WIFSIGNALED for process " << childPid);
}
