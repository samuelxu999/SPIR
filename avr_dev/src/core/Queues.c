/*
************************* Queues.c **************************
	Author: Xu Ronghua
	mail: rxu22@binghamton.edu
	Create date: 2017-08-06
	Function: Implement circular queues for receive and transmit data
*/

#include "Queues.h"

/* Initialize specified queue */
void qInit (queue_struct *qvar, uByte *qbuf, uWord qsize) {
	/* Set in=out, and full=0, ie. buffer empty */
	qvar->in = 0x0001;
	qvar->full = 0x0000;
	qvar->out = 0x0001;
	qvar->pbuf=qbuf;
	qvar->buffsize=qsize;
}


/* Return the number of valid bytes in a specified buffer */
uWord qStatus (queue_struct *qvar) {
	uWord qin, qfull, qout, qsize;
	
	/* Read q buffer variables into locals */
	qin = qvar->in;
	qfull = qvar->full;
	qout = qvar->out;
	qsize = qvar->buffsize;
	
	if (qin>qout)
	{
		/* Data in buffer, no pointer wrap */
		return (qin-qout);
	}
	if (qin<qout)
	{
		/* Data in buffer with pointer wrap */
		return (qsize-qout+qin);
	}
	/* qin must be = qout */
	if (qfull)
	{
		/* Buffer full */
		return (qsize);
	}
	
	/* Buffer empty */
	return (0);
}


/* Dequeue a byte from the buffer
   Return 0 if no byte available, or non zero if byte read OK */
unsigned char qRead_byte (queue_struct *qvar, uByte *rData) {
	uLword q_full_out=0;
	uWord qin, qfull, qout, qsize;

	/* Read queue buffer variables into locals */
	qin = qvar->in;
	qfull = qvar->full;
	qout = qvar->out;
	qsize = qvar->buffsize;

	/* IF (queue data available) */
	if ((qin!=qout)||qfull) {
		/* read and dequeue data byte from buffer*/
		//*rData = qvar->buff[qout]; 	
		*rData = *(qvar->pbuf+qout); 
		
		/* update pointer..*/
		qout++; 					

		/* check for wraparound */
		if (qout>(qsize-1)) {
			qout = 0;
		}

		/* prepare for coherent write..*/
		q_full_out |= (uLword)qout<<16; 
		*(uLword *)(&(qvar->full)) = q_full_out; /* of qout with qfull zero */

		return 1; /* return 1 for successful read */
	}
	else {
		return 0; /* return 0 as no byte available */
	}

}


/* Queue a byte into the buffer 
   Return 0 if buffer already full, or non zero if byte queued OK */
unsigned char qWrite_byte (queue_struct *qvar, uByte wData) {
	uLword q_in_full=0;
	uWord qin, qfull, qout, qsize;

	/* Read queue buffer variables into locals */
	qin = qvar->in;
	qfull = qvar->full;
	qout = qvar->out;
	qsize = qvar->buffsize;

	/* IF (queue buffer not full) */
	if ((qin!=qout)||!qfull) {	
		/* write and queue data byte to buffer */
		//qvar->buff[qin] = wData;
		*(qvar->pbuf+qin) = wData;
		
		/* update pointer..*/
		qin++;

		/* check for wraparound */
		if (qin>(qsize-1)) {
			qin = 0; 
		}

		/* Buffer is now full, need to set FULL flag coherently with update of qin */
		/* Also generate warning that buffer is now full */
		if (qin==qout) {
			/* prepare for coherent write..*/
			q_in_full = (uLword)qin|(uLword)1<<16; 
			*(uLword *)(&(qvar->in)) = q_in_full;	/* of qin with qfull non-zero */
		}
		else {
			/* Buffer not full yet, so don't set FULL */
			qvar->in = qin;
		}
		return (1);
	}
	else {
		return 0; /* Return error code as no buffer space to queue data */
	}

}






