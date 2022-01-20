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
        float prev;
        float targ;
    private:
};
#endif
