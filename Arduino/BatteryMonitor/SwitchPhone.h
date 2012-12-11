#ifndef __SWITCHPHONE_H__
#define __SWITCHPHONE_H__

#include "SoftwareSerial.h"

/**
 * Switches the power-button and then makes sure that the phone is actually 
 * switched on 
 */
void switchOn(SoftwareSerial* mySerial);

/**
 * Simple subroutine that "presses" the button for a few seconds.
 */
void powerUpOrDown();

#endif __SWITCHPHONE_H__
