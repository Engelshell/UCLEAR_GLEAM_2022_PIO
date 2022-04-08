#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <stdint.h>
#include <string.h>
#include <MsgPack.h> //msgpack library
#include <PacketSerial.h> //PacketSerial library

PacketSerial_<COBS, 0, 8096> packetSerial;


void onPacketReceived(const void* sender, const uint8_t* buffer, size_t size)
{

}

void setup()
{
    //USB serial
    Serial.begin(115200);
    //Serial.setTimeout();

    //Xbee Serial
    Serial1.begin(460800);
    packetSerial.setStream(&Serial1);
    packetSerial.setPacketHandler(&onPacketReceived);
}

union count
{
  uint8_t counter8[8];
  uint64_t counter {0};
};

static uint32_t loop_last_micros = 0;
static String userInput = "";
static count counter1;
void loop() {
    
    packetSerial.update();
    // Check for a receive buffer overflow.
    if (packetSerial.overflow())
    {
      Serial.print("Error: PacketSerial Overflow");
       //error?
    }

    //Do this instead of using delay()
    if(micros() - loop_last_micros > 500) {

        
        //120 bytes
        
        // Send the data
        packetSerial.send(counter1.counter8, sizeof(counter1.counter8));
        counter1.counter++;
        
        loop_last_micros = micros();
      
    }

}
