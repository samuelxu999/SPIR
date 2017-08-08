/*
************************* Sensor.c **************************
	 Author: Xu Ronghua 
	 mail: rxu22@binghamton.edu
	 Create date: 2017-04-30
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

/******** paser command packet from rx_USART buffer and retrun sensor value **********************
	Example usage:	SensorHandler()
	Function: 	1) Listen command from rx_USART buffer
				2) parse command to get operation information
		  		3) Read sensor data based on pasered sensor id.
		  		4) Queue data to tx_USART buffer that will be sent by USART0_sendbuffer()
*/
void SensorHandler() {
		//define cmd_buff to save received command packet.
		char cmd_buff[4] = {0};

		//check wheter buffered packet data is available
		if(4==qStatus(&rx_USART)){
			uByte *pBuff=cmd_buff;
			uByte data;
			/* Then dequeue data until rx_USART empty */
			while (qStatus(&rx_USART)) 
			{
				qRead_byte(&rx_USART, &data);
				//save data to cmd_buff;
				*pBuff=data;
				pBuff++;
			}
			
			//refer to head
			pBuff=cmd_buff;
			//verify header of Req command packet
			/*+++++++++++ REQ packet format +++++++++++++++++
				Header | data: sensor id | padding
			  0x FF 01 |      XX         | FF 
			  FF: REQ  01:Read sensor
			+++++++++++++++++++++++++++++++++++++++++++++++*/
			if(*pBuff==0xff && *(pBuff+1)==0x01){
				//verify sensor id
				uByte sensor_id=*(pBuff+2);
				if(SENSOR_MAX_CHAN > sensor_id){
					unsigned int sensor_data;
					//read sensor data based on sensor_id
					sensor_data=Sensor_SingleRead(sensor_id);
					
					/********* Send sensor data to PC vie RS232 ****************/
					/*+++++++++++ ACK packet format +++++++++++++++++
					      Header | Error code | data  | padding
						0x FE FF |   XX XX    | XX XX | FF 
						FE: ACK  01:Read sensor
						FFFF: no error
						data field: XX XX
					  +++++++++++++++++++++++++++++++++++++++++++++*/
					//ack header with correct ret
					qWrite_byte(&tx_USART, 0xfe);
					qWrite_byte(&tx_USART, 0x01);
					qWrite_byte(&tx_USART, 0xff);
					qWrite_byte(&tx_USART, 0xff);
					//data field to pack sensor data: id+value
					qWrite_byte(&tx_USART, sensor_id);
					qWrite_byte(&tx_USART, (sensor_data>>8)&0xff);
					qWrite_byte(&tx_USART, sensor_data&0xff);
					//add padding
					//USART0_Transmit(0xff);
					qWrite_byte(&tx_USART, 0xff);
				}
				//return error code:0xff02  sensor id not vaild
				else{
					qWrite_byte(&tx_USART, 0xfe);
					qWrite_byte(&tx_USART, 0x01);
					qWrite_byte(&tx_USART, 0xff);
					qWrite_byte(&tx_USART, 0x02);
					qWrite_byte(&tx_USART, 0xff);
					qWrite_byte(&tx_USART, 0x00);
					qWrite_byte(&tx_USART, 0x00);
					qWrite_byte(&tx_USART, 0x00);
				}
				
			}
			//return error code:0xff01  not supported command
			else{
				qWrite_byte(&tx_USART, 0xfe);
				qWrite_byte(&tx_USART, 0x01);
				qWrite_byte(&tx_USART, 0xff);
				qWrite_byte(&tx_USART, 0x01);
				qWrite_byte(&tx_USART, 0xff);
				qWrite_byte(&tx_USART, 0x00);
				qWrite_byte(&tx_USART, 0x00);
				qWrite_byte(&tx_USART, 0x00);
			}
		}

}


