#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <MsgPack.h>
#include <PacketSerial.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h> 
#include <Adafruit_BMP085.h> 
#include <Adafruit_Si7021.h>
#include <utility/imumaths.h> // needed by BNO055
#include "serialize.h"
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

  if (packetSerial.overflow())
  {
      //ignore
  }

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
