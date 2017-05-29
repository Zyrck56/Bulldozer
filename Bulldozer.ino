#include <Servo.h>
#include "Wire.h"

Servo lServo;
Servo rServo;
Servo bServo;

int lPos = 80;
int rPos = 10;
int bPos = 0;

bool blinkState = false;

//pin numbers
int rightPWM  = 6;
int leftPWM = 11;
int left1 = 8;
int left2 = 7;
int right1 = 12;
int right2 = 13;

char R = 'R';
char L = 'L';
char F = 'F';
char B = 'B';
char A = 'A';

String message;
String value = "";
int velocity;
	
void setup()
{
    Wire.begin();
    Serial.begin(57600);
    
    //pinMode(LED_PIN, OUTPUT);
    lServo.attach(3);
    rServo.attach(5);
    bServo.attach(9);
    //4 digital pins
    pinMode(right1, OUTPUT);
    pinMode(right2, OUTPUT);
    pinMode(left1,  OUTPUT);
    pinMode(left2,  OUTPUT);
    //2 PWM pins
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
    message = Serial.readStringUntil('\n');
    Serial.println(message);
    value += (char)message.charAt(2); 
    value += (char)message.charAt(3);
    velocity = value.toInt();
    
    if (message.charAt(0) == 'L')
    {
      if (message.charAt(1) == '+')
        setDir(L, F);
      else
        setDir(L, B);
      setVel(L, velocity);
    }
    
    if (message.charAt(0) == 'R')
    {
      if (message.charAt(1) == '+')
        setDir(R, F);
      else
        setDir(R, B);
      setVel(R, velocity);
    }
    
    if (message.charAt(0) == 'A')
    {
      if (message.charAt(1) == '+')
      {
        while (velocity != 0 && lPos <= 80 && rPos >= 10)
        {
          lPos += 1;
          rPos -= 1;
          lServo.write(lPos);
          rServo.write(rPos);
          delay(15);
          if (Serial.available() > 0)
            velocity = 0;
          
        }
      }
      if (message.charAt(1) == '-')
      {
        while (velocity != 0 && lPos >= 0 && rPos <= 90)
        {
          lPos -= 1;
          rPos += 1;
          lServo.write(lPos);
          rServo.write(rPos);
          int delayTime = map(velocity, 0, 99, 20, 15);
          delay(delayTime);
          if (Serial.available() > 0)
            velocity = 0;
        }
      } 
    }
   
   if (message.charAt(0) == 'B')
   {
     if (message.charAt(1) == '+')
     {
       while (velocity != 0 && bPos >= 0)
       {
         bPos -= 1;
         bServo.write(bPos);
         delay(15);
         if (Serial.available() > 0)
           velocity = 0;
       }
     }
     if (message.charAt(1) == '-')
     {
       while (velocity != 0 && bPos <= 80)
       {
         bPos += 1;
         bServo.write(bPos);
         int delayTime = map(velocity, 0, 99, 20, 15);
         delay(delayTime);
         if (Serial.available() > 0)
           velocity = 0;
       }
     }
   } 
  value = "";  
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

void setDir(char side, char movement)
{
  if (side == 'L')
  {
    if (movement == 'F')
    {
      digitalWrite(left1, HIGH);
      digitalWrite(left2, LOW);
    }
    if (movement == 'B')
    {
      digitalWrite(left1, LOW);
      digitalWrite(left2, HIGH);
    }
  }
  if (side == 'R')
  {
    if (movement == 'F')
    {
      digitalWrite(right1, HIGH);
      digitalWrite(right2, LOW);
    }
    if (movement == 'B')
    {
      digitalWrite(right1, LOW);
      digitalWrite(right2, HIGH);
    }
  }
}
