/*!
 * @file test_hexaphobus_pwm.ino
 * 
 * @authors
 *          - Cabana,       Gabriel  | cabg2101
 *          - Guay-Tanguay, Carolane | guac3201
 *          - Lalonde,      Philippe | lalp2803
 * 
 * @date
 *       - 2020-02-25 (Creation)
 *       - 2020-03-25 (Last modification)
 * 
 * Motion control, communication, and sequencing (test).
 * 
 * <b>S4-H20 | GRO400</b>
 */

/********************************************/

#  include <AUnit.h>
#  include <AUnitVerbose.h>
#  include "../hexaphobus_pwm.h"

/********************************************/

/*!
 * @brief Sets up the Arduino Mega 2560 R3.
 *
 *        This function sets the serial data transmission, the I2C
 *        communication with the PWM servo driver, and the update
 *        frequency.
 */
void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(FREQUENCY);
}

/*!
 * @brief Runs the Arduino Mega 2560 R3.
 *
 *        This function sets the test-running class.
 */
void loop() {
  TestRunner::run();
}

/*!
 * @brief Test macro.
 *
 *        This function defines all the tests that must be run once to
 *        validate the "hexaphobus_pwm" header code.
 */
test(RaiseLeg) {
  assertEqual(UpAndDown(1, 4, 5, POS_UP, POS_UP, POS_UP), 0);
  assertEqual(FowardAndBackwards(1, 4, 5, POS_UP, POS_UP, POS_UP), 0);
  assertEqual(UpAndDown(1, 4, -1, POS_UP, POS_UP, POS_UP), -1);
  assertEqual(UpAndDown(1, -1, 5, POS_UP, POS_UP, POS_UP), -1);
  assertEqual(FowardAndBackwards(-1, 1, 5, POS_UP, POS_UP, POS_UP), -1);
  assertEqual(UpAndDown(1, 4, 5, -2, POS_UP, POS_UP), 0);
  assertEqual(FowardAndBackwards(1, 4, 5, -2, POS_UP, POS_UP), 0);
}
