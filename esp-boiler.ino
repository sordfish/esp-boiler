/*
* IRremote: IRsendDemo - demonstrates sending IR codes with IRsend
* An IR LED/radio transmitter must be connected to Arduino PWM pin 3.
* Version 0.1 July, 2009
* Copyright 2009 Ken Shirriff
* http://arcfn.com
*/
 
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
setup() {
    Serial.begin(9600);
}
 
void loop() {// altered the code just to send/test my raw code
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
}//close main loop
