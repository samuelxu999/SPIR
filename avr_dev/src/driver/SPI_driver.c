/*
******************************** SPI_driver.c **********************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-4 -29
	 Function: Interface implementation for SPI driver
*/

#include "SPI_driver.h"


/***************** SPI master default setting initialize **************************
	Example usage:	SPI_MasterInit();
	Input:
		void
*/
void SPI_MasterInit(void){
	//deactivate slave
	SPI_SS_H();

	/* Set MOSI, SS and SCK output, MISO as input */
	SPI_DDR |= ((1<<SPI_MOSI_LOC)|(1<<SPI_SCK_LOC)|(1<<SPI_SS_LOC));

	
	/*		SPI Control Register - SPCR strucutre:
		 ------------------------------------------------
		| SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0 |
		 ------------------------------------------------
		
		Function: 
			SPE=1: 			Enable SPI
			MSTR=1:			Master select
			DORD=0: 			MSB of the data word is transmitted first
			CPOL-CPHA=00:	SPI mode 00
			SPR1~0:			set clock rate fck/16 
	*/
	SPCR = ((1<<SPE)|(1<<MSTR)|(1<<SPR0));
}

/***************** SPI master date transmit **************************
	Example usage:	SPI_MasterTransmit(cData);
	Input:
		@cData		data to be transmitted by byte block
*/
void SPI_MasterTransmit(unsigned char cData){
	/* Start transmission */
	SPDR = cData;
	/* 	SPI Status Register - SPSR strucutre:
		 ----------------------------------
		| SPIF | WCOL | - | - | - | - | - | SPI2X |
		 ---------------------------------- 
	Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
}


/***************** SPI slave default setting initialize **************************
	Example usage:	SPI_SlaveInit();
	Input:
		void
*/
void SPI_SlaveInit(void){
	//deactivate slave
	SPI_SS_H();

	/* Set MISO output, all others are input */
	SPI_DDR |= (1<<SPI_MISO_LOC);

	
	/*		SPI Control Register - SPCR strucutre:
		 ------------------------------------------------
		| SPIE | SPE | DORD | MSTR | CPOL | CPHA | SPR1 | SPR0 |
		 ------------------------------------------------
		
		Function: 
			SPE=1: 			Enable SPI
			MSTR=0:			Slave select			
	*/
	SPCR |= (1<<SPE);
}


/***************** SPI slave date transmit **************************
	Example usage:	cData=SPI_SlaveReceive();
	Output:
		@cData		received data by byte block
*/
unsigned char SPI_SlaveReceive(void){
	/*Wait for transmission complete */
	while(!(SPSR & (1<<SPIF)));
	
	/* Return data register */
	return SPDR;
}


