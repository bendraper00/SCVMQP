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
  this->prev = curr;
  float error = target - actual;
  this->errorSum += error;
  float dError = (error - this->prevError)/(millis()-prevTime);
  this->prevError = error;
  this->prevTime = millis();
  

  // Serial.print("Error: ");
  // Serial.print(error);
  // Serial.print("\t\tErrorSum: ");
  // Serial.print(this->errorSum);
  // Serial.print("\t\tdError: ");
  // Serial.println(dError);

  int32_t output = kp*error + kd*dError + ki*this->errorSum;
  
  // Serial.print(actual);
  // Serial.print(",");
  // Serial.println(output);

  if(output > cap){
    output = cap;
  }
  if(output < -1*cap){
    output = -1*cap;
  }

  return output;
}

int PIDController::calcPID(float target, float curr, int32_t cap){
  if(this->rs == true){
    this->rs = false;
    this->errorSum = 0.0;
    this->prevError = 0.0;
    this->prevTime = 0;
    this->prev = 0;
  }

  float error = target - curr;
  this->errorSum += error;
  float dError = (error - this->prevError)/(millis()-prevTime);
  this->prevError = error;
  this->prevTime = millis();

  // Serial.print("Error: ");
  // Serial.print(error);
  // Serial.print("\t\tErrorSum: ");
  // Serial.print(this->errorSum);
  // Serial.print("\t\tdError: ");
  // Serial.println(dError);

  int output = kp*error + kd*dError + ki*this->errorSum;

  if(output > cap){
    output = cap;
  }
  if(output < -1*cap){
    output = -1*cap;
  }

  return output;
}

int PIDController::calcPID(float target, float curr, int32_t cap, uint32_t integralCap){
  if(this->rs == true){
    this->rs = false;
    this->errorSum = 0.0;
    this->prevError = 0.0;
    this->prevTime = 0;
    this->prev = 0;
  }

  float error = target - curr;
  if(abs(errorSum) < integralCap){this->errorSum += error;}
  float dError = (error - this->prevError)/(millis()-prevTime);
  this->prevError = error;
  this->prevTime = millis();

  int output = kp*error + kd*dError + ki*this->errorSum;

  if(output > cap){
    output = cap;
  }
  if(output < -1*cap){
    output = -1*cap;
  }

  return output;
}

void PIDController::reset(){
  this->rs = true;
}