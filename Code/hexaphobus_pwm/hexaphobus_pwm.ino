#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> 

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2650
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

#define UP 45
#define DOWN 0
#define FRONT 90 
#define BACK 0
#define CENTER (FRONT-BACK)/2+BACK
  
// our servo # counter
uint8_t servonum = 0;

void setup() {
    Serial.begin(9600);
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
}

void loop() {
  ForwardAndBackwards(1,4,5,0);
  UpAndDown(1,4,5,1);
  ForwardAndBackwards(1,4,5,1);
  UpAndDown(1,4,5,0);

  init_mouv();
  
}

int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

int AnalogToAngle(int analog_value)
{
  int angle_wide, pulse_wide;
  pulse_wide = int(float(analog_value) * 1000000 / FREQUENCY / 4096);
  angle_wide = map(pulse_wide, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0 ,180);
  return angle_wide;
}

void init_mouv()
{
  UpAndDown(1,4,5,0)
  UpAndDown(2,3,6,1)
  UpAndDown(2,3,6,1)
  
}

//Up = 0
//Down = 1
//Leg= de 1 à 6 
void UpAndDown(int Leg1,int Leg2,int Leg3,int pos)
{
     //modifier l'angle
     int Legs [3] = {Leg1, Leg2, Leg3};
     int angle;
     if (pos==0)
     {
        angle=DOWN; 
     }
     else
     {
        angle=UP;
     }
     
     for (int i=0; i<3; i++)
     {
       if (Legs[i]%2 == 0)
       {
          pwm.setPWM(Legs[i]+6, 0, pulseWidth(180-angle));
       }
       
       else
       {
          pwm.setPWM(Legs[i]+6, 0, pulseWidth(angle));
       }
     }
     delay(500);
}

//Back = 0
//Front = 1
//Leg= de 1 à 6
//gérer cas d'erreur 
void ForwardAndBackwards(int Leg1,int Leg2,int Leg3,int pos)
{
   //modifier l'angle
   int Legs [3] = {Leg1, Leg2, Leg3};
   int angle;
   if (pos==0)
   {
      angle=BACK; 
   }
   else if(pos==1)
   {
      angle=CENTER;
   }
   else 
   {
      angle=FRONT;
   }
   
   for (int i=0; i<3; i++)
   {
     if (Legs[i]%2 == 0)
     {
        pwm.setPWM(Legs[i], 0, pulseWidth(180-angle));
     }
     
     else
     {
        pwm.setPWM(Legs[i], 0, pulseWidth(angle));
     }
   }
   delay(500);
} 
   
    
  

















  
