/*
************* menu_driver.c ×××××××××××××××××××××××
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-11-23
Change date: 2016-11-30
   Function: Implementation of menu driver
*/

#include "menu_driver.h"

#define MOD_MS			50
#define LCD_REFRESH_TIME	100	

volatile unsigned long tickCount = 0;	// incremented by 20 every T1 OCF (units of 20 us)
volatile unsigned long msCount = 0;	// incremented by 100 every tickCount (units of 1 ms)

/*timer counter initialize: TCCR setting, generate tick frequence*/
void initialize_menu()
{	
	TCCR1A |= 0x62;	// CTC-10, set on match A-11
	
	TCCR1B |= 0x01;	// timer1 ticks at 100KHz, 10us (prescaler = 1)

	OCR1A=9;	// OCRA = 9 (compare at counting 9)

	TIFR1=0;	//clear flag

	TIMSK1=0;	//clear flag
		
	sei();		// enable global interrupts

	//initialize LCD driver
	initialize_LCD_driver();
	//initialize ADC driver
	initialize_ADC();
	// turn on LCD
	LCD_execute_command(TURN_ON_DISPLAY);

	TIFR1 |= (1 << TIMER_EN);	// enable timer1 interrupt
	TIMSK1 |= (1 << TIMER_IS_EN);	// set timer1 interrupt flag	
}

/********** Used for refresh menu on lcd ************/
void refresh_lcd()
{
    	//calculate ms
	tickCount+=1;
	if(MOD_MS==tickCount) 
	{
		msCount++;
		tickCount=0;		
	}
	//calculate LCD refresh rate-100ms	
	if(LCD_REFRESH_TIME==msCount)
	{
		msCount=0;
		lcd_display();
	}
}

/*Timer1 COMA vect function: calculate timer tick count
 lcd refresh rate is 100*/
ISR(TIMER1_COMPA_vect) {
	refresh_lcd();
}

/********* Used for display menu information on lcd ***************/
void lcd_display()
{	
	//turn on LCD after startup
	LCD_execute_command(CLEAR_DISPLAY);		
	
	unsigned int line=0;
	// used for saving sensors data
	unsigned int g_sensors[5]={0};	
	
	// read sensor data and line position.
	line=traceLine_ADC(g_sensors);

	// display middle sensor data.
	LCD_move_cursor_to_col_row(4, 2);
	LCD_print_hex16(g_sensors[2]);
	
	// display line position.
	LCD_move_cursor_to_col_row(0, 2);
	LCD_print_String("L");
	LCD_move_cursor_to_col_row(1, 2);
	LCD_print_hex8(line);

	// display menu of "acquire line" for left button.
	LCD_move_cursor_to_col_row(0, 1);
	LCD_print_String("AL");

	// display menu of "PID" for middle button.
	LCD_move_cursor_to_col_row(3, 1);
	LCD_print_String("PID");

	// display menu of "Calibration" for right button.
	LCD_move_cursor_to_col_row(7, 1);
	LCD_print_String("C");	
}

