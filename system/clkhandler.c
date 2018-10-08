/* clkhandler.c - clkhandler */

#include <xinu.h>

/*------------------------------------------------------------------------
 * clkhandler - high level clock interrupt handler
 *------------------------------------------------------------------------
 */
void	clkhandler()
{
	static	uint32	count1000 = 1000;	/* Count to 1000 ms	*/
	/* Decrement the ms counter, and see if a second has passed */
	//intmask mask;
	//mask = disable();
	struct procent *prptr = &proctab[currpid];
	if(currpid != NULLPROC) {
		prptr->CPUTime = prptr->CPUTime + 1;
		if(prptr->CPUTime >= 10) {
			prptr->prrecent = prptr->prrecent+1;
			prptr->CPUTime = (prptr->CPUTime % 10);
		}
	}
	//restore(mask);
	/*if(count1000 % 10 == 0) {
	  intmask mask;
	  mask = disable();
	  if(isbadpid(currpid)) {
	  restore(mask);
	  return;
	  }
	  prptr->prrecent = prptr->prrecent + 1;
	  restore(mask);
	  }*/
	if((--count1000) <= 0) {

		/* One second has passed, so increment seconds count */

		clktime++;

		/* Reset the local ms counter for the next second */

		count1000 = 1000;
	}

	/* Handle sleeping processes if any exist */

	if(!isempty(sleepq)) {

		/* Decrement the delay for the first process on the	*/
		/*   sleep queue, and awaken if the count reaches zero	*/

		if((--queuetab[firstid(sleepq)].qkey) <= 0) {
			wakeup();
		}
	}

	/* Decrement the preemption counter, and reschedule when the */
	/*   remaining time reaches zero			     */

	if((--preempt) <= 0) {
		preempt = prptr->prquantum;
		resched();
	}
}
