#include "Scissor.h"
#include "Chassis.h"
#include "Vacuum.h"
#include <Arduino.h>
#include <Wire.h>


class Robot{
    public:
        void init();
        Robot();
        Scissor frontScissor;
        Scissor rearScissor;
        Chassis frontDrive;
        Chassis rearDrive;
        Vacuum vac;
        L298N imu;
    private:
};