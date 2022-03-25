#ifndef ROBOT_H_
#define ROBOT_H_
#include "..\DriveModule\DriveModule.h"
#include "..\Scissors\Scissors.h"
#include "..\Sensors\Sensors.h"
#include "..\PIDController\PIDcontroller.h"
#include "..\..\..\config.h"

class Robot{
    public:
        Robot();
        void init();
        DriveModule* frontDrive;
        DriveModule* rearDrive;
        Scissors* scissors;
        Encoder* encoders; //This reference is used to construct drive modules
        Sensors* sensors;
        PIDController* stagePID;
        void pidSpeed(int16_t speed);
        void stairFollow(int16_t speed, uint8_t dist);
        bool raiseFront(int16_t speed); //Raises front stage
        bool raiseMid(int16_t speed); //Closes front stage and opens rear state
        bool raiseRear(); //Closes rear stage
        enum RobotState{RAISINGFRONT, LOWERINGFRONT, RAISINGMID, IDLE};
        RobotState botState = IDLE;
        
    private:
};

#endif
