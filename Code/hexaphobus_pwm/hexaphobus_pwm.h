/*!
 * @file hexaphobus_pwm.h
 * 
 * @authors
 *          - Cabana,       Gabriel  | cabg2101
 *          - Guay-Tanguay, Carolane | guac3201
 *          - Lalonde,      Philippe | lalp2803
 *          - Roy,          Olivier  | royo2206
 * 
 * @date
 *       - 2020-02-25 (Creation)
 *       - 2020-03-24 (Last modification)
 * 
 * @details Motion control, communication, and sequencing (header).
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

/// Position value (up).
#  define POS_UP     1
/// Position value (down).
#  define POS_DOWN   0
/// Position value (back).
#  define POS_BACK   0
/// Position value (front).
#  define POS_FRONT  1
/// Position value (center).
#  define POS_CENTER 2

/// Angle value in degrees (up).
#  define UP     35
/// Angle value in degrees (down).
#  define DOWN   65
 /// Angle value in degrees (back).
#  define BACK   10
/// Angle value in degrees (front).
#  define FRONT  75
/// Angle value in degrees (center).
#  define CENTER 43

/// Adjustments for each servomotor angle.
int SHIFT[13] = {-1, -10, -5, 0, 0, -5, 5, 56, 68, 58, 56, 56, 68};
/// Servomotor angles.
int ANGLE[13] = {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/// Communication port with the PWM servo driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/********************************************/

/*!
 * @brief Motors Angle to HMI 
 *
 *       Print in the terminal the angle value for each motors
 *       and send the information to the HMI.
 * 
 * @return 0 after communication with the HMI.
 */
int AngleToHMI()
{
  String string_to_send = String(ANGLE[1]);

  for (int i = 2; i < 13; i++) {
    string_to_send += ";" + String(ANGLE[i]);
  }
  Serial.println(string_to_send);

  return 0;
}

/********************************************/

/*!
 * @brief Angle to Pulse width 
 *
 *        Convert an angle to pulse width for motors.
 * 
 * @param angle
 * 
 *        Angle want for a motor.
 * 
 * @return analog_value, which is the servomotor encoder value.
 */
int pulseWidth(int angle)
{
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  return analog_value;
}


/********************************************/

/*!
 * @brief Up and down mouvement for motors
 *
 *       Move up to 3 motors (Leg 1 to 6) up or down
 *       with the number of the leg associate with the 
 *       motor. Use Leg 0 to do nothing with a Leg.
 * 
 * @param Leg1
 * 
 *       first leg to move (number between 1 and 6).
 * 
 * @param Leg2
 * 
 *       Second leg to move (number between 1 and 6).
 *       Use 0 to not move a leg.
 * 
 * @param Leg3
 * 
 *       Third leg to move (number between 1 and 6).
 *       Use 0 to not move a leg.
 * 
 * @param pos1
 * 
 *       Pass the position of the Leg 1 to move. 
 *       POS_UP to move up (1) and POS_DOWN to move DOWN (0).
 * 
 * @param pos2
 * 
 *       Pass the position of the Leg 2 to move.
 *       POS_UP to move up (1) and POS_DOWN to move DOWN (0).
 * 
 * @param pos3
 * 
 *       Pass the position of the Leg 3 to move.
 *       POS_UP to move up (1) and POS_DOWN to move DOWN (0).
 * 
 * @param time
 * 
 *       Pass the delay to wait after send the information to motors. 
 * 
 * @return -1 if a problem occurs, 0 if not 
 */
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
        { angle[i]=UP+SHIFT[Legs[i]+6];}
        else
        { angle[i]=DOWN+SHIFT[Legs[i]+6];}

        if (Legs[i]%2 == 0)
        {
            pwm.setPWM(Legs[i]+6, 0, pulseWidth(180-angle[i]));
            ANGLE[Legs[i]+6]=180-angle[i];
            AngleToHMI();
        }
        else if (Legs[i]==0)
        {}
        else
        {
          pwm.setPWM(Legs[i]+6, 0, pulseWidth(angle[i]));
          ANGLE[Legs[i]+6]=180-angle[i];
          AngleToHMI();
        }
      }
      else if(Legs[i]==0){
      }
      else 
       {return -1;}
     }
     delay(time);
     return 0;
}

