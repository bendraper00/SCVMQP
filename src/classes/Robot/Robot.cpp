#include "Robot.h"

Robot::Robot(){}

void Robot::init(){
    this->frontDrive = new DriveModule(FRONT_DRIVE_EN, 
                                       FRONT_DRIVE_IN1, 
                                       FRONT_DRIVE_IN2, 
                                       FRONT_ENCODER_A, 
                                       FRONT_ENCODER_B, 
                                       FRONT_DRIVE_SERVO);
    this->rearDrive = new DriveModule(REAR_DRIVE_EN, 
                                       REAR_DRIVE_IN1, 
                                       REAR_DRIVE_IN2, 
                                       REAR_ENCODER_A, 
                                       REAR_ENCODER_B, 
                                       REAR_DRIVE_SERVO);
    this->frontDrive->init();
    this->rearDrive->init();
    //this->tofSensor.init();
}

// void Robot::stairFollow(uint8_t speed, uint8_t dist){
//     this->frontDrive->driveSpeed(speed);

//     if(speed > 0){
//         int error = dist - tofSensor.getRange();
//         int angle = SERVO_POS_3 + error;
//         if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
//         if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
//         this->frontDrive->setWheelAngle(angle);
//     }
//     if(speed < 0){
//         int error = tofSensor.getRange() - dist; //negative error
//         int angle = SERVO_POS_3 + error;
//         if(angle > SERVO_POS_4){angle = SERVO_POS_4;}
//         if(angle < SERVO_POS_2){angle = SERVO_POS_2;}
//         this->frontDrive->setWheelAngle(angle);
//     }
// }