/*
File: functionDefinition.h

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Lalonde,      Philippe | lalp2803
    Roy,          Olivier  | royo2206


Date(s):
    2020-02-25 (Creation)
    2020-03-11 (Last modification)

Description:
    Function definition for Arduino.

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

//Pos value for function ForwardAndBackward
#define POS_BACK 0
#define POS_FRONT 1
#define POS_CENTER 2


//Angle for each position
#define UP 35
#define DOWN 65
#define FRONT 75
#define BACK 10
#define CENTER 43
int DECALAGE[13]={-1,-10,-5,0,0,-5,5,56,68,58,56,56,68};
int ANGLE[13]={0,0,0,0,0,0,0,0,0,0,0,0,0};


//------------------------------------ Conversion function---------------------------------------- 
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
/*
int* Accel(int delta, float weight)
{
	int max_size = 9;
	int total_angle = 0;
	
	int move_sequence[max_size];
	
	int angle = 1;
	move_sequence[0] = angle;
	total_angle += angle;
	
	for ( int i = 1; total_angle < delta; i++){
		angle += int(ceil(angle*weight));
		total_angle += angle;
		if(total_angle > delta){
			angle -= (total_angle-delta);
		}
		move_sequence[i] = angle;
	}
	return move_sequence;
}*/

//------------------------------------ Motion function -------------------------------------------

// Active motors to raise or move down 3 legs to their 3 positions
//Up = POS_UP or 1 , Down = POS_DOWN or 0
//Pass 3 Leg in argument (1 to 6) and their 3 positions (0 or 1)
//Add management for errors cases
int UpAndDown(int Leg1,int Leg2,int Leg3,int pos1,int pos2,int pos3,int time)
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
        else if (Legs[i]==0)
        {}
        else
        {pwm.setPWM(Legs[i]+6, 0, pulseWidth(angle[i]));}
      }
      else if(Legs[i]==0){
      }
      else 
       {return -1;}
     }
     delay(time);
     return 0;
}

//Active motors to move foward, move backwards or move to center 3 legs to their 3 positions
//Back = POS_BACK or 0 , Front = POS_FRONT or 1 , Center= POS_CENTER or 2
//Pass 3 Leg in argument (1 to 6) and their 3 positions
//Add management for errors cases
int ForwardAndBackward(int Leg1,int Leg2,int Leg3,int pos1, int pos2, int pos3, int time)
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
      {pwm.setPWM(Legs[i], 0, pulseWidth(180-angle[i]));}
      
      else if (Legs[i]==0)
      {}
      else
      {pwm.setPWM(Legs[i], 0, pulseWidth(angle[i]));}
    }
    else if(Legs[i]==0){
    }
    else 
    {return -1;}
   }
   delay(time);
   return 0;
} 

//Mouvement d'une jambe
int MoveOneLeg(int leg, int direction, int time)
{
  UpAndDown(leg,0,0,POS_UP,POS_UP,POS_UP,time);
  if(direction==POS_FRONT)
  {ForwardAndBackward(leg,0,0,POS_FRONT,POS_CENTER,POS_CENTER,time);}

  else if (direction==POS_BACK)
  {ForwardAndBackward(leg,0,0,POS_BACK,POS_CENTER,POS_CENTER,time);}

  else
  {ForwardAndBackward(leg,0,0,POS_CENTER,POS_CENTER,POS_CENTER,time);}
  
  UpAndDown(leg,0,0,POS_DOWN,POS_DOWN,POS_DOWN,time);
  return 0;
}

