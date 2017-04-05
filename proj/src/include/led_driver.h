/*
************* led_driver.h ×××××××××××××××××××××××
     Author: Xu Ronghua 
       Mail: rxu22@binghamton.edu
Create date: 2016-10-20
   Function: Interface definition for LED driver
*/

#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <avr/io.h>
#include "port_macros.h"

#define LED_DDR DDR(D)
#define LED_PORT PORT(D)

#define RED_LED_LOC 1
#define GREEN_LED_LOC 7

/*****declear pushbutton driver interface******/

void initialize_red_led();

void initialize_green_led();

void turn_on_red_led();

void turn_off_red_led();

void turn_on_green_led();

void turn_off_green_led();

void toggle_red_led();

void toggle_green_led();

#endif
