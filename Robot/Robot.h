#include "Scissor.h"
#include "DriveModule.h""
#include "Vacuum.h"
#include <Arduino.h>
#include <Wire.h>

#ifndef ROBOT_H_
#define ROBOT_H_
class Robot{
    public:
        void init();
        Robot();
        Scissor frontScissor;
        Scissor rearScissor;
        DriveModule frontDrive;
        DriveModule rearDrive;
        Vacuum vac;
        L298N imu;
    private:
};
#endif
