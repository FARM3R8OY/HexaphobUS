##@file hexaphobus_ui.py
#
##@authors
#     - Cabana,       Gabriel  | cabg2101
#     - Guay-Tanguay, Carolane | guac3201
#     - Lalonde,      Philippe | lalp2803
#     - Roy,          Olivier  | royo2206
#
##@date
#     - 2020-01-29 (Creation)
#     - 2020-04-18 (Last modification)
#
# User interface designed for intuitive control and monitoring of the
# HexaphobUS robot.
#
# <b>S4-H20 | GRO400</b>

##@mainpage HexaphobUS
#
##@section redirect Links
#          - Python page: @ref page_py "UI on Python"
#          - Arduino page: @ref page_ino "PWM on Arduino"
#
#          For code documentation, have a look at the files <a
#          href="https://raw.githack.com/gabrielcabana21/HexaphobUS/code/docs/html/files.html">here</a>.

##@page page_py UI on Python
#
##@section intro_sec Introduction
#          Python code designed for intuitive control and monitoring of the
#          HexaphobUS robot.
#
#          The user interface works on a on-board computer
#          (<a href="https://www.raspberrypi.org/products/raspberry-pi-3-model-b/">
#          Raspberry Pi 3B</a>) to communicate, with a standard modulation rate
#          of 9600.
#
##@section dependencies Dependencies
#          This package depends on <a href="https://pypi.org/project/PyQt5/>
#          PyQt5</a>, <a href="https://pypi.org/project/pyserial/">pyserial</a>,
#          and other standard modules being present on your system.
#          Please make sure you have installed the latest versions before using
#          this interface.
#
##@section license License
#          This falls under the GitHub repository license (GPLv3). See more
#          <a href="https://github.com/gabrielcabana21/HexaphobUS/blob/master/LICENSE">
#          here</a>.

#********************************************#

# Computing
import math

# System
import os
import sys

# Timing and communication
import serial
from threading import Timer
import time

# User interface
from PyQt5.QtCore import Qt, QTimer, QPoint, QThread
from PyQt5.QtGui import (QColor, QIcon, QPainter, QPalette, QKeySequence,
                         QDoubleValidator, QPixmap)
from PyQt5.QtWidgets import (QApplication, QGridLayout, QHBoxLayout, QLabel,
                             QLineEdit, QPushButton, QVBoxLayout, QWidget,
                             QShortcut, QFrame, QSizePolicy)

#********************************************#


## Servomotor number (to define positioning and sequencing)
Servos_Num = [7, 9, 11, 1, 3, 5, 2, 4, 6, 8, 10, 12]
## Servomotor widgets (to display angular position)
SERVO_TABLE = list()

## Number of servomotors
SV_NBR = len(Servos_Num)
## Servomotor window width
SV_W = 100
## Servomotor window height
SV_H = 30
## Arrow button width
ARROW_W = 60
## Arrow button height
ARROW_H = 30
## Button width
BUTTON_W = 120
## Button height
BUTTON_H = 60
## Information window width
INFO_W = 190
## Information window height
INFO_H = 30
## Position tracking window width
TRACK_W = 250
## Position tracking window height
TRACK_H = 50
## User interface position (x)
UI_X = 80
## User interface position (y)
UI_Y = 80
## User interface width
UI_W = 800
## User interface height
UI_H = 600
## User interface width (minimum)
UI_MIN_W = 480
## User interface height (minimum)
UI_MIN_H = 360

## Flag (command number to define next position to reach)
NB_COMMAND = 0
## Movement command (in bytes)
ENCODED_VAR = b'55'
## Communication port directory
PORT = "/dev/ttyACM0"
## Modulation rate
BAUD_RATE = 9600
## Serial communication update rate
SERIAL_UPDATE_RATE = 0.1

## User interface window name
WINDOW_NAME = "HexaphobUS UI"
## Up arrow unicode string
BUTTON_UP = "\u2191"
## Down arrow unicode string
BUTTON_DOWN = "\u2193"
## Left arrow unicode string
BUTTON_LEFT = "\u2190"
## Right arrow unicode string
BUTTON_RIGHT = "\u2192"
## Movement string (forward)
STR_F = "FORWARD"
## Movement string (backward)
STR_B = "BACKWARD"
## Movement string (left)
STR_L = "LEFT"
## Movement string (right)
STR_R = "RIGHT"
## Serial communication string
BUTTON_SERIAL = "Start Serial"
## Initialization button string
BUTTON_INIT = "POS INIT"
## Program button string
BUTTON_PRG1 = "PRG1"
## Second level parent folder
SCRIPT_DIR = os.path.abspath(os.path.join(os.path.dirname(__file__),
                                          "..", ".."))
## Separator (OS dependent)
SEP = os.path.sep
## User interface logo directory
LOGO = 'img' + SEP + 'hexaphobus_logo.png'

#********************************************#


