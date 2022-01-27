#include "Robot.h"

Robot::Robot(){}

void Robot::init(){
    this->frontDrive = new DriveModule(FRONT_DRIVE_EN, 
                                       FRONT_DRIVE_IN1, 
                                       FRONT_DRIVE_IN2, 
                                       FRONT_ENCODER_A, 
                                       FRONT_ENCODER_B, 
                                       FRONT_DRIVE_SERVO);
    this->frontDrive->init();
    this->tofSensor.init();
}

void Robot::stairFollow(uint8_t speed, uint8_t dist){
    this->frontDrive->driveSpeed(speed);

    if(speed > 0){
        int error = dist - tofSensor.getRange();
        int angle = 90 + error;
        if(angle > 120){angle = 120;}
        if(angle < 60){angle = 60;}
        this->frontDrive->setWheelAngle(angle);
    }
    if(speed < 0){
        int error = tofSensor.getRange() - dist; //negative error
        int angle = 90 + error;
        if(angle > 120){angle = 120;}
        if(angle < 60){angle = 60;}
        this->frontDrive->setWheelAngle(angle);
    }
}