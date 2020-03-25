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
 *       - 2020-03-24 (Last modification)
 * 
 * @details Motion control, communication, and sequencing (test).
 * 
 * <b>S4-H20 | GRO400</b>
 */

/********************************************/

#  include <AUnit.h>
#  include <AUnitVerbose.h>
#  include "../hexaphobus_pwm.h"

/********************************************/

void setup() {
    Serial.begin(9600);
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
}

void loop() {
    TestRunner::run();
}

test(RaiseLeg) {
    assertEqual(UpAndDown(1, 4, 5, POS_UP, POS_UP, POS_UP), 0);
    assertEqual(FowardAndBackwards(1, 4, 5, POS_UP, POS_UP, POS_UP), 0);
    assertEqual(UpAndDown(1, 4, -1, POS_UP, POS_UP, POS_UP), -1);
    assertEqual(UpAndDown(1, -1, 5, POS_UP, POS_UP, POS_UP), -1);
    assertEqual(FowardAndBackwards(-1, 1, 5, POS_UP, POS_UP, POS_UP), -1);
    assertEqual(UpAndDown(1, 4, 5, -2, POS_UP, POS_UP), 0);
    assertEqual(FowardAndBackwards(1, 4, 5, -2, POS_UP, POS_UP), 0);
}








