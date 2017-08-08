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

void Queuetest();

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
		USART0_sendbuffer();
	}
	return 0;
}
