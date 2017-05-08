/*
****************************************************************************************
 Assignment: SPIR Project
     Author: Xu Ronghua 		
       mail: rxu22@binghamton.edu	
Create date: 2017-03-25
   Function: Read sensor data through ADC.  

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


int main(void)
{

	// used for saving sensors data	
	//unsigned int g_sensors[8]={0};
		
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

		//Start single monitor as receiving cmd=0x01;
		if(0x00!=cmd_data){
			//_delay_ms(10);
			//continue;
			/*************Sensor Test: used foreach sensor data in array ******************/
			unsigned int sensor_data;

			//for(int i=0;i<8;i++){
			/********* Read sensor data  ****************/
			if(cmd_data>8)
				continue;
			int i=cmd_data;
			sensor_data=Sensor_SingleRead(i);
		
			/********* Send sensor data to PC vie RS232 ****************/
			USART0_Transmit(i);
			USART0_Transmit((sensor_data>>8)&0xff);
			USART0_Transmit(sensor_data&0xff);
			_delay_ms(10);
			//}
			//clear cmd_data
			cmd_data=0x00;
		}
	}
	return 0;
}
