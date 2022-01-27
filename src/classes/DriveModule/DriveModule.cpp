#include "DriveModule.h"
#include <Arduino.h>

volatile uint8_t readyToPID = 0;
Encoder* encP;
volatile int16_t speedCounts = 0;

DriveModule::DriveModule(const uint8_t EN, const uint8_t IN1, const uint8_t IN2, const uint8_t chA, const uint8_t chB, const uint8_t servoPin){
  this->motor = new L298N(EN, IN1, IN2);
  this->enc = new Encoder(chA, chB);
  this->servoPin = servoPin;
}

void DriveModule::init(){
  this->enc->init();
  encP = this->enc; //For ISR
  this->servo.attach(this->servoPin);
  noInterrupts();
  //USE TIMER 3 (16 bit) (Had to comment out block in Servo.h, this timer also controls PWM for pins 2,3, and 5)
  TCCR3A = 0;
  TCCR3B &= ~(1<<WGM33); //CTC Mode
  TCCR3B |= (1<<WGM32); //CTC Mode
  TCCR3B |= (1<<CS32); //Prescalar
  TCCR3B &= ~(1<<CS31); //Prescalar
  TCCR3B &= ~(1<<CS30); //Prescalar
  TCNT3 = 0;
  OCR3A = 6249;
  TIMSK3 = (1<<OCIE3A);
  interrupts();
  sei();
}

void DriveModule::driveSpeed(uint8_t target){
    this->motor->setSpeed(abs(target));
    if(target > 0){
      this->motor->forward();
    }
    else if(target < 0){
      this->motor->backward();
    }
    else{
      this->motor->stop();
    }
}

void DriveModule::pidSpeed(uint8_t target){
  if(readyToPID){
    readyToPID = 0;
    static int16_t prev = 0;
    static int16_t sum = 0;
    static const float kp = 3;
    static const float ki = .5;

    noInterrupts();
    int16_t speed = speedCounts - prev;
    prev = speedCounts;
    interrupts();

    int16_t error = target - speed;
    sum += error;

    int effort = kp*error + ki*sum;
    if(effort > 255){effort = 255;}
    if(effort < -255){effort = -255;}

    this->driveSpeed(effort);
    Serial.println(speed);
  }
}

boolean DriveModule::setWheelAngle(int8_t target){
  this->servo.write(int((1.388888889*double(target))+10.0));
  //10 is the zero point
  //135 is 90
}

boolean DriveModule::driveDist(float target){
  static int dir = 1;
  static uint16_t error;
  static const float kp = 2;
  static State state = WAITING;
  static uint16_t startTime;
  boolean status = false;
  static uint16_t effortSpeed = 0;
  this->enc->updateCounts();
  int16_t currCounts = this->enc->getCounts();

  //Serial.println("STATE: " + String(state) + "\t\tEffort: " + String(effortSpeed) + "\t\tDirection: " + String(dir));
  //Serial.println("TARGET: " + String(this->targetCounts) + "\t\tCURRENT: " + String(currCounts) + "\t\tERROR: " + String(error));

  //For initial call of this function, set the target counts
  if(this->moving == false){
    this->targetCounts = currCounts + this->enc->distToCounts(target);
    state = DRIVING;
    this->moving = true; 
  }

  //DRIVE DISTANCE STATE MACHINE
  switch(state){
    case DRIVING: //Drive toward target until target is met, then wait. If time elapsed is met without leaving threashold, exit and return true and set moving to false.
      //Calculate absolute error
      error = abs(this->targetCounts - currCounts);
      //if error is negative, direction is backwards
      if(this->targetCounts - currCounts < 0){dir = -1;}
      else{ dir = true;}
      //Effort speed (actually 0-255) is calculated
      effortSpeed = int(kp * error);
      if(effortSpeed > 75){effortSpeed = 75;} //75 can be replaced with desired max speed
      driveSpeed(effortSpeed * dir);
      
      //If current counts is within 5 counts of goal, change to waiting state
      if(abs(this->targetCounts - currCounts) < 10){
        startTime = millis();
        state = WAITING;
      }
      break;

    case WAITING:
      //If 1 second has elapsed since definition of startTime, return true
      this->motor->stop(); 
      if(millis() - startTime >= 1000){
        this->moving = false;
        status = true;
      }
      //If one second has not passed and the counts left target range, return to DRIVING
      else if(abs(this->targetCounts - currCounts) >= 10){
        state = DRIVING;
      }
      break;
  }
  return status;
}

ISR(TIMER3_COMPA_vect)
{
  encP->updateCounts();
  speedCounts = encP->getCounts();
  readyToPID = 1;
}