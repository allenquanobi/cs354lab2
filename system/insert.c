/* insert.c - insert */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  insert  -  Insert a process into a queue in descending key order
 *------------------------------------------------------------------------
 */
status	insert(
	  pid32		pid,		/* ID of process to insert	*/
	  qid16		q,		/* ID of queue to use		*/
	  int32		key		/* Key for the inserted process	*/
	)
{
	//kprintf("Entered insert\n");
	//struct procent *prptr = &proctab[pid];
	//kprintf("Process being added PID: %d\n", pid);
	//kprintf("Process being added name: %s\n", prptr->prname);
	//kprintf("Process being added priority: %d\n", prptr->prprio);
	//kprintf("Key being passed, should be same as previous line: %d\n", key);
	int16	curr;			/* Runs through items in a queue*/
	int16	prev;			/* Holds previous node index	*/

	if (isbadqid(q) || isbadpid(pid)) {
		//kprintf("ENTERED SYSERR QID: %d, PID: %d\n", q, pid);
		//kprintf("isbadqid: %d\n", isbadqid(q));
		//kprintf("isbadpid: %d\n", isbadpid(pid));
		//kprintf("pid state: %d\n", proctab[(pid)].prstate);
		//kprintf("pid state check: %d\n", (proctab[(pid)].prstate == PR_FREE));
		//kprintf("pid NPROC check: %d\n", ((pid32)(pid) >= NPROC));
		return SYSERR;
	}

	curr = firstid(q);
	//kprintf("CURRENT BEFORE WHILE AND EVERYTHING: %d\n", curr);
	//kprintf("CURRENT QUEUE: \n");
	//kprintf("TEST TO SEE IF PID 0 is getting updated\n");
	//kprintf("PID 0 KEY: %d\n", queuetab[0].qkey);
	//kprintf("PID 0 PREV: %d\n", queuetab[0].qprev);
	//kprintf("PID 0 NEXT: %d\n", queuetab[0].qnext);
	while (queuetab[curr].qkey <= key && queuetab[curr].qkey != MAXKEY) {
		//prptr = &proctab[curr];
		//kprintf("Process PID: %d\n", prptr->prparent);
		//kprintf("Process Name: %s\n", prptr->prname);
		//kprintf("Process Priority: %d\n", prptr->prprio);
		curr = queuetab[curr].qnext;
	}
	//kprintf("PROCESS BEING INSERTED AT: %d\n", pid);
	//kprintf("PROCESS WILL BE INSERTED BETWEEN CURRENT AND PREVIOUS\n");
	//kprintf("CURRENT NODE ID: %d\n", curr);
	//kprintf("PREVIOUS NODE ID: %d\n", queuetab[curr].qprev);
	/* Insert process between curr node and previous node */
	prev = queuetab[curr].qprev;	/* Get index of previous node	*/
	queuetab[pid].qnext = curr;
	queuetab[pid].qprev = prev;
	queuetab[pid].qkey = key;
	queuetab[prev].qnext = pid;
	queuetab[curr].qprev = pid;	
	//kprintf("PROCESS CHECKS AFTER INSERTION for PID: %d\n", pid);
	//kprintf("NEXT NODE ID (SHOULD BE CURRENT): %d\n", queuetab[pid].qnext);
	//kprintf("PREVIOUS NODE ID (SHOULD BE PREV): %d\n", queuetab[pid].qprev);
	//kprintf("KEY of ID:%d\n", queuetab[0].qkey);
	return OK;
}
