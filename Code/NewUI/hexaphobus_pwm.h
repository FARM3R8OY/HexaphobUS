 /*!
 * @file hexaphobus_pwm.h
 * 
 * @authors
 *          - Boulet,       Philippe | boup2341
 *          - Cabana,       Gabriel  | cabg2101
 *          - Guay-Tanguay, Carolane | guac3201
 *          - Lalonde,      Philippe | lalp2803
 *          - Roy,          Olivier  | royo2206
 * 
 * @date
 *       - 2020-02-25 (Creation)
 *       - 2020-03-27 (Last modification)
 * 
 * Motion control, communication, and sequencing (header).
 * 
 * <b>S4-H20 | GRO400</b>
 */

/********************************************/

#  include <Adafruit_PWMServoDriver.h>

/// Encoder value (min).
#  define MIN_PULSE_WIDTH     650
/// Encoder value (max).
#  define MAX_PULSE_WIDTH     2650
/// Encoder value (calibrated zero).
#  define DEFAULT_PULSE_WIDTH 1500
/// Communication frequency.
#  define FREQUENCY           50


/// Communication port with the PWM servo driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

void motorCommand(int motorId, int motorAngle){
  pwm.setPWM(motorId, 0, pulseWidth(motorAngle));
}

int getMotorId(String input_string){
  int motorId = int(input_string.substring(0,2));
  return motorId;
}

int getMotorAngle(String input_string){
  int motorAngle = int(input_string.substring(3);
  return motorAngle;
}

String readSerial()
{
  String input_string;
  if (Serial.available() > 0)
    input_string = Serial.readStringUntil('|');
  else
    input_string = "None";
  return input_string;
}

void writeSerial(String output_string)
{
  if(output_string.length() > 0)
    Serial.println(output_string);
}

int pulseWidth(int angle) {
  if(angle>180)
  {return -1;}
  
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  
  return analog_value;
}
