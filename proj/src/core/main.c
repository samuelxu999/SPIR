/***********************************************************************************************************
 Assignment: SPIR Project
     Author: Xu Ronghua 		
       mail: rxu22@binghamton.edu	
Create date: 2017-03-25
   Function: Read sensor data through ADC.  

************************************************************************************************************/

#include "menu_driver.h"
#include "ADC_driver.h"
#include "pushbutton_driver.h"
#include <util/delay.h>
#include <stdlib.h>


/***************************** initialize port *******************************/
void initialize_port()
{	
	initialize_pushbutton_array();		
	initialize_ADC();	
	initialize_menu();
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
			//_delay_ms(2000);                
		}
		if(right_button_is_pressed())
		{ 
			// Auto calibrate before test			
			wait_button_released(RIGHT_BUTTON_LOC); 
			//_delay_ms(1000);			 
	 			            
		}
		if(middle_button_is_pressed())
		{									
                        wait_button_released(MIDDLE_BUTTON_LOC);
			//_delay_ms(2000);	                        
		}
		
	}
	return 0;
}
