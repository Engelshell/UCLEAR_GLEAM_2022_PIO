#pragma once

bool hasSensorBNO055 = false;
bool hasSensorSI7021 = false;
bool hasSensorBMP085 = false;

Sensors sensors;

//BNO055
uint16_t BNO055_SAMPLERATE_DELAY_MS = 100;
//BNO055_ADDRESS_A (0x28)
//BNO055_ID (0xA0)

//Adafruit_BNO055 sensor_BNO055 = Adafruit_BNO055(-1, BNO055_ADDRESS_A, &Wire);
Adafruit_BNO055 sensor_BNO055 = Adafruit_BNO055(55, 0x28, &Wire);

//https://docs.kubos.com/1.0.0/kubos-core/sensors/bno055.html
Adafruit_BNO055::adafruit_bno055_opmode_t sensor_BNO055_mode = 
          Adafruit_BNO055::adafruit_bno055_opmode_t::OPERATION_MODE_CONFIG;

//BMP085_I2CADDR 0x77
Adafruit_BMP085 sensor_BMP085;
uint8_t sensor_BMP085_mode = BMP085_ULTRAHIGHRES;

Adafruit_Si7021 sensor_SI7021;



void sensorsSetup() {
  Wire.setSCL(19);
  Wire.setSDA(18);
  
  Wire1.setSCL(37);
  Wire1.setSDA(38);

  Wire2.setSCL(3);
  Wire2.setSDA(4);

  sensor_SI7021 = Adafruit_Si7021( &Wire1);

  //When deploying to a live environment it may be possible that a 
  //sensor is disconnected, But we can still allow the rest 
  //of the sensors to work
  //sensor_BNO055_mode
  if(sensor_BNO055.begin()) {
    hasSensorBNO055 = true;
  } else {
    networkPrintMessage("ERROR: Could not find BNO055 Sensor.");
  }

  if(sensor_SI7021.begin()) {
    hasSensorSI7021 = true;
  } else {
    networkPrintMessage("ERROR: Could not find SI7021 Sensor.");
  }

  if(sensor_BMP085.begin(sensor_BMP085_mode, &Wire2)) {
    hasSensorBMP085 = true;
  } else {
    networkPrintMessage("ERROR: Could not find BMP085 Sensor.");
  }

}

void updateSensorData() {
 


  if(hasSensorBNO055) {
    sensors.bno055.orientation = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_EULER));
    sensors.bno055.angvelocity = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE));
    sensors.bno055.linearAccel = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_LINEARACCEL));
    sensors.bno055.magnetometer = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_MAGNETOMETER));
    sensors.bno055.acceleration = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_ACCELEROMETER));
    sensors.bno055.gravity = Vec3(sensor_BNO055.getVector(Adafruit_BNO055::VECTOR_GRAVITY));
  }
  
  if(hasSensorSI7021) {

    sensors.si7021.humidity = sensor_SI7021.readHumidity();
    sensors.si7021.temperature = sensor_SI7021.readTemperature();

  }

  if(hasSensorBMP085) {

    sensors.bmp085.pressure = sensor_BMP085.readPressure();
    sensors.bmp085.altitude = sensor_BMP085.readAltitude();
  }
  


}

