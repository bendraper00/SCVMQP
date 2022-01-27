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

        int calcPIDSpeed(int16_t curr);
    private:
};
#endif
