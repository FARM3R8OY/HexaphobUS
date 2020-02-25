/*
File: tets_hexaphobus_pwm.ino

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Lalonde,      Philippe | lalp2803

Date(s):
    2020-02-25 (Creation)

Description:
    Test for arduino code designed for intuitive control and monitoring of the
    HexaphobUS robot.

S4-H20 | GRO400
*/

/********************************************/

#include <AUnit.h>
#include 'functionDefinition.h'

void setup()
{
    Serial.begin(9600);
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);
}

void loop()
{
    TestRunner::run();
}

test(RaiseLeg)
{
    assertEqual(UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP),0);
    assertEqual(FowardAndBackwards(1,4,5,POS_UP,POS_UP,POS_UP),0);
    assertEqual(UpAndDown(1,4,-1,POS_UP,POS_UP,POS_UP),-1);
    assertEqual(UpAndDown(1,-1,5,POS_UP,POS_UP,POS_UP),-1);
    assertEqual(FowardAndBackwards(-1,1,5,POS_UP,POS_UP,POS_UP),-1);
    assertEqual(UpAndDown(1,4,5,-2,POS_UP,POS_UP),0);
    assertEqual(FowardAndBackwards(1,4,5,-2,POS_UP,POS_UP),0);
}








