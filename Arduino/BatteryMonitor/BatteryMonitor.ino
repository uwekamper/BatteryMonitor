/****************************************************************
 * $Id: BatteryMonitor.ino 935 2012-12-04 11:14:30Z uk $
 *
 * Battery Monitor for ITfM GmbH
 * using an Arduino + Seeedstudio GSM/GPRS Shield
 *
 * Copyright (c) 2012, Uwe Kamper, All rights reserved
 ****************************************************************/

#include "stdio.h"
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "SoftwareSerial.h"

#include "Voltage.h"
#include "HttpRequest.h"

// variable to store the watchdog flag
volatile int wdt=0;

/****************************************************************
 ** Configuration
 ** Change the variables to your needs.
 ****************************************************************/ 

// APN name for GPRS (e.g. "web.vodafone.de" or "internet.t-d1.de")
#define APN "web.vodafone.de"

// username for GPRS (usually empty, "internet" for T-Mobile, Germany/D1)
#define USERNAME "" 

// password for GPRS (usually empty, "t-d1" for D1)
#define PASSWORD "" 

// hostname of the HTTP server where we send our data ...
#define SERVERNAME "myserver.example.com"

// ... and its port number (e.g. 5000 for testing or 80 for production).
#define PORT 80

// a path prefix, must be empty or must start with a slash. 
//Must never end with a slash.
#define PATH_PREFIX "/battery"

// nuber if sleep cycles between each send operation, each cycle is approx. 8 s
#define SLEEP_CYCLES 413

// The name of this host in the icinga configuration
#define HOSTNAME "my-nagios-hostname"

/****************************************************************
 ** End of Configuration
 ** Do not change things below this line.
 ****************************************************************/ 


int sensorValue = 0;        // value read from the pot
double voltage = 0.0;
char msg[128] = "battery/host/test/report_voltage/20.000"; // initialize string, will be overwritten later

char server_msg[50];
int numdata;
boolean started=false;

SoftwareSerial mySerial(7, 8);

void printConfig() {
  Serial.print("APN...............");
  Serial.println(APN);
  Serial.print("GPRS username.....");
  Serial.println(USERNAME);
  Serial.print("GPRS password.....");
  Serial.println(PASSWORD);
  Serial.print("Servername........");
  Serial.println(SERVERNAME);
  Serial.print("Port..............");

  // for some reason I have to do it this way or else the program crashes or freezes here
  int port = PORT    ;
  char port_str[5] = "    ";  //reserve the string space first
  itoa(port, port_str, 10); 
  Serial.println(port_str);
  Serial.print("Sleep........");
  // for some reason I have to do it this way or else the program crashes or freezes here
  int sleep = SLEEP_CYCLES;
  char sleep_str[5] = "    ";  //reserve the string space first
  itoa(sleep, sleep_str, 10); 
  Serial.println(sleep_str);
  delay(100);

  Serial.print("Nagios hostname...");
  Serial.println(HOSTNAME);

}

/**
 * Configure the watchdog timer and then present the config menu for 10 .
 */
void setup() {
  // disable the watchdog first in case something went wrong before. This is
  // because watchdog settings stay permanently.
  wdt_disable();

  mySerial.begin(19200);
  
  Serial.begin(19200);
  Serial.println("BatteryMonitor (c) by Uwe Kamper <me@uwekamper.de>");
  printConfig();
  Serial.println("Waiting 10 seconds ...");
  
  setup_watchdog();                        // set prescaller and enable interrupt
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);     // sleep mode is set here Power Down uses the least current
                                           // system clock is turned off, so millis won't be reliable!
  delay(10000);
  
  
}

void loop() {

  measureVoltage(HOSTNAME, PATH_PREFIX, msg);
  
  int i = 0;
  
  // try at least three times
  for (i = 0; i < 3; i++) {
    
    // if the request worked, stop trying.
    if (submitHttpRequest(&mySerial, APN, USERNAME, PASSWORD, SERVERNAME, PORT, msg) == 200) {
      break;
    }
  }

  delay(1000);
  
  // one request takes approx. 48 s, so the sleep time is decreased by 6
  // sleep cycles per request.
  wdt = (i + 1) * 6;
  system_sleep(); 

  delay(5000);
  
  // always reboot to avoid with hanging GPRS. I did not look into what the exact 
  // problem is, but GPRS sometimes hangs after many hours of operation. 
  // Rebooting then helps.
  reboot();
}

void system_sleep() {
  ADCSRA |= (0<<ADEN);                     // disable ADC
  sleep_enable();                          // enable sleeping
  sleep_mode();                            // activate system sleep
  // sleeping ..
  
  // first action after leaving WDT Interrupt Vector:
  if (wdt >= SLEEP_CYCLES) {               // sleep for this number times 8 seconds
    sleep_disable();                       // disable sleep  
    ADCSRA |= (1<<ADEN);                   // switch ADC on    
    wdt = 0;                               // reset watchdog counter  
  }
  else {
    system_sleep();                        // go back to sleep until it's time
  }
}


void setup_watchdog() {
  cli(); //disable global interrupts
  MCUSR = 0x00;  //clear all reset flags
  //set WD_ChangeEnable and WD_resetEnable to alter the register
  WDTCSR |= (1<<WDCE) | (1<<WDE);   // this is a timed sequence to protect WDTCSR
  // set new watchdog timeout value to 1024K cycles (~8.0 sec)
  WDTCSR = (1<<WDP3) | (1<<WDP0);
  //enable watchdog interrupt
  WDTCSR |= (1<<WDIE);    
  sei(); //enable global interrupts
}

// Watchdog Interrupt Service Routine.
// Very first thing after sleep wakes with WDT Interrupt
ISR(WDT_vect) {
  wdt++;  // increment the watchdog timer
}

void clearSerial() {
  char buffer[10];
  Serial.readBytesUntil('\n', buffer, 9);
  // Serial.println("<ign");
}

void reboot() {
  wdt_disable();  
  wdt_enable(WDTO_15MS);
  while (1) {}
}

