#include "Scissors.h"

Scissors::Scissors(){
    this->frontMotor = new L298N(FRONT_SCISSOR_EN, FRONT_SCISSOR_IN1, FRONT_SCISSOR_IN2);
    this->rearMotor = new L298N(REAR_SCISSOR_EN, REAR_SCISSOR_IN1, REAR_SCISSOR_IN2);
    this->speedPID = new PIDController(PID_SCISSOR_KP, PID_SCISSOR_KI, PID_SCISSOR_KD);
    this->frontOpenEs = FRONT_OPEN_ENDSTOP;
    this->frontClosedEs = FRONT_CLOSED_ENDSTOP;
    this->rearOpenEs = REAR_OPEN_ENDSTOP;
    this->rearClosedEs = REAR_CLOSED_ENDSTOP;
}

void Scissors::init(){
    pinMode(this->frontOpenEs, INPUT_PULLUP)
    pinMode(this->frontClosedEs, INPUT_PULLUP)
    pinMode(this->rearOpenEs, INPUT_PULLUP)
    pinMode(this->rearClosedEs, INPUT_PULLUP)
    attachInterrupt(digitalPinToInterrupt(this->frontOpenEs), Scissors::scissorISR, FALLING)
    attachInterrupt(digitalPinToInterrupt(this->frontClosedEs), Scissors::scissorISR, FALLING)
    attachInterrupt(digitalPinToInterrupt(this->rearOpenEs), Scissors::scissorISR, FALLING)
    attachInterrupt(digitalPinToInterrupt(this->rearClosedEs), Scissors::scissorISR, FALLING)
}

void Scissors::scissorISR(){
    return;
}