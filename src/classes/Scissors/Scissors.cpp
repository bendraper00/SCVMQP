#include "Scissors.h"

static Scissors::ScissorState frontState;
static Scissors::ScissorState rearState;

Scissors::Scissors(){
    this->frontMotor = new L298N(FRONT_SCISSOR_EN, FRONT_SCISSOR_IN1, FRONT_SCISSOR_IN2);
    this->rearMotor = new L298N(REAR_SCISSOR_EN, REAR_SCISSOR_IN1, REAR_SCISSOR_IN2);
    this->speedPID = new PIDController(PID_SCISSOR_KP, PID_SCISSOR_KI, PID_SCISSOR_KD);
    this->frontOpenEs = FRONT_OPEN_ENDSTOP;
    this->frontClosedEs = FRONT_CLOSED_ENDSTOP;
    this->rearOpenEs = REAR_OPEN_ENDSTOP;
    this->rearClosedEs = REAR_CLOSED_ENDSTOP;

    frontState = OPENING;
    rearState = OPENING;
}

void Scissors::init(){
    pinMode(this->frontOpenEs, INPUT_PULLUP);
    pinMode(this->frontClosedEs, INPUT_PULLUP);
    pinMode(this->rearOpenEs, INPUT_PULLUP);
    pinMode(this->rearClosedEs, INPUT_PULLUP);

    delay(10);
    noInterrupts();
    attachInterrupt(digitalPinToInterrupt(this->frontOpenEs), Scissors::scissorFOISR, FALLING);
    attachInterrupt(digitalPinToInterrupt(this->rearOpenEs), Scissors::scissorROISR, FALLING);
    interrupts();
    delay(10);

    if(digitalRead(this->frontOpenEs)== HIGH){
        frontState = CLOSED;
        this->fState = CLOSED;
    }
    if(digitalRead(this->rearOpenEs)== HIGH){
        rearState = CLOSED;
        this->rState = CLOSED;
    }
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


/*
ALL OF THE OPEN AND CLOSE FUNCTIONS SHOULD BE RUN IN A LOOP UNTIL THE DESIRED STATE IS MET
*/
void Scissors::raiseFront(int16_t speed){
    if(frontState == OPEN || frontState == OPENING){frontState = CLOSING;}
    switch (frontState){
        case OPENING:
            //Error case
            this->frontSpeed(0);
            break;

        case CLOSING:
            if(digitalRead(this->frontClosedEs) == LOW){
                this->frontSpeed(0);
                frontState = CLOSED;
                break;
                }
            this->frontSpeed(speed * -1);
            break;

        case OPEN:
        //ERROR CASE
            this->frontSpeed(0);
            break;

        case CLOSED:
            this->frontSpeed(0);
            break;
    }
    this->fState = frontState;
}

void Scissors::lowerFront(int16_t speed){
    if(frontState == CLOSED || frontState == CLOSING){frontState = OPENING;}
    switch (frontState){
        case OPENING:
            if(digitalRead(this->frontOpenEs) == LOW){ //If for some reason you try to lower and the ISR can't run
                frontState = OPEN;
                this->frontSpeed(0);
                break;
            }
            this->frontSpeed(speed);
            break;
        case CLOSING:
            //Error Case
            this->frontSpeed(0);
            break;
        case OPEN:
            this->frontSpeed(0);
            break;
        case CLOSED:
            //Error case
            this->frontSpeed(0);
            break;
    }
    this->fState = frontState;
}

void Scissors::lowerRear(int16_t speed){
    if(rearState == OPEN || rearState == OPENING){rearState = CLOSING;}
    switch (rearState){
        case OPENING:
            //Error Case
            this->rearSpeed(0);
            break;

        case CLOSING:
             if(digitalRead(this->rearClosedEs) == LOW){
                this->rearSpeed(0);
                rearState = CLOSED;
                break;
            }
            this->rearSpeed(speed);
            break;

        case OPEN:
            //Error case
            this->rearSpeed(0);
            break;

        case CLOSED:
            this->rearSpeed(0);
            break;
    }
    this->rState = rearState;
}




void Scissors::raiseRear(int16_t speed){
    if(rearState == CLOSED || rearState == CLOSING){rearState = OPENING;}
    switch (rearState){
        case OPENING:
            if(digitalRead(this->rearOpenEs) == LOW){ //If for some reason you try to raise and the ISR can't run
                this->rearSpeed(0);
                rearState = OPEN;
                break;
            }
            this->rearSpeed(speed * -1);
            break;

        case CLOSING:
            //Error case
            this->rearSpeed(0);
            break;

        case OPEN:
            this->rearSpeed(0);
            break;

        case CLOSED:
            //Error case
            this->rearSpeed(0);
            break;
    }
    this->rState = rearState;
}




bool Scissors::stepRearDown(){
    static long t = 0;
    static bool flag = true;
    if(flag){
        flag = false;
        t = millis();
    }
    if(millis()-t >= 250){
        this->rearSpeed(0);
        flag = true;
        return true;
    }
    else{
        this->lowerRear(MAX_DRIVE_SPEED);
        return false;
    }
}

//Front open ISR
void Scissors::scissorFOISR(){
    frontState = OPEN;
    return;
}

//Rear open ISR
void Scissors::scissorROISR(){
    rearState = OPEN;
    return;
}

//If the closed positions become interrupts, then add FCISR and RCISR
