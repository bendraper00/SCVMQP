#ifndef ROBOT_H_
#define ROBOT_H_
#include "..\DriveModule\DriveModule.h"
#include "..\Sensors\Sensors.h"
#include "..\..\..\config.h"

class Robot{
    public:
        Robot();
        void init();
        DriveModule* frontDrive;
        DriveModule* rearDrive;
        Encoder* encoders;
        Sensors sensors;

        //void stairFollow(uint8_t speed, uint8_t dist);
        
    private:
};

#endif
