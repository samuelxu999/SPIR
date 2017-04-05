/*
************* motor_driver.h ×××××××××××××××××××××××
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-11-9
   Function: Interface definition for motor driver
*/

#ifndef MOTOR_DRIVER_H
#define MOTOR_DRIVER_H

#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include "port_macros.h"
#include <util/delay.h>

#define LEFT_MOTOR_A_LOC	6
#define LEFT_MOTOR_A_PORT	D
#define LEFT_MOTOR_B_LOC	5
#define LEFT_MOTOR_B_PORT	D
#define LEFT_MOTOR_TIMER	0

#define RIGHT_MOTOR_A_LOC	3
#define RIGHT_MOTOR_A_PORT	B
#define RIGHT_MOTOR_B_LOC	3
#define RIGHT_MOTOR_B_PORT	D
#define RIGHT_MOTOR_TIMER	2

/************** declear pushbutton driver interface ************/

void initialize_left_motor();
void brake_left_motor();
void coast_left_motor();
void engage_left_motor_forward();
void engage_left_motor_backward();
void set_left_motor_duty_cycle(uint8_t duty_cycle_u8);
uint8_t get_left_motor_duty_cycle();
void speedup_left_motor_forward(uint8_t duty_cycle_u8);

void initialize_right_motor();
void brake_right_motor();
void coast_right_motor();
void engage_right_motor_forward();
void engage_right_motor_backward();
void set_right_motor_duty_cycle(uint8_t duty_cycle_u8);
uint8_t get_right_motor_duty_cycle();

void initialize_motor();
void activate_motors(int speed_left,int speed_right);
void brake_motors();

#endif
