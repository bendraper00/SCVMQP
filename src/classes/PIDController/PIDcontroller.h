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
        uint32_t prevTime = 0;
        float prevError = 0.0;
        float errorSum = 0.0;

        int calcPIDSpeed(int32_t target, int32_t curr, int32_t cap);
        int calcPID(float target, float curr, int32_t cap);

    private:
};
#endif
