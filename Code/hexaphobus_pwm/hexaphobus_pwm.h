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

/// Position value (down).
#  define POS_DOWN   0
/// Position value (up).
#  define POS_UP     1
/// Position value (back).
#  define POS_BACK   0
/// Position value (front).
#  define POS_FRONT  1
/// Position value (center).
#  define POS_CENTER 2

/// Angle value in degrees (down).
#  define DOWN   65-15
/// Angle value in degrees (up).
#  define UP     25-15
/// Angle value in degrees (back).
#  define BACK   10
/// Angle value in degrees (front).
#  define FRONT  75
/// Angle value in degrees (center).
#  define CENTER 43

/// Variable for commucation
boolean newData = false;
boolean serialReady = true;
boolean flag = false;
String input_string;
int returnstate = 0;



/// Adjustments for each servomotor angle.
int SHIFT[13] = {-1, -10, -5, 0, 0, -5, 5, 56, 68, 58, 56, 56, 68};
/// Servomotor angles.
int ANGLE[13] = {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/// Communication port with the PWM servo driver.
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

/********************************************/

/*******************************************************to modify****************
/*!
 * @brief Verify a new instruction from the HMI.
 *        
 *        TBD
 * 
 * @return The state that the robot should be in:
 *  FORWARD = 1
 *  BACKWARD = 2
 *  LEFT = 3
 *  RIGHT = 4.
 */
int writeCommand(String string_to_write )
{  
  Serial.println(string_to_write);
 
}

/*!
 * @brief Every time a command arrive in the serial buffer this function is called.
 *
 *        TBD
 * 
 * @return 0 after communication with the HMI.
 */
/*void serialEvent() 
{
  flag = true;
}*/

/*!
 * @brief Verify a new instruction from the HMI.
 *        
 *        TBD
 * 
 * @return The state that the robot should be in:
 *  FORWARD = 1
 *  BACKWARD = 2
 *  LEFT = 3
 *  RIGHT = 4.
 */
int UpdateCommand()
{
  
  //if (serialReady == true)
  //{
   // if (flag == true)
    //{
      while (/*Serial.available() &&*/ newData == false)
      {
        input_string = Serial.readStringUntil('|');
        input_string = String(input_string);
        
        if (sizeof(input_string) > 0) {
          newData = true;
        } 
      }
    
     /* while(Serial.available())
      {
        char nimportequoi = Serial.read(); 
      }*/
    
      //if new command
      if (newData == true)
      {
        writeCommand(input_string);
        if (input_string == "FORWARD")
        { returnstate = 1;}
        else if (input_string == "BACKWARD")
        { returnstate = 2;}
        else if (input_string == "LEFT")
        { returnstate = 3;}
        else if (input_string == "RIGHT")
        { returnstate = 4;}
        else
        {//writeCommand("invalid information");
          //flag = false;
          newData = false;
          return -1;}
        newData = false;
        return returnstate;
      }
      
      // else idle
      else
      {returnstate = 0;}
      //flag = false;
    //}
 // }
  return 0;
}

/*!
 * @brief Sends servomotor angles to the HMI.
 *
 *       Prints the angle values in the terminal for each servomotor
 *       and sends the information to the HMI via serial communication.
 * 
 * @return 0 after communication with the HMI.
 */
int AngleToHMI() {
  
  String string_to_send = String(ANGLE[1]);

  for (int i = 2; i < 13; i++) {
    string_to_send += ";" + String(ANGLE[i]);
  }

  if (sizeof(string_to_send) < 6) {
    return -1;
  }
  
  writeCommand(string_to_send);
  return 0;
}

/********************************************/

/*!
 * @brief Converts an angle to a servomotor encoder value. 
 *
 *        Converts a desired angular position for a certain leg part
 *        to an encoder value for compatibility with the servomotors.
 * 
 * @param angle
 * 
 *        Desired angle for a specific servomotor.
 * 
 * @return analog_value, which is the servomotor encoder value.
 */
int pulseWidth(int angle) {
  if(angle>180)
  {return -1;}
  
  int pulse_wide, analog_value;
  pulse_wide   = map(angle, 0, 180, MIN_PULSE_WIDTH, MAX_PULSE_WIDTH);
  analog_value = int(float(pulse_wide) / 1000000 * FREQUENCY * 4096);
  
  return analog_value;
}

/********************************************/

/*!
 * @brief Up and down movement sequence for the servomotors.
 *
 *        Moves up to three tibia servomotors (any motor from #1 to #6)
 *        up or down with the corresponding leg part.
 * 
 * @param Leg1
 * 
 *        First leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param Leg2
 * 
 *        Second leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param Leg3
 * 
 *        Third leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param pos1
 * 
 *        Direction of movement for the first leg.
 *        - POS_DOWN (0): move down;
 *        - POS_UP (1): move up.
 * 
 * @param pos2
 * 
 *        Direction of movement for the second leg.
 *        - POS_DOWN (0): move down;
 *        - POS_UP (1): move up.
 * 
 * @param pos3
 * 
 *        Direction of movement for the third leg.
 *        - POS_DOWN (0): move down;
 *        - POS_UP (1): move up.
 * 
 * @param Time
 * 
 *        Delay after sending the information to the servomotors.
 *        Crucial for optimized sequencing. 
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int UpAndDown(int Leg1,
              int Leg2,
              int Leg3,
              int pos1,
              int pos2,
              int pos3,
              int Time) {
  
  serialReady = false;
  int Legs[3]  = {Leg1, Leg2, Leg3};
  int pos[3]   = {pos1, pos2, pos3};
  int angle[3] = {0, 0, 0};

  for (int i = 0; i < 3; i++) {
    if(Legs[i] > 0 && Legs[i] <= 6) {
      if (pos[i] == 1) {
        angle[i] = UP + SHIFT[Legs[i] + 6];
      }
      else {
        angle[i] = DOWN + SHIFT[Legs[i] + 6];
      }

      if (Legs[i] % 2 == 0) {
        pwm.setPWM(Legs[i] + 6, 0, pulseWidth(180 - angle[i]));
        ANGLE[Legs[i] + 6] = 180 - angle[i];
        //AngleToHMI();
      }
      else if (Legs[i] == 0)
      {}
      else
      {
        pwm.setPWM(Legs[i] + 6, 0, pulseWidth(angle[i]));
        ANGLE[Legs[i] + 6] = angle[i];
        //AngleToHMI();
      }
    }
    else if (Legs[i] == 0) {
    }
    else {
      serialReady = true;
      return -1;
    }
  }

  delay(Time);
  serialReady = true;
  return 0;
}

/*!
 * @brief Forward, center, and backward movement sequence for the
 *        servomotors.
 *
 *        Moves up to three coxa servomotors (any motor from #1 to #6)
 *        forward, center or backward with the corresponding leg part.
 * 
 * @param Leg1
 * 
 *        First leg to move (any from #1 to #6). If leg izeof(input_string) > 0parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param Leg2
 * 
 *        Second leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param Leg3
 * 
 *        Third leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param pos1
 * 
 *        Direction of movement for the first leg.
 *        - POS_BACK (0): move forward;
 *        - POS_FRONT (1): move backward;
 *        - POS_CENTER (2): move to the center.
 * 
 * @param pos2
 * 
 *        Direction of movement for the second leg.
 *        - POS_BACK (0): move forward;
 *        - POS_FRONT (1): move backward;
 *        - POS_CENTER (2): move to the center.
 * 
 * @param pos3
 * 
 *        Direction of movement for the third leg.
 *        - POS_BACK (0): move forward;
 *        - POS_FRONT (1): move backward;
 *        - POS_CENTER (2): move to the center.
 * 
 * @param Time
 * 
 *        Delay after sending the information to the servomotors.
 *        Crucial for optimized sequencing. 
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int ForwardAndBackward(int Leg1,
                       int Leg2,
                       int Leg3,
                       int pos1,
                       int pos2,
                       int pos3,
                       int Time) {
  serialReady = false;
  if (Time < 0) {
    return -1;
  }
  
  int Legs[3]  = {Leg1, Leg2, Leg3};
  int pos[3]   = {pos1, pos2, pos3};
  int angle[3] = {0, 0, 0};

  for (int i = 0; i < 3; i++) {
    if (Legs[i] > 0 && Legs[i] <= 6) {
      if (pos[i] == 0) {
        angle[i] = BACK + SHIFT[Legs[i]];
      }
      else if (pos[i] == 1) {
        angle[i] = FRONT + SHIFT[Legs[i]];
      }
      else {
        angle[i] = CENTER + SHIFT[Legs[i]];
      }
    
      if (Legs[i] % 2 == 0) {
        pwm.setPWM(Legs[i], 0, pulseWidth(180 - angle[i]));
        ANGLE[Legs[i]] = 180 - angle[i];
        //AngleToHMI();
      }
      else if (Legs[i] == 0) {
      }
      else {
        pwm.setPWM(Legs[i], 0, pulseWidth(angle[i]));
        ANGLE[Legs[i]] = angle[i];
       // AngleToHMI();
      }
    }
    else if (Legs[i] == 0) {
    }
    else {
      serialReady = true;
      return -1;
    }
  }

  delay(Time);
  serialReady = true;
  return 0;
} 

/*!
 * @brief Sequence to move one leg (tibia + coxa servomotors).
 *
 *        Defines a complete sequence to move a leg. Combines coxa
 *        movements (forward, backward, center) and tibia movements
 *        (up, down).
 * 
 * @param leg
 * 
 *        Leg to move (any from #1 to #6). If leg parameter is
 *        equal to 0, the corresponding leg will not move.
 * 
 * @param Direction
 * 
 *       Pass the coxa direction to the servomotor.
 *       - POS_BACK (0): move forward;
 *       - POS_FRONT (1): move backward;
 *       - POS_CENTER (2): move to the center.
 * 
 * @param Time
 * 
 *        Delay after sending the information to the servomotors.
 *        Crucial for optimized sequencing. 
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int MoveOneLeg(int leg,
               int Direction,
               int Time) {
                
  UpAndDown(leg, 0, 0, POS_UP, POS_UP, POS_UP, 50);
  
  if(Direction == POS_FRONT) {
    ForwardAndBackward(leg, 0, 0, POS_FRONT, POS_CENTER, POS_CENTER, 50);
  }
  else if (Direction == POS_BACK) {
    ForwardAndBackward(leg, 0, 0, POS_BACK, POS_CENTER, POS_CENTER, 50);
    }
  else {
    ForwardAndBackward(leg, 0, 0, POS_CENTER, POS_CENTER, POS_CENTER, 50);
  }
  
  UpAndDown(leg, 0, 0, POS_DOWN, POS_DOWN, POS_DOWN, 50);
  delay(Time);
  return 0;
}

/********************************************/

/*!
 * @brief Initialization of all servomotors.
 *
 *        Initializes all the servomotors to a central position
 *        (passive calibration). Gets the robot ready to move.
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int init_move() {
  
  UpAndDown(2, 3, 6, POS_DOWN, POS_DOWN, POS_DOWN, 0);
  UpAndDown(1, 4, 5, POS_DOWN, POS_DOWN, POS_DOWN, 0);
  
  delay(200);
  
  MoveOneLeg(2, POS_CENTER, 75);
  MoveOneLeg(3, POS_CENTER, 75);
  MoveOneLeg(6, POS_CENTER, 75);
  MoveOneLeg(1, POS_CENTER, 75);
  MoveOneLeg(4, POS_CENTER, 75);
  MoveOneLeg(5, POS_CENTER, 75);
  
  return 0;
}

/*!
 * @brief Moves the robot forward or backward.
 *
 *        Defines the entire sequence to move the robot forward or
 *        backward, with all six legs in sync.
 * 
 * @param B_Moving
 * 
 *        TBD
 * 
 * @param nb_sequence
 * 
 *        TBD
 * 
 * @param dir
 * 
 *        Direction of the robot.
 *        - POS_BACK (0): move forward;
 *        - POS_FRONT (1): move backward.
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int Moving(int dir) 
{
  int inv_dir=0;
  
  if (dir == POS_FRONT) 
  {
    inv_dir = POS_BACK;
  }
  else if (dir == POS_BACK)
  {
    inv_dir=POS_FRONT;
  }

    MoveOneLeg(2, dir, 100);
    MoveOneLeg(3, dir, 100);
    MoveOneLeg(6, dir, 100);
    ForwardAndBackward(1, 4, 5, inv_dir, inv_dir, inv_dir, 0);
    ForwardAndBackward(2, 3, 6, POS_CENTER, POS_CENTER, POS_CENTER, 0);
    delay(200);
    
    MoveOneLeg(1, dir, 100);
    MoveOneLeg(4, dir, 100);
    MoveOneLeg(5, dir, 100);
    ForwardAndBackward(2, 3, 6, inv_dir, inv_dir, inv_dir, 0);
    ForwardAndBackward(1, 4, 5, POS_CENTER, POS_CENTER, POS_CENTER, 0);   
    delay(200);

  return 0;
}

/*!
 * @brief Rotates the robot to the right (clockwise).
 *
 *        Defines the entire sequence to rotate the robot to the right
 *        (clockwise), with all six legs in sync.
 * 
 * @param B_MovingRight
 * 
 *        TBD
 * 
 * @param nb_sequence
 * 
 *        TBD
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int MovingRight() 
{

  MoveOneLeg(2, POS_FRONT, 100);
  MoveOneLeg(3, POS_BACK, 100);
  MoveOneLeg(6, POS_FRONT, 100);
  ForwardAndBackward(2, 3, 6, POS_CENTER, POS_CENTER, POS_CENTER, 0);
  ForwardAndBackward(1, 4, 5, POS_FRONT, POS_BACK, POS_FRONT, 0);
  delay(500);
  
  MoveOneLeg(1, POS_BACK, 150);
  MoveOneLeg(4, POS_FRONT, 100);
  MoveOneLeg(5, POS_BACK, 100);
  ForwardAndBackward(2, 3, 6, POS_BACK, POS_FRONT, POS_BACK, 0);
  ForwardAndBackward(1, 4, 5, POS_CENTER, POS_CENTER, POS_CENTER, 0);
  delay(500);
  
  return 0;
}

/*!
 * @brief Rotates the robot to the left (anticlockwise).
 *
 *        Defines the entire sequence to rotate the robot to the left
 *        (anticlockwise), with all six legs in sync.
 * 
 * @param B_MovingLeft
 * 
 *        TBD
 * 
 * @param nb_sequence
 * 
 *        TBD
 * 
 * @return -1 for handling errors, 0 if execution was correct.
 */
int MovingLeft() 
{

  MoveOneLeg(1, POS_FRONT, 100);
  MoveOneLeg(4, POS_BACK, 100);
  MoveOneLeg(5, POS_FRONT, 100);
  ForwardAndBackward(1, 4, 5, POS_CENTER, POS_CENTER, POS_CENTER, 0);
  ForwardAndBackward(2, 3, 6, POS_FRONT, POS_BACK, POS_FRONT, 0);
  delay(250);
  
  MoveOneLeg(2, POS_BACK, 100);
  MoveOneLeg(3, POS_FRONT, 100);
  MoveOneLeg(6, POS_BACK, 100);
  ForwardAndBackward(1, 4, 5, POS_BACK, POS_FRONT, POS_BACK, 0);
  ForwardAndBackward(2, 3, 6, POS_CENTER, POS_CENTER, POS_CENTER, 0);
  delay(250);
  
  return 0;
}


/*!
 * @brief Shows a new instruction from the HMI.
 *
 *        TBD
 * 
 * @return 0 after communication with the HMI.
 */
void showNewData() {
  if (newData == true) {
    Serial.flush();
    delay(100);
    Serial.println(input_string);
    newData = false;
  }
}
