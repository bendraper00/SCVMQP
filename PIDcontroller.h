#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_
class PIDController{
    public:
        PIDController(float p, float i, float d);
        float kp;
        float ki;
        float kd;
        float prev;
        float targ;
    private:
};
#endif