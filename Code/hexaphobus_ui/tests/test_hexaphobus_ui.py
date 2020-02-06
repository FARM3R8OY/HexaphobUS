"""
File: test_hexaphobus_ui.py

Contributor(s):
    Cabana,  Gabriel  | cabg2101

Date(s):
    2020-02-05 (Creation)

Description:
    Tests for user interface.

S4-H20 | GRO400
"""

# --------------------------------------------

import unittest
from .. import hexaphobus_ui

# --------------------------------------------

TEST_SERVO_ANGLES = [str(x) for x in range(1, 13)]

# --------------------------------------------


class TestHexaphobusUI(unittest.TestCase):
    """
    The 'TestHexaphobusUI' class is a TestCase subclass that allows the
    user to automate the protocol verification of the user interface
    module, 'hexaphobus_ui.py'.
    """
    def __init__(self):
        super().__init__()
        self._window = hexaphobus_ui.MainWindow()

    def test_servoValues(self, values):
        self._window.setServoValues(values)

        for count, edit in enumerate(self._window.getServoEdits()):
            self.assertEqual(values[count], edit.text())

if __name__ == '__main__':
    unittest.main()
