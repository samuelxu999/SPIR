/*
************************* port_macros.h ****************************
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-11-9
   Function: Defination for macros of drives
*/

#ifndef PROT_MACROS_H
#define PROT_MACROS_H

#define CONCATENATE(A,B)	A##B
#define CONCATENATE3(A,B,C)	A##B##C

#define PORT(letter)		CONCATENATE(PORT,letter)
#define DDR(letter)		CONCATENATE(DDR,letter)
#define PIN(letter)		CONCATENATE(PIN,letter)
#define PULLUP(letter)		CONCATENATE(PORT,letter)
#define TOGGLE(letter)		CONCATENATE(PIN,letter)

#define TCCR(number,letter)	CONCATENATE3(TCCR,number,letter)
#define OCR(number,letter)	CONCATENATE3(OCR,number,letter)


#endif
