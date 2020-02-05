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
import sys
from hexaphobus_ui import hexaphobus_ui

# --------------------------------------------


class TestHexaphobusUI(unittest.TestCase):
    """
    The 'TestHexaphobusUI' class is a TestCase subclass that allows the
    user to automate the protocol verification of the user interface
    module, 'hexaphobus_ui.py'.
    """

    def test_is_vowel(self):
        for l in ['a', 'e', 'i', 'o', 'u', 'y']:
            self.assertTrue(wordstats.is_vowel(l),
                            "Is not a vowel :" + l)
            self.assertTrue(wordstats.is_vowel(l.upper()),
                            "Is not a vowel :" + l.upper())

        self.assertFalse(wordstats.is_vowel('F'),
                         "Is a vowel : F")

    def test_start_w_vowel(self):
        self.assertTrue(wordstats.starts_w_vowel("Aim"),
                        "Does not start with a vowel: 'Aim'")
        self.assertFalse(wordstats.starts_w_vowel("False"),
                         "Starts with a vowel: 'False'")

    def test_ends_w_cons(self):
        self.assertTrue(wordstats.ends_w_cons("Class"),
                        "Doesn't end with a consonna: 'Class'")
        self.assertFalse(wordstats.ends_w_cons("True"),
                         "Ends with a consonna: 'True'")

    def test_vowel_filter(self):
        words = sentence.split()
        words_vowels = wordstats.words_start_w_vowel(words)
        self.assertEqual(words_vowels, ['yellow', 'in'])

    def test_cons_filter(self):
        words = sentence.split()
        words_cons = wordstats.words_end_w_cons(words)
        self.assertEqual(words_cons, ['yellow', 'in', 'backyard'])

if __name__ == '__main__':
    unittest.main()
