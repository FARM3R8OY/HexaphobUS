"""
File: hexaphobus_ui.py

Contributor(s):
    Cabana,  Gabriel  | cabg2101
    Lalonde, Philippe | lalp2803

Date(s):
    2020-01-29 (Creation)
    2020-04-01 (Last modification)

Description:
    User interface designed for intuitive control and monitoring of the
    HexaphobUS robot.

S4-H20 | GRO400
"""

# --------------------------------------------

import math
import os
import sys
import time

# import struct
# import binascii

from threading import Timer
import serial

from PyQt5.QtCore import Qt, QTimer, QPoint, QThread
from PyQt5.QtGui import (QColor, QIcon, QPainter, QPalette, QKeySequence,
                         QDoubleValidator, QPixmap)
from PyQt5.QtWidgets import (QApplication, QGridLayout, QHBoxLayout, QLabel,
                             QLineEdit, QPushButton, QVBoxLayout, QWidget,
                             QShortcut, QFrame, QSizePolicy)

# --------------------------------------------

Servos_Num = [7, 9, 11, 1, 3, 5, 2, 4, 6, 8, 10, 12]

SERIAL_UPDATE_RATE = 0.1

ARROW_W = 60
ARROW_H = 30
BUTTON_W = 120
BUTTON_H = 60
INFO_W = 190
INFO_H = 30
SV_NBR = 12
SV_W = 100
SV_H = 30
TRACK_W = 250
TRACK_H = 50
UI_X = 80
UI_Y = 80
UI_W = 800
UI_H = 600
UI_MIN_W = 480
UI_MIN_H = 360

NB_COMMAND = 0
ENCODED_VAR = b'55'
EDIT_PLACE = 0
SERVO_TABLE = list()

PORT = "/dev/ttyACM0"
BAUD_RATE = 9600

WINDOW_NAME = "HexaphobUS UI"
BUTTON_UP = "\u2191"
BUTTON_DOWN = "\u2193"
BUTTON_LEFT = "\u2190"
BUTTON_RIGHT = "\u2192"
BUTTON_SERIAL = "Start Serial"
BUTTON_INIT = "POS INIT"
BUTTON_PRG1 = "PRG1"
SCRIPT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                          "..", ".."))
SEP = os.path.sep
LOGO = 'img' + SEP + 'hexaphobus_logo.png'

# --------------------------------------------


def stringToByte(string):
    """
    Encodes string and returns byte values.
    """
    string = string + '|'
    encoded_string = string.encode()
    ''' Uncomment for struct encoding
    string_size = len(string)
    my_format = str(string_size) + "s"
    packed_data = struct.pack(my_format, encoded_string)
    encoded_string = binascii.hexlify(packed_data)
    '''
    return encoded_string

def byteToString(encoded_string):
    """
    Decodes byte values and returns a string.
    """
    ''' Uncomment for struct decoding
    packed_data = binascii.unhexlify(encoded_string)
    string_size = len(encoded_string)/2
    my_format = str(int(string_size))+"s"
    string = struct.unpack(my_format, packed_data)
    '''
    string = encoded_string.decode().strip()
    return string

# --------------------------------------------


class RepeatedTimer(object):
    """
    The 'RepeatedTimer' class allows the user to time some operations
    at a specified frequency.
    """
    def __init__(self, interval, function, *args, **kwargs):
        """
        The 'RepeatedTimer' class constructor initializes the flagging
        interval, the operation to launch, the state, and other
        arguments.
        """
        self._timer = None
        self.interval = interval
        self.function = function
        self.args = args
        self.kwargs = kwargs
        self.is_running = False
        self.start()

    def _run(self):
        """
        Runs the timer and its operation.
        """
        self.is_running = False
        self.start()
        self.function(*self.args, **self.kwargs)

    def start(self):
        """
        Starts the timer.
        """
        if not self.is_running:
            self._timer = Timer(self.interval, self._run)
            self._timer.start()
            self.is_running = True

    def stop(self):
        """
        Stops the timer.
        """
        self._timer.cancel()
        self.is_running = False

class SerialChecker(QThread):
    """
    The 'SerialChecker' class is a QThread subclass that allows the user
     to initiate a serial communication with a specified port.
    """
    def __init__(self):
        """
        The 'SerialChecker' class constructor initializes the serial
        port, and the timer.
        """
        super().__init__()
        self.ser = None
        self.rt = None

    def SerialRun(self):
        """
        Initiates the serial communication.
        """
        self.ser = serial.Serial(PORT, BAUD_RATE)
        self.ser.reset_input_buffer()
        
        
        #self.rt = RepeatedTimer(SERIAL_UPDATE_RATE, self.serialReceive)
