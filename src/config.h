
#pragma once

//Could be changed to other serials
#define XBEE_SERIAL Serial1
#define XBEE_SEND_LOOP_DELAY 100
#define ENABLE_USB 1
#define USB_SERIAL_SPEED 230400
#define XBEE_SERIAL_SPEED 230400

//HC-SR04
#define HCSR04_TRIGGER_PIN 10
#define HCSR04_ECHO_PIN 9



//KY
PacketSerial_<COBS, 0, 1024> packetSerial;


/* 

--Xbee3 Configuration
   See Rover_200D.xpro

--Teensy 3.5 Configuration
//https://www.pjrc.com/teensy/td_uart.html
Teensey 3.5 has 4 hardware serial ports, named the same as arduinos
Serial is used by USB, Serial1 in the code here is used by our Xbee3






*/