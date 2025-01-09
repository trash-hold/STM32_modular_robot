# Imports from external modules
from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QPushButton, QWidget, QLabel, QSlider, QLineEdit
from PySide6.QtGui import QDoubleValidator, QIntValidator

import PySide6.QtCore as QtC
from enum import Enum

class FloatSlider(QWidget):
    valueChange = QtC.Signal(float)
    
    def __init__(self, desc, step_size = 1, min_value = -90, max_value = 90):
        super().__init__()
        # Set slider controls
        self.desc = desc
        self.value = 0.00
        self.step_size = step_size * 100
        self.min_value = min_value * 100
        self.max_value = max_value * 100
        self.return_function = None

        self.slider = QSlider(orientation = QtC.Qt.Orientation.Horizontal)
        self.value_label = QLineEdit(str(self.value))

        self.create_widget()


    def create_widget(self) -> None:
        # Layout 
        main_container = QVBoxLayout()
        slider_widget = QWidget()
        slider_container = QHBoxLayout()

        # Widgets
        description = QLabel(self.desc)
        lower_pb = QPushButton("<")
        raise_pb = QPushButton(">")

        # Setting up buttons
        lower_pb.clicked.connect(self.decrement_value)
        raise_pb.clicked.connect(self.increment_value)

        # Setting up slider
        self.slider.setMaximum(self.max_value)
        self.slider.setMinimum(self.min_value)
        self.slider.setTickInterval(self.step_size)
        self.slider.valueChanged.connect(self.slider_new_value)


        # Setting up line edit
        validator = QDoubleValidator(self.min_value, self.max_value, 2)
        self.value_label.setValidator(validator)
        self.value_label.editingFinished.connect(self.update_slider)

        # Adding widgets
        main_container.addWidget(description)

        slider_container.addWidget(lower_pb)
        slider_container.addWidget(self.slider)
        slider_container.addWidget(raise_pb)
        slider_container.addWidget(self.value_label)

        slider_widget.setLayout(slider_container)
        main_container.addWidget(slider_widget)
        
        # Widget layout
        self.setLayout(main_container)

    def decrement_value(self) -> None:
        old_val = self.slider.value()
        self.slider.setValue(old_val - self.step_size)

    def increment_value(self) -> None:
        old_val = self.slider.value()
        self.slider.setValue(old_val + self.step_size)

    def slider_new_value(self, value) -> None:
        new_value = value / 100
        self.value_label.setText(f"{new_value: .2f}")
        self.valueChange.emit(new_value)

    def update_slider(self) -> None:
        new_val = float(self.value_label.text())
        self.slider.setValue(int(new_val*100))


