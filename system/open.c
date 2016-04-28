/* open.c - open */

#include <xinu.h>

/*------------------------------------------------------------------------
 *  open  -  Open a device (some devices ignore name and mode parameters)
 *------------------------------------------------------------------------
 */
syscall	open(
	  did32		descrp,		/* Descriptor for device	*/
	  char		*name,		/* Name to use, if any		*/
	  char		*mode		/* Mode for device, if any	*/
	)
{
	intmask		mask;		/* Saved interrupt mask		*/
	struct dentry	*devptr;	/* Entry in device switch table	*/
	struct	procent *prptr;		/* Ptr to process' table entry	*/
	int32		retval;		/* Value to return to caller	*/
	int32 pid;
	int32 i,unused_desc_found = 0;

	mask = disable();
	if (isbaddev(descrp)) {
		restore(mask);
		return SYSERR;
	}
	pid = getpid();
	prptr = &proctab[pid]; //Get the process entry from the process table.
	devptr = (struct dentry *) &devtab[descrp];
	

	for(i=0;i<prptr->prdesc_count;i++)
	{
		if(prptr->prdesc[i] == -1)
		{
			unused_desc_found = 1;
			prptr->prdesc[i] = descrp;
			break;
		}
	}

	if(prptr->prdesc_count == NDESC && unused_desc_found == 0) //The process has opened maximum number of files.
	{
		restore(mask);
		return SYSERR;
	}
	if(unused_desc_found == 0)
		prptr->prdesc[prptr->prdesc_count++] = descrp; //Add the device to the process descriptor table.
	retval = (*devptr->dvopen) (devptr, name, mode);
	restore(mask);
	return retval;
}
