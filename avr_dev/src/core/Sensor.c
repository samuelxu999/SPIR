/*
************************* Sensor.c **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-4 -30
	 Function: Implementation for Sensor related function
*/


#include "Sensor.h"


/***************** Initialize the Sensor low level driver  **************************
	Example usage:	Sensor_Init();
	Function: Call low level driver,scuh as SPI and ADC
*/
void Sensor_Init(){

	//initialize SPI as master mode
	SPI_MasterInit();
	
	//initialize ADC default setting
	ADC_Init();
}


/************** Read Sensor value from single selected channel **********************
	Example usage:	Sensor_SingleRead(ch_num)
	Input:
		@ch_id:		selected channel id
	Output:
		@sensor_data:	return sensor data as unsigned int
*/
unsigned int Sensor_SingleRead(unsigned char ch_id){

	unsigned int sensor_data = 0;
	//Verify channel id validation
	if(ch_id<SENSOR_MAX_CHAN){
		/********** Switch sensor channel ******************/
		SPI_SS_L();
		// WRITE command
		SPI_MasterTransmit(0x2a);
		// write channel number into slave
		SPI_MasterTransmit(ch_id);
		SPI_SS_H();

		/********** Read ADC data on pin ADC0 ******************/
		sensor_data=ADC_Read();
	}	
	return sensor_data;
}

