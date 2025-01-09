# Imports from external modules
from PySide6.QtWidgets import QVBoxLayout, QHBoxLayout, QPushButton, QWidget, QLabel, QSlider, QLineEdit, QFrame, QSplitter
from PySide6.QtGui import QDoubleValidator, QIntValidator, QFont

import PySide6.QtCore as QtC
from enum import Enum

class HeaderLabel(QLabel):
    def __init__(self, name):
        super().__init__(name)
        self.setStyleSheet(".HeaderLabel{font-size: 16px; margin-left: 10px}")

class Separator(QFrame):
    def __init__(self):
        super().__init__()

        self.setStyleSheet(".Separator{border: 1px solid gray; background: gray; margin-left: 20px; margin-right: 20px}")
        self.setFrameShape(QFrame.Shape.HLine)

class BorderFrame(QFrame):
    def __init__(self):
        super().__init__()
        self.setStyleSheet(".BorderFrame{border: 3px solid green; border-radius: 10px}")

class InputArray(QWidget):
    def __init__(self, data_list: list):
        super().__init__()

        self.data = data_list
        self.inputs = list()

        self.create_widget()

    def __count__(self)->int:
        if self.data is None:
            return None
        
        return len(self.data)
    
    def create_widget(self):
        if self.data is None:
            return
        
        main_container = QHBoxLayout()
        for box in self.data:
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

                self.inputs.append(line)
                container.addWidget(line)

            elif isinstance(box[1], int):
                validator = QIntValidator()
                line = QLineEdit()
                line.setText("0")
                line.setReadOnly(True)
                line.setValidator(validator)

                self.inputs.append(line)
                container.addWidget(line)

            elif isinstance(box[1], str):
                line = QLineEdit()
                line.setReadOnly(True)
                line.setText("---")

                self.inputs.append(line)
                container.addWidget(line)

            # Finalize creation
            info_box.setLayout(container)
            main_container.addWidget(info_box)

        self.setLayout(main_container)



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
        self.name = name

        self.create_widget()

    def create_widget(self) -> None:
        # Layout 
        frame = BorderFrame()

        main_container = QVBoxLayout(frame)
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

        info = [["Current position [deg]", 0.0] , ["Temperature [deg C]", 0], ["Status", ""]]
        info_panel = InputArray(info)
        self.info_widgets = info_panel.inputs

        
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
        
        seprator_controls = Separator()
        seprator_info = Separator()

        # Add all panels
        main_container.addWidget(sliders)
        main_container.addWidget(seprator_controls)
        main_container.addWidget(info_panel)
        main_container.addWidget(seprator_info)
        main_container.addWidget(com_buttons)

        # Establish layout
        name_label = HeaderLabel(self.name)
        layout = QVBoxLayout(self)
        layout.addWidget(name_label)
        layout.addWidget(frame)

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


class AccelerometerPanel(QWidget):
    request_angles = QtC.Signal()
    request_status = QtC.Signal()

    def __init__(self, name):
        super().__init__()

        self.info_widgets = []
        self.name = name

        self.create_widget()

    def create_widget(self) -> None:
        # Layout 
        frame = BorderFrame()
        main_container = QVBoxLayout(frame)

        #==========================================
        # Data
        #==========================================
        labels = [["A", 0.00], ["B", 0.00], ["C", 0.00], ["Status", '']]
        data_panel = InputArray(labels)

        for widget in data_panel.inputs:
            self.info_widgets.append(widget)

        #==========================================
        # COM buttons
        #==========================================
        pb_panel = QWidget()
        pb_container = QHBoxLayout(pb_panel)
        status_pb = QPushButton("Get status")
        angles_pb = QPushButton("Get angles")

        angles_pb.clicked.connect(lambda: self.request_angles.emit())
        status_pb.clicked.connect(lambda: self.request_status.emit())
        pb_container.addWidget(angles_pb)
        pb_container.addWidget(status_pb)

        #==========================================
        # Add widgets
        #==========================================
        main_container.addWidget(data_panel)
        main_container.addWidget(pb_panel)

        # Establish layout
        name_label = HeaderLabel(self.name)

        layout = QVBoxLayout(self)
        layout.addWidget(name_label)
        layout.addWidget(frame)

    def writeAngles(self, ang: list) -> None:
        for index in range(0,3):
            self.inputs[index].setText(ang[index])

    def writeStatus(self, value) -> None:
        self.updateInfo(value, self.inputs)
