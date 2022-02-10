#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <MsgPack.h> //msgpack library
#include <PacketSerial.h> //PacketSerial library

//COBS protocol, max of 1024 bytes in a single message
PacketSerial_<COBS, 0, 8096> packetSerial;


//Structure that defines a simple message
struct Message {
  MsgPack::str_t messageStr; //String
  MSGPACK_DEFINE(messageStr);
};
//Structure that holds all the data we will be sending in a packet
struct Data {
  MsgPack::str_t key{"m"};
  Message message;
  MSGPACK_DEFINE_MAP(key, message);
};


//setup msgpack unpacker object for messages we will be receiving
arduino::msgpack::Unpacker dataUnpacker = MsgPack::Unpacker();
//seetup msgpack packer object for messages we will be sending
arduino::msgpack::Packer dataPacker = MsgPack::Packer();


void onPacketReceived(const void* sender, const uint8_t* buffer, size_t size)
{
    if (sender == &packetSerial)
    {
        //print to show we have entered this function
        Serial.print("Mars: ");

        //clear previous data in unpacker
        dataUnpacker.clear();
        //feed whole packet into the unpacker 
        dataUnpacker.feed(buffer, size);
        //Create new data object to store the data
        Data newData;
        //deserialize the data to our data object
        dataUnpacker.deserialize(newData);

        //get Message object from Data
        Message receiveMessage = newData.message;
        //Now we can access the contents of Message
        //Copy contents of the message to another variable for fun
        String receivedMessage = newData.message.messageStr;

        Serial.println(receivedMessage);

        return;
    }
}

void setup()
{
    //USB serial
    Serial.begin(115200);
    //Serial.setTimeout(500);

    //Xbee Serial
    Serial1.begin(115200);
    packetSerial.setStream(&Serial1);
    packetSerial.setPacketHandler(&onPacketReceived);
}


static uint32_t loop_last_millis = 0;
static uint32_t sendDelay = 10; //in ms (milliseconds)
static String userInput = "";
void loop() {

    packetSerial.update();
    // Check for a receive buffer overflow.
    if (packetSerial.overflow())
    {
       Serial.print("Error: PacketSerial Overflow");
       //error?
    }

    //Grab user input from Serial Monitor if we have any
    if (Serial.available() > 0) {
      userInput = Serial.readStringUntil('\n', 1024);
      if(userInput.length() >= 1024) {
        Serial.println("Input too large, 1024 byte maximum");
        userInput = "";
      } else {
        Serial.print("Earth: ");
        Serial.println(userInput);
      }
    }

    //Do this instead of using delay()
    if(millis() - loop_last_millis > sendDelay) {

      //If we have a message being sent from Serial Monitor
      if(userInput.length() > 0) {
        String tuserInput = userInput;
        

        //new Data object we will put things into
        Data sendData;

        //Assign messageStr to the user input
        sendData.message.messageStr = tuserInput;
        dataPacker.clear();
        //serialize our Data object into msgpack format
        dataPacker.serialize(sendData);

        // Send the data
        packetSerial.send(dataPacker.data(), dataPacker.size());

        userInput = ""; //empty userInput
        loop_last_millis = millis();
      }
    }

}
