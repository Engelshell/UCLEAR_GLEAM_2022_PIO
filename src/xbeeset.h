
#pragma once

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

//Xbee type but with useful callbacks, saves us from having to create 
//an event system ourselves, see below how events are subscribed and used.
XBeeWithCallbacks xbee;

//You can get the 64-bit MAC of an Xbee with the XCTU software
//Add an 0x infront of it, example: 1234 becomes 0x1234

//XBeeAddress64 MajorTomMAC = XBeeAddress64(0x0013A20041BE7661);
XBeeAddress64 MajorTomMAC = XBeeAddress64(0x0013A20041BE7B12);
//XBeeAddress64 GroundControlMAC = XBeeAddress64(0x0013A20041C0275F);
XBeeAddress64 GroundControlMAC = XBeeAddress64(0x0013A20041BE7661);
  
void zbReceive(ZBRxResponse& rx, uintptr_t) {
  Serial.println(F("zbReceive"));
}


void xbeeSetup() {

  //Start Xbee Serial
  //Arduino Mega Serial1 are pins 18 and 19.
  //Teensey 3.5 pins are 21(rx) and 5(tx), Serial2/3/4 have different pins and can also be used.
  //baud rate here must match what the xbee3 was programmed with with XCTU
  Serial1.begin(115200); 
  //nss.begin(9600);
  xbee.setSerial(Serial1);
  xbee.begin(Serial1);

  // If packet has an error probably
  xbee.onPacketError(printErrorCb, (uintptr_t)(Print*)&Serial);

  xbee.onTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);
  // Make sure that any errors are logged to Serial. 
  xbee.onZBTxStatusResponse(printErrorCb, (uintptr_t)(Print*)&Serial);
  // These are called when an actual packet received
  xbee.onZBRxResponse(zbReceive);
  // Print any unhandled response with proper formatting
  xbee.onOtherResponse(printResponseCb, (uintptr_t)(Print*)&Serial);

  //Test if Xbee3 Serial communication is working
  uint8_t value = 0;
  AtCommandRequest req((uint8_t*)"AO", &value, sizeof(value));
  req.setFrameId(xbee.getNextFrameId());
  // Send the command and wait up to 150ms for a response
  uint8_t status = xbee.sendAndWait(req, 150);
  if (status == 0) {
    Serial.println(F("Set AO=0"));
  } else {
    Serial.println(F("Failed to set AO (this is expected on series1)"));
  }
}
