#include "Voltage.h"
#include "Arduino.h"

double convertVoltage(int value) {
  // ATMega gives 0 for 0 V and 1024 for 5 V. The voltage divider has a ratio of 4.
  return value * (5.0/1024.0) * 4.0;
}

double sampleVoltage(int runs) {
  double retVal = 0;
  for(int i = 0; i < runs; i++) {
    int sensorValue = 0;
    sensorValue = analogRead(analogInPin);
    double voltage = convertVoltage(sensorValue); 
    retVal = retVal + voltage;
  }
  retVal = (retVal / runs) + 0.78; // to account for the 0.78 V drop at the diode
  return retVal;
}

void measureVoltage(char* hostname, char* path_prefix, char* url) {	
  char tmpstr[10];
  double voltage = sampleVoltage(1);
  strcpy(url, "");
  strcat(url, path_prefix);
  strcat(url, "/host/"); 
  strcat(url, hostname);
  strcat(url, "/report_voltage/");
  dtostrf(voltage, 3, 3, tmpstr); 
  strcat(url, tmpstr);
  
  // print the results to the serial monitor: 
  Serial.println(url); 
}
