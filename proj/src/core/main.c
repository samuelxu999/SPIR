/***********************************************************************************************************
 Assignment: Final Project
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-12-5
   Function: Follow line based on pid control.  

************************************************************************************************************/

#include "menu_driver.h"
#include "motor_driver.h"
#include "ADC_driver.h"
#include "pushbutton_driver.h"
#include <util/delay.h>
#include <stdlib.h>

#define MOTOR_SPEED_MAX		128
#define SENSORS_CHECK_TIME	20
#define CAPTURE_LINE_TIMEOUT	1000
#define BROKEN_LINE_TIMEOUT	500
#define INTEGRAL_THRESHOLD	2000

typedef enum {IDLE, AUTOCALIBRATE, CAPTURELINE, PID,TURNAROUND} strategy;

/*------------------------ Global variable defination ----------------------*/
// used for saving sensors data
unsigned int g_sensors[5]={0};
// used for saving sensors data
int g_last_proportional=0;
// used for saved accumulative difference for integral 
long g_integral=0;

// used for task scheduling, default as IDLE
strategy g_strategy = IDLE;

// used for control rotate direction: 0-anti-clockwise; 1-clockwise
unsigned int rotate_direction=0;

// used for maximun motor speed setting for capture line and pid task
int g_motor_speed=MOTOR_SPEED_MAX;

// used for enable turn around function
unsigned int en_turnaround=0;
/*--------------------------------------------------------------------------*/


/***************************** initialize port *******************************/
void initialize_port()
{	
	initialize_pushbutton_array();		
	initialize_ADC();
	initialize_motor();
	initialize_menu();
}

/***************************** auto_calibration ********************************
Function: Auto-calibration
1) spin clockwise and anticlockwize while calibrating the sensors.
2) records a set of sensor readings by executing calibrate_ADC()
3) keeps track of the minimum and maximum values 
*******************************************************************************/
void auto_calibration()
{
	// config motor speed=32
	int motor_speed=MOTOR_SPEED_MAX/4;
	// timer for calibration running
	unsigned int timer_calibrate; 

	// Since out counter runs from 0 to 80
	// the total delay will be 80*20 = 1600 ms		
	for(timer_calibrate=0;timer_calibrate<80;timer_calibrate++)
	{
		if(timer_calibrate < 20 || timer_calibrate >= 60)
		{
			activate_motors(motor_speed,-motor_speed);
		}
		else
		{
			activate_motors(-motor_speed,motor_speed);
		}
		// record calibrated data.  
		calibrate_ADC();

		// sensors data checking delay according to SENSORS_CHECK_TIME (ms)
		_delay_ms(SENSORS_CHECK_TIME);
	}
	// auto calibration finish, switch to IDLE task
	g_strategy = IDLE;
}

/***************************** followline_pid ********************************
Function: follow line running based on PID control
1) keeps track of line position
2) calculate control parameter: KP,KI,KD
3) steer motor direction to follow line
4) auto turn around as run into end of the line
*****************************************************************************/
void followline_pid()
{
	/****************** local variable define *******************/
	// config the max motor speed
	int max=g_motor_speed;

	// PID parameters:Kp-proportional
	// accounts for present values of the error
	unsigned int Kp=12;
	// PID parameters:Ki-integral
	// accounts for past values of the error
	unsigned int Ki=200;
	// PID parameters:Kd-derivative
	// accounts for possible future trends of the error
	float Kd=1.5;

	// used for monitoring proportional of line position 
	int proportional=0;
	// used for monitoring derivative of line position
	int derivative=0;
	// used for monitoring motor difference 
	int motor_difference=0;
	// used for monitoring actual line position
	unsigned int position=0;
	
	//calculate sum of all sensor to avoid noise
	int sum_sensor=0;

	while(1)
	{
		//read line position
		position = traceLine_ADC(g_sensors);

		/**************** turn around: 3pi turn around at the end of line *****************/		
		// if run to end of line, then switch to TURNAROUND to reverse direction     
		if((g_sensors[0]==0)&&(g_sensors[1]==0)&&(g_sensors[2]==0)&& \
		   (g_sensors[3]==0)&&(g_sensors[4]==0)&&((position==0)||(position==40)))

		{			
			// switch to TURNAROUND mode for reversing direction based on enable flag
			if(en_turnaround==1)			
			{
				g_strategy = TURNAROUND;
				if(position==0)
				{
					rotate_direction=0;
				}
				if(position==40)
				{			
					rotate_direction=1;
				}
				return;
			}			
		}		
		/***********************************************************************************/

		/************** preprocedure: check line boundary to run over broken area **********/
		sum_sensor=(g_sensors[0]+g_sensors[1]+g_sensors[2]+g_sensors[3]+g_sensors[4]);
		
		if(sum_sensor<=200)
		{
			// line is beyond the left most or right senor
			// running on broken area, keep current steering setting.
			_delay_ms(SENSORS_CHECK_TIME/20);
			continue;
		}		
		/**********************************************************************************/

		/************* PID procedure: calculate PID control parameter *********************/
		// When proportional=0, line is on the center, below middle sensor[2].
		// when proportional<0, line is on the left side of middle sensor[2]
		// when proportional>0, line is on the right side of middle sensor[2]
		proportional = ((int)position) - ((ADC_MAX_SENSORS-1)*ADC_SENSOR_VALUE_WEIGHT/2);

		// calculate the derivative (change) of the position.
		derivative = proportional - g_last_proportional;
		
		// limit integral error threshold to prevent Ki from becoming satiated.
		if((g_integral>=-INTEGRAL_THRESHOLD)&&(g_integral<=INTEGRAL_THRESHOLD))
		{
			// calcuate the integral (sum) of the position
			g_integral+= proportional;
		}	

		// Save the last position.
		g_last_proportional = proportional;

		// calculate the difference between the two motor speed settings,
		motor_difference = proportional*Kp + g_integral/Ki + derivative*Kd;		

		//set up boundary for speed difference
		if(motor_difference > max)
		{
			motor_difference = max;
		}
		//set down boundary for speed difference
		if(motor_difference < -max)
		{
			motor_difference = -max;
		}
		//if line position is left side, then steer to left
		if(motor_difference < 0)
		{
			activate_motors(max+motor_difference, max);		
		}
		//if line position is right side, then steer to right
		else
		{
			activate_motors(max, max-motor_difference);		
		}
		// sensors data checking delay according to SENSORS_CHECK_TIME(ms)
		_delay_ms(SENSORS_CHECK_TIME/20);
		/***************************************************************************/		
	}		
}

