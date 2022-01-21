#include "PIDcontroller.h"

PIDController::PIDController(){}

PIDController::PIDController(float p, float i, float d){
  this->kp = p;
  this->ki = i;
  this->kd = d;
}