def stringToByte(string):
    """
    Encodes string and returns byte values.
    """
    string = string + '|'
    encoded_string = string.encode()
    return encoded_string

def byteToString(encoded_string):
    """
    Decodes byte values and returns a string.
    """
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

    def serialReceive(self):
        """
        Gets the information from the serial port.
        """
        global SERVO_TABLE

        if self.ser.in_waiting > 0:
            try:
                stringData = self.ser.read_until()
            except:
                return

            servoAngle = byteToString(stringData)
            
            try:
                tableData = servoAngle.split(";")
            except:
                print("Erreur")

            if len(tableData) == SV_NBR:
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

        if direction == STR_F and self._robot_y_pos > 0:
            self._robot_y_pos -= self._speed
            ENCODED_VAR = stringToByte('FORWARD')
        elif direction == STR_B and self._robot_y_pos < UI_Graph_H:
            self._robot_y_pos += self._speed
            ENCODED_VAR = stringToByte('BACKWARD')
        elif direction == STR_R and self._robot_x_pos < UI_Graph_W:
            self._robot_x_pos += self._speed
            ENCODED_VAR = stringToByte('RIGHT')
        elif direction == STR_L and self._robot_x_pos > 0:
            self._robot_x_pos -= self._speed
            ENCODED_VAR = stringToByte('LEFT')

        self.moveRobot(self._robot_x_pos, self._robot_y_pos)
        self.update()

    def moveRobot(self, robotX, robotY):
        """
        Event that updates the target's distance and position label
        relative to the origin according to the target's movements.
        """
        self._distance_origin = round(
            ((robotY - self._target_y_pos)**2
             + (robotX - self._target_x_pos)**2)**0.5
        )

        self.label.setText(
            "Coordinates (x; y): ({}; {})\nDistance from origin: {}"
            .format(robotX, robotY, self._distance_origin)
        )

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
        q.drawLine(self._robot_x_pos, self._robot_y_pos,
                   self._target_x_pos, self._target_y_pos)
        moving_command = byteToString(ENCODED_VAR)
        
        '''
        if moving_command == STR_F or moving_command == STR_B:
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
        '''

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
        self.tracking.setSizePolicy(
            QSizePolicy.Expanding, QSizePolicy.Expanding
        )
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
        self.button_prog.clicked.connect(self.printVariable)
        self.button_init.clicked.connect(self.tracking.initPosition)
        self.button_up.clicked.connect(
            lambda: self.tracking.changePosition(STR_F)
        )
        self.shortcut_up.activated.connect(
            lambda: self.tracking.changePosition(STR_F)
        )
        self.button_up.clicked.connect(
            lambda: self.serialCheck.serialSend(STR_F)
        )
        self.shortcut_up.activated.connect(
            lambda: self.serialCheck.serialSend(STR_F)
        )
        self.button_down.clicked.connect(
            lambda: self.tracking.changePosition(STR_B)
        )
        self.shortcut_down.activated.connect(
            lambda: self.tracking.changePosition(STR_B)
        )
        self.button_down.clicked.connect(
            lambda: self.serialCheck.serialSend(STR_B)
        )
        self.shortcut_down.activated.connect(
            lambda: self.serialCheck.serialSend(STR_B)
        )
        self.button_left.clicked.connect(
            lambda: self.tracking.changePosition(STR_L)
        )
        self.shortcut_left.activated.connect(
            lambda: self.tracking.changePosition(STR_L)
        )
        self.button_left.clicked.connect(
            lambda: self.serialCheck.serialSend(STR_L)
        )
        self.shortcut_left.activated.connect(
            lambda: self.serialCheck.serialSend(STR_L)
        )
        self.button_right.clicked.connect(
            lambda: self.tracking.changePosition(STR_R)
        )
        self.shortcut_right.activated.connect(
            lambda: self.tracking.changePosition(STR_R)
        )
        self.button_right.clicked.connect(
            lambda: self.serialCheck.serialSend(STR_R)
        )
        self.shortcut_right.activated.connect(
            lambda: self.serialCheck.serialSend(STR_R)
        )

    def cleanUp(self):
        """
        Ends the serial communication.
        """
        print("Exiting program...")
        self.serialCheck.serialQuit()
        self.repeater.stop()
        print("END")

    def removeButton(self):
        """
        Disables the serial communication button after initialization.
        """
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

    def printVariable(self):
        """
        Displays variable in the corresponding widget.
        """
        print(ENCODED_VAR)

    def setServoValues(self):
        """
        Set the servomotor edit text.
        """
        self.serialCheck.serialReceive()
        for (angle, servo) in zip(SERVO_TABLE, self._servo_edits):
            servo.setText(angle)
            time.sleep(0.05)

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
    ## Create a Qt application and window to display.
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon(SCRIPT_DIR + SEP + LOGO))

    ## Create the widget window.
    window = MainWindow()
    app.aboutToQuit.connect(window.cleanUp)

    ## Style setup
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
