from PySide6.QtWidgets import *
import serial
import serial.tools
import serial.tools.list_ports

import time

class ControlPanelGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.port = serial.Serial(baudrate = 115200, bytesize = 8, timeout = 5)

        self.initGUI()

    def initGUI(self) -> None:
        layout = QVBoxLayout()

        #==================================
        # COM port picker
        #==================================
        com_port_widget = QWidget()
        com_port_container = QHBoxLayout()

        # List all com ports
        port_list_widget = QComboBox()
        com_ports = serial.tools.list_ports.comports()

        # Add filler option
        port_list_widget.addItem("<Pick COM port>")

        for port in com_ports:
            name: str = "{0} ({1})".format(port.manufacturer, port.name) 
            port_list_widget.addItem(name)
        
        # Connect button
        connect_port = QPushButton("Connect")
        connect_port.clicked.connect(lambda: self.connectionCOMPort(True, connect_port))

        # Connect widget
        port_list_widget.currentIndexChanged.connect(lambda: self.connectionCOMPort(False, connect_port))
        port_list_widget.currentIndexChanged.connect(lambda index: self.changeCOMPort(index))

        # Add widgets
        com_port_container.addWidget(port_list_widget)
        com_port_container.addWidget(connect_port)
        com_port_widget.setLayout(com_port_container)

        #==================================
        # Data send
        #==================================
        data_operations = QWidget()
        data_operations_container = QHBoxLayout()

        send_pb = QPushButton("Send")
        send_pb.clicked.connect(lambda: self.sendData(55))

        receive_pb = QPushButton("Receive")
        receive_pb.clicked.connect(lambda: self.receiveData())
        
        data_operations_container.addWidget(send_pb)
        data_operations_container.addWidget(receive_pb)
        data_operations.setLayout(data_operations_container)

        #==================================
        # Main layout
        #==================================
        layout.addWidget(com_port_widget)
        layout.addWidget(data_operations)
        self.setLayout(layout)

    def changeCOMPort(self, index: int) -> None:
        # If port was open - close it
        if self.port.is_open == True:
            self.port.close()
         
        if index == 0:
            # Return if picked port is filler option
            return

         # Get port info
        com_ports = serial.tools.list_ports.comports()
        self.port.port = com_ports[index - 1].name
        print(self.port)
    
    def connectionCOMPort(self, is_connected: bool, pb: QPushButton) -> None:
        if self.port.is_open == True and is_connected == False:
            # Disconnect routine
            self.port.close()

            # Pushbutton config
            pb.clicked.disconnect()
            pb.clicked.connect(lambda: self.connectionCOMPort(True, pb))
            pb.setText("Connect")
            print("Disconnected")

        if self.port.is_open == False and is_connected == True:
            # Connect routine
            self.port.open()

            # Pushbutton config
            pb.clicked.disconnect()
            pb.clicked.connect(lambda: self.connectionCOMPort(False, pb))
            pb.setText("Disconnect")
            print("Connected")

    def sendData(self, value):
        if self.port.is_open == False:
            return
        
        bytes_sent = 0
        tx_buffer = bytearray([0x01, 0x02, 0x00, 0x00, 0xAA, 0x0B, 0xBB, 0x32, 0x50])
        # First send the amount of data to receive
        bytes_sent += self.port.write(b'\x0A')
        # Then send remaining data 
        bytes_sent += self.port.write(tx_buffer)
        print("Send data: {0}".format(bytes_sent))
        
        # Wait for status msg
        bytes_received = 0
        upcoming = 0
        while self.port.in_waiting == 0:
            print("Waiting")

        raw_upcoming: bytes = self.port.read(1)
        upcoming = int.from_bytes(raw_upcoming) - 1
        print("Raw: {0}, Decoded: {1}".format(raw_upcoming, upcoming))
        data = self.port.read(upcoming)
        
        bytes_received = upcoming

        print("Transmitted: {0}, Received data: {1}".format(bytes_sent, data))

    def receiveData(self):
        if self.port.is_open == False:
            return
        
        #==============================================
        # Request data transmission
        #==============================================
        bytes_sent = 0
        
        tx_buffer = bytearray([0x01, 0x01, 0x00, 0xF8])
        # First send the amount of data to receive
        bytes_sent += self.port.write(b'\x05')
        # Then send remaining data 
        bytes_sent += self.port.write(tx_buffer)

        #==============================================
        # Receive data
        #==============================================
        bytes_received = 0
        while self.port.in_waiting == 0:
            print("Waiting")
        raw_upcoming: bytes = self.port.read(1)
        upcoming = int.from_bytes(raw_upcoming) - 1
        print("Raw: {0}, Decoded: {1}".format(raw_upcoming, upcoming))
        data = self.port.read(upcoming)
        bytes_received = upcoming

        print("Transmitted: {0}, Received data: {1}".format(bytes_sent, data))

        