//-------------------------------------Movement sequencing function-------------------------------------------
//Robot Initialisation 
//Legs are in initial position
int init_mouv()
{
  UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN,0);
  UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN,0);
  delay(200);
  MoveOneLeg(2,POS_CENTER,75);
  MoveOneLeg(3,POS_CENTER,75);
  MoveOneLeg(6,POS_CENTER,75);
  MoveOneLeg(1,POS_CENTER,75);
  MoveOneLeg(4,POS_CENTER,75);
  MoveOneLeg(5,POS_CENTER,75);
  return 0;
}
//Function allowing the robot to move forward or backward as long as the button is pressed
//Connect the button UP of the HMI
//Move 1 leg at a time 
int Moving(int B_Moving,int nb_sequence,int dir)
{
    int inv_dir=0;
    if(dir==POS_FRONT)
    {inv_dir=POS_BACK;}
    else if(dir==POS_BACK)
    {inv_dir=POS_FRONT;}

    while (B_Moving<nb_sequence)
    {
      MoveOneLeg(2,dir,100);
      MoveOneLeg(3,dir,100);
      MoveOneLeg(6,dir,100);
      ForwardAndBackward(1,4,5,inv_dir,inv_dir,inv_dir,0);
      ForwardAndBackward(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER,0);
      delay(200);
      
      MoveOneLeg(1,dir,100);
      MoveOneLeg(4,dir,100);
      MoveOneLeg(5,dir,100);
      ForwardAndBackward(2,3,6,inv_dir,inv_dir,inv_dir,0);
      ForwardAndBackward(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER,0);   
      delay(200);
      B_Moving+=1;
    }
    return 0;
}
//Another function to move forward or backward
//All legs in position before moving
int Moving_2(int B_MovingForward,int nb_sequence,int dir)
{
    
    while (B_MovingForward<=nb_sequence)
    {
      MoveOneLeg(2,dir,10);
      MoveOneLeg(3,dir,10);
      MoveOneLeg(6,dir,10);
      MoveOneLeg(1,dir,10);
      MoveOneLeg(4,dir,10);
      MoveOneLeg(5,dir,10);
      ForwardAndBackward(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER,0);
      ForwardAndBackward(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER,0);
      delay(100);
      B_MovingForward+=1;
    }
    return 0;
}


//Function allowing the robot to back up as long as the button is pressed
//Connect the button DOWN of the HMI
//To be remove if Moving works---------------------------------------------------********************
int MovingBackward(int B_MovingBackward,int nb_sequence)
{
    while (B_MovingBackward<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP,POS_UP,POS_UP,0);
      ForwardAndBackward(2,3,6,POS_BACK,POS_BACK,POS_BACK,0);
      ForwardAndBackward(1,4,5,POS_FRONT,POS_FRONT,POS_FRONT,0);
      UpAndDown(2,3,6,POS_DOWN,POS_DOWN,POS_DOWN,0);

      UpAndDown(1,4,5,POS_UP,POS_UP,POS_UP,0);
      ForwardAndBackward(1,4,5,POS_BACK,POS_BACK,POS_BACK,0);
      ForwardAndBackward(2,3,6,POS_FRONT,POS_FRONT,POS_FRONT,0);
      UpAndDown(1,4,5,POS_DOWN,POS_DOWN,POS_DOWN,0); 
      B_MovingBackward+=1;
    }
    return 0;
}

//Function allowing the robot to turn right as long as the button is pressed
//Connect the button RIGHT of the HMI
int MovingRight(int B_MovingRight,int nb_sequence)
{
    init_mouv();
    while (B_MovingRight<nb_sequence)
    {
      MoveOneLeg(2,POS_FRONT,100);
      MoveOneLeg(3,POS_BACK,100);
      MoveOneLeg(6,POS_FRONT,100);
      ForwardAndBackward(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER,0);
      ForwardAndBackward(1,4,5,POS_FRONT,POS_BACK,POS_FRONT,0);
      delay(250);
      
      MoveOneLeg(1,POS_BACK,100);
      MoveOneLeg(4,POS_FRONT,100);
      MoveOneLeg(5,POS_BACK,100);
      ForwardAndBackward(2,3,6,POS_BACK,POS_FRONT,POS_BACK,0);
      ForwardAndBackward(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER,0);
      delay(250);
      B_MovingRight+=1;
    } 
  return 0;
}

//Function allowing the robot to turn left as long as the button is pressed
//Connect the button LEFT of the HMI
int MovingLeft(int B_MovingLeft,int nb_sequence)
{
    while (B_MovingLeft<nb_sequence)
    {
      MoveOneLeg(1,POS_FRONT,100);
      MoveOneLeg(4,POS_BACK,100);
      MoveOneLeg(5,POS_FRONT,100);
      ForwardAndBackward(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER,0);
      ForwardAndBackward(2,3,6,POS_FRONT,POS_BACK,POS_FRONT,0);
      delay(250);
      
      MoveOneLeg(2,POS_BACK,100);
      MoveOneLeg(3,POS_FRONT,100);
      MoveOneLeg(6,POS_BACK,100);
      ForwardAndBackward(1,4,5,POS_BACK,POS_FRONT,POS_BACK,0);
      ForwardAndBackward(2,3,6,POS_CENTER,POS_CENTER,POS_CENTER,0);
      delay(250);
      B_MovingLeft+=1;
    } 
    return 0;
}





