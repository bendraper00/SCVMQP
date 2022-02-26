#include "Scissors.h"

enum ScissorState{OPENING, CLOSING, OPEN, CLOSED};
ScissorState frontState = CLOSED;
ScissorState rearState = CLOSED;

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
    pinMode(this->frontOpenEs, INPUT_PULLUP);
    pinMode(this->frontClosedEs, INPUT_PULLUP);
    pinMode(this->rearOpenEs, INPUT_PULLUP);
    pinMode(this->rearClosedEs, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(this->frontOpenEs), Scissors::scissorFOISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(this->frontClosedEs), Scissors::scissorFCISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(this->rearOpenEs), Scissors::scissorROISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(this->rearClosedEs), Scissors::scissorRCISR, FALLING);
}

void Scissors::frontSpeed(int16_t speed){
    this->frontMotor->setSpeed(abs(speed));
    if(speed > 0){
      this->frontMotor->forward();
    }
    else if(speed < 0){
      this->frontMotor->backward();
    }
    else{
      this->frontMotor->stop();
    }
}

void Scissors::rearSpeed(int16_t speed){
    this->rearMotor->setSpeed(abs(speed));
    if(speed > 0){
      this->rearMotor->forward();
    }
    else if(speed < 0){
      this->rearMotor->backward();
    }
    else{
      this->rearMotor->stop();
    }
}

void Scissors::openFront(int16_t speed){
    if(frontState == CLOSED){frontState = OPENING};
    switch (frontState){
        case OPENING:
            //Run motors
            break;
        case CLOSING:
            //Error case
            break;
        case OPEN:
            //Do Nothing
            break;
        case CLOSED:
            //Error case
            break;
    }
    this->fState = frontState;
    this->rState = rearState;
}

void Scissors::closeFront(int16_t speed){
    if(frontState == OPEN){frontState = CLOSING};
    switch (frontState){
        case OPENING:
            //Error case
            break;
        case CLOSING:
            //Run motors
            break;
        case OPEN:
            //Error case
            break;
        case CLOSED:
            //Do Nothing
            break;
    }
    this->fState = frontState;
    this->rState = rearState;
}

void Scissors::openRear(int16_t speed){
    if(rearState == CLOSED){rearState = OPENING};
    switch (rearState){
        case OPENING:
            //Run motors
            break;
        case CLOSING:
            //Error case
            break;
        case OPEN:
            //Do Nothing
            break;
        case CLOSED:
            //Error case
            break;
    }
    this->fState = frontState;
    this->rState = rearState;
}

void Scissors::closeRear(int16_t speed){
    if(frontState == OPEN){frontState = CLOSING};
    switch (frontState){
        case OPENING:
            //Error case
            break;
        case CLOSING:
            //Run motors
            break;
        case OPEN:
            //Error case
            break;
        case CLOSED:
            //Do Nothing
            break;
    }
    this->fState = frontState;
    this->rState = rearState;
}

void Scissors::scissorFOISR(){
    frontState = OPEN;
    return;
}

void Scissors::scissorFCISR(){
    frontState = CLOSED;
    return;
}

void Scissors::scissorROISR(){
    rearState = OPEN;
    return;
}

void Scissors::scissorRCISR(){
    rearState = CLOSED;
    return;
}