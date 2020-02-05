#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2650
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50
// our servo # counter
uint8_t servonum = 0;

void setup() {
    Serial.begin(9600);
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
}

void loop() {
  
  pwm.setPWM(0, 0, pulseWidth(0));
  pwm.setPWM(1, 0, pulseWidth(0));
  pwm.setPWM(6, 0, pulseWidth(0));
  pwm.setPWM(7, 0, pulseWidth(0));
  pwm.setPWM(8, 0, pulseWidth(0));
  pwm.setPWM(9, 0, pulseWidth(0));
  pwm.setPWM(10, 0, pulseWidth(0));
  pwm.setPWM(11, 0, pulseWidth(0));
  pwm.setPWM(12, 0, pulseWidth(0));
  pwm.setPWM(13, 0, pulseWidth(0));
  pwm.setPWM(14, 0, pulseWidth(0));
  pwm.setPWM(15, 0, pulseWidth(0));
  delay(2000);
  pwm.setPWM(0, 0, pulseWidth(0));
  pwm.setPWM(1, 0, pulseWidth(0));
  pwm.setPWM(6, 0, pulseWidth(0));
  pwm.setPWM(7, 0, pulseWidth(0));
  pwm.setPWM(8, 0, pulseWidth(0));
  pwm.setPWM(9, 0, pulseWidth(0));
  pwm.setPWM(10, 0, pulseWidth(0));
  pwm.setPWM(11, 0, pulseWidth(0));
  pwm.setPWM(12, 0, pulseWidth(0));
  pwm.setPWM(13, 0, pulseWidth(0));
  pwm.setPWM(14, 0, pulseWidth(0));
  pwm.setPWM(15, 0, pulseWidth(0));
  delay(2000);

  //test=angleWidth(650);


  
  
  //Serial.println(angle_moteur);
  //angleWidth(angle_moteur);
  
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  Serial.println(analog_value);
  return analog_value;
}

int angleWidth(int pulse)
{
  int angle_wide, analog_value;
  angle_wide = map(pulse, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0 ,180);
  Serial.println("L'angle du moteur");
  return angle_wide;
}