/*
************* led_driver.c ×××××××××××××××××××××××
     Author: Xu Ronghua 
       mail: rxu22@binghamton.edu
Create date: 2016-10-20
   Function: Implementation of LED driver
*/

#include "led_driver.h"

void initialize_red_led()
{
	/*define direction for DDRD1 as output*/
	LED_DDR|=(1<<RED_LED_LOC);	
}

void initialize_green_led()
{
	/*define direction for DDRD7 as output*/
	LED_DDR|=(1<<GREEN_LED_LOC);
}

void turn_on_red_led()
{
	LED_PORT|=(1<<RED_LED_LOC);
}

void turn_off_red_led()
{
	LED_PORT&=~(1<<RED_LED_LOC);
}

void turn_on_green_led()
{
	LED_PORT|=(1<<GREEN_LED_LOC);
}

void turn_off_green_led()
{
	LED_PORT&=~(1<<GREEN_LED_LOC);
}

void toggle_red_led()
{
	LED_PORT^=(1<<RED_LED_LOC);
}

void toggle_green_led()
{
	LED_PORT^=(1<<GREEN_LED_LOC);
}


