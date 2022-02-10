#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <MsgPack.h>
#include <Printers.h> //arduino-xbee-master
#include <XBee.h>  //arduino-xbee-master
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h> //imu sensor, get library in arduino software
#include <Adafruit_BMP085.h> //get library in arduino software
#include <Adafruit_CCS811.h> //air quality sensor, library in arduino software
#include <utility/imumaths.h> // needed by BNO055
#include "serialize.h" //our own serializer utility
#include "sensors.h"
#include "xbeeset.h"

/* Xbee3 Configuration
   CE (Device Role) = Join Network[0]
   ID (Extended PAN ID) = 1337
   NJ (Node Join Time) = FF
   AP (API Enable) = API Mode With Escapes [2]
   BD (UART Baud Rate) = 115200
 
 Note that some code will use "AT" commands that can change these settings, causing
 the devices to not work correctly with each other.
*/

/* Teensy 3.5 Configuration
//https://www.pjrc.com/teensy/td_uart.html
Teensey 3.5 has 4 hardware serial ports, named the same as arduinos
Serial is used by USB, Serial1 in the code here is used by our Xbee3


*/



//Time to wait between xbee sending data
#define XBEE_SEND_LOOP_DELAY 100


unsigned long setupTime; //used for benchmarking performance

void setup() {
  //README: The pins for Serial1 and others can be different for different boards
  //Start USB serial

  Serial.begin(115200);
  Serial.readString();
  
  xbeeSetup();

  Serial.println("SETUP STAGE 2: Waiting for Xbee Initialization");
  delay(500); //wait for xbee to initialize 0.5 seconds

  sensorsSetup();

  Serial.println(F("SETUP STAGE 2: Sensor Setup Completed"));

  setupSerializer();

  setupTime = millis();
}




int loop_last_millis_xbee = 0;
int loop_last_millis_sensors = 0;
int miaros = 0;
void loop() {
  

  xbee.loop();

  
  if(millis() - loop_last_millis_sensors > XBEE_SEND_LOOP_DELAY) {
    miaros = micros();
    //update sensor data
    doSensors();
    //update time in sensors data
    sensors.time.millis = millis();
  
    //clear data from previous msgpack
    sensorpack.clear();
    //pack sensors data into msgpack format
    sensorpack.serialize(sensors);
    
    //Send a maximum of 240 bytes with modified Xbee Library. Max of 80 to 100 bytes with stock arduino-xbee library.
    ZBTxRequest zbtx = ZBTxRequest(GroundControlMAC, (uint8_t *) sensorpack.data(), sensorpack.size());
    
    //send data through xbee
    xbee.send(zbtx);
    
    //Serial.println(micros() - miaros);
    loop_last_millis_sensors = millis();
  }


}
