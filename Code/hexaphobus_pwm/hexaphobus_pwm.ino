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
    User interface designed for intuitive control and monitoring of the
    HexaphobUS robot.

S4-H20 | GRO400
*/

/********************************************/

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h> 

/********************************************/

#define MIN_PULSE_WIDTH       650
#define MAX_PULSE_WIDTH       2650
#define DEFAULT_PULSE_WIDTH   1500
#define FREQUENCY             50

//Pos value for function UpAndDown 
#define POS_UP 1
#define POS_DOWN 0

//Pos value for function ForwardAndBackwards
#define POS_BACK 0
#define POS_FRONT 1
#define POS_CENTER 2


//Angle for each position
#define UP 110
#define DOWN 92
#define FRONT 170
#define BACK 90
#define CENTER 120
  
// our servo # counter
uint8_t servonum = 0;

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/********************************************/

void setup() {
    Serial.begin(9600);
    Serial.print("test");
    pwm.begin();
    pwm.setPWMFreq(FREQUENCY);  // Analog servos run at ~60 Hz updates
}

void loop() {


 init_mouv();   
 delay(50000);
 /*int B_MovingForward=1;
 MovingForward(B_MovingForward,3);
 delay(1000);
 B_MovingForward=0;
 int B_MovingBackwards=1;
 MovingBackwards(B_MovingBackwards,3);
 delay(5000);
 B_MovingBackwards=0;
 delay(2000);
 int B_MovingRight=1;
 MovingRight(B_MovingRight,3);
 B_MovingRight=0;*/
}

//Angle to analog value 
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

//Analog value to angle
int AnalogToAngle(int analog_value)
{
  int angle_wide, pulse_wide;
  pulse_wide = int(float(analog_value) * 1000000 / FREQUENCY / 4096);
  angle_wide = map(pulse_wide, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0 ,180);
  return angle_wide;
}

//Robot Initialisation 
//Legs are in initial position
void init_mouv()
{
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
  ForwardAndBackwards(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER);
  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
  ForwardAndBackwards(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN, POS_DOWN);
}
//-------------------------------------Movement sequencing function-------------------------------------------
//Function allowing the robot to move forward as long as the button is pressed
//Connect the button UP of the HMI
void MovingForward(int B_MovingForward,int nb_sequence)
{
    while (B_MovingForward<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
      ForwardAndBackwards(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT);
      ForwardAndBackwards(1,4,5,POS_BACK,POS_BACK,POS_BACK);
      UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
      while (B_MovingForward<nb_sequence)
      {
        UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(1,4,5,POS_FRONT,POS_FRONT,POS_FRONT);
        ForwardAndBackwards(2,3,6,POS_BACK,POS_BACK,POS_BACK);
        UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT);
        ForwardAndBackwards(1,4,5,POS_BACK,POS_BACK,POS_BACK);
        UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
        B_MovingForward+=B_MovingForward;
      }
      UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
      ForwardAndBackwards(1,4,5,POS_FRONT,POS_FRONT,POS_FRONT);
      ForwardAndBackwards(2,3,6,POS_BACK,POS_BACK,POS_BACK);
      UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN); 
      init_mouv();
    }
}

//Function allowing the robot to back up as long as the button is pressed
//Connect the button DOWN of the HMI
void MovingBackwards(int B_MovingBackwards,int nb_sequence)
{
    while (B_MovingBackwards<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
      ForwardAndBackwards(2,3,6,POS_BACK,POS_BACK,POS_BACK);
      ForwardAndBackwards(1,4,5,POS_FRONT,POS_FRONT,POS_FRONT);
      UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
      while (B_MovingBackwards<nb_sequence)
      {
        UpAndDown(1,4,5,POS_UP,POS_DOWN,POS_DOWN);
        ForwardAndBackwards(1,4,5,POS_BACK,POS_BACK,POS_BACK);
        ForwardAndBackwards(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT);
        UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(2,3,6,POS_BACK,POS_BACK,POS_BACK);
        ForwardAndBackwards(1,4,5,POS_FRONT,POS_FRONT,POS_FRONT);
        UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
        B_MovingBackwards+=B_MovingBackwards;
      }
      
      UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
      ForwardAndBackwards(1,4,5,POS_BACK,POS_BACK,POS_BACK);
      ForwardAndBackwards(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT);
      UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN); 
      init_mouv();
    }
}

