#ifndef SCISSORS_H_
#define SCISSORS_H_
#include <Arduino.h>
#include <L298N.h>
#include <stdint.h>
#include "..\PIDController\PIDcontroller.h"
#include "../../../config.h"
class Scissors{
    public:
        Scissors();
        void init();
        static void scissorISR();
    private:
        L298N* frontMotor;
        L298N* rearMotor;
        const uint8_t frontOpenEs;
        const uint8_t frontClosedEs;
        const uint8_t rearOpenEs;
        const uint8_t rearClosedEs;
        PIDController* speedPID;
};
#endif