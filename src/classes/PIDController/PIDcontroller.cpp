#include "PIDcontroller.h"

PIDController::PIDController(){}

PIDController::PIDController(float p, float i, float d){
  this->kp = p;
  this->ki = i;
  this->kd = d;
}

int PIDController::calcPIDSpeed(uint8_t target, int16_t curr, uint16_t cap){
  noInterrupts();
  int16_t actual = curr - prev;
  prev = curr;
  interrupts();

  int16_t error = target - actual;
  errorSum += error;

  int output = kp*error + ki*errorSum;
  if(output > cap){output = cap;}
  if(output < -1*cap){output = -1*cap;}

  return output;
}
