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
#     - 2020-04-17 (Last modification)
#
# User interface designed for intuitive control and monitoring of the
# HexaphobUS robot.
#
# <b>S4-H20 | GRO400</b>

#********************************************#

import math
import os
import sys
import time

from threading import Timer
import serial

from PyQt5 import QtSerialPort

from PyQt5.QtCore import Qt, QTimer, QPoint, QThread, pyqtSlot, QIODevice
from PyQt5.QtGui import (QColor, QIcon, QPainter, QPalette, QKeySequence,
                         QDoubleValidator, QPixmap)
from PyQt5.QtWidgets import (QApplication, QGridLayout, QHBoxLayout, QLabel,
                             QLineEdit, QPushButton, QVBoxLayout, QWidget,
                             QShortcut, QFrame, QSizePolicy)

#********************************************#

## Servomotor number (to define positioning and sequencing)
Servos_Num = [7, 9, 11, 1, 3, 5, 2, 4, 6, 8, 10, 12]

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

## Servomotor angle (down position)
DOWN = 65-15
## Servomotor angle (up position)
UP = 25-15
## Servomotor angle (back position)
BACK = 10
## Servomotor angle (front position)
FRONT =  75
## Servomotor angle (central position)
CENTER = 43
## Servomotor encoder compensation (for calibration)
SHIFT = [-1, -10, -5, 0, 0, -5, 5, 56, 68, 58, 56, 56, 68];

## Flag (command number to define next position to reach)
NB_COMMAND = 0
## Flag (communication ready)
IS_READY = "0"

#global FORWARD, BACKWARD, LEFT, RIGHT
## Flag (forward movement)
FORWARD = False
## Flag (backward movement)
BACKWARD = False
## Flag (left turn)
LEFT = False
## Flag (right turn)
RIGHT = False

## Communication port directory
PORT = "/dev/ttyACM0"

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

