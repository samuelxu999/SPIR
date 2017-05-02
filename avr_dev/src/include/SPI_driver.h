/*
************************* SPI_driver.h **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-4 -29
	 Function: Interface definition for SPI driver
*/

#ifndef SPI_DRIVER_H
#define SPI_DRIVER_H

#include <avr/io.h>
#include "port_macros.h"

/******************** Define SPI control register ****************************/
#define SPI_DDR					DDR(B)
#define SPI_PORT				PORT(B)
#define SPI_PING				PORT(B)

#define SPI_SS_LOC				PB0
#define SPI_SCK_LOC			PB1
#define SPI_MOSI_LOC			PB2
#define SPI_MISO_LOC			PB3
#define SPI_RESET_LOC			PB4
#define SPI_DRDY_LOC			PB5

//Used for driving the SS pin of the SPI slave: 0-activate; 1-deactivate
#define SPI_SS_H() 			SPI_PORT|=(1<<SPI_SS_LOC)
#define SPI_SS_L() 			SPI_PORT&=~(1<<SPI_SS_LOC)






/***************** SPI master default setting initialize **************************
	Example usage:	SPI_MasterInit();
	Input:
		void
*/
void SPI_MasterInit(void);

/***************** SPI master date transmit **************************
	Example usage:	SPI_MasterTransmit(cData);
	Input:
		@cData		data to be transmitted by byte block
*/
void SPI_MasterTransmit(unsigned char cData);


/***************** SPI slave default setting initialize **************************
	Example usage:	SPI_SlaveInit();
	Input:
		void
*/
void SPI_SlaveInit(void);

/***************** SPI slave date transmit **************************
	Example usage:	cData=SPI_SlaveReceive();
	Output:
		@cData		received data by byte block
*/
unsigned char SPI_SlaveReceive(void);

#endif