#=----------------------------------------------------------------------------------------
    def serialReceive(self):
        """
        Gets the information from the serial port.
        """
        if self.ser.in_waiting > 0:
            global SERVO_TABLE
            try:
                stringData = self.ser.read_until()
            except:
                return 
            #print(stringData)
            servoAngle = byteToString(stringData)
            
            
            try:
                tableData = servoAngle.split(";")
            except:
                print("Erreur")

            if len(tableData) == 12:
                SERVO_TABLE = tableData  

    def serialQuit(self):
        """
        Ends the serial communication.
        """
        self.rt.stop()

    def serialSend(self, command):
        """
        Sends the information to the serial port.
        """
        bytesData = stringToByte(command)
        print(command)
        self.ser.write(bytesData)

class RobotTracking(QWidget):
    """
    The 'RobotTracking' class is a QWidget subclass that allows the user
     to follow the robot position from an initialized origin point.
    """

    def __init__(self):
        """
        The 'RobotTracking' class constructor initializes, notably, the
        widget to follow the robot's position. It builds upon the
        constructor of its parent class, 'QWidget'.
        """
        super().__init__()
        self.setStyleSheet("border:1px solid rgb(0, 0, 0)")
        self._distance_origin = 0
        self._robot_x_pos = 200
        self._robot_y_pos = 200
        self._target_x_pos = 250
        self._target_y_pos = 250
        self._vel = 60  # pixels per second
        self._speed = 10

        self.timer = QTimer(self)
        self.timer.start(round(1000 / self._vel))
        self.initTracking()

    def initTracking(self):
        """
        Sets the size of the widget and its label (tracking distance and
        position).
        """
        self.setMinimumSize(UI_MIN_W, UI_MIN_H)
        self.label = QLabel(self)
        self.label.resize(TRACK_W, TRACK_H)
        self.label.setStyleSheet("color : rgb(0,0,0)")
        self.show()

    def changePosition(self, direction):
        """
        Updates the target's distance and position according to
        geometry.
        """
        UI_Graph_W = self.geometry().width()
        UI_Graph_H = self.geometry().height()

        if direction == "FORWARD" and self._robot_y_pos > 0:
            self._robot_y_pos -= self._speed
            # Send direction to call program in Arduino
            ENCODED_VAR = stringToByte('FORWARD')
        elif direction == "BACKWARD" and self._robot_y_pos < UI_Graph_H:
            self._robot_y_pos += self._speed
            # Send direction to call program in Arduino
            ENCODED_VAR = stringToByte('BACKWARD')
        elif direction == "RIGHT" and self._robot_x_pos < UI_Graph_W:
            self._robot_x_pos += self._speed
            # Send direction to call program in Arduino
            ENCODED_VAR = stringToByte('RIGHT')
        elif direction == "LEFT" and self._robot_x_pos > 0:
            self._robot_x_pos -= self._speed
            # Send direction to call program in Arduino
            ENCODED_VAR = stringToByte('LEFT')

        self.moveRobot(self._robot_x_pos, self._robot_y_pos)
        self.update()

    def moveRobot(self, robotX, robotY):
        """
        Event that updates the target's distance and position label
        relative to the origin according to the target's movements.
        """
        _distance_origin = round(
            ((robotY - self._target_y_pos)**2
             + (robotX - self._target_x_pos)**2)**0.5
        )

        self.label.setText("Coordinates (x; y): ({}; {})\
                           \nDistance from origin: {}"
                           .format(robotX, robotY, _distance_origin))

        self.update()

    def initPosition(self):
        """
        Event that updates the origin point according to reset command.
        """
        self._target_x_pos = self._robot_x_pos
        self._target_y_pos = self._robot_y_pos
        self.update()

    def paintEvent(self, event):
        """
        Event that draws a line between the target's position and its
        origin.
        """
        robot = QPixmap(SCRIPT_DIR + SEP + LOGO)
        pix_robot = robot.scaledToHeight(40)

        q = QPainter()
        q.begin(self)
        q.setPen(QColor(0, 0, 0))
        q.drawPixmap(QPoint(self._robot_x_pos-20, self._robot_y_pos-20),
                     pix_robot)
        #From center to robot
        q.drawLine(self._robot_x_pos, self._robot_y_pos,
                   self._target_x_pos, self._target_y_pos)

        moving_command = byteToString(ENCODED_VAR)

        #Suggest to use feedback from the angles of servomotor
        if moving_command == "FORWARD" or moving_command == "BACKWARD":
            if NB_COMMAND % 2 == 0 and NB_COMMAND >= 0:
                move_leg = -6
            elif NB_COMMAND % 2 == 1 and NB_COMMAND >= 0:
                move_leg = 6
            NB_COMMAND += 1
        else:
            move_leg = 0

        #Leg 1
        q.drawLine(self._robot_x_pos+20, self._robot_y_pos,
                   self._robot_x_pos+40, self._robot_y_pos+move_leg)
        #Leg 2
        q.drawLine(self._robot_x_pos+12, self._robot_y_pos+13,
                   self._robot_x_pos+32, self._robot_y_pos+13-move_leg)
        #Leg 3
        q.drawLine(self._robot_x_pos+12, self._robot_y_pos-13,
                   self._robot_x_pos+32, self._robot_y_pos-13-move_leg)
        #Leg 4
        q.drawLine(self._robot_x_pos-20, self._robot_y_pos,
                   self._robot_x_pos-40, self._robot_y_pos-move_leg)
        #Leg 5
        q.drawLine(self._robot_x_pos-12, self._robot_y_pos+13,
                   self._robot_x_pos-32, self._robot_y_pos+13+move_leg)
        #Leg 6
        q.drawLine(self._robot_x_pos-12, self._robot_y_pos-13,
                   self._robot_x_pos-32, self._robot_y_pos-13+move_leg)


