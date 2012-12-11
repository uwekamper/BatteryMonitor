#ifndef __VOLTAGE_H__
#define __VOLTAGE_H__

#include "Arduino.h"

// Analog input pin that the voltage divider is attached to
const int analogInPin = A0;  

/**
 * Convert the reading of the ADC in to a voltage value in Volts.
 */
double convertVoltage(int value);

/**
 * Collect the voltage by sampling 'runs' times and then calculation the avg. 
 */
double sampleVoltage(int runs);

/**
 * Measures the voltage of the 12 V battery through an 4-to-1 voltage divider
 */
void measureVoltage(char* hostname, char* path_prefix, char* url);

#endif
