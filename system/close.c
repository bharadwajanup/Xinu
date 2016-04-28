/* close.c - close */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  close  -  Close a device
 *------------------------------------------------------------------------
 */
syscall	close(
	  did32		descrp		/* Descriptor for device	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct	dentry	*devptr;	/* Entry in device switch table	*/
	struct	procent *prptr;
	int32		retval;		/* Value to return to caller	*/
	int32 pid,i;


	mask = disable();

	pid = getpid();
	prptr = &proctab[pid];
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}

	for(i=0;i<prptr->prdesc_count;i++)
	{
		if(prptr->prdesc[i] == descrp)
		{
			prptr->prdesc[i] = -1;
			break;
		}
	}

	//prptr->prdesc_count--; //Decrement the counter since the process programmatically closes it.
	devptr = (struct dentry *) &devtab[descrp];
	retval = (*devptr->dvclose) (devptr);
	restore(mask);
	return retval;
}
