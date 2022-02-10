
#!/usr/bin/python3
from serial import Serial
import os
from typing import List, Callable, Any
from cobs import cobs # smart binary serial encoding and decoding

class PacketSerial(Serial):
    def __init__(self, EncoderType="COBS", PacketMarker=0, ReceiveBufferSize=256):
        self._stream:Serial = None
        self._onPacketFunction:Callable[[bytes, int], None] = None
        self._onPacketFunctionWithSender:Callable[[Any, bytes, int], None] = None
        self._receiveBuffer:bytes = bytearray(ReceiveBufferSize)
        self._receiveBufferIndex:int = 0
        self._receiveBufferOverflow:bool = False
        self._sender = None
        self._EncoderType = None
        if(EncoderType == "COBS"):
            self._EncoderType = cobs
        #no SLIP yet
        self._PacketMarker = PacketMarker
        self._ReceiveBufferSize = ReceiveBufferSize


    def setStream(self, stream:Serial):
        self._stream = stream

    def getStream(self)-> Serial:
        return self._stream

    def setPacketHandler(self, onPacketFunction, sender=None):
        if( sender == None ): 
            self._onPacketFunction = onPacketFunction
            self._onPacketFunctionWithSender = None
        else:
            self._onPacketFunction = None
            self._onPacketFunctionWithSender = onPacketFunction
            self._sender = sender
    
    def update(self):
        if (self._stream == None or not self._stream.isOpen()): return

        while(self._stream.inWaiting() > 0 and self._stream.isOpen()):
            data:bytes = self._stream.read()
            if(data[0] == self._PacketMarker):
                if(self._onPacketFunction or self._onPacketFunctionWithSender):

                    decodeBuffer:bytes = self._EncoderType.decode( self._receiveBuffer[0:self._receiveBufferIndex] )
                    numDecoded:int = len(decodeBuffer)
                
                    # clear the index here so that the callback function can call update() if needed and receive more data
                    self._receiveBufferIndex = 0
                    self._receiveBufferOverflow = False

                    if(self._onPacketFunction):
                        self._onPacketFunction(decodeBuffer, numDecoded)
                    elif(self._onPacketFunctionWithSender):
                        self._onPacketFunctionWithSender(self._sender, decodeBuffer, numDecoded)
                else:
                    self._receiveBufferIndex = 0
                    self._receiveBufferOverflow = False
            else:
                if((self._receiveBufferIndex+1) < self._ReceiveBufferSize):
                    #TODO: maybe fix
                    self._receiveBuffer[self._receiveBufferIndex] = data[0]
                    self._receiveBufferIndex+=1
                else:
                    self._receiveBufferOverflow = True

    def send(self, buffer:bytes) -> None:
    
        if(self._stream == None or buffer == None):
            return

        _encodeBuffer:bytearray = bytearray(self._EncoderType.encode(buffer))
        #numEncoded:int = len(_encodeBuffer)
        _encodeBuffer.append(0)
        sent:int = self._stream.write(_encodeBuffer)
        #print(sent)
        #self._stream.write(bytes(0))
        self._stream.flush()

    def overflow(self) -> bool:
        return self._receiveBufferOverflow
 
    @staticmethod
    def getSerialPorts() -> List[str]: 
        #from pyserial
        if os.name == 'nt': 
            from serial.tools.list_ports_windows import comports
        elif os.name == 'posix':
            from serial.tools.list_ports_posix import comports
        else:
            raise ImportError("Sorry: no implementation for your platform ('{}') available".format(os.name))

        comportsList:List[str] = []
        for (port, desc, hwid) in comports():
            comportsList.append(port)
        return comportsList