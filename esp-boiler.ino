#include <IRsend.h>
#include <IRremoteESP8266.h>

#include <ESP8266WiFi.h>          //ESP8266 Core WiFi Library (you most likely already have this in your sketch)

#include <DNSServer.h>            //Local DNS Server used for redirecting all requests to the configuration portal
#include <ESP8266WebServer.h>     //Local WebServer used to serve the configuration portal
#include <WiFiManager.h>     
 
IRsend irsend(7);
 
// just added my own array for the raw signal
uint16_t digitalOne[4] = {0,900,325,0};
uint16_t radio_preamble_a[3] = {4500,900,0};
uint16_t radio_preamble_b[3] = {4500,0,0};
uint16_t digitalZero[4] = {0,300,305,0};

//off
const unsigned int boiler_on_code[33]      = {1,0,0,0,1,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1};
const unsigned int boiler_on_inv_code[33]  = {1,1,1,1,0,1,1,1,0,1,0,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,0};

const unsigned int boiler_off_code[33]     = {1,0,0,0,1,0,0,0,1,1,1,0,0,1,0,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0};
const unsigned int boiler_off_inv_code[33] = {1,1,1,1,0,1,1,1,0,0,0,1,1,0,1,0,1,1,1,1,1,1,1,1,0,1,1,1,1,1,1,1,1};

ESP8266WebServer server(80);

void boilerOn(){

  unsigned int pulse_count=0;

  for (pulse_count=0;pulse_count < 4;pulse_count++) {
    int bit_count =0;
    irsend.sendRaw(radio_preamble_b,3,40);
    for (bit_count=0;bit_count < 33;bit_count++) {
      if (boiler_on_code[bit_count] == 1) {
        irsend.sendRaw(digitalOne,4,40);
      } else {
        irsend.sendRaw(digitalZero,4,40);
      }
    }

    delay(28);//Big delay between two pulses.
    irsend.sendRaw(radio_preamble_a,3,40);
    for (bit_count=0;bit_count < 33;bit_count++) {
      if (boiler_on_inv_code[bit_count] == 1) {
        irsend.sendRaw(digitalOne,4,40);
      } else {
        irsend.sendRaw(digitalZero,4,40);
      }
    }
    delay(15);
  }
}

void boilerOff(){
  unsigned int pulse_count=0;

  for (pulse_count=0;pulse_count < 4;pulse_count++) {
    int bit_count =0;
    irsend.sendRaw(radio_preamble_b,3,40);
    for (bit_count=0;bit_count < 33;bit_count++) {
      if (boiler_off_code[bit_count] == 1) {
        irsend.sendRaw(digitalOne,4,40);
      } else {
        irsend.sendRaw(digitalZero,4,40);
      }
    }

    delay(28);//Big delay between two pulses.
    irsend.sendRaw(radio_preamble_a,3,40);
    for (bit_count=0;bit_count < 33;bit_count++) {
      if (boiler_off_inv_code[bit_count] == 1) {
        irsend.sendRaw(digitalOne,4,40);
      } else {
        irsend.sendRaw(digitalZero,4,40);
      }
    }
    delay(15);
  }  
}


void setup() {
  WiFiManager wifiManager;
  wifiManager.autoConnect("ESP-BOILER");

  //ESP8266WebServer server(80);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    //Serial.print(".");
  }

  irsend.begin();
  
  server.on("/", [](){
    server.send(200, "text/plain", "root");
  });

  server.on("/on", [](){
    boilerOn();
    server.send(200, "text/plain", "on signal sent");
  });
  
  server.on("/off", [](){
    boilerOff();
    server.send(200, "text/plain", "off signal sent");
  });

  server.begin();
  
}

void loop() {
  server.handleClient();
}
