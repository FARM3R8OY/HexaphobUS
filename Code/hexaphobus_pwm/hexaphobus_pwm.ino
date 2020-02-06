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
#define FRONT 90
#define BACK 160
#define CENTER 125
  
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

  UpAndDown(1,4,6,POS_UP);
  ForwardAndBackwards(1,4,6,POS_FRONT);
  delay(1000);
  ForwardAndBackwards(1,4,6,POS_BACK);
  delay(2000);
  ForwardAndBackwards(1,4,6,POS_CENTER);
  UpAndDown(1,4,6,POS_DOWN);
  delay(1000);
 /*init_mouv();   
 delay(2000);
 int B_MovingForward=1;
 MovingForward(B_MovingForward,3);
 delay(10000);
 B_MovingForward=0;
 int B_MovingBackwards=1;
 MovingBackwards(B_MovingBackwards,2);
 delay(5000);
 B_MovingBackwards=0;
 delay(200000);*/
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
  UpAndDown(1,4,5,POS_DOWN);
  UpAndDown(2,3,6,POS_UP);
  ForwardAndBackwards(2,3,6,POS_CENTER);
  UpAndDown(2,3,6,POS_DOWN);
  UpAndDown(1,4,5,POS_UP);
  ForwardAndBackwards(1,4,5,POS_CENTER);
  UpAndDown(1,4,5,POS_DOWN);
}

//Fonction permettant au robot d'avancer tant que le bouton est activé
//ajouter le bouton de linterface
void MovingForward(int B_MovingForward,int nb_sequence)
{
    while (B_MovingForward<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP);
      ForwardAndBackwards(2,3,6,POS_FRONT);
      ForwardAndBackwards(2,3,6,POS_BACK);
      UpAndDown(2,3,6,POS_DOWN);
      while (B_MovingForward<nb_sequence)
      {
        UpAndDown(1,4,5,POS_UP);
        ForwardAndBackwards(1,4,5,POS_FRONT);
        ForwardAndBackwards(2,3,6,POS_BACK);
        UpAndDown(1,4,5,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP);
        ForwardAndBackwards(2,3,6,POS_FRONT);
        ForwardAndBackwards(2,3,6,POS_BACK);
        UpAndDown(2,3,6,POS_DOWN);
        B_MovingForward+=B_MovingForward;
      }
      UpAndDown(1,4,5,POS_UP);
      ForwardAndBackwards(1,4,5,POS_FRONT);
      ForwardAndBackwards(2,3,6,POS_BACK);
      UpAndDown(1,4,5,POS_DOWN); 
      init_mouv();
    }
}

//Fonction permettant au robot de reculer tant que le bouton est activé
//Bouton 
void MovingBackwards(int B_MovingBackwards,int nb_sequence)
{
    while (B_MovingBackwards<nb_sequence)
    {
      UpAndDown(2,3,6,POS_UP);
      ForwardAndBackwards(2,3,6,POS_BACK);
      ForwardAndBackwards(1,4,5,POS_FRONT);
      UpAndDown(2,3,6,POS_DOWN);
      while (B_MovingBackwards<nb_sequence)
      {
        UpAndDown(1,4,5,POS_UP);
        ForwardAndBackwards(1,4,5,POS_BACK);
        ForwardAndBackwards(2,3,6,POS_FRONT);
        UpAndDown(1,4,5,POS_DOWN);        
        
        UpAndDown(2,3,6,POS_UP);
        ForwardAndBackwards(2,3,6,POS_BACK);
        ForwardAndBackwards(1,4,5,POS_FRONT);
        UpAndDown(2,3,6,POS_DOWN);
        B_MovingBackwards+=B_MovingBackwards;
      }
      
      UpAndDown(1,4,5,POS_UP);
      ForwardAndBackwards(1,4,5,POS_BACK);
      ForwardAndBackwards(2,3,6,POS_FRONT);
      UpAndDown(1,4,5,POS_DOWN); 
      init_mouv();
    }
}




//Active les moteurs pour lever ou descendre 3 jambes, 2 d'un côté, 1 de l'autre en même temps
//Up = 1
//Down = 0
//Passer 3 Leg en argument de 1 à 6 
//gérer cas d'erreur ----------------------------------------
void UpAndDown(int Leg1,int Leg2,int Leg3,int pos)
{

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
       Serial.print(angle);
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


//Active les moteurs pour avancer, centrer ou reculer les 3 jambes, 2 d'un côté, 1 de l'autre en même temps
//Back = 0
//Front=1
//Center = 2 ou autres
//Passer 3 Leg en argument de 1 à 6 
//gérer cas d'erreur ----------------------------------------
void ForwardAndBackwards(int Leg1,int Leg2,int Leg3,int pos)
{
   //modifier l'angle
   int Legs [3] = {Leg1, Leg2, Leg3};
   int angle;
   if (pos==0)
   {
      angle=BACK; 
   }
   /*else if (pos==1)
   {
      angle=FRONT;
   }*/
   else
   {
      angle=CENTER;
   }
   Serial.print("angle fetB /n");
   Serial.print(angle);
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




  
