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
        void openFront(int16_t speed);
        void closeFront(int16_t speed);
        void openRear(int16_t speed);
        void closeRear(int16_t speed);
        static void scissorFOISR();
        static void scissorFCISR();
        static void scissorROISR();
        static void scissorRCISR();
        void frontSpeed(int16_t speed);
        void rearSpeed(int16_t speed);
        enum ScissorState{OPENING, CLOSING, OPEN, CLOSED};
        ScissorState fState = CLOSED;
        ScissorState rState = CLOSED;
        
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