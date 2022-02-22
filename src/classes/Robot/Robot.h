#ifndef ROBOT_H_
#define ROBOT_H_
#include "..\DriveModule\DriveModule.h"
#include "..\Scissors\Scissors.h"
#include "..\Sensors\Sensors.h"
#include "..\..\..\config.h"

class Robot{
    public:
        Robot();
        void init();
        DriveModule* frontDrive;
        DriveModule* rearDrive;
        Scissors* scissors;
        Encoder* encoders;
        Sensors* sensors;
        void pidSpeed(int16_t speed);

        void stairFollow(uint8_t speed, uint8_t dist);
        
    private:
};

#endif
