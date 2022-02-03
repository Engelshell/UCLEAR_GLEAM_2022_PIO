#pragma once


//object we pack our sensor data into for wireless sending
arduino::msgpack::Packer sensorpack;

void setupSerializer() {

    sensorpack = MsgPack::Packer();

}




//A vector of 3 doubles
struct Vec3 {
    Vec3(){};
    Vec3(double _x, double _y, double _z) {x=_x;y=_y;z=_z;};
    Vec3(imu::Vector<3> ivec3) {x=ivec3.x();y=ivec3.y();z=ivec3.z();};
    double x; //4
    double y; //4
    double z; //4
    MSGPACK_DEFINE(x, y, z);
};
//Hold data for BNO055 sensor
struct BNO055 {
    private:
        uint8_t key1{1}, key2{2}, key3{3}, key4{4}, key5{5}, key6{6}, key7{7}; //7
       // MsgPack::str_t key1{"1"}, key2{"2"}, key3{"3"},
        // key4{"4"}, key5{"5"}, key6{"6"}, key7{"7"};
    public:
        int8_t temperature; //1
        Vec3 acceleration; //12
        Vec3 orientation; //12
        Vec3 angvelocity; //12 
        Vec3 linearAccel; //12
        Vec3 magnetometer; //12
        Vec3 gravity; //12
        MSGPACK_DEFINE_MAP(
            key1, temperature,
            key2, acceleration, 
            key3, orientation,
            key4, angvelocity,
            key5, linearAccel,
            key6, magnetometer,
            key7, gravity
        );
}; //80
struct CCS811 {
    private:
      //MsgPack::str_t key1{"1"}, key2{"2"};
        uint8_t key1{1}, key2{2}; //2
    public:
        uint16_t co2; //2
        uint16_t ppm_tvoc; //2
        MSGPACK_DEFINE_MAP(
            key1, co2,
            key2, ppm_tvoc
        );
}; //8
struct BMP085 {
        private:
          //MsgPack::str_t key1{"1"}, key2{"2"};
            uint8_t key1{1}, key2{1}; //2
        public:
            int32_t pressure; //4
            float altitude; //4
            MSGPACK_DEFINE_MAP(
                key1, pressure,
                key2, altitude
            );
}; //10
struct Time {
    private:
    uint8_t m{1};
      //MsgPack::str_t m {"m"};
    public:
      uint32_t millis;
      MSGPACK_DEFINE_MAP(
          m, millis
      ) 
};
struct Sensors {
    private:
        uint8_t key1 {1}, key2 {2}, key3 {3}, key4 {4}, key5{5}; //4
        //MsgPack::str_t key1 {"s"}, key2{"1"}, key3{"2"}, key4{"3"}, key5{"t"};
    public:
        uint8_t signature = 0x97; //unique code identifying this data structure //1
        BNO055 bno055;  //80
        CCS811 ccs811; //8
        BMP085 bmp085; //10
        Time time;
        MSGPACK_DEFINE_MAP(
            key1, signature,
            key2, bno055, 
            key3, ccs811,
            key4, bmp085,
            key5, time
        );
}; //102



