#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <MsgPack.h>
#include <PacketSerial.h> //PacketSerial
#include <Printers.h> //arduino-xbee-master
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h> //imu sensor, get library in arduino software
#include <Adafruit_BMP085.h> //get library in arduino software
#include <Adafruit_Si7021.h>
#include <utility/imumaths.h> // needed by BNO055
#include "serialize.h" //our own serializer utility
#include "config.h"
#include "network.h"

#include "sensors.h"


void setup() {

  //Set up serial communications
  setupNetwork(); 
  
  sensorsSetup();
  
  //short delay just because
  delay(250);

  networkPrintMessage("SETUP STAGE 2: Sensor Setup Completed");

  setupSerializer();

}


int loop_last_millis_xbee = 0;
int loop_last_millis_sensors = 0;
void loop() {

  packetSerial.update();
  // Check for a receive buffer overflow.
  if (packetSerial.overflow())
  {
      // Send an alert via a pin (e.g. make an overflow LED) or return a
      // user-defined packet to the sender.
      //
      // Ultimately you may need to just increase your recieve buffer via the
      // template parameters.
  }

/*
  if(millis() - loop_last_millis_sensors > 100) {
    loop_last_millis_sensors = millis();
  
    LogMessage message;
    message.message = "hi";
    logpack.clear();
    logpack.serialize(message);
    packetSerial.send(logpack.data(), logpack.size());
  }*/

  
  if(millis() - loop_last_millis_sensors > XBEE_SEND_LOOP_DELAY) {
    loop_last_millis_sensors = millis();

    //update sensor data
    updateSensorData();
    //update time in sensors data
    sensors.time.millis = millis();
    SensorData data;
    data.sensors = sensors;
    //clear data from previous msgpack
    sensorpack.clear();
    //pack sensors data into msgpack format
    sensorpack.serialize(data);
    
    
    packetSerial.send(sensorpack.data(), sensorpack.size());
  }
  

}
