/*
****************************** ADC_driver.c *********************************
	Author: Xu Ronghua 
	mail: rxu22@binghamton.edu
	Create date: 2017-4 -25
	Function: Implementation of ADC Sensor driver
*/

#include "ADC_driver.h"

/**************** Define ADC related PORT register **********************/
#define ANALOG_DDR				DDR(F)
#define ANALOG_PORT			PORT(F)
#define ANALOG_PING			PORT(F)



/***************** Initialize the global value and setting. **************************
	Example usage:	initialize_ADC(adc_max_semsor);
	Input:
	@adc_max_semsor: 	maximum sensor nodes to be monitored
*/
void ADC_Init(){

	//Disable ADC before initialization
	ADCSRA=0x00;

	/*			 ADC Control and Status Register-ADCSRA 
		----------------------------------------------------
		| ADEN | ADSC | ADFR | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0 |
		----------------------------------------------------
	*/
	// set ADCSRA: 1)enable ADC; 2)prescale is 128 	
	ADCSRA=((1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0));

	/*			ADC Multiplexer Selection Register-ADMUX:
		-----------------------------------------------------
		| REFS1 | REFS0 | ADLAR | MUX4 | MUX3 | MUX2 | MUX1 | MUX0 |
		-----------------------------------------------------
	*/
	// set ADMUX: 1)Ref is Vcc; 2)ADLAR=0 ADC right Adjust Result 	 	
    ADMUX=(1<<REFS0); 
}

/************** Read ADC value from channel ADC0 on chip **********************
	Example usage:	ADC_Read
	Output:
		@adc_value:	return ADC value as unsigned int
*/
unsigned int ADC_Read(){	
	// before start conversion, wait for another conversion finish.
	while((ADCSRA & (1<<ADSC)));	


	// set ports 0 as inputs		
	ANALOG_DDR&=0xfe;
	// turn off pull ups		
	ANALOG_PORT&=0xfe;

	//Config ADMUX for sensor MUX.
	ADMUX&=0xf0; 

	//start conversion
	ADCSRA|=(1<<ADSC);

	// polling for conversion finish.
	while((ADCSRA & (1<<ADSC)));
	// saved sensor data.
	return ADC;		
		
}

