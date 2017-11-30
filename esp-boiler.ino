#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager WiFi Configuration Magic

#include <IRremote.h>
 
IRsend irsend;
 
// just added my own array for the raw signal
unsigned int digitalOne[4] = {0,820,370,0};
unsigned int radio_preamble_a[3] = {4800,820,0};
unsigned int radio_preamble_b[3] = {4800,0,0};
unsigned int digitalZero[4] = {0,200,370,0};
//off
unsigned int heating_code_first[33];
const unsigned int heating_code_second[33] =
{1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1};
String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is completevoid 


void setup() {
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP-BOILER");
}

void loop() {
   unsigned int input_num = inputString.toInt();
 inputString = "";
 stringComplete = false;
 unsigned int i=0;
 for (i=0;i < 33;i++) {
    heating_code_first[i] = heating_code_second[i];
 }
 if (input_num == 1) {//heating off
 if (heating_code_first[32] == 0) {
 heating_code_first[32] =1;
 } else {
 heating_code_first[32] =0;
 } 
  if (heating_code_first[9] == 0) {
  heating_code_first[9] =1;
  } else {
  heating_code_first[9] =0;
  }
 }
 unsigned int pulse_count=0;
 for (pulse_count=0;pulse_count < 4;pulse_count++) {
  int bit_count =0;
  irsend.sendRaw(radio_preamble_b,3,40);
  for (bit_count=0;bit_count < 33;bit_count++) {
   if (heating_code_first[bit_count] == 1) {
   irsend.sendRaw(digitalOne,4,40);
   } else {
   irsend.sendRaw(digitalZero,4,40);
  }
 }
 
  delay(28);//Big delay between two pulses.
  irsend.sendRaw(radio_preamble_a,3,40);
  for (bit_count=0;bit_count < 33;bit_count++) {
   if (heating_code_first[bit_count] == 0) {
   irsend.sendRaw(digitalOne,4,40);
   } else {
   irsend.sendRaw(digitalZero,4,40);
   }
  }
 delay(15);
 }
 }
 
//delay(2000);

}
