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
        int32_t prev = 0;
        int32_t errorSum = 0;

        int calcPIDSpeed(int16_t target, int16_t curr, int16_t cap);
    private:
};
#endif