#********************************************#

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
        global FORWARD,BACKWARD,LEFT,RIGHT
        UI_Graph_W = self.geometry().width()
        UI_Graph_H = self.geometry().height()

        if direction == "FORWARD" and self._robot_y_pos > 0:
            self._robot_y_pos -= self._speed
            # Send direction to call program in Arduino
            FORWARD = True
        elif direction == "BACKWARD" and self._robot_y_pos < UI_Graph_H:
            self._robot_y_pos += self._speed
            # Send direction to call program in Arduino
            BACKWARD = True
        elif direction == "RIGHT" and self._robot_x_pos < UI_Graph_W:
            self._robot_x_pos += self._speed
            # Send direction to call program in Arduino
            RIGHT = True
        elif direction == "LEFT" and self._robot_x_pos > 0:
            self._robot_x_pos -= self._speed
            # Send direction to call program in Arduino
            LEFT = True

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

        #Suggest to use feedback from the angles of servomotor
        if FORWARD or BACKWARD:
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
        SERVO_TABLE = list()

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

        self.ser = QtSerialPort.QSerialPort(
            PORT,
            baudRate = QtSerialPort.QSerialPort.Baud57600,
            readyRead = self.serialReceive
        )
        self.ser.open(QIODevice.ReadWrite)


        self._timer = QTimer(self)
        self._timer.start(100)
        self._timer.timeout.connect(self.checkSerialState)


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

    def checkSerialState(self):
        global step
        if IS_READY == "69":
            print("check")
            step += 1
            if ForwardActivated:
                self.moveForward(step)
            elif BackwardActivated:
                self.moveBackward(step)
            elif LeftActivated:
                self.moveLeft(step)
            elif RightActivated:
                self.moveRight(step)
            IS_READY = "0"


    @pyqtSlot()
    def serialReceive(self):
        """
        Gets the information from the serial port.
        """
        global IS_READY
        try:
            stringData = self.ser.read_until()
            print("lalal")
            
        except:
            return
        IS_READY = byteToString(stringData)
        

    def serialSend(self, motor, angle):
        """
        Sends the information to the serial port.
        """
        motor = str(motor)
        angle = str(angle)

        if len(angle) == 1:
            angle = "00" + angle
        elif len(angle) == 2:
            angle = "0" + angle
        
        if len(motor) == 1:
            motor = "0" + motor
            
        commandString = motor + ';' + angle
        bytesData = stringToByte(commandString)
        self.ser.write(bytesData)


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

        self.button_init.setFixedSize(BUTTON_W, BUTTON_H)
        self.button_prog.setFixedSize(BUTTON_W, BUTTON_H)

    def setConnections(self):
        """
        Connect the buttons and shortcuts to the corresponding
        functions.
        """
        self.button_prog.clicked.connect(self.runProgram)
        self.button_init.clicked.connect(self.tracking.initPosition)
        self.button_up.clicked.connect(
            lambda: self.tracking.changePosition("FORWARD")
        )
        self.shortcut_up.activated.connect(
            lambda: self.tracking.changePosition("FORWARD")
        )
        self.button_up.clicked.connect(
            lambda: self.moveForward(0)
        )
        self.shortcut_up.activated.connect(
            lambda: self.moveForward(0)
        )
        self.button_down.clicked.connect(
            lambda: self.tracking.changePosition("BACKWARD")
        )
        self.shortcut_down.activated.connect(
            lambda: self.tracking.changePosition("BACKWARD")
        )
        self.button_down.clicked.connect(
            lambda: self.moveBackward(0)
        )
        self.shortcut_down.activated.connect(
            lambda: self.moveBackward(0)
        )
        self.button_right.clicked.connect(
            lambda: self.tracking.changePosition("RIGHT")
        )
        self.shortcut_right.activated.connect(
            lambda: self.tracking.changePosition("RIGHT")
        )
        self.button_right.clicked.connect(
            lambda: self.moveRight(0)
        )
        self.shortcut_right.activated.connect(
            lambda: self.moveRight(0)
        )
        self.button_left.clicked.connect(
            lambda: self.tracking.changePosition("LEFT")
        )
        self.shortcut_left.activated.connect(
            lambda: self.tracking.changePosition("LEFT")
        )
        self.button_left.clicked.connect(
            lambda: self.moveLeft(0)
        )
        self.shortcut_left.activated.connect(
            lambda: self.moveLeft(0)
        )


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
        global IS_READY
        IS_READY = "69"
        self.checkSerialState()

    def setServoValues(self, pos, angle):
        """
        Set the servomotor edit text.
        """
        if pos == 7:
            pos = 0
        elif pos == 9:
            pos = 1
        elif pos == 11:
            pos = 2
        elif pos == 1:
            pos = 3
        elif pos == 3:
            pos = 4
        elif pos == 2:
            pos = 6
        elif pos == 4:
            pos = 7
        elif pos == 6:
            pos = 8
        elif pos == 8:
            pos = 9
        elif pos == 12:
            pos = 11

        self._servo_edits[pos].setText(str(angle))

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

    def moveForward(self,index):
        """
        Sequencing for moving forward.
        """
        global ForwardActivated, step
        motorOrder = [8,9,12,2,5,6,1,4,5,8,9,12,7,10,11,1,4,5,2,3,
                    6,7,10,11]
        posOrder = [UP,UP,UP,FRONT,FRONT,FRONT,BACK,BACK,BACK,DOWN,
                    DOWN,DOWN,UP,UP,UP,FRONT,FRONT,FRONT,BACK,BACK,
                    BACK,DOWN,DOWN,DOWN]

        print(motorOrder[index])
        if index == 0:
            ForwardActivated = True
        elif index == len[motorOrder]+1:
            step = 0
            ForwardActivated = False
            return
        self.serialSend(motorOrder[index],
                        posOrder[index]+SHIFT[motorOrder[index]-1])
        self.setServoValues(motorOrder[index],
                            posOrder[index]+SHIFT[motorOrder[index]-1]) 


    def moveBackward(self,index):
        """
        Sequencing for moving backward.
        """
        global BackwardActivated, step
        motorOrder = [8,9,12,2,5,6,1,4,5,8,9,12,7,10,11,1,4,5,2,3,
                    6,7,10,11]
        posOrder = [UP,UP,UP,BACK,BACK,BACK,FRONT,FRONT,FRONT,DOWN,
                    DOWN,DOWN,UP,UP,UP,BACK,BACK,BACK,FRONT,FRONT,
                    FRONT,DOWN,DOWN,DOWN]
        if index == 0:
            BackwardActivated = True
        elif index == len[motorOrder]+1:
            step = 0
            BackwardActivated = False
            return 
        self.serialSend(motorOrder[index],
                        posOrder[index]+SHIFT[motorOrder[index]-1])
        self.setServoValues(motorOrder[index],
                            posOrder[index]+SHIFT[motorOrder[index]-1]) 


    def moveLeft(self,index):
        """
        Sequencing for turning left.
        """
        global LeftActivated, step
        motorOrder = [8,9,12,2,6,3,1,5,4,8,9,12,7,10,11,1,5,4,2,6,
                    3,7,10,11]
        posOrder = [UP,UP,UP,FRONT,FRONT,BACK,FRONT,FRONT,BACK,DOWN,
                    DOWN,DOWN,UP,UP,UP,BACK,BACK,FRONT,BACK,BACK,
                    FRONT,DOWN,DOWN,DOWN]
        if index == 0:
            LeftActivated = True
        elif index == len[motorOrder]+1:
            step = 0
            LeftActivated = False
            return
        self.serialSend(motorOrder[index],
                        posOrder[index]+SHIFT[motorOrder[index]-1])
        self.setServoValues(motorOrder[index],
                            posOrder[index]+SHIFT[motorOrder[index]-1]) 
        

    def moveRight(self,index):
        """
        Sequencing for turning right.
        """
        global RightActivated, step
        motorOrder = [8,9,12,2,6,3,1,5,4,8,9,12,7,10,11,1,5,4,2,6,
                    3,7,10,11]
        posOrder = [UP,UP,UP,BACK,BACK,FRONT,BACK,BACK,FRONT,DOWN,
                    DOWN,DOWN,UP,UP,UP,FRONT,FRONT,BACK,FRONT,FRONT,
                    BACK,DOWN,DOWN,DOWN]
        if index == 0:
            RightActivated = True
        elif index == len[motorOrder]+1:
            step = 0
            RightActivated = False
            return
        self.serialSend(motorOrder[index],
                        posOrder[index]+SHIFT[motorOrder[index]-1])
        self.setServoValues(motorOrder[index],
                            posOrder[index]+SHIFT[motorOrder[index]-1]) 


#********************************************#


if __name__ == '__main__':
    # Create a Qt application and window to display.
    app = QApplication(sys.argv)
    app.setWindowIcon(QIcon(SCRIPT_DIR + SEP + LOGO))
    window = MainWindow()

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