//Function allowing the robot to turn right as long as the button is pressed
//Connect the button RIGHT of the HMI
//Function to complete **********************************************
void MovingRight(int B_MovingRight,int nb_sequence)
{
    while (B_MovingRight<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP, POS_UP,POS_UP);
      ForwardAndBackwards(2,3,6,POS_FRONT,POS_BACK,POS_FRONT);
      ForwardAndBackwards(1,4,5,POS_FRONT,POS_BACK,POS_FRONT);
      UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
      while (B_MovingRight<nb_sequence)
      {
        UpAndDown(7,10,11,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(1,4,5,POS_BACK,POS_FRONT,POS_BACK);
        ForwardAndBackwards(2,3,6,POS_BACK,POS_FRONT,POS_BACK);
        UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(2,3,6,POS_FRONT,POS_BACK,POS_FRONT);
        ForwardAndBackwards(1,4,5,POS_FRONT,POS_BACK,POS_FRONT);
        UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
        
        B_MovingRight+=B_MovingRight;
      }
      
        UpAndDown(7,10,11,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(1,4,5,POS_BACK,POS_FRONT,POS_BACK);
        ForwardAndBackwards(2,3,6,POS_BACK,POS_FRONT,POS_BACK);
        UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
    } 
  
  
}

//Function allowing the robot to turn left as long as the button is pressed
//Connect the button LEFT of the HMI
//Function to complete **********************************************
void MovingLeft()
{

}

//------------------------------------ Motion function -------------------------------------------

// Active motors to raise or move down 3 legs to their 3 positions
//Up = POS_UP or 1 , Down = POS_DOWN or 0
//Pass 3 Leg in argument (1 to 6) and their 3 positions (0 or 1)
//Add management for errors cases

void UpAndDown(int Leg1,int Leg2,int Leg3,int pos1,int pos2,int pos3)
{

     int Legs [3] = {Leg1, Leg2, Leg3};
     int pos [3] = {pos1,pos2,pos3};
     int angle[3]={0,0,0};

     for (int i=0; i<3; i++)
     {
       if (pos[i]==0)
       { angle[i]=DOWN;}
       else
       { angle[i]=UP;}

       if (Legs[i]%2 == 0)
       {
          pwm.setPWM(Legs[i]+6, 0, pulseWidth(180-angle[i]));
       }
       
       else
       {
          pwm.setPWM(Legs[i]+6, 0, pulseWidth(angle[i]));
       }
     }
     delay(300);
}

//Active motors to move foward, move backwards or move to center 3 legs to their 3 positions
//Back = POS_BACK or 0 , Front = POS_FRONT or 1 , Center= POS_CENTER or 2
//Pass 3 Leg in argument (1 to 6) and their 3 positions
//Add management for errors cases
void ForwardAndBackwards(int Leg1,int Leg2,int Leg3,int pos1, int pos2, int pos3)
{
   int Legs [3] = {Leg1, Leg2, Leg3};
   int pos[3] = {pos1, pos2, pos3};
   int angle[3]={0,0,0};
   for (int i=0; i<3; i++)
   {
    if (pos[i]==0)
      {angle[i]=BACK;}
    else if (pos[i]==1)
      {angle[i]=FRONT;}
    else
      {angle[i]=CENTER;}
   
     if (Legs[i]%2 == 0)
     {
        pwm.setPWM(Legs[i], 0, pulseWidth(180-angle[i]));
     }
     
     else
     {
        pwm.setPWM(Legs[i], 0, pulseWidth(angle[i]));
     }
   }
   delay(300);
} 




  
