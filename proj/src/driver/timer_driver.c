/*
************* timer_driver.c ×××××××××××××××××××××××
     Author: Xu Ronghua 
       mail: rxu22@binghamton.edu
Create date: 2016-11-23
   Function: Implementation of Timer driver
*/

#include "timer_driver.h"

volatile unsigned long tickCount = 0;	// incremented by 1024 every T2 OVF (units of 50 us)
volatile unsigned long msCount = 0;	// incremented by 20 every tickCount (units of 1 ms)
volatile unsigned long secCount = 0;	// incremented by 1000 every msCount (units of 1 sec)
volatile unsigned long minCount = 0;	// incremented by 60 every secCount (units of 1 min)

void initialize_timer()
{	
	TCCR2A |= 0x03;		// fast PWM, TOP = 0xFF
	TCCR2B &= 0xF0;
	TCCR2B |= 0x02;		// timer2 ticks at 10KHz, 0.1ms(f=20MHz) (prescaler = 8)	
		
	sei();			// enable global interrupts
}

ISR(TIMER2_OVF_vect) {
    	//calculate ms
	tickCount+=1;
	if(10==tickCount) 
	{
		msCount++;
		tickCount=0;
	}
	//calculate sec	
	if(1000==msCount)
	{		
		secCount++;
		msCount=0;
	}
	//calculate min	
	if(60==secCount)
	{
		minCount++;
		secCount=0;
	}	
}

unsigned long get_ticks() { return tickCount; }
unsigned long get_msCount() { return msCount; }
unsigned long get_secCount() { return secCount; }
unsigned long get_minCount() { return minCount; }

void start_timer()
{
	TIFR2 |= (1 << TOV2);		// clear timer2 overflow flag
	TIMSK2 |= (1 << TOIE2);		// enable timer2 overflow interrupt		
}

void stop_timer()
{
	TIFR2 &= ~(1 << TOV2);		// set timer2 overflow flag
	TIMSK2 &= ~(1 << TOIE2);	// disable timer2 overflow interrupt		
}

void reset_timer()
{
	tickCount=0;
	msCount=0;
	secCount=0;
	minCount=0;
}

