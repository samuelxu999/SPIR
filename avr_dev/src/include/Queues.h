/*
************************* Queues.h **************************
	Author: Xu Ronghua
	mail: rxu22@binghamton.edu
	Create date: 2017-08-06
	Function: Define circular queues prototype
*/

#ifndef QUEUES_H
#define QUEUES_H

#include "Types.h"


//#define QSIZE 128 	/* Queue size - bytes */

/* Typedef for queue structures */
typedef struct {
	uWord in; 				/* Queue input/write pointer */
	uWord full; 			/* Queue full flag */
	uWord out; 				/* Queue output/read pointer */
	//uByte buff[QSIZE]; 		/* Queue array */
	uByte *pbuf; 			/* Queue buffer head pointer */
	uWord buffsize; 		/* Queue buffer size */
} queue_struct;


/***************** Initialize a specified buffer **************************
	Example usage:	qInit(&queue_struct);
	Input:
		@*qvar		pointer of queue_struct
*/
void qInit (queue_struct *qvar, uByte *qbuf, uWord qsize);


/***************** Return the status of a specified buffer **************************
	Example usage:	qInit(&queue_struct);
	Input:
		@*qvar		pointer of queue_struct
	Output:
		@uWord		return sttus of queue
*/
uWord qStatus (queue_struct *qvar);


/***************** Read and dequeue a byte from the queue buffer **************************
	Example usage:	qRead_byte(&qvar, &byte);
	Input:
		@*qvar		pointer of queue_struct
	Output:
		@&rData		get read byte data 
		@uWord:		return executed result
*/
unsigned char qRead_byte (queue_struct *qvar, uByte *rData); 

/***************** Write and queue a byte to the queue buffer **************************
	Example usage:	qRead_byte(&qvar, byte);
	Input:
		@*qvar		pointer of queue_struct
		@wData		input data that will be written to queue
	Output:
		@uWord:		return executed result
		
*/
unsigned char qWrite_byte (queue_struct *qvar, uByte wData);


/************** Sensor handler to collect and send sensor data ***************
Function: 1) Listen command from RS232 through USART
		  2) Read sensor data based on command.
		  3) Transmit data to user through RS232.
*/
void SensorHandler();



#endif