class MainWindow(QWidget):
    """
    The 'MainWindow' class is a QWidget subclass that allows the user
    to monitor various robot items, control the robot and track its
    position with another QWidget subclass ('RobotTracking' class).
    """

    def __init__(self):
        """
        The 'MainWindow' class constructor initializes the GUI interface
        and all its components (layout boxes, push buttons, shortcuts,
        lines, edits, frames, validators). It builds upon the
        constructor of its parent class, 'QWidget'.
        """
        super().__init__()

        # Servos
        self._servo_edits = list()
        self._servo_labels = list()
        global SERVO_TABLE

        # Layouts
        self.global_layout = QVBoxLayout()

        self.top_layout = QHBoxLayout()
        self.bottom_layout = QHBoxLayout()

        self.servo_layout_1 = QVBoxLayout()
        self.servo_layout_2 = QVBoxLayout()
        self.servo_layout_3 = QVBoxLayout()
        self.servo_layout_4 = QVBoxLayout()
        self.tracking_layout = QVBoxLayout()


        self.info_layout = QVBoxLayout()
        self.prog_layout = QVBoxLayout()
        self.move_layout = QGridLayout()

        # Buttons
        self.button_up = QPushButton(BUTTON_UP)
        self.button_down = QPushButton(BUTTON_DOWN)
        self.button_left = QPushButton(BUTTON_LEFT)
        self.button_right = QPushButton(BUTTON_RIGHT)

        self.button_serial_start = QPushButton(BUTTON_SERIAL)
        self.button_init = QPushButton(BUTTON_INIT)
        self.button_prog = QPushButton(BUTTON_PRG1)

        #Shortcut
        self.shortcut_up = QShortcut(QKeySequence("alt+up"), self)
        self.shortcut_down = QShortcut(QKeySequence("alt+down"), self)
        self.shortcut_left = QShortcut(QKeySequence("alt+left"), self)
        self.shortcut_right = QShortcut(QKeySequence("alt+right"), self)

        # Edits and labels
        self.speed_edit = QLineEdit()
        self.energy_edit = QLineEdit()
        self.speed_label = QLabel()
        self.energy_label = QLabel()

        self.GraphFrame = QFrame()
        self.GraphFrame.setFrameShape(QFrame.Box)
        self.GraphFrame.setStyleSheet("background: rgb(180,180,180)")

        self.onlyFloat = QDoubleValidator()


        self.serialCheck = SerialChecker()
        self.serialCheck.SerialRun()
        

        self.repeater = RepeatedTimer(SERIAL_UPDATE_RATE, self.setServoValues)

        self.initUI()

    def getServoEdits(self):
        """
        Return the servomotor commands.
        """
        return self._servo_edits

    def getServoLabels(self):
        """
        Return the servomotor labels.
        """
        return self._servo_labels

    def initUI(self):
        """
        Initializes the user interface. It sets:
            -the geometry;
            -the window behavior;
            -the 'RobotTracking' widget;
            -the buttons;
            -the connections;
            -the robot information;
            -the dependencies (nested layouts);
            -the widgets.
        """
        self.setGeometry(UI_X, UI_Y, UI_W, UI_H)
        self.setWindowTitle(WINDOW_NAME)

        self.tracking = RobotTracking()
        self.tracking.setSizePolicy(QSizePolicy.Expanding,
                                    QSizePolicy.Expanding)

        self.addServos()
        self.setSizeButtons()
        self.setConnections()
        self.setInfo()
        self.setLayoutDependencies()
        self.addWidgets()
        self.setLayout(self.global_layout)
        self.setInfoValues("100", "200")

        self.show()


    def addServos(self):
        """
        Add servomotor edits and labels used to monitor their state.
        """

        for index in range(SV_NBR):
            self._servo_edits.append(QLineEdit())
            self._servo_edits[index].setFixedSize(SV_W, SV_H)
            self._servo_edits[index].setReadOnly(True)
            self._servo_edits[index].setStyleSheet("color : rgb(0,0,0)")
            self._servo_edits[index].setValidator(self.onlyFloat)

            self._servo_labels.append(QLabel())
            self._servo_labels[index].setAlignment(Qt.AlignCenter)
            self._servo_labels[index].setText("Servo{:02d}"
                                              .format(Servos_Num[index]))

    def setSizeButtons(self):
        """
        Fix button size according to type.
        """
        self.button_up.setFixedSize(ARROW_W, ARROW_H)
        self.button_down.setFixedSize(ARROW_W, ARROW_H)
        self.button_left.setFixedSize(ARROW_W, ARROW_H)
        self.button_right.setFixedSize(ARROW_W, ARROW_H)

        self.button_serial_start.setFixedSize(BUTTON_W, BUTTON_H)
        self.button_init.setFixedSize(BUTTON_W, BUTTON_H)
        self.button_prog.setFixedSize(BUTTON_W, BUTTON_H)

    def setConnections(self):
        """
        Connect the buttons and shortcuts to the corresponding
        functions.
        """
        self.button_serial_start.clicked.connect(self.serialCheck.SerialRun)
        self.button_serial_start.clicked.connect(self.removeButton)
        self.button_prog.clicked.connect(self.runProgram)
        self.button_init.clicked.connect(self.tracking.initPosition)
        self.button_up.clicked.connect(
            lambda: self.tracking.changePosition("FORWARD")
        )
        self.shortcut_up.activated.connect(
            lambda: self.tracking.changePosition("FORWARD")
        )
        self.button_up.clicked.connect(
            lambda: self.serialCheck.serialSend("FORWARD")
        )
        self.shortcut_up.activated.connect(
            lambda: self.serialCheck.serialSend("FORWARD")
        )
        self.button_down.clicked.connect(
            lambda: self.tracking.changePosition("BACKWARD")
        )
        self.shortcut_down.activated.connect(
            lambda: self.tracking.changePosition("BACKWARD")
        )
        self.button_down.clicked.connect(
            lambda: self.serialCheck.serialSend("BACKWARD")
        )
        self.shortcut_down.activated.connect(
            lambda: self.serialCheck.serialSend("BACKWARD")
        )
        self.button_right.clicked.connect(
            lambda: self.tracking.changePosition("RIGHT")
        )
        self.shortcut_right.activated.connect(
            lambda: self.tracking.changePosition("RIGHT")
        )
        self.button_right.clicked.connect(
            lambda: self.serialCheck.serialSend("RIGHT")
        )
        self.shortcut_right.activated.connect(
            lambda: self.serialCheck.serialSend("RIGHT")
        )
        self.button_left.clicked.connect(
            lambda: self.tracking.changePosition("LEFT")
        )
        self.shortcut_left.activated.connect(
            lambda: self.tracking.changePosition("LEFT")
        )
        self.button_left.clicked.connect(
            lambda: self.serialCheck.serialSend("LEFT")
        )
        self.shortcut_left.activated.connect(
            lambda: self.serialCheck.serialSend("LEFT")
        )

    def cleanUp(self):
        print("quitting")
        self.serialCheck.serialQuit()
        self.repeater.stop()
        print("end")

    def removeButton(self):
        self.button_serial_start.setEnabled(False)

    def setInfo(self):
        """
        Set information size and text display.
        """
        self.speed_edit.setFixedSize(INFO_W, INFO_H)
        self.speed_edit.setStyleSheet("color : rgb(0,0,0)")
        self.speed_edit.setReadOnly(True)
        self.energy_edit.setFixedSize(INFO_W, INFO_H)
        self.energy_edit.setStyleSheet("color : rgb(0,0,0)")
        self.energy_edit.setReadOnly(True)
        self.speed_label.setText("Vitesse :")
        self.energy_label.setText("Énergie :")

    def runProgram(self):
        print(ENCODED_VAR)
        # string = byteToString(ENCODED_VAR)
        # print('Unpacked Values:', string)

        
