from PySide6.QtWidgets import *
from src.GUI.main_window import ControlPanelGUI
import serial
import sys

import serial.tools
import serial.tools.list_ports

if __name__ == '__main__':
    # Set up PySide6 GUI app
    app = QApplication(sys.argv)
    window = QMainWindow()
    window.setWindowTitle("Robot control panel")
    window.show()

    gui = ControlPanelGUI()
    window.setCentralWidget(gui)
    

    app.exec()