/********************************* idle **************************************
Function: stop and wait for command
*****************************************************************************/
void idle()
{
	brake_motors();
}

/****************************** capture_line ********************************
Function: move forward until capturing line.
1) move forward and keep track of sensors data
2) switch to pid mode as long as capturing line
*****************************************************************************/
void capture_line()
{
	// set motor speed based on global motor speed setting
	int motor_speed=g_motor_speed;
	// used for read position and sensors data	
	unsigned int position=0;
	// used for calculate sum of sensors data
	int sum_sensor=0;	
	// timer for catch line capture
	unsigned int capture_timeout=0;
	while(1)
	{
		// read sensor data
		position = traceLine_ADC(g_sensors);
		// calculate sum of sensors data
		sum_sensor=g_sensors[1]+g_sensors[2]+g_sensors[3];
		// if sum value of middle, left and right sensor is above threshold
		// 3pi find the line, then PID task will work
		if((sum_sensor>ADC_CALIBRATED_SCALE)&&(position==20))		
		{
			// find the line, switch to PID task		
			g_strategy = PID;
			return;
		}
		// activate motor to move forward
		activate_motors(motor_speed, motor_speed);
		// wait fixed time to move, then sensor check line
		_delay_ms(SENSORS_CHECK_TIME/20);

		// time out for capture line is 2s
		capture_timeout++;
		if(capture_timeout>CAPTURE_LINE_TIMEOUT)
		{
			// if capture line fail as timeout happen
			// switch to IDLE and wait for command.
			g_strategy = IDLE;
			return;
		}
	}
		
}

/****************************** turn_around **********************************
Function: turn around as approaching to end of line.
1) turn around and rotate to find line.
2) keeps track of sensors data
3) switch to PID mode as long as finding line.
*****************************************************************************/
void turn_around()
{
	//set motor speed=64
	int motor_speed=MOTOR_SPEED_MAX/2;
	//read position and sensors data	
	unsigned int position=0;
	
	while(1)
	{
		// read sensor data
		position = traceLine_ADC(g_sensors);
		// line is below middle sensor[2], switch to PID mode
		if((g_sensors[2]>=900)&&(position==20))
		{		
			g_strategy = PID;			
			return;
		}
		if(rotate_direction==0)
		{
			// if rotate_direction=0, anti-clockwise
			activate_motors(-motor_speed, +motor_speed);
		}
		if(rotate_direction==1)
		{
			//if rotate_direction=1, clockwise
			activate_motors(motor_speed, -motor_speed);
		}		
	}
}


int main()
{

	initialize_port();

	while(1)
	{
		
		/*************** Waits for a button press *****************/ 
		if(left_button_is_pressed())
		{         
			wait_button_released(LEFT_BUTTON_LOC);
			_delay_ms(2000);
			// Start up with capture line mode
			g_strategy = CAPTURELINE;
			// set motor speed=128 
			g_motor_speed=MOTOR_SPEED_MAX;
			// enable turn around function.
			en_turnaround=1;                    
		}
		if(right_button_is_pressed())
		{ 
			// Auto calibrate before test			
			wait_button_released(RIGHT_BUTTON_LOC); 
			_delay_ms(1000);			 
			g_strategy = AUTOCALIBRATE; 			            
		}
		if(middle_button_is_pressed())
		{									
                        wait_button_released(MIDDLE_BUTTON_LOC);
			_delay_ms(2000);
			// Start up with PID mode
			g_strategy = PID;
			// set motor speed=64 
			g_motor_speed=MOTOR_SPEED_MAX/2;
			// disable turn around function.
			en_turnaround=0;   	                        
		}

		/********** schedule task based on strategy ***********/
		switch(g_strategy) 
		{
			case IDLE:
			{
				idle();
				break;
			}
			case AUTOCALIBRATE:
			{
				auto_calibration();
				break; 
			} 
			case CAPTURELINE:
			{
				capture_line();
				break; 
			}
			case PID:
			{
				followline_pid();
				break; 
			}
			case TURNAROUND:
			{
				turn_around();
				break; 
			}
			default :
			{
				idle();
				break;
		   	}		
		}		
	}
	return 0;
}
