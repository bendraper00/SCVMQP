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
}

void Robot::pidSpeed(int16_t speed){ //This may be better if the two wheels try to maintain equal counts rather than speeds. Will determine in testing
    this->frontDrive->pidFSpeed(speed);
    this->rearDrive->pidRSpeed(speed);
}


void Robot::stairFollow(int16_t speed, uint8_t dist){
    
     if(speed > 0){
        int error = sensors->getRangeMagnitude() - dist;
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    else if(speed < 0){
        int error = dist - sensors->getRangeMagnitude(); //negative error
        int angle = SERVO_POS_3 + error;
        if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
        if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
        this->frontDrive->setWheelAngle(angle);
        this->rearDrive->setWheelAngle(angle);
    }
    

    int16_t diff = sensors->getDifference();
    int16_t frontEffort = speed + diff;
    int16_t rearEffort = speed - diff;
    Serial.print(frontEffort);
    Serial.print("\t\t");
    Serial.print(rearEffort);
    Serial.println();
    
    this->frontDrive->pidFSpeed(frontEffort);
    this->rearDrive->pidRSpeed(rearEffort);
}


void Robot::raiseFront(){
    this->scissors->raiseFront(200);
}
void Robot::raiseMid(){
    //Use a gyro axis to keep stage level, speeding up stages to compensate
}
void Robot::raiseRear(){
    this->scissors->raiseRear(200);
}