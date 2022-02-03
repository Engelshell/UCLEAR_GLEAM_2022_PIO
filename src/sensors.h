#pragma once

bool hasSensorBNO055 = false;
bool hasSensorCCS811 = false;
bool hasSensorBMP085 = false;

Sensors sensors;

//BNO055
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
//BNO055_ADDRESS_A (0x28)
//BNO055_ID (0xA0)
Adafruit_BNO055 sensor_BNO055 = Adafruit_BNO055();
//https://docs.kubos.com/1.0.0/kubos-core/sensors/bno055.html
//TODO: see if we need to change operating modes, currently default
Adafruit_BNO055::adafruit_bno055_opmode_t sensor_BNO055_mode = 
          Adafruit_BNO055::adafruit_bno055_opmode_t::OPERATION_MODE_CONFIG;

//CCS811_HW_ID_CODE 0x81
//CCS811_ADDRESS (0x5A)
Adafruit_CCS811 sensor_CCS811 = Adafruit_CCS811();

//BMP085_I2CADDR 0x77
Adafruit_BMP085 sensor_BMP085 = Adafruit_BMP085();
uint8_t sensor_BMP085_mode = BMP085_ULTRAHIGHRES;

//no includes, this file can be imagined to be a continuation of main.ino

void sensorsSetup() {

  //When deploying to a live environment it may be possible that a sensor is disconnected,
  //But we can still allow the rest of the sensors to work
  if(sensor_BNO055.begin(sensor_BNO055_mode)) {
    hasSensorBNO055 = true;
  } else {
    Serial.println("ERROR: Could not find BNO055 Sensor.");
  }

  if(sensor_CCS811.begin()) { 
    hasSensorCCS811 = true;  
  } else {
    Serial.println("ERROR: Could not find CCS811 Sensor."); 
  }

  if(sensor_BMP085.begin(sensor_BMP085_mode)) {
    hasSensorBMP085 = true;  
  } else {
    Serial.println("ERROR: Could not find BMP085 Sensor.");
  }


  //hasSensorBNO055 = false;
  Serial.println(F("SETUP STAGE 2: Sensor Setup Completed"));
}



void doSensors() {
 

  if(hasSensorBNO055) {
      //sensors_event_t orientationData , angVelocityData , linearAccelData, magnetometerData, accelerometerData, gravityData;

      sensors.bno055.orientation = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_EULER));
      sensors.bno055.angvelocity = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE));
      sensors.bno055.linearAccel = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL));
      sensors.bno055.magnetometer = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER));
      sensors.bno055.acceleration = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER));
      sensors.bno055.gravity = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_GRAVITY));

      //Get temperature
      sensors.bno055.temperature = sensor_BNO055.getTemp();
  
  }
  
  
  if(hasSensorCCS811) {
      //Give sensor temp and humidity data from another sensor
      if(hasSensorBNO055) {
        //TODO: Get humidity, change 0
        //sensor_CCS811.setEnvironmentalData(0, (float)sensor_BNO055.getTemp());
      }
      //if sensor has data
      if(sensor_CCS811.available()) {
        //read data
        if(!sensor_CCS811.readData()) {
          sensors.ccs811.co2 = sensor_CCS811.geteCO2();
          sensors.ccs811.ppm_tvoc = sensor_CCS811.getTVOC();
        } else {
          //error, turn off reading from sensor
          hasSensorCCS811 = false;
          Serial.println("ERROR: Sensor CCS811 readData returned false.");
        }
      }
  }

  if(hasSensorBMP085) {
    sensors.bmp085.pressure = sensor_BMP085.readPressure();
    sensors.bmp085.altitude = sensor_BMP085.readAltitude();
  }
  

  
}