class ServoPanel(QWidget):
    change_pos = QtC.Signal(list)
    request_temp = QtC.Signal()
    request_status = QtC.Signal()
    request_position = QtC.Signal()

    class SliderType(Enum):
        POS = 0x00
        SPEED = 0x01
        ACC = 0x02

    class InfoBox(Enum):
        POS = 0x00
        TEMP = 0x01
        STATUS = 0x02

    def __init__(self, name: str):
        super().__init__()

        self.movement_parameters = [0, 0, 0]
        self.info_widgets = list()

        self.create_widget()

    def create_widget(self) -> None:
        # Layout 
        main_container = QVBoxLayout()
        slider_container = QVBoxLayout()

        #==============================================
        # Sliders
        #==============================================
        sliders = QWidget()
        position = FloatSlider("Position [deg]", 1, -50, 50)
        speed = FloatSlider("Speed [%]", 10, 0, 100)
        acceleration = FloatSlider("Acceleration [%]", 10, 0, 100)

        position.valueChange.connect(lambda value: self.movementChange(value, self.SliderType.POS))
        speed.valueChange.connect(lambda value: self.movementChange(value, self.SliderType.SPEED))
        acceleration.valueChange.connect(lambda value: self.movementChange(value, self.SliderType.ACC))

        slider_container.addWidget(position)
        slider_container.addWidget(speed)
        slider_container.addWidget(acceleration)
        sliders.setLayout(slider_container)

        #==============================================
        # Info panel
        #==============================================
        info_panel = QWidget()
        info_panel_container = QHBoxLayout()
        info = [["Current position [deg]", 0.0] , ["Temperature [deg C]", 0], ["Status", ""]]
        for box in info:
            info_box = QWidget()
            container = QVBoxLayout()

            # Create label
            label = QLabel(box[0])
            container.addWidget(label)

            # Create value box
            if isinstance(box[1], float):
                validator = QDoubleValidator(decimals=2)
                line = QLineEdit()
                line.setText("0.00")
                line.setReadOnly(True)
                line.setValidator(validator)

                self.info_widgets.append(line)
                container.addWidget(line)

            elif isinstance(box[1], int):
                validator = QIntValidator()
                line = QLineEdit()
                line.setText("0")
                line.setReadOnly(True)
                line.setValidator(validator)

                self.info_widgets.append(line)
                container.addWidget(line)

            elif isinstance(box[1], str):
                line = QLineEdit()
                line.setReadOnly(True)
                line.setText("---")

                self.info_widgets.append(line)
                container.addWidget(line)

            # Finalize creation
            info_box.setLayout(container)
            info_panel_container.addWidget(info_box)

        info_panel.setLayout(info_panel_container)
        
        #==============================================
        # COM buttons
        #==============================================
        com_buttons = QWidget()
        com_buttons_container = QHBoxLayout()

        write_com_widget = QWidget()
        read_com_widget = QWidget()

        write_com_widget_container = QVBoxLayout()
        read_com_widget_container = QVBoxLayout()

        write_com_widget_container.addWidget(QLabel("Write commands"))
        read_com_widget_container.addWidget(QLabel("Read commands"))

        write_pb = QWidget()
        read_pb = QWidget()
        write_pb_container = QHBoxLayout()
        read_pb_container = QHBoxLayout()

        write_pos_pb = QPushButton("Move to given position")
        write_pos_pb.clicked.connect(lambda: self.change_pos.emit(self.movement_parameters))
        write_pb_container.addWidget(write_pos_pb)
        write_pb.setLayout(write_pb_container)
        write_com_widget_container.addWidget(write_pb)

        req_pos = QPushButton("Get position")
        req_temp = QPushButton("Get temperature")
        req_status = QPushButton("Get status")

        req_pos.clicked.connect(lambda: self.request_position.emit())
        req_temp.clicked.connect(lambda: self.request_temp.emit())
        req_status.clicked.connect(lambda: self.request_status.emit())

        read_pb_container.addWidget(req_pos)
        read_pb_container.addWidget(req_temp)
        read_pb_container.addWidget(req_status)
        read_pb.setLayout(read_pb_container)
        read_com_widget_container.addWidget(read_pb)

        write_com_widget.setLayout(write_com_widget_container)
        read_com_widget.setLayout(read_com_widget_container)

        com_buttons_container.addWidget(write_com_widget)
        com_buttons_container.addWidget(read_com_widget)
        com_buttons.setLayout(com_buttons_container)
        

        # Add all panels
        main_container.addWidget(sliders)
        main_container.addWidget(info_panel)
        main_container.addWidget(com_buttons)

        self.setLayout(main_container)

    def movementChange(self, value: float, slider_type: SliderType) -> None:
        if slider_type == self.SliderType.POS:
            new_pos = int(value * 4095 / 360)
            self.movement_parameters[0] = new_pos

        elif slider_type == self.SliderType.SPEED:
            new_speed = int(4000 * value / 100)
            self.movement_parameters[1] = new_speed

        elif slider_type == self.SliderType.ACC:
            new_acc = int(100 * value / 100)
            self.movement_parameters[2] = new_acc

        print(self.movement_parameters)

    def updateInfo(self, value, info_type: InfoBox) -> None:
        self.info_widgets[info_type.value].setText(value)

    def writeTemp(self, value) -> None:
        self.updateInfo(value, self.InfoBox.TEMP)

    def writePos(self, value) -> None:
        self.updateInfo(value, self.InfoBox.POS)

    def writeStatus(self, value) -> None:
        self.updateInfo(value, self.InfoBox.STATUS)