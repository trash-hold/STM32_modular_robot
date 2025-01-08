import serial
import serial.tools
import serial.tools.list_ports
from .definitions import *

import time

class SerialDataPacket():
    def __init__(self, bytes_sent:int, bytes_received:int, input_data: bytearray, output_data: bytearray, timestamp: str = None):
        self.sent = bytes_sent
        self.received = bytes_received
        self.tx = input_data
        self.rx = output_data
        self.time = timestamp

    def verify(self) -> bool:
        # Check parameters of tx buffer
        if self.tx.count != int.from_bytes(self.tx[0]):
            return False
        
        checksum = sum(self.tx[0:-2])
        checksum = ((~checksum) & 0xFF)
        if checksum != int.from_bytes(self.tx[-1]):
            return False

        # Check parameters of rx buffer
        if self.rx.count != int.from_bytes(self.rx[0]):
            return False
        
        checksum = sum(self.rx[0:-2])
        checksum = ((~checksum) & 0xFF)
        if checksum != int.from_bytes(self.rx[-1]):
            return False
        
        return True
    
    def get_data(self) -> list:
        print("XD")


class SerialDriver(serial.Serial):
    def __init__(self):
        super().__init__(baudrate = UART_DEF_BAUDRATE, bytesize = 8, timeout = UART_TIMEOUT )

    def transmit(self, op_code: UART_OP_CODES, data: bytearray, module_id:int = 0x01,) -> SerialDataPacket: 
        # Check if port is open
        if self.is_open == False:
            return None
        #====================================
        # Prepare data
        #====================================
        # Create standard header
        data_len = len(data) + 0x04
        tx_buffer = bytearray([data_len, module_id, op_code.value])
        # Append data
        if len(data) != 0:
            tx_buffer.extend(data)
        # Calculate checksum
        checksum = sum(tx_buffer)
        checksum = ((~checksum) & 0xFF)
        # Add checksum to buffer
        tx_buffer.append(checksum)

        #====================================
        # Send data 
        #====================================
        # First send amount of data
        tx_bytes = self.write(bytearray([tx_buffer[0]]))
        # Then send rest of data
        tx_bytes += self.write(bytearray(tx_buffer[1:]))

        #====================================
        # Get status 
        #====================================
        start_time = time.time()
        # Wait for data
        while self.in_waiting == 0:
            delta = time.time() - start_time
            if (delta > UART_TIMEOUT):
                return None
            
        # Check how much data to receive
        raw_byte: bytes = self.read(1)
        data_len = int.from_bytes(raw_byte) - 1

        rx_bytes = self.read(data_len)

        print("Succesful transmission!")
        print("TX_bytes: {0}, RX_bytes: {1}, TX_buff {2}, RX_buff {3}".format(tx_bytes, len(rx_bytes) + 1, bytes(tx_buffer).hex(), bytes(rx_bytes).hex()))
        return SerialDataPacket(tx_bytes, len(rx_bytes) + 1, tx_buffer, rx_bytes, start_time)
    
    def transmittemp(self, op_code: UART_OP_CODES, data: bytearray, module_id:int = 0x01,) -> SerialDataPacket: 
        if self.is_open == False:
            return
        
        bytes_sent = 0
        tx_buffer = bytearray([0x01, 0x02, 0x00, 0x00, 0xAA, 0x0B, 0xBB, 0x32, 0x50])
        # First send the amount of data to receive
        bytes_sent += self.write(b'\x0A')
        # Then send remaining data 
        bytes_sent += self.write(tx_buffer)
        print("Send data: {0}".format(bytes_sent))
        
        # Wait for status msg
        bytes_received = 0
        upcoming = 0
        while self.in_waiting == 0:
            print("Waiting")

        raw_upcoming: bytes = self.read(1)
        upcoming = int.from_bytes(raw_upcoming) - 1
        print("Raw: {0}, Decoded: {1}".format(raw_upcoming, upcoming))
        data = self.read(upcoming)
        
        bytes_received = upcoming

        print("Transmitted: {0}, Received data: {1}".format(bytes_sent, data))


        

        

        

        


