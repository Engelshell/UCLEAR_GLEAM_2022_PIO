#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <string.h>
#include <MsgPack.h> //msgpack library
//The arduino-xbee-master library has a max packet sending of 100 Bytes.
#include <Printers.h> //arduino-xbee-master
#include <XBee.h>  //arduino-xbee-master

//Xbee object with built in event system
XBeeWithCallbacks xbee;

//You can get the 64-bit MAC of an Xbee with the XCTU software
//Add an 0x infront of it, example: 1234 becomes 0x1234
//SET YOUR OWN MAC ADDRESSES OR THIS WONT WORK! :)
//This is us
XBeeAddress64 EarthMAC = XBeeAddress64(0x0013A20041BE7B12);
//this is the receiver
XBeeAddress64 MarsMAC = XBeeAddress64(0x0013A20041BE7661);

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

void zbReceive(ZBRxResponse& rx, uintptr_t) {
  //print to show we have entered this function
  Serial.println("Earth zbReceive");

  //clear previous data in unpacker
  dataUnpacker.clear();
  //feed whole packet into the unpacker 
  dataUnpacker.feed(rx.getFrameData(), rx.getFrameDataLength());
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


void setup() {
  
  //USB serial
  Serial.begin(115200);
  Serial.setTimeout(50);

  //Start Xbee Serial
  //On Teensy 3.5 Serial1 is pins 0 and 1
  //baud rate here must match what the xbee3 was programmed with with XCTU
  Serial1.begin(115200); 
  xbee.setSerial(Serial1);
  xbee.begin(Serial1);

  //---Setup "callback" functions that the XBeeWithCallbacks event system will use
  // Called when a data packet received, we defined zbReceive function above
  xbee.onZBRxResponse(zbReceive);

  //---The rest of these are setup to print to serial, we did not impliment seperate functions for them.
  // If packet has an error
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&Serial);
  xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);
  // Make sure that any errors are logged to Serial. 
  xbee.onZBTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);
  // Print any unhandled response with proper formatting
  xbee.onOtherResponse(printResponseCb, (uintptr_t)(Print*)&Serial);

  //Test if Xbee3 Serial communication is working
  uint8_t value = 0;
  AtCommandRequest req((uint8_t*)"AO", &value, sizeof(value));
  req.setFrameId(xbee.getNextFrameId());
  // Send the command and wait up to 150ms for a response
  uint8_t status = xbee.sendAndWait(req, 150);
  if (status == 0) {
    Serial.println(F("Succcess Set AO=0"));
  } else {
    Serial.println(F("Failed to set AO, Cannot communicate with Xbee3."));
  }


}

uint32_t loop_last_millis = 0;
uint32_t sendDelay = 2000; //in ms (milliseconds)
String userInput = "";
void loop() {

    //loops through xbee events, should not block this or use delays
    xbee.loop();

    //Grab user input from Serial Monitor if we have any
    
    if (Serial.available() > 0) {
      userInput = Serial.readString();
      if(userInput.length() > 80) {
        Serial.println("Input too large, 80 byte maximum");
        userInput = "";
        //Sidenote the Xbee library can be modified to allow up to 240 bytes
        //via MAX_FRAME_DATA_SIZE but by default it's about 80 bytes (80+30=110)
      }
    }

    //Do this instead of using delay()
    if(millis() - loop_last_millis > sendDelay) {

      //If we have a message being sent from Serial Monitor
      if(userInput.length() > 0) {
        String tuserInput = userInput;
        userInput = ""; //empty userInput

        //new Data object we will put things into
        Data sendData;

        //Assign messageStr to the user input
        sendData.message.messageStr = tuserInput;
        //clear previously serialized data
        dataPacker.clear();
        //serialize our Data object into msgpack format
        dataPacker.serialize(sendData);

        //Create Zigbee Transmission Request object to send over Xbee
        ZBTxRequest zbtx = ZBTxRequest(EarthMAC, (uint8_t *) dataPacker.data(), dataPacker.size());

        //Send the Request through Xbee
        xbee.send(zbtx);

      }

      loop_last_millis = millis();
    }

}


