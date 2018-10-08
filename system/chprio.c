/* chprio.c - chprio */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  chprio  -  Change the scheduling priority of a process
 *------------------------------------------------------------------------
 */
pri16	chprio(
		pid32		pid,		/* ID of process to change	*/
		pri16		newprio		/* New priority			*/
	      )
{
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	pri16	oldprio;		/* Priority to return		*/
	intmask mask;
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return (pri16) SYSERR;
	}
	prptr = &proctab[pid];
	oldprio = prptr->prextprio;
	if(pid != NULLPROC) {
		if(newprio > 20) {
			prptr->prextprio = 20;
		} else if (newprio < -20) {
			prptr->prextprio = -20;
		} else {
			prptr->prextprio = newprio;
		}
		prptr->prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;
		if(prptr->prprio > 127) {
//			kprintf("CHPRIO: ENTERED FOR PROCESSES: %s\n", prptr->prname);
//			kprintf("CHPRIO: PROCESS PRIO: %d\n", prptr->prextprio);
			prptr->prprio = 127;
		} else if (prptr->prprio < 0) {
//			kprintf("CHPRIO: ENTERED FOR PROCESSES: %s\n", prptr->prname);
//			kprintf("CHPRIO: PROCESS PRIO: %d\n", prptr->prextprio);
			prptr->prprio = 0;
		}
	}
	resched();
	restore(mask);
	return oldprio;
}

syscall system(pid32 pid){
	intmask mask;
	struct procent *prptr;
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	prptr = &proctab[pid];
	if(pid != NULLPROC) {
		prptr->prbaseprio = 0;
		prptr->prprio = prptr->prbaseprio + (2 * prptr->prextprio) + prptr->prrecent;
		if(prptr->prprio > 127) {	
//			kprintf("chprio system127: entered for processes: %s\n", prptr->prname);
//			kprintf("chprio system127: process prio: %d\n", prptr->prextprio);
//			kprintf("chprio system127: process baseprio %d\n", prptr->prbaseprio);
//			kprintf("chprio system127: process extprio %d\n", prptr->prextprio);
//			kprintf("chprio system127: process prrecent %d\n", prptr->prrecent);
			prptr->prprio = 127;
		} else if (prptr->prprio < 0) {
//			
//			kprintf("chprio system0: entered for processes: %s\n", prptr->prname);
//			kprintf("chprio system0: process prio: %d\n", prptr->prextprio);
//			kprintf("chprio system0: process baseprio %d\n", prptr->prbaseprio);
//			kprintf("chprio system0: process extprio %d\n", prptr->prextprio);
//			kprintf("chprio system0: process prrecent %d\n", prptr->prrecent);
			prptr->prprio = 0;
		}
	}
	resched();
	restore(mask);
	return OK;
}
