#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_
class StateMachine{
    public:
        StateMachine();
        void init();
        void updateStateMachine();
    private:
        enum robotState{IDLE, ASCENDING, DESCENDING, SWEEP};
};
#endif