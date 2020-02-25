

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
    Serial.print("test");
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
}

void loop() {
   init_mouv();   
   delay(20000000000);
   int B_MovingForward=1;
   MovingForward(B_MovingForward,3);
   delay(2000);
   B_MovingForward=0;
   int B_MovingBackwards=1;
   MovingBackwards(B_MovingBackwards,3);
   delay(2000);
   B_MovingBackwards=0;
   delay(2000);
   //int B_MovingRight=1;
   //MovingRight(B_MovingRight,3);
   //B_MovingRight=0;*/

  /*
  ForwardAndBackwards(1,4,5,POS_BACK, POS_BACK, POS_BACK);
  ForwardAndBackwards(2,3,6,POS_FRONT, POS_FRONT, POS_FRONT);
  delay(500);
  ForwardAndBackwards(1,4,5,POS_CENTER, POS_CENTER, POS_CENTER);
  ForwardAndBackwards(2,3,6,POS_CENTER, POS_CENTER, POS_CENTER);
  delay(500);
  ForwardAndBackwards(1,4,5,POS_FRONT, POS_FRONT, POS_FRONT);
  ForwardAndBackwards(2,3,6,POS_BACK, POS_BACK, POS_BACK);
  delay(500);
  ForwardAndBackwards(1,4,5,POS_CENTER, POS_CENTER, POS_CENTER);
  ForwardAndBackwards(2,3,6,POS_CENTER, POS_CENTER, POS_CENTER);
  delay(500);
  
  /*
  UpAndDown(1,4,5,POS_UP, POS_UP, POS_UP);
  UpAndDown(1,4,5,POS_DOWN, POS_DOWN, POS_DOWN);

  delay(200);
  UpAndDown(2,3,6,POS_UP, POS_UP, POS_UP);
  UpAndDown(2,3,6,POS_DOWN, POS_DOWN, POS_DOWN);

  /*
  pwm.setPWM(1, 0, pulseWidth(18-15));
  pwm.setPWM(2, 0, pulseWidth(180-0));
  pwm.setPWM(3, 0, pulseWidth(68));
  pwm.setPWM(4, 0, pulseWidth(180-0));
  pwm.setPWM(5, 0, pulseWidth(0));
  pwm.setPWM(6, 0, pulseWidth(180-10));
  pwm.setPWM(7, 0, pulseWidth(0));
  pwm.setPWM(8, 0, pulseWidth(180-0));
  pwm.setPWM(9, 0, pulseWidth(0));
  pwm.setPWM(10, 0, pulseWidth(180-0));
  pwm.setPWM(11, 0, pulseWidth(0));
  pwm.setPWM(12, 0, pulseWidth(180-0));/*
  /*UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
  UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
  delay(1000);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);*/


}



  
