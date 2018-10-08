/* resched.c - resched, resched_cntl */

#include <xinu.h>

struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];
	//kprintf("PRCURR: %s\n", ptold->prname);
	//kprintf("PRCURR PRRECENT: %d\n", ptold->prrecent);
	//int32 i;
	/*for(i = 0; i < NPROC; i++) {
	  struct procent *prptr = &proctab[i];
	  kprintf("PRSTATE: %d\n", prptr->prstate);
	  kprintf("PRPRIO:  %d\n", prptr->prprio);
	  kprintf("PRNAME:  %s\n", prptr->prname);
	  }*/
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		//kprintf("ptold name: %s\n", ptold->prname);
		//kprintf("ptold prprio %d\n", ptold->prprio);
		intmask mask;
		if(ptold->prprio != 128) {
			ptold->prprio = ptold->prbaseprio + (2 * ptold->prextprio) + ptold->prrecent;
			if(ptold->prprio > 127) {
				//kprintf("resched: entered for processes: %s\n", ptold->prname);
				//kprintf("resched: process prio: %d\n", ptold->prextprio);
				ptold->prprio = 127;
			} else if (ptold->prprio < 0) {

				//kprintf("resched: entered for processes: %s\n", ptold->prname);
				//kprintf("resched: process prio: %d\n", ptold->prextprio);
				ptold->prprio = 0;
			}
		}

		//if(firstkey(readylist) < 128) {
			if (ptold->prprio < firstkey(readylist)) {
				//kprintf("PTOLD PRIO: %d\n", ptold->prprio);
				//kprintf("PTOLD NAME: %s\n", ptold->prname);
				//kprintf("FIRSTKEY: %d\n", firstkey(readylist));
				return;
			}
		//}

		/* Old process will no longer remain current */
		ptold->prstate = PR_READY;
		insert(currpid, readylist, ptold->prprio);
	}

	//kprintf("RESCHED: PROCESS CHECKS AFTER INSERTION for PID: %d\n", currpid);
	//kprintf("RESCHED: NEXT NODE ID (SHOULD BE CURRENT): %d\n", queuetab[currpid].qnext);
	//kprintf("RESCHED: PREVIOUS NODE ID (SHOULD BE PREV): %d\n", queuetab[currpid].qprev);
	//kprintf("RESCHED: KEY of ID:%d\n", queuetab[currpid].qkey);
	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);

	//kprintf("RESCHED2: PROCESS CHECKS AFTER INSERTION for PID: %d\n", 0);
	//kprintf("RESCHED2: NEXT NODE ID (SHOULD BE CURRENT): %d\n", queuetab[currpid].qnext);
	//kprintf("RESCHED2: PREVIOUS NODE ID (SHOULD BE PREV): %d\n", queuetab[currpid].qprev);
	//kprintf("RESCHED2: KEY of ID:%d\n", queuetab[currpid].qkey);
	ptnew = &proctab[currpid];
	ptnew->prstate = PR_CURR;
	preempt = ptnew->prquantum;		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
		int32	defer		/* Either DEFER_START or DEFER_STOP	*/
		)
{
	switch (defer) {

		case DEFER_START:	/* Handle a deferral request */

			if (Defer.ndefers++ == 0) {
				Defer.attempt = FALSE;
			}
			return OK;

		case DEFER_STOP:	/* Handle end of deferral */
			if (Defer.ndefers <= 0) {
				return SYSERR;
			}
			if ( (--Defer.ndefers == 0) && Defer.attempt ) {
				resched();
			}
			return OK;

		default:
			return SYSERR;
	}
}
