/*
************* menu_driver.h ×××××××××××××××××××××××
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-12-12
   Function: Interface definition for menu driver
*/

#ifndef MENU_DRIVER_H
#define MENU_DRIVER_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "lcd_driver.h"
#include "ADC_driver.h"

#define TIMER_EN 	OCIE1A
#define TIMER_IS_EN 	OCF1A

/*****declear timer driver interface******/

void initialize_menu();

void refresh_lcd();

void lcd_display();

#endif
