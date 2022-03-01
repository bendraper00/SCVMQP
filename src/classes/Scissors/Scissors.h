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
        void raiseFront(int16_t speed);
        void lowerFront(int16_t speed);
        void lowerRear(int16_t speed);
        void raiseRear(int16_t speed);
        static void scissorFOISR();
        static void scissorROISR();
        void frontSpeed(int16_t speed);
        void rearSpeed(int16_t speed);

        enum ScissorState{OPENING, CLOSING, OPEN, CLOSED};
        ScissorState fState = OPEN;
        ScissorState rState = OPEN;
        
    private:
        L298N* frontMotor;
        L298N* rearMotor;
        PIDController* speedPID;
        uint8_t frontOpenEs;
        uint8_t frontClosedEs;
        uint8_t rearOpenEs;
        uint8_t rearClosedEs;
};
#endif