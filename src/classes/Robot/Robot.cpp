#include "Robot.h"

Robot::Robot(){}

void Robot::init(){
    this->encoders = new Encoder(FRONT_ENCODER_A, FRONT_ENCODER_B, REAR_ENCODER_A, REAR_ENCODER_B);
    this->encoders->init();
    this->frontDrive = new DriveModule(FRONT_DRIVE_EN, 
                                       FRONT_DRIVE_IN1, 
                                       FRONT_DRIVE_IN2, 
                                       this->encoders, 
                                       FRONT_DRIVE_SERVO);
    this->rearDrive = new DriveModule(REAR_DRIVE_EN, 
                                       REAR_DRIVE_IN1, 
                                       REAR_DRIVE_IN2, 
                                       this->encoders, 
                                       REAR_DRIVE_SERVO);
    this->frontDrive->init();
    Serial.println("Front Drive Module Initialized");
    this->rearDrive->init();
    Serial.println("Rear Drive Module Initialized");
    this->sensors = new Sensors();
    this->sensors->init();
    this->scissors = new Scissors();
    this->scissors->init();
    Serial.println("Scissor Lifts Initialized");
    this->stagePID = new PIDController(PID_SCISSOR_KP, PID_SCISSOR_KI, PID_SCISSOR_KD);

    pinMode(FRONT_WHEEL_SWITCH, INPUT_PULLUP);
    pinMode(REAR_WHEEL_SWITCH, INPUT_PULLUP);
    pinMode(BUMP_LEFT, INPUT_PULLUP);
    pinMode(BUMP_RIGHT, INPUT_PULLUP);

}

void Robot::pidSpeed(int16_t speed){ //This may be better if the two wheels try to maintain equal counts rather than speeds. Will determine in testing
    this->frontDrive->pidFSpeed(speed);
    this->rearDrive->pidRSpeed(speed);
}

void Robot::stairFollow(int16_t speed, uint8_t dist){
    int magnitude;
    int difference;
    sensors->getRangeData(magnitude, difference);
     if(speed > 0){
        int error = magnitude - dist;
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    else if(speed < 0){
        int error = dist - magnitude; //negative error
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    
    int16_t frontEffort = speed + difference;
    int16_t rearEffort = speed - difference;
    
    this->frontDrive->pidFSpeed(frontEffort);
    this->rearDrive->pidRSpeed(rearEffort);
}

bool Robot::home(){
    if(!digitalRead(FRONT_OPEN_ENDSTOP) && !digitalRead(REAR_OPEN_ENDSTOP)){
        return true;
    }
    else{
        this->scissors->lowerFront(MAX_DRIVE_SPEED);
        this->scissors->raiseRear(MAX_DRIVE_SPEED);
        return false;
    }
}

bool Robot::raiseFront(int16_t speed){
    switch(botState){
        case IDLE:
            this->botState = RAISINGFRONT;
            break;

        case RAISINGFRONT:
            if(this->scissors->fState == Scissors::CLOSED){botState = LOWERINGFRONT;}
            this->scissors->raiseFront(speed);
            break;

        //ADD CASE FOR MOVING FORWARD x MM AWAY FROM STEP

        case LOWERINGFRONT:
            if(digitalRead(FRONT_WHEEL_SWITCH)){
                this->scissors->frontSpeed(0);
                botState = IDLE;
                return true;
            }
            this->scissors->lowerFront(speed);
            break;

    }
    return false;
}

bool Robot::raiseMid(int16_t speed){
    //Use a gyro axis to keep stage level, speeding up stages to compensate
    //Only run when rear is open and front is closed or opening
    float angle;
    switch(botState){
        case IDLE:
            if(this->scissors->fState == Scissors::OPENING){botState = RAISINGMID;}
            break;

        case RAISINGMID:
            this->sensors->getPitch(angle);
            int effort = this->stagePID->calcPID(0.0, angle, MAX_DRIVE_SPEED);
            if(effort<=0){
                this->scissors->lowerFront(speed);
                this->scissors->lowerRear(speed-abs(effort));
            }
            else{
                this->scissors->lowerFront(speed-abs(effort));
                this->scissors->lowerRear(speed);
            }
            if(this->scissors->fState == Scissors::OPEN){
                this->scissors->frontSpeed(0);
                this->scissors->rearSpeed(0);
                botState = IDLE;
                return true;
            }
            Serial.print("ANGLE: ");
            Serial.print(angle);
            Serial.print("\t\tEffort:");
            Serial.println(effort);
            break;
    }

    return false;
}

bool Robot::raiseRear(){
    this->scissors->raiseRear(200);
}