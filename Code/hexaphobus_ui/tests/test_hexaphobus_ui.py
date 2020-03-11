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

    def test_encoding_type(self):
        global encoded_data
        encoded_data = pack_string('ABCDE')

    def test_decoding_type(self):
        my_string = unpack(encoded_data)
        self.assertTrue(my_string.isString())

    def test_encoding_size(self):
        my_string = unpack(encoded_data)
        self.assertEqual(len(my_string),5)

if __name__ == '__main__':
    unittest.main()
