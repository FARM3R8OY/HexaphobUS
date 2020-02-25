/*
File: test_UpAndDown.h

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Lalonde,      Philippe | lalp2803

Date(s):
    2020-02-25 (Creation)

Description:
    Function definition for arduino

S4-H20 | GRO400
*/

/********************************************/


#include <Adafruit_PWMServoDriver.h>
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

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
#define UP 40
#define DOWN 65
#define FRONT 68-20
#define BACK 18+20
#define CENTER 43
int DECALAGE[13]={-1,-10,-5,0,0,-5,5,-15,0,-8,-15,-10,-5};

//Angle to analog value 
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}


//------------------------------------ Motion function -------------------------------------------

// Active motors to raise or move down 3 legs to their 3 positions
//Up = POS_UP or 1 , Down = POS_DOWN or 0
//Pass 3 Leg in argument (1 to 6) and their 3 positions (0 or 1)
//Add management for errors cases

int UpAndDown(int Leg1,int Leg2,int Leg3,int pos1,int pos2,int pos3)
{

     int Legs [3] = {Leg1, Leg2, Leg3};
     int pos [3] = {pos1,pos2,pos3};
     int angle[3]={0,0,0};

     for (int i=0; i<3; i++)
     {
      if(Legs[i]>0 && Legs[i]<7) 
      {
        if (pos[i]==1)
        { angle[i]=UP+DECALAGE[Legs[i]+6];}
        else
        { angle[i]=DOWN+DECALAGE[Legs[i]+6];}

        if (Legs[i]%2 == 0)
        {
            pwm.setPWM(Legs[i]+6, 0, pulseWidth(180-angle[i]));
        }
        
        else
        {
            pwm.setPWM(Legs[i]+6, 0, pulseWidth(angle[i]));
        }
      }
      else 
        {return -1;}
    
     }
     delay(500);
     return 0;
}

//Active motors to move foward, move backwards or move to center 3 legs to their 3 positions
//Back = POS_BACK or 0 , Front = POS_FRONT or 1 , Center= POS_CENTER or 2
//Pass 3 Leg in argument (1 to 6) and their 3 positions
//Add management for errors cases
int ForwardAndBackwards(int Leg1,int Leg2,int Leg3,int pos1, int pos2, int pos3)
{
   int Legs [3] = {Leg1, Leg2, Leg3};
   int pos[3] = {pos1, pos2, pos3};
   int angle[3]={0,0,0};
   for (int i=0; i<3; i++)
   {
    if(Legs[i]>0 && Legs[i]<7) 
    {
      if (pos[i]==0)
        {angle[i]=BACK+DECALAGE[Legs[i]];}
      else if (pos[i]==1)
        {angle[i]=FRONT+DECALAGE[Legs[i]];}
      else
        {angle[i]=CENTER+DECALAGE[Legs[i]];}
    
      if (Legs[i]%2 == 0)
      {
          pwm.setPWM(Legs[i], 0, pulseWidth(180-angle[i]));
      }
      
      else if (Legs[i]==0)
      {
      }
      else {
        pwm.setPWM(Legs[i], 0, pulseWidth(angle[i]));
      }
    }
    else 
    {return -1;}
    
   }
   delay(500);
   return 0;
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
int init_mouv()
{
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
  ForwardAndBackwards(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER);
  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
  UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
  ForwardAndBackwards(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN, POS_DOWN);
  return 0;
}
//-------------------------------------Movement sequencing function-------------------------------------------

//Initial Calibration for all the motors
//
int calibrationFunction()
{
  int a=-1;
  //Indivitual test for angles UP AND DOWN
  /*for (int i=1; i<7;i++)
  {
    UpAndDown(i,0,0,POS_DOWN,POS_DOWN,POS_DOWN);
    a=pwm.getPWM(i);
    Serial.print(" down: ");
    Serial.print(a);
    delay(2000);
    UpAndDown(i,0,0,POS_UP,POS_UP,POS_UP);
    Serial.print(" up: ");
    a=pwm.getPWM(i);
    Serial.print(a);
    delay(2000);
    UpAndDown(i,0,0,POS_DOWN,POS_DOWN,POS_DOWN);
    delay(2000);
  }*/
  //TEST Up and down for all motors
  /*
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
  delay(1000);
  UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP);
  delay(1000);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);

  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
  delay(1000);
  UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP);
  delay(1000);
  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
*/
  //TEST Foward and backwards for angle FRONT, BACK and CENTER
  
  for (int i=1; i<3;i++)
  {
  ForwardAndBackwards(i,0,0,POS_CENTER,POS_DOWN,POS_DOWN);
  delay(2000);
  ForwardAndBackwards(i,0,0,POS_FRONT,POS_UP,POS_UP);
  delay(2000);
  ForwardAndBackwards(i,0,0,POS_BACK,POS_DOWN,POS_DOWN);
  delay(2000);
  }
  
  //TEST Foward and backwards for all motors
  /*
  ForwardAndBackwards(1,4,6,POS_CENTER,POS_CENTER,POS_CENTER);
  delay(2000);
  ForwardAndBackwards(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT);
  delay(2000);
  ForwardAndBackwards(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER);
  delay(2000);
  ForwardAndBackwards(1,4,6,POS_BACK,POS_BACK,POS_BACK);
  delay(2000);
  */
  return 0;
}

//Function allowing the robot to move forward as long as the button is pressed
//Connect the button UP of the HMI
int MovingForward(int B_MovingForward,int nb_sequence)
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
    return 0;
}

//Function allowing the robot to back up as long as the button is pressed
//Connect the button DOWN of the HMI
int MovingBackwards(int B_MovingBackwards,int nb_sequence)
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
    return 0;
}

//Function allowing the robot to turn right as long as the button is pressed
//Connect the button RIGHT of the HMI
int MovingRight(int B_MovingRight,int nb_sequence)
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
  
  return 0;
}

//Function allowing the robot to turn left as long as the button is pressed
//Connect the button LEFT of the HMI
int MovingLeft(int B_MovingLeft,int nb_sequence)
{
    while (B_MovingLeft<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP, POS_UP,POS_UP);
      ForwardAndBackwards(2,3,6,POS_BACK,POS_FRONT,POS_BACK);
      ForwardAndBackwards(1,4,5,POS_BACK,POS_FRONT,POS_BACK);
      UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
      while (B_MovingLeft<nb_sequence)
      {
        UpAndDown(7,10,11,POS_UP,POS_UP,POS_UP);
        ForwardAndBackwards(1,4,5,POS_FRONT,POS_BACK,POS_FRONT);
        ForwardAndBackwards(2,3,6,POS_FRONT,POS_BACK,POS_FRONT);
        UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP, POS_UP,POS_UP);
        ForwardAndBackwards(2,3,6,POS_BACK,POS_FRONT,POS_BACK);
        ForwardAndBackwards(1,4,5,POS_BACK,POS_FRONT,POS_BACK);
        UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN);
        
        B_MovingLeft+=B_MovingLeft;
      }
      
      UpAndDown(7,10,11,POS_UP,POS_UP,POS_UP);
      ForwardAndBackwards(1,4,5,POS_FRONT,POS_BACK,POS_FRONT);
      ForwardAndBackwards(2,3,6,POS_FRONT,POS_BACK,POS_FRONT);
      UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN);
    } 
    return 0;
}










