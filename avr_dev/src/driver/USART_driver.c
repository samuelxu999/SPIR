/*
************************* USART_driver.c ***************************************
	Author: Xu Ronghua
	mail: rxu22@binghamton.edu
	Create date: 2017-04-27
	Function: Implementation for USART driver
****************************************************************************/


#include "USART_driver.h"


/********* Define USART0-Control and Status Register *********/
#define USART0_UCSRA	UCSR(0,A)
#define USART0_UCSRB	UCSR(0,B)
#define USART0_UCSRC	UCSR(0,C)


/********* Define USART0-Baud Rate Registers *********/
#define USART0_UBRRL	UBRR(0,L)
#define USART0_UBRRH	UBRR(0,H)


/********* Define Global variable for *******************/
volatile unsigned char cmd_data = 0x00;

/********* Define USART0 PORT register*******************/
#define USART_PORT 	PORT(E)
#define USART_DDR 		DDR(E)
#define USART_PIN 		PIN(E)
#define USART_RXD0		PE0
#define USART_TXD0		PE1

/********* Define CPU frequency and Baud rate*******************/
#define F_CPU 			8000000UL
#define BAUD 			9600


/*Calculate ubrr based on Baud:FOSC/16/ui_Baud-1, 
	When FOSC=8MHz 
	Baud	|	U2X = 0		|	U2X = 1
------------------------------------------
	9600	|	51 			|	103
	14.4k	|	34			|	68 
	19.2k	|	25			|	51
	28.8k	|	17			|	34
*/


/****************** USART0 initialize funtion to set USART0 *************************
	Function: Initialize the USART0 default seting based on usage.
			1) set the baud rate
			2) set frame format
			3) enabling the Transmitter or the Receiver
	Input:
		@ui_Baud: Baud rate
*/
void USART0_Init(void){
	//used for ubrr setting
	unsigned int ui_ubrr=0;

	/******** Disable USART0 before setting baud rate *********/
	USART0_UCSRB = 0x00;	

	/********* Set USART Control and Status Register A ***********
		U2Xn=0, disable double the USART Transmission Speed.		
			This bit only has effect for the asynchronous operation:
				--U2X = 0; the divisor of the baud rate divider is 16
				--U2X = 1; the divisor of the baud rate divider is 8
	******************************************************/
	USART0_UCSRA = 0x00;
	

	/**************** Set baud rate-UBRR ********************/	
	ui_ubrr=F_CPU/16/BAUD-1;
	USART0_UBRRH = (unsigned char)(ui_ubrr>>8);
	USART0_UBRRL = (unsigned char)ui_ubrr;
	
		
	/****************** Set USART Control and Status Register C ************
		USART Control and Status Register-AUCSRnC:
		------------------------------------------------------------
		| -- | UMSELn | UPMn1 | UPMn0 | USBSn | UCSZn1 | UCSZn0 | UCPOLn |
		------------------------------------------------------------
		UMSELn=0:		Asynchronous operation
		UPMn1:0=00:		Disable parity mode
		USBSn=0:			1stop bit
		UCSZ1:0=11		8 bits data
	****************************************************************/
	USART0_UCSRC |= ((1<<UCSZ01)|(1<<UCSZ00));


	/**************** Set USART Control and Status Register B **************
		USART Control and Status Register-AUCSRnC:
		------------------------------------------------------------
		| RXCIEn | TXCIEn | UDRIEn | RXENn | TXENn | UCSZn2 | RXB8n | TXB8n |
		------------------------------------------------------------
		RXEN0=1:			Enable receiver 
		TXEN0=1:			Enable transmitter 
		UCSZ02=0:		Disable 9 bits data
	****************************************************************/
	USART0_UCSRB |= ((1<<RXCIE)|(1<<RXEN0)|(1<<TXEN0));
}



/******************** USART0 transmit function implementation **********************
	Function: load data to transmit buffer.
	Input:
		@uc_data: byte data to be transmitted
*/
void USART0_Transmit(unsigned char uc_data){
	/*	USART Control and Status Register-AUCSRnA:
		---------------------------------------------------
		| RXCn | TXCn | UDREn | FEn | DORn | UPEn | U2Xn | MPCMn |
		---------------------------------------------------		
	*/

	/*Wait for empty transmit buffer by checking UDREn in UCSRA
	/UDRE0:	indicates if the transmit buffer (UDRn) is ready to receive new data.*/
	while ( !( USART0_UCSRA & (1<<UDRE0)) );
	
	/* Put data into buffer, sends the data */
	UDR0 = uc_data;
}


/********************* USART0 receive function implementation **********************
	Function: return data from received buffer.
	Input:
		@void
*/
unsigned char USART0_Receive( void ){
	/******** Wait for data to be received by checking RXC0 in UCSRA *****/
	while ( !(USART0_UCSRA & (1<<RXC0)) );
	
	/* Get and return received data from buffer */
	return UDR0;
}


/*************************** USART0 send data as string **************************
	Function: return data from received buffer.
	Input:
		@p_data:		string data to be sent
*/
void USART0_putstr(unsigned char *p_data){
	while(*p_data)
	{
		USART0_Transmit(*p_data);
		p_data++;
	}
}

/*************************** USART0 receive interrupt handle fucntion **************************
	Function: 1) handle receive interrupt to read command data from PC.
			2) Saved received command data to @cmd_data
*/
ISR(USART0_RX_vect) {
	//unsigned char cmd_data;
	//cmd_data=USART0_Receive();
	/*if(0x01==cmd_data){
			//USART0_putstr("Start monitor!");
	}
	if(0x00==cmd_data){
			//USART0_putstr("Stop monitor!");
	}*/
	cmd_data=USART0_Receive();
}



