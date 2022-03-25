#include "PIDcontroller.h"

PIDController::PIDController(){}

PIDController::PIDController(float p, float i, float d){
  this->kp = p;
  this->ki = i;
  this->kd = d;
}

int PIDController::calcPIDSpeed(int32_t target, int32_t curr, int32_t cap){
  float actual = 0;
  if(curr > 0 && this->prev < 0) {actual = target;} //A hack to solve issue with variable rollover
  else if(curr < 0 && this->prev > 0) {actual = target;}
  else{actual = curr - this->prev;}
  //Serial.println(curr);
  this->prev = curr;
  float error = target - actual;
  this->errorSum += error;
  float dError = (error - this->prevError)/(millis()-prevTime);
  this->prevError = error;
  this->prevTime = millis();
  
  int32_t output = kp*error + kd*dError + ki*this->errorSum;
  
  if(output > cap){output = cap;}
  if(output < -1*cap){output = -1*cap;}
  return output;
}