/*!
 * @brief Foward, backward and center mouvement for motors
 *
 *       Move up to 3 motors (Leg 1 to 6) foward, center or backward
 *       with the number of the leg associate with the 
 *       motor. Use Leg 0 to do nothing with a Leg.
 * 
 * @param Leg1
 * 
 *       First leg to move (number between 1 and 6).
 * 
 * @param Leg2
 * 
 *       Second leg to move (number between 1 and 6).
 *       Use 0 to not move a leg.
 * 
 * @param Leg3
 * 
 *       Third leg to move (number between 1 and 6).
 *       Use 0 to not move a leg.
 * 
 * @param pos1
 * 
 *       Pass the position of the Leg 1 to move. 
 *       POS_FRONT to move foward (1) and POS_BACK to move backward (0)
 *       POS_CENTER to move to center (2).
 * 
 * @param pos2
 * 
 *       Pass the position of the Leg 2 to move. 
 *       POS_FRONT to move foward (1) and POS_BACK to move backward (0)
 *       POS_CENTER to move to center (2).
 * 
 * @param pos3
 * 
 *       Pass the position of the Leg 3 to move .
 *       POS_FRONT to move foward (1) and POS_BACK to move backward (0)
 *       POS_CENTER to move to center (2).
 * 
 * @param time
 * 
 *       Pass the delay to wait after send the information to motors. 
 * 
 * @return -1 if a problem occurs, 0 if not 
 */
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
        {angle[i]=BACK+SHIFT[Legs[i]];}
      else if (pos[i]==1)
        {angle[i]=FRONT+SHIFT[Legs[i]];}
      else
        {angle[i]=CENTER+SHIFT[Legs[i]];}
    
      if (Legs[i]%2 == 0)
      {
        pwm.setPWM(Legs[i], 0, pulseWidth(180-angle[i]));
        ANGLE[Legs[i]]=angle[i];
        AngleToHMI();
      }
      
      else if (Legs[i]==0)
      {}
      else
      {
        pwm.setPWM(Legs[i], 0, pulseWidth(angle[i]));
        ANGLE[Legs[i]]=angle[i];
        AngleToHMI();
      }
    }
    else if(Legs[i]==0){
    }
    else 
    {return -1;}
   }
   delay(time);
   return 0;
} 

/*!
 * @brief Move one leg 
 *
 *        Make the complete movement to move one leg 
 *        into the disered direction.
 * 
 * @param leg
 * 
 *        Number of the leg to move (1 to 6).
 * 
 * @param direction
 * 
 *       Pass the direction of the leg to move. 
 *       POS_FRONT to move foward (1) and POS_BACK to move backward (0)
 *       POS_CENTER to move to center (2).
 * 
 * @param time
 * 
 *       Pass the delay to wait after send the information to motors.
 * 
 * @return -1 if a problem occurs, 0 if not 
 */
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

/********************************************/

/*!
 * @brief Initialisation of all the motors
 *
 *        Initialite all the motors to the center position.
 *        Make the robot ready to move.
 * 
 * @return -1 if a problem occurs, 0 if not
 */
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

/*!
 * @brief Moving the robot foward or backward
 *
 *        Function allowing the robot to move forward or backward as 
 *        long as the button is pressed. 
 * 
 * @param dir
 * 
 *        Direction to move.
 *        POS_FRONT to move foward (1) and POS_BACK to move backward (0)
 * 
 * @param varname
 * 
 *        A MODIFIER !!!!!!!!!!!!!!
 * 
 * @return -1 if a problem occurs, 0 if not
 */
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

/*!
 * @brief Move the robot to the right
 *
 *        Function allowing the robot to turn to the right as 
 *        long as the button is pressed.
 * 
 * @param varname
 * 
 *        A MODIFIER !!!!!!!!!!!!!!
 * 
 * @return -1 if a problem occurs, 0 if not
 */
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
      delay(500);
      
      MoveOneLeg(1,POS_BACK,150);
      MoveOneLeg(4,POS_FRONT,100);
      MoveOneLeg(5,POS_BACK,100);
      ForwardAndBackward(2,3,6,POS_BACK,POS_FRONT,POS_BACK,0);
      ForwardAndBackward(1,4,5,POS_CENTER,POS_CENTER,POS_CENTER,0);
      delay(500);
      B_MovingRight+=1;
    } 
  return 0;
}

/*!
 * @brief Move the robot to the left
 *
 *        Function allowing the robot to turn to the left as 
 *        long as the button is pressed.
 * 
 * @param varname
 * 
 *        A MODIFIER !!!!!!!!!!!!!!
 * 
 * @return -1 if a problem occurs, 0 if not
 */
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
