#pragma once


//object we pack our sensor data into for wireless sending
arduino::msgpack::Packer sensorpack;
//object we pack our sensor data into for wireless sending
arduino::msgpack::Packer logpack;


void setupSerializer() {

    sensorpack = MsgPack::Packer();
    logpack = MsgPack::Packer();
    
}




//A vector of 3 floats
struct Vec3 {
    Vec3(){};
    Vec3(double _x, double _y, double _z) {x=_x;y=_y;z=_z;};
    Vec3(imu::Vector<3> ivec3) {x=ivec3.x();y=ivec3.y();z=ivec3.z();};
    float x;
    float y;
    float z;
    MSGPACK_DEFINE(x, y, z);
};

struct BNO055 {
    public:
        Vec3 acceleration; //Acceleration Vector (100Hz) Three axis of acceleration (gravity + linear motion) in m/s^2
        Vec3 orientation; //Absolute Orientation (Euler Vector, 100Hz) Three axis orientation data based on a 360° sphere
        Vec3 angvelocity; //Angular Velocity Vector (100Hz) Three axis of 'rotation speed' in rad/s
        Vec3 linearAccel; //Linear Acceleration Vector (100Hz) Three axis of linear acceleration data (acceleration minus gravity) in m/s^2
        Vec3 magnetometer; //Magnetic Field Strength Vector (20Hz) Three axis of magnetic field sensing in micro Tesla (uT)
        Vec3 gravity; //Gravity Vector (100Hz) Three axis of gravitational acceleration (minus any movement) in m/s^2
        MSGPACK_DEFINE(
            acceleration, 
            orientation,
            angvelocity,
            linearAccel,
            magnetometer,
            gravity
        );
};

struct SI7021 {
    public:
        float temperature;
        float humidity;
        MSGPACK_DEFINE(
         temperature,
         humidity
        );
};


struct BMP085 {
    public:
        int32_t pressure;
        float altitude;
        MSGPACK_DEFINE(
            pressure,
            altitude
        );
};

struct SensorsFront {
    public:
        float temperature;
        float magnetism;
        float distance;
    MSGPACK_DEFINE(
          temperature,
          magnetism,
          distance
      )
};

struct Time {
    public:
        uint32_t millis;
        MSGPACK_DEFINE(
            millis
        )
};

struct Sensors {
    public:
        BNO055 bno055;
        SI7021 si7021;
        BMP085 bmp085;
        SensorsFront sensorsfront;
        Time time;
        MSGPACK_DEFINE(
            bno055,
            si7021,
            bmp085,
            sensorsfront,
            time
        );
};



struct LogMessage {
private:
    uint8_t type {5};
public:
    MsgPack::str_t message{""};
    MSGPACK_DEFINE(type, message);
};


struct SensorData {
private:
    uint8_t type {4};
public:
    Sensors sensors;
    MSGPACK_DEFINE(type, sensors);
};