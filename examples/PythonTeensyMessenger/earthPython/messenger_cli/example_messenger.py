#!/usr/bin/python3
import time
import serial
import time
from typing import List
import os
import msgpack
from PacketSerial import PacketSerial


def messageCallback(message:bytes, size:int):
    m = msgpack.unpackb(message, use_list=False, raw=False)
    print("Mars: " + m[1])

def main():

    ports = PacketSerial.getSerialPorts()
    if(len(ports) < 1):
        print("No serial ports available")
        exit(0)
    #ser = serial.Serial('/dev/ttyACM0', baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=3)
    userSerial = serial.Serial()
    userSerial.baudrate = 115200
    userSerial.port = ports[0]
    packetSerial:PacketSerial = PacketSerial(ReceiveBufferSize=8096)
    packetSerial.setStream(userSerial)
    packetSerial.setPacketHandler(messageCallback)

    userSerial.open()

    run = True
    lastTime:float = 0
    while(run):
        try:
            packetSerial.update()
            if(packetSerial.overflow()):
                print("Error: Overflow")

            #if(time.monotonic() - lastTime > 1): 
            #    lastTime = time.monotonic()
            #    d = ["m": ["hi"]]
            #    p = msgpack.packb(d)
            #    packetSerial.send(p)
            
        except KeyboardInterrupt as err:
            print("caught keyboard ctrl-c:".format(err))
            print("exiting.")
            userSerial.close()
            exit(0)
        
    userSerial.close()
    return
    
if __name__ == "__main__":
    main()