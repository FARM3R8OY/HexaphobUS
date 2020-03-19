/*
File: hexaphobus_pwm.ino

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Lalonde,      Philippe | lalp2803
    Roy,          Olivier  | royo2206

Date(s):
    2020-01-29 (Creation)
    2020-03-11 (Last modification)

Description:
    Arduino code designed for intuitive control and monitoring of the
    HexaphobUS robot.

S4-H20 | GRO400
*/

/********************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
#include <Arduino.h>
#include "functionDefinition.h"

/********************************************/
  
// our servo # counter
uint8_t servonum = 0;



/********************************************/

void setup() {
    Serial.begin(9600);
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
}

void loop() {  
/*
  pwm.setPWM(1, 0, pulseWidth(90));
  Serial.println(pwm.getPWM(1))
  */
  init_mouv();
  delay(1000);
  int B_Moving=1;
  Moving(B_Moving,5,POS_FRONT);
  delay(2000);
  B_Moving=1;
  Moving(B_Moving,5,POS_BACK);
  B_Moving=1;
  delay(2000);
  MovingRight(B_Moving,10);
  B_Moving=1;
  delay(2000);
  MovingLeft(B_Moving,10);
  delay(2000);
}



  
