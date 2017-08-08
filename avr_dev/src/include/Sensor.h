/*
************************* Sensor.h **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-04-30
	 Function: Interface definition for Sensor related function
*/

#ifndef SENSOR_H
#define SENSOR_H

#include <avr/io.h>
#include "ADC_driver.h"
#include "SPI_driver.h"
#include "USART_driver.h"


#define SENSOR_MAX_CHAN	10


/***************** Initialize the Sensor low level driver  **************************
	Example usage:	Sensor_Init();
	Function: Call low level driver,scuh as SPI and ADC
*/
void Sensor_Init();

/************** Read Sensor value from single selected channel **********************
	Example usage:	Sensor_SingleRead(ch_num)
	Input:
		@ch_id:		selected channel id

	Output:
		@sensor_data:	return sensor data as unsigned int
*/
unsigned int Sensor_SingleRead(unsigned char sensor_id);

/******** paser command packet from rx_USART buffer and retrun sensor value **********************
	Example usage:	SensorHandler()
	Function: 	1) Listen command from rx_USART buffer
				2) parse command to get operation information
		  		3) Read sensor data based on pasered sensor id.
		  		4) Queue data to tx_USART buffer that will be sent by USART0_sendbuffer()
*/
void SensorHandler();



#endif

