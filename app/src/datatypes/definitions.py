from enum import Enum

UART_DEF_BAUDRATE = 115200
UART_TIMEOUT = 5

class UART_OP_CODES(Enum):
    COM_IDLE = 0x00
    COM_SERVO_POS_READ = 0x01
    COM_SERVO_POS_SET = 0x02
    COM_SERVO_READ_TEMP = 0x03
    COM_SERVO_PING = 0x04
    COM_ACC_ANGLES_READ = 0x05
    COM_ACC_STATUS = 0x06

class UART_STATUS_CODES(Enum):
    G_ERROR = 0x00
    G_SUCCESS = 0x01
    C_MOUNT_ERROR = 0x02
    G_FILE_WRITE = 0x03
    G_FILE_READ = 0x04
    G_COM_OVERFLOW = 0x05
    G_COM_TRANSMIT = 0x06
    G_COM_RECEIVE = 0x07
    C_RTC_ERROR = 0x08
    C_UART_TRANSMIT = 0x09
    C_UART_RECEIVE = 0x0A
    C_UART_HANDLE = 0x0B
    G_SERVO_WRTIE = 0x0C
    G_SERVO_READ = 0x0D
    C_ACC_HANDLE = 0x0E
    C_I2C_TRANSMIT = 0x0F
    C_I2C_RECEIVE = 0x10
    G_ACC_READ = 0x11