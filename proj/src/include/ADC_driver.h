/*
************************* ADC_driver.h **************************
     Author: Xu Ronghua 		Chen Jiayuan
       mail: rxu22@binghamton.edu	jchen360@binghamton.edu
Create date: 2016-12-5
   Function: Interface definition for ADC Sensor driver
*/

#ifndef ADC_DRIVER_H
#define ADC_DRIVER_H

#include <avr/io.h>
#include "port_macros.h"
#include <util/delay.h>

#define ADC_MAX_SENSORS 		5
#define ADC_CALIBRATED_SCALE		1000
#define ADC_SENSOR_VALUE_WEIGHT		10
#define LINE_NOISE_THRESHOLD		20

/*****declear ADC driver interface******/

void initialize_ADC();

void read_ADC(unsigned int *sensor_values);

void readCalibrated_ADC(unsigned int *sensor_values);

void calibrate_ADC();

unsigned int* get_adc_calibratedMin();

unsigned int* get_adc_calibratedMax();

unsigned int traceLine_ADC(unsigned int *sensor_values);

#endif
