#!/usr/bin/python3
from PySide6.QtCore import Qt, Slot
from PySide6.QtWidgets import QApplication, QMainWindow
from PySide6.QtGui import QCloseEvent
import sys
from example_gui_messenger_ui import Ui_MainWindow
from PacketSerial import PacketSerial
from serial import Serial
import msgpack
from typing import List, Any

class MainWindow(QMainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)

        #Notice how we are missing a main loop in this program
        #This is normal for GUI applications, because in this case QT has an underlying
        #main loop that processes everything as events
        #One way we can add our own code to the event loop is by overriding this objects
        #timer with this statement. With a value of 0 every event loop will call the 
        #timerEvent of this object, that will cause the program to spike to very high
        #CPU Usage, and is unnecessary. We can instead just set it to 1.
        #Remember, this will not be totally accurate, PC's are not microcontrollers
        self.startTimer(1, Qt.TimerType.PreciseTimer)

        #Connect signal and slots so GUI interaction translates to our functions below
        #example: when serialOpenButton is clicked call self.serialOpenButton_clicked
        self.ui.serialOpenButton.clicked.connect(self.serialOpenButton_clicked)
        self.ui.serialCloseButton.clicked.connect(self.serialCloseButton_clicked)
        self.ui.serialCloseButton.setEnabled(False)
        self.ui.sendButton.clicked.connect(self.sendButton_clicked)
        #When using enter key on the text input act as if we clicked the send button
        self.ui.messageEdit.returnPressed.connect(self.ui.sendButton.click)

        #Add names of serial ports to combo box so user can select from multiple
        ports:List[str] = PacketSerial.getSerialPorts()
        self.ui.serialComboBox.addItems(ports)
        if(len(ports) < 1):
            self.ui.labelInfo.setText("No Serial ports. Plug in a USB to UART adapter.")

        baudRates:List[str] = ["9600", "19200", "38400", "57600", "115200", "230400", "460800", "921600"]
        self.ui.baudComboBox.addItems(baudRates)

        #Setup Serial and PacketSerial, we modify these when the user interacts with the application
        self.userSerial:Serial = Serial()
        self.packetSerial:PacketSerial = PacketSerial(ReceiveBufferSize=8096)
        self.packetSerial.setPacketHandler(self.serialMessageCallback)

    #Event Loop
    def timerEvent(self, event):
        #Have to call this to read bytes from serial
        self.packetSerial.update()
        if(self.packetSerial.overflow()):
            print("overflow")

    #When we receive a message from "Mars"
    def serialMessageCallback(self, message:str, size:int):
        #unpacker = msgpack.Unpacker(message, raw=False)
        #for unpacked in unpacker:
        #    print(unpacked)

        if(len(message) > 0):
            m = msgpack.unpackb(message, use_list=False, raw=False)
            localMessageC:str = "Mars: " + m["m"][0]
            self.ui.consoleEdit.appendPlainText(localMessageC)
            print(localMessageC)

    #Send a message to "Mars" from us, Earth
    def sendMessage(self, message:str):
        if(self.userSerial.isOpen() and len(message) > 0):
            #Using the same format we have in our arduino
            mesb = {"m": [message]}
            packedmessage = msgpack.packb(mesb)
            self.packetSerial.send(packedmessage)
            #The message we see
            localMessage:str = "Earth: " + message
            self.ui.consoleEdit.appendPlainText(localMessage)
            print(localMessage)

    @Slot()
    def sendButton_clicked(self):
        self.sendMessage(self.ui.messageEdit.text())

    @Slot()
    def serialOpenButton_clicked(self):
        port:str = self.ui.serialComboBox.currentText()
        baud:int = int(self.ui.baudComboBox.currentText())
        print("Port: " + port)
        print("Baudrate: " + str(baud))
        self.userSerial.baudrate = baud
        self.userSerial.port = port
        self.packetSerial.setStream(self.userSerial)
        self.userSerial.open()
        self.ui.serialCloseButton.setEnabled(True)
        self.ui.serialComboBox.setEnabled(False)
        self.ui.serialOpenButton.setEnabled(False)
        print("Serial Opened")

    @Slot()
    def serialCloseButton_clicked(self):
        self.userSerial.close()
        self.ui.serialCloseButton.setEnabled(False)
        self.ui.serialComboBox.setEnabled(True)
        self.ui.serialOpenButton.setEnabled(True)
        print("Serial Closed")

    def closeEvent(self, event: QCloseEvent):
        print("Application Closed")
        if(self.userSerial.isOpen()):
            self.userSerial.close()


def run():
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()

    sys.exit(app.exec())

if __name__ == '__main__':
    run()