#ifndef __COMMON_H__
#define __COMMON_H__

#include "SoftwareSerial.h"

#define DEBUG 1

/**
 * Removes any characters from the input buffer of the SoftwareSerial.
 */
void flushSerial(SoftwareSerial* mySerial);

/**
 * Throws away characters from mySerial until the searchTerm is found.
 */
int scanUntil(SoftwareSerial* mySerial, char* searchTerm, int timeout);

#endif
