#include "DriveModule.h"
#include <Arduino.h>

DriveModule::DriveModule(const unsigned int EN, const unsigned int IN1, const unsigned int IN2, uint8_t chA, uint8_t chB){
  this->motor = new L298N(EN, IN1, IN2);
  this->enc = new Encoder(chA, chB);
}

void DriveModule::init(){
  this->enc->init();
}

void DriveModule::driveSpeed(uint8_t target, boolean dir){
    this->motor->setSpeed(target);
    if(dir == true){
      this->motor->forward();
    }
    else{
      this->motor->backward();
    }
}

boolean DriveModule::setWheelAngle(uint8_t target){}

boolean DriveModule::driveDist(float target){
  static boolean dir = true;
  static int16_t error;
  static float kp = 1;
  static State state = WAITING;
  static uint16_t startTime;
  boolean status = false;;
  static int effortSpeed = 0;
  this->enc->updateCounts();
  uint16_t currCounts = this->enc->getCounts();

  //Serial.println("STATE: " + String(state) + "\t\tEffort: " + String(effortSpeed) + "\t\tDirection: " + String(dir));
  Serial.println("TARGET: " + String(this->targetCounts) + "\t\tCURRENT: " + String(currCounts));

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
      if(this->targetCounts - currCounts < 0){dir = false;}
      //Effort speed (actually 0-255) is calculated
      effortSpeed = int(kp * error);
      if(effortSpeed > 255){effortSpeed = 255;}
      driveSpeed(effortSpeed, dir);
      
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
