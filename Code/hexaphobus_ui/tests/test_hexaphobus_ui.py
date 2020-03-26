"""
File: test_hexaphobus_ui.py

Contributor(s):
    Cabana,  Gabriel  | cabg2101
    Lalonde, Philippe | lalp2803

Date(s):
    2020-02-05 (Creation)
    2020-03-11 (Last modification)

Description:
    Tests for user interface.

S4-H20 | GRO400
"""

# --------------------------------------------

import os
import sys
import unittest
sys.path.append(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
import hexaphobus_ui

# --------------------------------------------

TEST_SERVO_ANGLES = [str(x) for x in range(1, 13)]

# --------------------------------------------


class TestHexaphobusUI(unittest.TestCase):
    """
    The 'TestHexaphobusUI' class is a TestCase subclass that allows the
    user to automate the protocol verification of the user interface
    module, 'hexaphobus_ui.py'.
    """
    def __init__(self, *args, **kwargs):
        super().__init__(*args, **kwargs)
        # self._app = QApplication(sys.argv)
        # self._window = hexaphobus_ui.MainWindow()
        self._encoded_data = None

    # def test_servoValues(self):
    #     self._window.setServoValues(TEST_SERVO_ANGLES)
    #     for count, edit in enumerate(self._window.getServoEdits()):
    #         self.assertEqual(TEST_SERVO_ANGLES[count], edit.text())

    def test_encodingType(self):
        string = "ABCDE"
        self._encoded_data = hexaphobus_ui.stringToByte(string)
        self.assertTrue(isinstance(self._encoded_data, bytes))

    def test_decodingType(self):
        self._encoded_data = b'5550'
        string = hexaphobus_ui.byteToString(self._encoded_data)
        self.assertTrue(isinstance(string, str))

    def test_decodingSize(self):
        self._encoded_data = b'5550'
        string = hexaphobus_ui.byteToString(self._encoded_data)
        self.assertEqual(len(string), 4)

if __name__ == '__main__':
    unittest.main()
