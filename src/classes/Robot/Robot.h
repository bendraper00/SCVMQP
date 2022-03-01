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
        //Sensors* sensors;
        PIDController* stagePID;
        void pidSpeed(int16_t speed);
        void stairFollow(uint8_t speed, uint8_t dist);
        void raiseFront(); //Raises front stage
        void raiseMid(); //Closes front stage and opens rear state
        void raiseRear(); //Closes rear stage
        
    private:
};

#endif
