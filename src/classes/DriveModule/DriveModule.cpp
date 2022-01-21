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
  ///PROBABLY NEED TO IMPLEMENT EVENT TIMER HERE
  
  static boolean dir = true;
  static int16_t error;
  static float kp = 200;
  
  if(this->moving == false){
    this->enc->updateCounts();
    this->targetCounts = this->enc->getCounts() + this->enc->distToCounts(target);
    this->moving = true; 
  }

  while(abs(this->targetCounts - this->enc->getCounts()) >= 5){
    this->enc->updateCounts();
    error = this->targetCounts - this->enc->getCounts();
    if(this->targetCounts - this->enc->getCounts() < 0){dir = false;}
    float effortSpeed = kp * error;
    driveSpeed(effortSpeed, dir);
    //Serial.println(this->enc->getCounts());
  }

  return true;
}
