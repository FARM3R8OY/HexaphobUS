/*!
 * @file hexaphobus_pwm.ino
 * 
 * @authors
 *          - Cabana,       Gabriel  | cabg2101
 *          - Guay-Tanguay, Carolane | guac3201
 *          - Lalonde,      Philippe | lalp2803
 *          - Roy,          Olivier  | royo2206
 * 
 * @date
 *       - 2020-01-29 (Creation)
 *       - 2020-03-27 (Last modification)
 * 
 * Motion control, communication, and sequencing (main).
 * 
 * <b>S4-H20 | GRO400</b>
 */

/*!
 * @page page_ino PWM on Arduino
 *
 * @section intro_sec Introduction
 *          Arduino code designed for intuitive control and monitoring of the
 *          HexaphobUS robot. 
 * 
 *          The servomotors use an I2C-bus controller
 *          (<a href="http://wiki.sunfounder.cc/index.php?title=PCA9685_16_Channel_12_Bit_PWM_Servo_Driver">
 *          Sunfounder PCA9685</a>) to communicate, with a 12-bit resolution
 *          (4096 steps).
 *
 * @section dependencies Dependencies
 *          This library depends on
 *          <a href="https://github.com/adafruit/Adafruit-PWM-Servo-Driver-Library">
 *          Adafruit PWM Servo Driver Library</a> being present on your system.
 *          Please make sure you have installed the latest version before using
 *          this library.
 *
 * @section license License
 *          This falls under the GitHub repository license (GPLv3). See more
 *          <a href="https://github.com/gabrielcabana21/HexaphobUS/blob/master/LICENSE">
 *          here</a>. 
 */

/********************************************/

#  include <Wire.h>
#  include <Adafruit_PWMServoDriver.h>
#  include <Arduino.h>

#  include "hexaphobus_pwm.h" ///< Header of custom functions.

/********************************************/

uint8_t servonum = 0; ///< Servomotor counter.
int State = 0; ///State of the robot
/*
IDLE = 0
FORWARD = 1
BACKWARD = 2
LEFT = 3
RIGHT = 4
*/

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
  delay(1000);
  AngleToHMI();
  init_move();
}

/*!
 * @brief Runs the Arduino Mega 2560 R3.
 * 
 *        This function calibrates the servomotors and allows the robot
 *        to move according to user input.
 */
void loop() { 
  
   
  //showNewData();
  
  State = UpdateCommand();
  if (State != 0)
  {
    if (State == 1)
    {
      //Move forward
      Moving(POS_FRONT);
    }
    if (State == 2)
    {
      //Move Backward
      Moving(POS_BACK);
    }
    if (State == 3)
    {
      //Move Left
      MovingLeft();
    }
    if (State == 4)
    {
      //Move Right
      MovingRight();
    }
  }

  delay(100);
   
  /*
   delay(1000);
   int B_Moving=1;
   Moving(B_Moving,2,POS_FRONT);
   delay(2000);
   B_Moving=1;
   Moving(B_Moving,2,POS_BACK);
   B_Moving=1;
   delay(2000);
   MovingRight(B_Moving,7);
   B_Moving=1;
   delay(2000);
   MovingLeft(B_Moving,7);
   delay(2000);
   */
}
