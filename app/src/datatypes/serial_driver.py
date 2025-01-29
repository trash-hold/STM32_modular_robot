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
    
    def get_data(self, op_code: UART_OP_CODES) -> list:
        if op_code == UART_OP_CODES.COM_SERVO_POS_READ:
            if len(self.rx) != 6:
                return None
            
            lower_byte = self.rx[4]
            higher_byte = self.rx[3]

            pos = (higher_byte << 8) | (lower_byte)
            return [pos]
        
        elif op_code == UART_OP_CODES.COM_SERVO_READ_TEMP:
            if len(self.rx) != 5:
                return None
            
            pos = self.rx[3]
            return [pos]
        
        elif op_code == UART_OP_CODES.COM_SERVO_PING or op_code == UART_OP_CODES.COM_ACC_STATUS:
            if len(self.rx) != 4:
                return None
            
            pos = UART_STATUS_CODES(self.rx[2]).name
            return [pos]
        
        elif op_code == UART_OP_CODES.COM_ACC_ANGLES_READ:
            if len(self.rx) != 10:
                return None
            
            angles = []
            for i in range(0,3):
                lower_byte = self.rx[3 + i*2]
                higher_byte = self.rx[4 + i*2]

                sign = -1 if (higher_byte & 0x80) == 1 else 1
                val = float(higher_byte & 0x7F) + float(lower_byte / 100)
                val = val * sign
                angles.append(f'{val: .2f}')

            return angles



class SerialDriver(serial.Serial):
    def __init__(self):
        super().__init__(baudrate = UART_DEF_BAUDRATE, bytesize = 8, timeout = UART_TIMEOUT )
        self.busy = False

    def transmit(self, op_code: UART_OP_CODES, data: bytearray, module_id:int = 0x01,) -> SerialDataPacket: 
        # Check if port is open
        if self.is_open == False:
            return None
        
        if self.busy == True:
            return None
        
        self.busy = True
        #====================================
        # Prepare data
        #====================================
        # Create standard header
        data_len = len(data) + 0x04
        tx_buffer = bytearray([data_len, module_id, op_code.value])
        # Append data
        if data is not None:
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
        rx_buffer = bytearray( [int.from_bytes(raw_byte)] )
        rx_buffer.extend(rx_bytes)

        print("Succesful transmission!")
        print("TX_bytes: {0}, RX_bytes: {1}, TX_buff {2}, RX_buff {3}".format(tx_bytes, len(rx_buffer) + 1, bytes(tx_buffer).hex(), bytes(rx_buffer).hex()))

        self.busy = False
        return SerialDataPacket(tx_bytes, len(rx_buffer), tx_buffer, rx_buffer, start_time)