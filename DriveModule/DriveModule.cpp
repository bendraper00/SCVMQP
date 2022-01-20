#include "DriveModule.h"
#include <Arduino.h>

DriveModule::DriveModule(void){
  this->motor = new L298N();
  this->pid = new PIDController(1.0, 1.0, 1.0);
  }

void DriveModule::init(){}

void DriveModule::driveSpeed(uint8_t target, boolean dir){
    
  }

boolean DriveModule::setWheelAngle(uint8_t target){}

boolean DriveModule::driveDist(float target, float speed){}
