/*
********************* pushbutton_driver.c *********************
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-10-20
   Function: Implementation of push-button driver
*/

#include "pushbutton_driver.h"

void initialize_pushbutton_array()
{
	//config DDRB1, DDRB4 and DDRB5 as input
	PUSHBUTTON_DDR&=~((1<<LEFT_BUTTON_LOC)|(1<<MIDDLE_BUTTON_LOC)|(1<<RIGHT_BUTTON_LOC));
		
	//enable internal pull-up by writing 1 to PORTB1, PORTB4 and PORTB5
	PUSHBUTTON_PULLUP|=(1<<LEFT_BUTTON_LOC)|(1<<MIDDLE_BUTTON_LOC)|(1<<RIGHT_BUTTON_LOC);
}

bool left_button_is_pressed()
{	
	if(0==(PUSHBUTTON_PIN&(1<<LEFT_BUTTON_LOC)))
	{
		return true;
	}
	return false;
}

bool right_button_is_pressed()
{	
	if(0==(PUSHBUTTON_PIN&(1<<RIGHT_BUTTON_LOC)))
	{
		return true;
	}
	return false;
}

bool middle_button_is_pressed()
{
	if(0==(PUSHBUTTON_PIN&(1<<MIDDLE_BUTTON_LOC)))
	{
		return true;
	}
	return false;
}

void wait_button_released(uint8_t button)
{
	while(!(PUSHBUTTON_PIN&(1<<button)));
	_delay_ms(10);	//debounce the button release	
}

