#include "HttpRequest.h"

#include "Arduino.h"
#include "string.h"
#include "Common.h"
#include "SwitchPhone.h"

int submitHttpRequest(SoftwareSerial* mySerial, char* apn, char* username, char* password, char* server, int port, char* path) {
  char atcmd[128]; // used to concatenate more complex AT commands.
  
  switchOn(mySerial);
  
  Serial.println("Waiting 20 s for network to come up.");
  delay(18000);
  
  Serial.println(">AT+CSQ");
  mySerial->println("AT+CSQ");
  delay(100);
 
  flushSerial(mySerial);// this code is to show the data from gprs shield, in order to easily see the process of how the gprs shield submit a http request, and the following is for this purpose too.
 
  Serial.println(">AT+CGATT?");
  mySerial->println("AT+CGATT?");
  delay(100);
 
  flushSerial(mySerial);
 
  Serial.println(">AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");
  mySerial->println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR, the connection type is using gprs
  delay(1000);
 
  flushSerial(mySerial);
  
  strcpy(atcmd, "");
  strcpy(atcmd, "AT+SAPBR=3,1,\"APN\",\"");
  strcat(atcmd, apn);
  strcat(atcmd, "\"");
  
  Serial.print(">");
  Serial.println(atcmd);

  if(strcmp(username, "") != 0) {  
    strcpy(atcmd, "");
    strcpy(atcmd, "AT+SAPBR=3,1,\"USER\",\"");
    strcat(atcmd, username);
    strcat(atcmd, "\"");
  
    Serial.print(">");
    Serial.println(atcmd);
  }
  
  if(strcmp(password, "") != 0) {  
    strcpy(atcmd, "");
    strcpy(atcmd, "AT+SAPBR=3,1,\"PWD\",\"");
    strcat(atcmd, password);
    strcat(atcmd, "\"");
    
    Serial.print(">");
    Serial.println(atcmd);
  }
  
  mySerial->println(atcmd);//setting the APN, the second need you fill in your local apn server
  delay(4000);
 
  flushSerial(mySerial);
  Serial.println(">AT+SAPBR=1,1");
  mySerial->println("AT+SAPBR=1,1");//setting the SAPBR, for detail you can refer to the AT command mamual
  delay(6000);
 
  flushSerial(mySerial);
  Serial.println(">AT+HTTPINIT");
  mySerial->println("AT+HTTPINIT"); //init the HTTP request
 
  delay(2000); 
  flushSerial(mySerial);
 
  strcpy(atcmd, "");
  strcpy(atcmd, "AT+HTTPPARA=\"URL\",\"");
  strcat(atcmd, server);
  strcat(atcmd, ":");
  
  char port_str[6] = "     ";  //reserve the string space first
  itoa(port, port_str, 10);
  strcat(atcmd, port_str);
  
  strcat(atcmd, path);
  
  strcat(atcmd, "\"");
  
  Serial.print(">");
  Serial.println(atcmd);
  mySerial->println(atcmd);// setting the httppara, the second parameter is the website you want to access
  delay(1000);
 
  flushSerial(mySerial);
 
  Serial.println(">AT+HTTPACTION=0");//submit the request 
  mySerial->println("AT+HTTPACTION=0");//submit the request 
  delay(10000);//the delay is very important, the delay time is base on the return from the website, if the return datas are very large, the time required longer.
 
  int code = readHttpStatusCode(mySerial);
  Serial.print("Status code was: ");
  Serial.println(code);
  
  flushSerial(mySerial);
 
  Serial.println(">AT+HTTPREAD");// read the data from the website you access
  mySerial->println("AT+HTTPREAD");// read the data from the website you access
  delay(300);
 
  flushSerial(mySerial);
 
  delay(100);
  
  Serial.println(">AT+HTTPTERM");
  mySerial->println("AT+HTTPTERM"); //init the HTTP request
  delay(300);
 
  flushSerial(mySerial);
  
  Serial.println("done sending ...");
  powerUpOrDown();
  
  return code;
}

int readHttpStatusCode(SoftwareSerial* mySerial) {
  char buffer[4] = "000"; // holds the status code.
  
  // wait for the response code of the AT+HTTPACTION command
  int worked = scanUntil(mySerial, "+HTTPACTION:0,", 20);
  if (worked < 0) {
    return 642;
  }
  
  // the next 3 characters are the status code.
  for (int i = 0; i < 3; i++) {
    if (mySerial->available() != 0) {
      buffer[i] = mySerial->read();
    }
  }
  
  return atoi(buffer);
}