#--------------------------------------------------------------------------------------
    def setServoValues(self):
        """
        Set the servomotor edit text.
        """
        self.serialCheck.serialReceive()
        #print(SERVO_TABLE)
        for (angle, servo) in zip(SERVO_TABLE, self._servo_edits):
            servo.setText(angle)
            time.sleep(0.05)
            #print("Servo: "+ servo.text())


    def setInfoValues(self, Speed, Energy):
        """
        Set additional information values.
        """
        #Read values from Arduino (Speed? et energy battery )
        self.speed_edit.setText(Speed)
        self.energy_edit.setText(Energy)

    def setLayoutDependencies(self):
        """
        Add inner layouts to outer layouts (creates a parent-child
        structure with nested layouts).
        """
        self.global_layout.addLayout(self.top_layout)
        self.global_layout.addLayout(self.bottom_layout)

        self.top_layout.addLayout(self.servo_layout_1)
        self.top_layout.addLayout(self.servo_layout_2)
        self.top_layout.addWidget(self.GraphFrame)
        self.top_layout.addLayout(self.servo_layout_3)
        self.top_layout.addLayout(self.servo_layout_4)

        self.bottom_layout.addLayout(self.info_layout)
        self.bottom_layout.addLayout(self.prog_layout)
        self.bottom_layout.addLayout(self.move_layout)
        self.bottom_layout.addWidget(self.button_serial_start)

    def strechServoLayouts(self):
        """
        Add padding between widgets and/or layout walls in the
        servomotor layouts.
        """
        self.servo_layout_1.addStretch(1)
        self.servo_layout_2.addStretch(1)
        self.servo_layout_3.addStretch(1)
        self.servo_layout_4.addStretch(1)

    def addWidgets(self):
        """
        Add widgets to various layouts.
        """
        self.strechServoLayouts()

        for counter, (edit, label) in \
            enumerate(zip(self._servo_edits, self._servo_labels)):

            if counter < math.ceil(SV_NBR/4):
                self.servo_layout_1.addWidget(label)
                self.servo_layout_1.addWidget(edit)
                self.servo_layout_1.addStretch(1)
            elif counter < math.ceil(SV_NBR/2):
                self.servo_layout_2.addWidget(label)
                self.servo_layout_2.addWidget(edit)
                self.servo_layout_2.addStretch(1)
            elif counter < math.ceil(SV_NBR*3/4):
                self.servo_layout_3.addWidget(label)
                self.servo_layout_3.addWidget(edit)
                self.servo_layout_3.addStretch(1)
            elif counter < SV_NBR:
                self.servo_layout_4.addWidget(label)
                self.servo_layout_4.addWidget(edit)
                self.servo_layout_4.addStretch(1)

        self.tracking_layout.addWidget(self.tracking)
        self.GraphFrame.setLayout(self.tracking_layout)

        self.info_layout.addWidget(self.speed_label)
        self.info_layout.addWidget(self.speed_edit)
        self.info_layout.addWidget(self.energy_label)
        self.info_layout.addWidget(self.energy_edit)

        self.prog_layout.addWidget(self.button_init)
        self.prog_layout.addWidget(self.button_prog)

        self.move_layout.addWidget(self.button_up, 0, 1, 1, 1)
        self.move_layout.addWidget(self.button_down, 1, 1)
        self.move_layout.addWidget(self.button_left, 1, 0)
        self.move_layout.addWidget(self.button_right, 1, 2)

# --------------------------------------------


if __name__ == '__main__':
    # Create a Qt application and window to display.
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon(SCRIPT_DIR + SEP + LOGO))
    window = MainWindow()
    app.aboutToQuit.connect(window.cleanUp)

    # Set style
    palette = window.palette()
    palette.setColor(QPalette.Window, QColor(53, 53, 53))
    palette.setColor(QPalette.WindowText, Qt.white)
    palette.setColor(QPalette.Base, QColor(25, 25, 25))
    palette.setColor(QPalette.AlternateBase, QColor(53, 53, 53))
    palette.setColor(QPalette.ToolTipBase, Qt.white)
    palette.setColor(QPalette.ToolTipText, Qt.white)
    palette.setColor(QPalette.Text, Qt.white)
    palette.setColor(QPalette.Button, QColor(53, 53, 53))
    palette.setColor(QPalette.ButtonText, Qt.black)
    palette.setColor(QPalette.BrightText, Qt.red)
    palette.setColor(QPalette.Link, QColor(42, 130, 218))
    palette.setColor(QPalette.Highlight, QColor(42, 130, 218))
    palette.setColor(QPalette.HighlightedText, Qt.black)
    window.setPalette(palette)

    # Kill display
    sys.exit(app.exec_())
