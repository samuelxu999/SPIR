/*
****************************** motor_driver.c *************************
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-11-9
   Function: Implementation of motor driver
*/

#include "motor_driver.h"

#define TCCRA(number)	TCCR(number,A) 
#define TCCRB(number)	TCCR(number,B) 
#define OCRA(number)	OCR(number,A) 
#define OCRB(number)	OCR(number,B)

#define COMxA1		7
#define COMxA0		6
#define COMxB1		5
#define COMxB0		4

/********************** implementation for left motor **********************************/
void initialize_left_motor()
{
	/*default duty cycle as 50%*/
	set_left_motor_duty_cycle(0x80);
	/*set motor as brake mode*/
	brake_left_motor();	
	
	/*IO port direction initialization, PB5 and PB6 as output*/
	DDR(LEFT_MOTOR_A_PORT)|=(1<<LEFT_MOTOR_A_LOC);
	DDR(LEFT_MOTOR_B_PORT)|=(1<<LEFT_MOTOR_B_LOC);

	/*TCCR initialization, turn on TCN0*/
	TCCRA(LEFT_MOTOR_TIMER)=0x03;	//fast PWM mode
	TCCRB(LEFT_MOTOR_TIMER)=0x01;	//no prescaler
}

void brake_left_motor()
{
	/*turn off output COMA and COMB */
	TCCRA(LEFT_MOTOR_TIMER)&=~((1<<COMxA1)|(1<<COMxA0)|(1<<COMxB1)|(1<<COMxB0));	
	/*set PB5 and PB6 to brake motor*/
	PORT(LEFT_MOTOR_A_PORT)|=(1<<LEFT_MOTOR_A_LOC);
	PORT(LEFT_MOTOR_B_PORT)|=(1<<LEFT_MOTOR_B_LOC);
}

void coast_left_motor()
{
	/*turn off output COMA and COMB */
	TCCRA(LEFT_MOTOR_TIMER)&=~((1<<COMxA1)|(1<<COMxA0)|(1<<COMxB1)|(1<<COMxB0));	
	/*clear PB5 and PB6 to brake motor*/
	PORT(LEFT_MOTOR_A_PORT)&=~(1<<LEFT_MOTOR_A_LOC);
	PORT(LEFT_MOTOR_B_PORT)&=~(1<<LEFT_MOTOR_B_LOC);
}

void engage_left_motor_forward()
{
	/*set motor as brake mode*/
	brake_left_motor();
	/*set on match mode, [COMB1:COMB0]-1:1  */
	TCCRA(LEFT_MOTOR_TIMER)|=((1<<COMxB1)|(1<<COMxB0));	
}

void engage_left_motor_backward()
{
	/*set motor as brake mode*/
	brake_left_motor();
	/*set on match mode, [COMA1:COMA0]-1:1  */
	TCCRA(LEFT_MOTOR_TIMER)|=((1<<COMxA1)|(1<<COMxA0));	
}

/*speed range from 0%:0x00 to 100%:0xff*/
void set_left_motor_duty_cycle(uint8_t duty_cycle_u8)
{
	/*set duty_cycle to OCRA and OCRB*/
	OCRA(LEFT_MOTOR_TIMER)=duty_cycle_u8;
	OCRB(LEFT_MOTOR_TIMER)=duty_cycle_u8;
}

uint8_t get_left_motor_duty_cycle()
{
	return OCRA(LEFT_MOTOR_TIMER);
}

void speedup_left_motor_forward(uint8_t duty_cycle_u8)
{	
	//startup from 0
	set_left_motor_duty_cycle(0x00);
	engage_left_motor_forward();
	while(get_left_motor_duty_cycle()<duty_cycle_u8)
	{
		set_left_motor_duty_cycle(get_left_motor_duty_cycle()+1);		
		_delay_ms(10);
	}
	
}

/********************** implementation for right motor **********************************/
void initialize_right_motor()
{
	/*default duty cycle as 50%*/
	set_right_motor_duty_cycle(0x80);
	/*set motor as brake mode*/
	brake_right_motor();	
	
	/*IO port direction initialization, PD3 and PB3 as output*/
	DDR(RIGHT_MOTOR_A_PORT)|=(1<<RIGHT_MOTOR_A_LOC);
	DDR(RIGHT_MOTOR_B_PORT)|=(1<<RIGHT_MOTOR_B_LOC);

	/*TCCR initialization,turn on TCN2*/
	TCCRA(RIGHT_MOTOR_TIMER)=0x03;	//fast PWM mode
	TCCRB(RIGHT_MOTOR_TIMER)=0x01;	//no prescaler
}

void brake_right_motor()
{
	/*turn off output COMA and COMB */
	TCCRA(RIGHT_MOTOR_TIMER)&=~((1<<COMxA1)|(1<<COMxA0)|(1<<COMxB1)|(1<<COMxB0));	
	/*set PD3 and PB3 to brake motor*/
	PORT(RIGHT_MOTOR_A_PORT)|=(1<<RIGHT_MOTOR_A_LOC);
	PORT(RIGHT_MOTOR_B_PORT)|=(1<<RIGHT_MOTOR_B_LOC);
}

void coast_right_motor()
{
	/*turn off output COMA and COMB */
	TCCRA(RIGHT_MOTOR_TIMER)&=~((1<<COMxA1)|(1<<COMxA0)|(1<<COMxB1)|(1<<COMxB0));	
	/*clear PD3 and PB3 to brake motor*/
	PORT(RIGHT_MOTOR_A_PORT)&=~(1<<RIGHT_MOTOR_A_LOC);
	PORT(RIGHT_MOTOR_B_PORT)&=~(1<<RIGHT_MOTOR_B_LOC);
}

void engage_right_motor_forward()
{
	/*set motor as brake mode*/
	brake_right_motor();
	/*set on match mode, [COMB1:COMB0]-1:1  */
	TCCRA(RIGHT_MOTOR_TIMER)|=((1<<COMxB1)|(1<<COMxB0));
	
}

void engage_right_motor_backward()
{
	/*set motor as brake mode*/
	brake_right_motor();
	/*set on match mode, [COMA1:COMA0]-1:1  */
	TCCRA(RIGHT_MOTOR_TIMER)|=((1<<COMxA1)|(1<<COMxA0));
	
}

/*speed range from 0%:0x00 to 100%:0xff*/
void set_right_motor_duty_cycle(uint8_t duty_cycle_u8)
{
	/*set duty_cycle to OCRA and OCRB*/
	OCRA(RIGHT_MOTOR_TIMER)=duty_cycle_u8;
	OCRB(RIGHT_MOTOR_TIMER)=duty_cycle_u8;
}

uint8_t get_right_motor_duty_cycle()
{
	return OCRA(RIGHT_MOTOR_TIMER);
}


void initialize_motor()
{
	initialize_left_motor();
	initialize_right_motor();
}

/* run motors by setting speed for left and right */
void activate_motors(int speed_left,int speed_right)
{
	set_left_motor_duty_cycle(abs(speed_left));	
	set_right_motor_duty_cycle(abs(speed_right));
	if(speed_left>=0)
	{
		engage_left_motor_forward();
	}
	else
	{
		engage_left_motor_backward();
		
	}
	if(speed_right>=0)
	{
		engage_right_motor_forward();		
	}
	else
	{
		engage_right_motor_backward();
	}
}

/* brake left and right motor*/
void brake_motors()
{
	brake_left_motor();
	brake_right_motor();
}


