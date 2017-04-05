/*
************************ ADC_driver.c ****************************
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-12-5
   Function: Implementation of ADC Sensor driver
*/

#include "ADC_driver.h"

#define ANALOG_DDR			DDR(C)
#define ANALOG_PORT			PORT(C)
#define ANALOG_PING			PORT(C)

#define ADC_SENSOR_LEFTMOST_LOC 	0
#define ADC_SENSOR_LEFT_LOC 		1
#define ADC_SENSOR_MIDDLE_LOC 		2
#define ADC_SENSOR_RIGHT_LOC 		3
#define ADC_SENSOR_RIGHTMOST_LOC 	4
#define ADC_SENSOR_EN_LOC 		5

/************************** private global variable define ***************************/
uint8_t         _PortCMask;					
unsigned int    _maxValue;
unsigned int    _numSensors;
unsigned int    _adc_calibrated_Minimum[ADC_MAX_SENSORS]={0};
unsigned int    _adc_calibrated_Maximum[ADC_MAX_SENSORS]={0};
/************************************************************************************/

// Initialize the global value and setting.
// Example usage:
// initialize_ADC();
// ...
void initialize_ADC()
{
	uint8_t sensor_id=0;
	_numSensors=ADC_MAX_SENSORS;	
	//set bit mask for PC0~PC5
	_PortCMask|=((1<<ADC_SENSOR_LEFTMOST_LOC)|(1<<ADC_SENSOR_LEFT_LOC)|(1<<ADC_SENSOR_MIDDLE_LOC)| \
		    (1<<ADC_SENSOR_RIGHT_LOC)|(1<<ADC_SENSOR_RIGHTMOST_LOC)|(1<<ADC_SENSOR_EN_LOC));

	// set ADCSRA: 1)enable ADC; 2)prescale is 128 	
	ADCSRA=((1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0)); 
	// set ADMUX: 1)Ref is Vcc; 2)ADLAR=1 	 	
    	ADMUX=(1<<REFS0)|(1<<ADLAR); 

	//initialize calibration maximum and minimum global variable
	for(sensor_id=0;sensor_id<_numSensors;sensor_id++)
	{		
		// set maximum=0, updated by growing from 0 to larger value.
		_adc_calibrated_Maximum[sensor_id] = 0;	
		// set minimum=0, updated by growing from 0xffff to smaller value.	
		_adc_calibrated_Minimum[sensor_id] = 0xffff;					
	}	
}

// read ADC sensor value and saved in array sensor_values[5].
// Example usage:
// unsigned int sensor_values[5];
// read_ADC(sensor_values);
// ...
void read_ADC(unsigned int *sensor_values)
{
	// used foreach sensor data in array	
	uint8_t sensor_id=0;
	
	// before start conversion, wait for another conversion finish.
	while((ADCSRA & (1<<ADSC)));	

	// read sensor data and saved in sensor_values
	for (sensor_id = 0; sensor_id < _numSensors; sensor_id++)
	{
		// set sensor pins to outputs		
		ANALOG_DDR|=((1<<sensor_id)|1<<ADC_SENSOR_EN_LOC);	
		// drive high for 10 us to charge		
		ANALOG_PORT|=((1<<sensor_id)|1<<ADC_SENSOR_EN_LOC);
		_delay_us(10);

		// set ports 0 as inputs		
		ANALOG_DDR&=~((1<<sensor_id)|1<<ADC_SENSOR_EN_LOC);
		// turn off pull ups		
		ANALOG_PORT&=~((1<<sensor_id)|1<<ADC_SENSOR_EN_LOC);

		//Config ADMUX for sensor MUX.
		ADMUX=(ADMUX&0xf0)|sensor_id; 
		//start conversion
		ADCSRA|=(1<<ADSC);
	
		// polling for conversion finish.
		while((ADCSRA & (1<<ADSC)));
		// saved sensor data.
		sensor_values[sensor_id] = ADC;		
	}		
}

// Calibrate sensors to get actually Minimum and Maximum range list.
// Example usage:
// calibrate_ADC();
// ...
// Algorithm description:
// Maximum[sensor_id] = 0;  	Up search
// start from downbound-0, for each calibration update Maximum with min_value of sensor		
// Minimum[sensor_id] = 0xffff; Down search
// start from upbound-0xffff, for each calibration update maximum for each calibrate of sensor
void calibrate_ADC()
{
        unsigned int sensor_id=0;
        unsigned int j=0;
        unsigned int sensor_values[ADC_MAX_SENSORS];
	unsigned int max_sensor_values[ADC_MAX_SENSORS];
	unsigned int min_sensor_values[ADC_MAX_SENSORS];      
        
        //read sensor 10 times to get Maximum value range as calibration
	for(j=0;j<10;j++)
	{
		// read ADC sensor value
		read_ADC(sensor_values);

		// foreach sensor data to calculate current max and min value 
		for(sensor_id=0;sensor_id<_numSensors;sensor_id++)
		{
			// set the max sensor value we found this time
			if(j == 0 || max_sensor_values[sensor_id] < sensor_values[sensor_id])
			{
				max_sensor_values[sensor_id] = sensor_values[sensor_id];
			}
			// set the min sensor value we found this time
			if(j == 0 || min_sensor_values[sensor_id] > sensor_values[sensor_id])
			{
				min_sensor_values[sensor_id] = sensor_values[sensor_id];
			}
		}
	}
	
        // record calbarated value in global calibration values list:
        // calibrated Maximum and Minimum and saved in global value as actual work range of each sensor   
	for(sensor_id=0;sensor_id<_numSensors;sensor_id++)
	{
		//update maximum value
		if(min_sensor_values[sensor_id] > _adc_calibrated_Maximum[sensor_id])		
		{
			_adc_calibrated_Maximum[sensor_id] = min_sensor_values[sensor_id];
		}
		//update minmum value
		if(max_sensor_values[sensor_id] < _adc_calibrated_Minimum[sensor_id])
		{
			_adc_calibrated_Minimum[sensor_id] = max_sensor_values[sensor_id];
		}
					
	}	
}
// get private variable "_adc_calibrated_Minimum"
unsigned int* get_adc_calibratedMin()
{return _adc_calibrated_Minimum;}

