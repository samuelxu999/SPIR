/*
************************* ADC_driver.h **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-4 -25
	 Function: Interface definition for ADC Sensor driver
*/

#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <avr/io.h>
#include "port_macros.h"
#include <util/delay.h>


#define ADC_MAX_CHAN 			8
#define ADC_CALIBRATED_SCALE		1000


/***************** Initialize the global value and setting. **************************
	Example usage:	initialize_ADC(adc_max_semsor);
	Input:
	@adc_max_semsor: 	maximum sensor nodes to be monitored
*/
void ADC_Init();

/************** Read ADC value from channel ADC0 on chip **********************
	Example usage:	ADC_Read
	Output:
		@adc_value:	return ADC value as unsigned int
*/
unsigned int ADC_Read();

#endif
