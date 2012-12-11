#include "SwitchPhone.h"

#include "Arduino.h"
#include "Common.h"

void switchOn(SoftwareSerial* mySerial) {
  // try 3 times to switch on the phone.
  for(int i = 0; i < 3; i++) {
    Serial.print("Switching on phone, attempt No. ");
    Serial.println(i + 1);
    powerUpOrDown();
    // make sure, baudrate is set correctly
    Serial.println(">AT+IPR=19200");
    mySerial->println("AT+IPR=19200");
  
     
    flushSerial(mySerial);
    mySerial->println("AT");
    int scanned = scanUntil(mySerial, "OK", 10);
    flushSerial(mySerial);
    if (scanned > 0) {
      Serial.println("found OK in response to AT command");
      return;
    }
       
  }
}

void powerUpOrDown() {
  pinMode(9, OUTPUT); 
  digitalWrite(9,LOW);
  delay(1000);
  digitalWrite(9,HIGH);
  delay(2000);
  digitalWrite(9,LOW);
  delay(3000);
}
