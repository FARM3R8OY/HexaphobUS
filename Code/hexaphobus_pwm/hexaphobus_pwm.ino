

/*
File: hexaphobus_pwm.ino

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Lalonde,      Philippe | lalp2803
    Roy,          Olivier  | royo2206

Date(s):
    2020-01-29 (Creation)
    2020-02-19 (Last modification)

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
  int B_MovingForward=1;
  MovingForward_test(B_MovingForward,5);
  //MovingForward_test2(B_MovingForward,5,POS_BACK);
  B_MovingForward=0;


}



  
