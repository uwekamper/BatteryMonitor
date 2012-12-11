#include "Common.h"

#include "Arduino.h"

void flushSerial(SoftwareSerial* mySerial) {
  while(mySerial->available()!=0) {
    
#ifdef DEBUG    
    Serial.write(mySerial->read());
#else
    mySerial->read();
#endif

  }
}

int scanUntil(SoftwareSerial* mySerial, char* searchTerm, int timeout) {
  int pos = 0; // position in searchTerm
  int len = strlen(searchTerm); // length of the searchTerm
  char current; // current character read from mySerial
  
  long startTime = millis();
  
  while (true) {
    if ( ((millis() - startTime) / 1000) >= timeout ) {
      Serial.println("Timeout!");
      return -1;
    }
    
    if (mySerial->available() != 0) {
      current = mySerial->read();
      
      // If the current character is equal to the character in searchTerm
      // advance to the next character.
      if (current == searchTerm[pos]) {
        pos++;
      }
      else {
        
        // if not, start from the beginning
        pos = 0;
      }
      
      // Return if the whole word was read.
      if (pos >= len) {
        return 1;
      }
      
    } // end if
  } // end while   
}
