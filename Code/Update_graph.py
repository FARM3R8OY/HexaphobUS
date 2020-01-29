import sys

from PyQt5.QtCore import QTimer
from PyQt5.QtWidgets import (QApplication, QLabel, QWidget, QPushButton)
from PyQt5.QtWidgets import (QVBoxLayout,QHBoxLayout,QGridLayout,QLineEdit)
from PyQt5.QtGui import QPainter

class HexaGraph(QWidget):
    distance_from_target = 0
    mouse_x_pos = 0
    mouse_y_pos = 0
    target_x_pos = 500
    target_y_pos = 250
    vel = 60  # pixels per second

    def __init__(self, parent=None):
        super().__init__(parent=parent)
        self.setMouseTracking(True)
        self.timer = QTimer(self)
        # self.timer.timeout.connect(self.changePosition)
        self.timer.start(1000 / self.vel)
        self.initUI()

    def initUI(self):
        self.setFixedSize(1000,500)
        self.label = QLabel(self)
        self.label.resize(500, 40)
        self.show()

    def changePosition(self):
        self.target_x_pos += 1
        self.update()

    def mouseMoveEvent(self, event):
        distance_from_target = round(
            ((event.y() - self.target_y_pos) ** 2 + (event.x() - self.target_x_pos) ** 2) ** 0.5)
        self.label.setText(
            'Coordinates: (%d : %d)' % (event.x(), event.y()) + "   Distance from target: " + str(distance_from_target))
        self.mouse_x_pos = event.x() #Suit la target de la souris
        self.mouse_y_pos = event.y()
        self.update()

    def mousePressEvent(self, event):
        self.target_x_pos = event.x()
        self.target_y_pos = event.y()
        self.update()

    def paintEvent(self, event):
        q = QPainter()
        q.begin(self)
        q.drawLine(self.mouse_x_pos, self.mouse_y_pos, self.target_x_pos, self.target_y_pos)

class HexaInterface(QWidget):

    def __init__(self, parent=None):
        super().__init__()
        self.initUI()

    def initUI(self):
        self.setGeometry(200, 200, 1000, 500)
        self.setWindowTitle('Mouse Tracker')

        self.Graph = HexaGraph(self)


        self.ButtonUp = QPushButton("UP",self)
        self.ButtonUp.setFixedSize(60, 20)
        self.ButtonDown = QPushButton("DOWN",self)
        self.ButtonDown.setFixedSize(60, 20)
        self.ButtonRight = QPushButton("RIGHT",self)
        self.ButtonRight.setFixedSize(60, 20)
        self.ButtonLeft = QPushButton("LEFT",self)
        self.ButtonLeft.setFixedSize(60, 20)
        self.ButtonStop = QPushButton("STOP",self)
        self.ButtonStop.setFixedSize(60,60)
        self.ButtonInit = QPushButton("POS INIT",self)
        self.ButtonInit.setFixedSize(60,60)
        self.ButtonProg = QPushButton("PROG 1",self)
        self.ButtonProg.setFixedSize(60,60)

        self.VitLabel = QLabel(self)
        self.VitLabel.setText("Vitesse :")
        self.EnerLabel = QLabel(self)
        self.EnerLabel.setText("Énergie :")
        self.VitEdit = QLineEdit(self)
        self.VitEdit.setFixedSize(190,30)
        self.EnerEdit = QLineEdit(self)
        self.EnerEdit.setFixedSize(190,30)

        self.BigLayout = QVBoxLayout(self)
        self.TopLineLayout = QHBoxLayout(self)
        self.BotLineLayout = QHBoxLayout(self)

        self.BigLayout.addLayout(self.TopLineLayout)
        self.BigLayout.addLayout(self.BotLineLayout)

        self.Servo1Layout = QVBoxLayout(self)
        self.GraphLayout = QVBoxLayout(self)
        self.Servo2Layout = QVBoxLayout(self)

        self.TopLineLayout.addLayout(self.Servo1Layout)
        self.TopLineLayout.addLayout(self.GraphLayout)
        self.TopLineLayout.addLayout(self.Servo2Layout)

        self.GraphLayout.addWidget(self.Graph)

        self.InfoLayout = QVBoxLayout(self)
        self.ProgLayout = QVBoxLayout(self)
        self.MoveLayout = QGridLayout(self)

        self.BotLineLayout.addLayout(self.InfoLayout)
        self.BotLineLayout.addLayout(self.ProgLayout)
        self.BotLineLayout.addLayout(self.MoveLayout)
        self.BotLineLayout.addWidget(self.ButtonStop)

        self.InfoLayout.addWidget(self.VitLabel)
        self.InfoLayout.addWidget(self.VitEdit)
        self.InfoLayout.addWidget(self.EnerLabel)
        self.InfoLayout.addWidget(self.EnerEdit)

        self.ProgLayout.addWidget(self.ButtonInit)
        self.ProgLayout.addWidget(self.ButtonProg)

        self.MoveLayout.addWidget(self.ButtonUp,0,1,1,1)
        self.MoveLayout.addWidget(self.ButtonDown,1,1)
        self.MoveLayout.addWidget(self.ButtonRight,1,0)
        self.MoveLayout.addWidget(self.ButtonLeft,1,2)
        self.show()


app = QApplication(sys.argv)
w = HexaInterface()
sys.exit(app.exec_())