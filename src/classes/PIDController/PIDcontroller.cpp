#include "PIDcontroller.h"

PIDController::PIDController(){}

PIDController::PIDController(float p, float i, float d){
  this->kp = p;
  this->ki = i;
  this->kd = d;
}

int PIDController::calcPIDSpeed(int16_t target, int16_t curr, int16_t cap){
  noInterrupts();
  int16_t actual = curr - this->prev;
  this->prev = curr;
  interrupts();

  int16_t error = target - actual;
  this->errorSum += error;
  int32_t output = kp*error + ki*this->errorSum;
  
  if(output > cap){output = cap;}

  if(output < -1*cap){output = -1*cap;}
  
  return output;
}
