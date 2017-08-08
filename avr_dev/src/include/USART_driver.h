/*
************************* USART_driver.h **************************
	Author: Xu Ronghua
	mail: rxu22@binghamton.edu
	Create date: 2017-04-27
	Function: Interface definition for USART driver
*/

#ifndef USART_DRIVER_H
#define USART_DRIVER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdint.h>
#include <stdlib.h>
#include "port_macros.h"
#include "Queues.h"


/********* Declear extern  global variable to provide command data*******************/
extern queue_struct rx_USART, tx_USART; 	/* Transmit and receive queue structures */



/*********** USART0 initialize funtion to set USART0 ************
	Function: Initialize the USART0 default seting based on usage.
			1) set the baud rate
			2) set frame format
			3) enabling the Transmitter or the Receiver
	Input:
		@ui_Baud: Baud rate
*/
void USART0_Init();


/*********** USART0 transmit function implementation ************
	Function: Initialize the USART0 default seting based on usage.
	Input:
		@uc_data: byte data to be transmitted
*/
void USART0_Transmit(unsigned char uc_data);


/*********** USART0 receive function implementation ************
	Function: return data from received buffer.
	Input:
		@void
*/
unsigned char USART0_Receive( void );


/*************************** USART0 send data as string **************************
	Function: return data from received buffer.
	Input:
		@p_data:		string data to be sent
*/
void USART0_putstr(unsigned char *p_data);


/******** dequeue data from tx_USART buffer and send to USART0 **********************
	Example usage:	SensorHandler()
*/
void USART0_sendbuffer();

#endif
