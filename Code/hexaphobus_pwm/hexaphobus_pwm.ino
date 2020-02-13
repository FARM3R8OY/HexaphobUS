/*
File: hexaphobus_pwm.ino

Contributor(s):
    Guay-Tanguay, Carolane | guac3201
    Roy,          Olivier  | royo2206

Date(s):
    2020-01-29 (Creation)
    2020-02-05 (Last modification)

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

//Valeur de Pos pour fonction UpAndDown 
#define POS_UP 1
#define POS_DOWN 0

//Valeur de Pos pour fonction ForwardAndBackwards
#define POS_BACK 0
#define POS_FRONT 1
#define POS_CENTER 2


//angle pour chaque position
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
    Serial.print("hhahha");
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

//Passer d'une valeur en angle à une valeur analogue
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}

//Passer d'une valeur analogue à un angle
int AnalogToAngle(int analog_value)
{
  int angle_wide, pulse_wide;
  pulse_wide = int(float(analog_value) * 1000000 / FREQUENCY / 4096);
  angle_wide = map(pulse_wide, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH, 0 ,180);
  return angle_wide;
}

//Permet au robot d'initialiser toutes ses moteurs 
//Les jambes sont à leur position initiale
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
//------------------------------------- Fonction de sequencement du mouvement-------------------------------------------
//Fonction permettant au robot d'avancer tant que le bouton est activé
//ajouter le bouton de linterface
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

//Fonction permettant au robot de reculer tant que le bouton est activé
//Bouton 
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

//Fonction pour tourner vers la droite
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

void MovingLeft()
{
  
}

//------------------------------------ Fonction de mouvement -------------------------------------------

//Active les moteurs pour lever ou descendre 3 jambes, 2 d'un côté, 1 de l'autre en même temps
//Up = 1
//Down = 0
//Passer 3 Leg en argument de 1 à 6 
//passer les trois positions des legs 
//gérer cas d'erreur ----------------------------------------
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


//Active les moteurs pour avancer, centrer ou reculer les 3 jambes, 2 d'un côté, 1 de l'autre en même temps
//Back = 0
//Front=1
//Center = 2 ou autres
//Passer 3 Leg en argument de 1 à 6 
//gérer cas d'erreur ----------------------------------------
void ForwardAndBackwards(int Leg1,int Leg2,int Leg3,int pos1, int pos2, int pos3)
{
   //modifier l'angle
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




  
