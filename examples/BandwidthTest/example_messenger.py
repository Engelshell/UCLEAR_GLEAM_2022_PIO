#!/usr/bin/python3
from serial import Serial
import time
from typing import List
import os
import msgpack
from PacketSerial import PacketSerial

numbytes = 0
t1_begin = time.perf_counter()
firstMessage = False
lastnum = 0

def messageCallback(message:bytes, size:int):
    global numbytes
    global t1_begin
    global firstMessage
    global lastnum
    m = int.from_bytes(bytes=message, byteorder="little", signed=False)
    if(not firstMessage):
        firstMessage = True
        t1_begin = time.perf_counter()
        lastnum = m
    
    if(lastnum != m):
        print("skipped value")
        lastnum = m
        #exit(1)

    lastnum+=1
    numbytes+=size
    #print(m)
    print(numbytes / (time.perf_counter() - t1_begin))

def main():

    ports = PacketSerial.getSerialPorts()
    if(len(ports) < 1):
        print("No serial ports available")
        exit(0)
    #ser = serial.Serial('/dev/ttyACM0', baudrate=115200, bytesize=8, parity='N', stopbits=1, timeout=3)
    userSerial = Serial()
    userSerial.baudrate = 460800
    print(ports[0])
    userSerial.port = "COM11"
    packetSerial:PacketSerial = PacketSerial(ReceiveBufferSize=32)
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
            #    d = {"m": ["hi"]}
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