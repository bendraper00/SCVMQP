#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_
#include <Arduino.h>

class PIDController{
    public:
        PIDController();
        PIDController(float p, float i, float d);
        float kp;
        float ki;
        float kd;
        float prev = 0;
        uint16_t errorSum = 0;

        int calcPIDSpeed(uint8_t target, int16_t curr, uint16_t cap);
    private:
};
#endif
