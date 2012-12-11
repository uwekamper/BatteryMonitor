#ifndef __HTTPREQUEST_H__
#define __HTTPREQUEST_H__

#include "SoftwareSerial.h"

// #define DEBUG 1

/**
 * Makes a HTTP GET request via GPRS.
 *
 * Returns the HTTP status code (e.g. 200 for success)
 */
int submitHttpRequest(SoftwareSerial* mySerial, char* apn, char* username, char* password, char* server, int port, char* path);

/**
 * Waits for the HTTP status code (e.g. '200' for success) after the request has 
 * been launched in SubmitHttpRequest(). 
 *
 * Returns the status code as an integer value.
 */
int readHttpStatusCode(SoftwareSerial* mySerial);

#endif
