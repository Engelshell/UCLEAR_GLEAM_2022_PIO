
#pragma once

void onPacketReceived(const void* sender, const uint8_t* buffer, size_t size)
{

   

};


void setupNetwork() {
    if(ENABLE_USB) {
        Serial.begin(USB_SERIAL_SPEED);
    }
    XBEE_SERIAL.begin(XBEE_SERIAL_SPEED);

    packetSerial.setStream(&XBEE_SERIAL);
    packetSerial.setPacketHandler(&onPacketReceived);

};


void networkPrintMessage(char * message) {

    if(ENABLE_USB) {
        Serial.println(message);
    } else {
        LogMessage message;
        message.message = "hi";
        logpack.clear();
        logpack.serialize(message);
        packetSerial.send(logpack.data(), logpack.size());
    }
}