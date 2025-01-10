from PySide6.QtWidgets import *
from ..datatypes.definitions import *
from ..datatypes.serial_driver import *

from .widgets import ServoPanel, AccelerometerPanel, Separator

import time

class ControlPanelGUI(QWidget):
    def __init__(self):
        super().__init__()
        self.port = SerialDriver()

        self.initGUI()

    def initGUI(self) -> None:
        layout = QVBoxLayout()

        #==================================
        # COM port picker
        #==================================
        com_port_widget = QWidget()
        com_port_container = QHBoxLayout()

        com_port_widget.setObjectName("com_port_widget")
        com_port_container.setObjectName("com_port_container")
        com_port_widget.setStyleSheet("QWidget#com_port_widget {background-color: darkGreen; margin: 0px}")

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

        split = Separator()
        split.setStyleSheet('''border: 3px solid green''')

        # Add widgets
        com_port_container.addWidget(port_list_widget)
        com_port_container.addWidget(connect_port)
        com_port_widget.setLayout(com_port_container)

        #==================================
        # Servo panel
        #==================================
        servo_panel = QWidget()
        servo_panel_container = QHBoxLayout()

        servo0 = ServoPanel("Servo 0")
        servo1 = ServoPanel("Servo 1")

        servo_list = [[servo0, 0x00], [servo1, 0x01]]

        for servo_info in servo_list:
            servo_index = servo_info[1]
            servo: ServoPanel = servo_info[0]
            servo.change_pos.connect(lambda value, servo_index = servo_index, servo = servo: self.servoSetPos(value, servo_index, servo))
            servo.request_position.connect(lambda servo_index = servo_index, ser = servo: self.servoUpdateInfo(UART_OP_CODES.COM_SERVO_POS_READ, servo_index, ser))
            servo.request_temp.connect(lambda servo_index = servo_index, ser = servo: self.servoUpdateInfo(UART_OP_CODES.COM_SERVO_READ_TEMP, servo_index, ser))
            servo.request_status.connect(lambda servo_index = servo_index, ser = servo: self.servoUpdateInfo(UART_OP_CODES.COM_SERVO_PING, servo_index, ser))
            servo_panel_container.addWidget(servo)

        servo_panel.setLayout(servo_panel_container)

        #==================================
        # Accelerometer  panel
        #==================================
        acc_panel = QWidget()
        acc_panel_container = QHBoxLayout(acc_panel)

        acc0 = AccelerometerPanel("Accelerometer  0")
        acc1 = AccelerometerPanel("Accelerometer  1")
        acc_list = [[acc0, 0x00], [acc1, 0x01]]
        for acc_info in acc_list:
            acc = acc_info[0]
            index = acc_info[1]
        
            acc.request_angles.connect(lambda index = index, acc = acc: self.accUpdateInfo(UART_OP_CODES.COM_ACC_ANGLES_READ, index, acc))
            acc.request_status.connect(lambda index = index, acc = acc: self.accUpdateInfo(UART_OP_CODES.COM_ACC_STATUS, index, acc))
            acc_panel_container.addWidget(acc)

        #==================================
        # Main layout
        #==================================
        layout.addWidget(com_port_widget)
        layout.addWidget(servo_panel)
        layout.addWidget(acc_panel)
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

    def sendData(self, op_code: UART_OP_CODES, data: bytearray) -> SerialDataPacket:
        return self.port.transmit(op_code = op_code, data = data)

    
    def servoUpdateInfo(self, op_code: UART_OP_CODES, index: int, servo: ServoPanel) -> None:
        packet: SerialDataPacket = self.sendData(op_code, bytearray([index]))

        if packet is None:
            return
        
        data = packet.get_data(op_code)

        if op_code == UART_OP_CODES.COM_SERVO_READ_TEMP:
            servo.writeTemp(str(data[0]))
        elif op_code == UART_OP_CODES.COM_SERVO_POS_READ:
            val = data[0] * 360 / 4095
            servo.writePos(f'{val: .2f}')
        elif op_code == UART_OP_CODES.COM_SERVO_PING:
            servo.writeStatus(data[0])
            

        print("Received: {0}".format(bytes(packet.rx).hex()))

    def accUpdateInfo(self, op_code: UART_OP_CODES, index:int, acc_panel: AccelerometerPanel) -> None:
        packet: SerialDataPacket = self.sendData(op_code, bytearray([index]))

        if packet is None:
            return
        
        data = packet.get_data(op_code)
        if op_code == UART_OP_CODES.COM_ACC_ANGLES_READ:
            acc_panel.writeAngles(data)
        elif op_code == UART_OP_CODES.COM_ACC_STATUS:
            acc_panel.writeStatus(data[0])

    def servoSetPos(self, data: list, index: int, servo: ServoPanel) -> None:
        pos_low_nibble = data[0] & 0xFF
        pos_high_nibble = (data[0] & 0xFF00) >> 8

        speed_low_nibble = data[1] & 0xFF
        speed_high_nibble = (data[1] & 0xFF00) >> 8
        
        # Prepare data
        print(index)
        array = bytearray([index, pos_high_nibble, pos_low_nibble, speed_high_nibble, speed_low_nibble, data[2]])
        print(array)
        # Send data
        datapacket = self.sendData(UART_OP_CODES.COM_SERVO_POS_SET, array)

        if datapacket is None:
            return
        # Wait for servo to load data
        time.sleep(0.2)
        # Update current position
        datapacket: SerialDataPacket = self.sendData(UART_OP_CODES.COM_SERVO_POS_READ, bytearray([index]))

        if datapacket is None:
            return
        
        print(datapacket.rx)
        pos = datapacket.get_data(UART_OP_CODES.COM_SERVO_POS_READ)
        val = pos[0] * 360 / 4095
        servo.writePos(f'{val: .2f}')