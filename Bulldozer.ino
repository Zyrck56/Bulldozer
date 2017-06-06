#include <Servo.h>
#include "Wire.h"

Servo lServo;  //left and right servo for adjustment axle
Servo rServo;
Servo bServo;  //boom servo

int lPos = 80; //default positions
int rPos = 10;
int bPos = 0;

bool blinkState = false;

int rightPWM  = 6;  //pin numbers for H bridge
int leftPWM = 11;
int left1 = 8;
int left2 = 7;
int right1 = 12;
int right2 = 13;

char R = 'R';  //characters for serial commands
char L = 'L';
char F = 'F';
char B = 'B';
char A = 'A';

String message;  //serial message
String value = "";
int velocity;
int baudrate = 57600;  //xbee's communicate at 57600
int servoDelay = 15;   //15ms delay so servo's dont move too fast	
void setup()
{
    Wire.begin();
    Serial.begin(baudrate); 
    
    lServo.attach(3);
    rServo.attach(5);
    bServo.attach(9);
    //4 digital pins for controlling H bridge
    pinMode(right1, OUTPUT);
    pinMode(right2, OUTPUT);
    pinMode(left1,  OUTPUT);
    pinMode(left2,  OUTPUT);
    //2 PWM pins for H bridge speed controll
    pinMode(leftPWM,  OUTPUT);
    pinMode(rightPWM, OUTPUT);
    pinMode(9, OUTPUT);
    lServo.write(lPos);
    rServo.write(rPos);
    bServo.write(bPos);
}

void loop()
{
  if (Serial.available() > 0)
  {
    /* message comes across serial transmission in a 4 character packet
     * first character designates left motor (L), right motor (R), ajustment axle (A), or boom (B)
     * second character designates direction forward/up (+) or backward/down (-)
     * third and fourth character designates magnitude (0-99)
     */
    message = Serial.readStringUntil('\n');
    Serial.println(message);
    value += (char)message.charAt(2); //get magnitude and store it as "velocity"
    value += (char)message.charAt(3);
    velocity = value.toInt();
    
    if (message.charAt(0) == 'L') //left motor
    {
      if (message.charAt(1) == '+') 
        setDir(L, F);     //set direction  
      else
        setDir(L, B);             
      setVel(L, velocity); //set velocity
    }
    
    if (message.charAt(0) == 'R') //right motor
    {
      if (message.charAt(1) == '+')
        setDir(R, F);       //set direction
      else
        setDir(R, B);
      setVel(R, velocity); //set velocity
    }
    
    if (message.charAt(0) == 'A') //adjustment axle
    {
      if (message.charAt(1) == '+')  //up
      {
        /*servos are mirrored so one approaches 80 as the other approaches 10
          (not 90 & 0 because that would be a waste, wheel is no longer touching track at 80 & 10)
          velocity value used to exit loop (see lines 101 & 102)*/           
        while (velocity != 0 && lPos <= 80 && rPos >= 10) 
        {                                                 
          lPos += 1; //move a little
          rPos -= 1;
          lServo.write(lPos);
          rServo.write(rPos);
          delay(servoDelay);  //delay so the motion feels smooth
          if (Serial.available() > 0) //for the loop exit
            velocity = 0;
          
        }
      }
      if (message.charAt(1) == '-') //down
      {
        while (velocity != 0 && lPos >= 0 && rPos <= 90) //down position can go to 90 degrees
        {
          lPos -= 1;
          rPos += 1;
          lServo.write(lPos);
          rServo.write(rPos);
          delay(servoDelay);
          if (Serial.available() > 0)
            velocity = 0;
        }
      } 
    }
   
   if (message.charAt(0) == 'B') //boom
   {
     if (message.charAt(1) == '+') //raise
     {
       while (velocity != 0 && bPos >= 0) 
       {
         bPos -= 1;
         bServo.write(bPos);
         delay(servoDelay);
         if (Serial.available() > 0)
           velocity = 0;
       }
     }
     if (message.charAt(1) == '-')  //lower
     {
       while (velocity != 0 && bPos <= 80)
       {
         bPos += 1;
         bServo.write(bPos);
         delay(servoDelay);
         if (Serial.available() > 0)
           velocity = 0;
       }
     }
   } 
  value = "";  //reset value
  }
}

void setVel(char side, int value)
{
  value = value * 2.55; //convert from value (0-100) to analog write (0-255)
  if (side == 'L')
    analogWrite(leftPWM, value);
  if (side == 'R')
    analogWrite(rightPWM, value);
}

void setDir(char side, char movement) // H bridge settings
{
  if (side == 'L') //left side
  {
    if (movement == 'F') //forward
    {
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
    }
    if (movement == 'B') //backward
    {
      digitalWrite(left1, LOW);
      digitalWrite(left2, HIGH);
    }
  }
  if (side == 'R') //right side
  {
    if (movement == 'F') //forward
    {
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
    }
    if (movement == 'B') //backward
    {
      digitalWrite(right1, LOW);
      digitalWrite(right2, HIGH);
    }
  }
}