// get private variable "_adc_calibrated_Maximum"
unsigned int* get_adc_calibratedMax()
{return _adc_calibrated_Maximum;}

// Reads the calibrated sensor values into an array, which will be used for pid control
// Example usage:
// unsigned int sensor_values[5];
// readCalibrated_ADC(sensor_values);
// ...
// The returned values are in range from 0 to QRT_CALIBRATED_SCALE
void readCalibrated_ADC(unsigned int *sensor_values)
{
	int sensor_id;

	// read the real values from sensors.
	read_ADC(sensor_values);

	// for each sensor, compute calibrated value for pid control.
	for(sensor_id=0;sensor_id<_numSensors;sensor_id++)
	{
		unsigned int calibratedmin,calibratedmax;
		unsigned int denominator;
		signed int calibrated_value = 0;

		// read the calibration data:calibrated_Minimum and calibrated_Maximum   
		calibratedmax = _adc_calibrated_Maximum[sensor_id];
		calibratedmin = _adc_calibrated_Minimum[sensor_id];

		// calculate calibrated range based on calibrated_Minimum and calibrated_Maximum
		denominator = calibratedmax - calibratedmin;

		// calcualte calibration value based on denominator-calibrated range
		if(denominator != 0)
		{
			calibrated_value = (((signed long)sensor_values[sensor_id]) - calibratedmin) * ADC_CALIBRATED_SCALE / denominator;
		}
		// handle exceptional case to avoid error.
		if(calibrated_value < 0)
		{
			calibrated_value = 0;                
		}
		else if(calibrated_value > ADC_CALIBRATED_SCALE)
		{
			calibrated_value = ADC_CALIBRATED_SCALE;                
		}

		sensor_values[sensor_id] = calibrated_value;
	}
}

// trace sensor position based on line.
// Example usage:
// unsigned int line_value;
// unsigned int sensor_values[5];
// line_value=traceLine_ADC(unsigned int *sensor_values);
// ...
// Description:
// 1)sensor_values: access calibrated sensors values
// 2)line_value: returns an estimated position of the robot with respect to a line,
// range from 0 to (ADC_MAX_SENSORS-1)*ADC_SENSOR_VALUE_WEIGHT
// 3)estimated line_value is calculated by using a weighted average of the sensor, 
// which indices multiplied by ADC_SENSOR_VALUE_WEIGHT, if we set weight 10, 
// line_value= 0 indicates line is directly below sensor 0 or beyong leftmost sensor
// line_value=10 indicates line is directly below sensor 1
// line_value=20 indicates line is directly below sensor 2
// line_value=30 indicates line is directly below sensor 3
// line_value=40 indicates line is directly below sensor 4 or beyong rightmost sensor
// Intermediate values indicate that the line is between two sensors.  
// The algorithm is:
// 
//    	       total=0*value0 + 10*value1 + 20*value2 + 30*value3 + 40*value4
// line_value=------------------------------------------------------------------
//               sum=value0  +  value1  +  value2 + value3 + value4
//
unsigned int traceLine_ADC(unsigned int *sensor_values)
{
	// used for index of sensor id
	int sensor_id;

	// used for calculating the weighted total, which is long before division
	unsigned long total; 
	// used for estimating sum value of all sensors data
	// this is also used for the denominator which is <= 5000(1000*5)	                    
	unsigned int sum; 

	// initially that the line is leftmost, bellow sensor 0.
	unsigned int line_value=0; 
	
	//read calculated value.
	readCalibrated_ADC(sensor_values);
	
	//initialize local variable
	total = 0;
	sum = 0;
  
	// foreach sensor data to calculate line position
	for(sensor_id=0;sensor_id<_numSensors;sensor_id++) 
	{
		unsigned int tmpvalue = sensor_values[sensor_id];
		
		// only total in values that are above a noise threshold - 5%
		if(tmpvalue > (ADC_CALIBRATED_SCALE/LINE_NOISE_THRESHOLD)) 
		{
			total += (long)(tmpvalue) * (sensor_id * ADC_SENSOR_VALUE_WEIGHT);
			sum += tmpvalue;
		}
	}

	// exception handle to avoid error.
	if(sum>0)
	{
		line_value = total/sum;
	}
	else
	{
		line_value=0;
	}

	return line_value;
}
