/*
****************************************************************************************
 Assignment: SPIR Project
     Author: Xu Ronghua 		
       mail: rxu22@binghamton.edu	
Create date: 2017-03-25
   Function: Collect environmental data and transmit sensoring data to user application

*****************************************************************************************/

#include "ADC_driver.h"
#include "USART_driver.h"
#include "SPI_driver.h"

#include "Sensor.h"


#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>


/***************************** initialize port *******************************/
void initialize_port()
{	
	cli();
	USART0_Init();
	Sensor_Init();
	sei();
}

/************** Sensor handler to collect and send sensor data ***************
Function: 1) Listen command from RS232 through USART
		  2) Read sensor data based on command.
		  3) Transmit data to user through RS232.
*/
void SensorHandler()
{
	
	//Start single monitor as receiving cmd=0x01;
	if(0x00!=cmd_data){		
		/*************Sensor Test: used foreach sensor data in array ******************/
		unsigned int sensor_data;
	
		/********* Read sensor data  ****************/
		if(cmd_data>SENSOR_MAX_CHAN)
			return;			
		
		//valid channel id is between 0 and SENSOR_MAX_CHAN-1
		int sensor_id=cmd_data;
		sensor_id--;
		
		//read sensor data based on sensor_id
		sensor_data=Sensor_SingleRead(sensor_id);
	
		/********* Send sensor data to PC vie RS232 ****************/
		USART0_Transmit(sensor_id);
		USART0_Transmit((sensor_data>>8)&0xff);
		USART0_Transmit(sensor_data&0xff);
		//_delay_ms(10);
		
		//clear cmd_data
		cmd_data=0x00;
	}
}

int main(void)
{		
	initialize_port();
	
	while(1){
		
		/******** Wait for data to be received by checking RXC0 in UCSRA *****/
		/*if(0x01==cmd_data){
			USART0_putstr("Cmd-1");
		}
		if(0x02==cmd_data){
			USART0_putstr("Cmd-2");
		}
		_delay_ms(1000);
		continue;*/

		SensorHandler();
	}
	return 0;
}
