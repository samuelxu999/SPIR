/*
************************* Sensor.h **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-4 -30
	 Function: Interface definition for Sensor related function
*/

#ifndef SENSOR_H
#define SENSOR_H

#include <avr/io.h>
#include "ADC_driver.h"
#include "SPI_driver.h"

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


#endif

