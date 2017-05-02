/*
************************* port_macros.h ****************************
	Author: Xu Ronghua 		
	mail: rxu22@binghamton.edu
	Create date: 2017-04-27
	Function: Defination for macros of drives
*/

#ifndef PROT_MACROS_H
#define PROT_MACROS_H

#define CONCATENATE(A,B)			A##B
#define CONCATENATE3(A,B,C)		A##B##C

/*********************** General port and IO concatenate ***************************/
#define PORT(letter)				CONCATENATE(PORT,letter)
#define DDR(letter)				CONCATENATE(DDR,letter)
#define PIN(letter)				CONCATENATE(PIN,letter)
#define PULLUP(letter)				CONCATENATE(PORT,letter)
#define TOGGLE(letter)				CONCATENATE(PIN,letter)

/************** Timer/Counter reltaed register concatenate ***************************/
#define TCCR(number,letter)		CONCATENATE3(TCCR,number,letter)
#define OCR(number,letter)			CONCATENATE3(OCR,number,letter)

/*********************** UART related register concatenate ***************************/
#define UCSR(number,letter)		CONCATENATE3(UCSR,number,letter)
#define UBRR(number,letter)		CONCATENATE3(UBRR,number,letter)


#endif
