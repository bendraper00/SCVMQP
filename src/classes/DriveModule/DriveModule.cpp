#include "DriveModule.h"
#include <Arduino.h>

volatile uint8_t readyToPID = 0;
Encoder* encP;
volatile int32_t speedCounts = 0;
boolean offGround = false;
uint32_t timeOffGround = 0;

DriveModule::DriveModule(const uint8_t EN, const uint8_t IN1, const uint8_t IN2, const uint8_t chA, const uint8_t chB, const uint8_t servoPin){
  this->motor = new L298N(EN, IN1, IN2);
  this->enc = new Encoder(chA, chB);
  this->servoPin = servoPin;
  this->speedPID = new PIDController(PID_SPEED_KP, PID_SPEED_KI, PID_SPEED_KD);
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

  attachInterrupt(FRONT_DRIVE_BUTTON, DriveModule::ButtonISR, FALLING);
}

void DriveModule::driveSpeed(int16_t target){
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

void DriveModule::pidSpeed(int16_t target){
   if(offGround == true && millis() - timeOffGround < OFF_GROUND_BUFFER_MS){
     this->driveSpeed(0);
   }
   else if(offGround == true && millis() - timeOffGround >= OFF_GROUND_BUFFER_MS){
     offGround = false;
   }
  else if(readyToPID){
    readyToPID = 0;
    int effort = this->speedPID->calcPIDSpeed(target, speedCounts, MAX_DRIVE_SPEED);
    this->driveSpeed(effort);
    //Serial.println(encP->getCounts());
  }
}

boolean DriveModule::setWheelAngle(int8_t target){
  //THIS MAPS SERVO PWM TO ACTUAL DRIVE MODULE POSITION
  this->servo.write((int)(1.41*(double)target)+7.0);
}

boolean DriveModule::driveDist(float target){
  //NEED TO IMPLEMENT PID CLASS INSTEAD IN THIS FUNCTION STILL
  static int dir = 1;
  static uint16_t error;
  static const float kp = PID_DIST_KP;
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
      if(effortSpeed > CONTROLLED_DRIVE_SPEED){effortSpeed = CONTROLLED_DRIVE_SPEED;} //75 can be replaced with desired max speed
      driveSpeed(effortSpeed * dir);
      
      //If current counts is within 5 counts of goal, change to waiting state
      if(abs(this->targetCounts - currCounts) < DRIVE_DIST_TOLERANCE_ENC){
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
      else if(abs(this->targetCounts - currCounts) >= DRIVE_DIST_TOLERANCE_ENC){
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

void DriveModule::ButtonISR(){
  offGround = true;
  timeOffGround = millis();
}