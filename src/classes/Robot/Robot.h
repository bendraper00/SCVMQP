#ifndef ROBOT_H_
#define ROBOT_H_
#include "..\DriveModule\DriveModule.h"
#include "..\Scissors\Scissors.h"
#include "..\Sensors\Sensors.h"
#include "..\PIDController\PIDcontroller.h"
#include "..\..\..\config.h"

class Robot{
    public:
        Robot();
        void init();
        DriveModule* frontDrive;
        DriveModule* rearDrive;
        Scissors* scissors;
        Encoder* encoders; //This reference is used to construct drive modules
        Sensors* sensors;
        

        bool home();
        void pidSpeed(int16_t speed);
        void stairFollow(int16_t speed, uint8_t dist);
        bool raiseFront(int16_t speed); //Raises front stage
        bool raiseMid(int16_t speed); //Closes front stage and opens rear state
        bool raiseRear(); //Closes rear stage
        bool allignStep();
        bool ascendStep();
        bool driveTo(float mm);

        enum RobotState{APPROACHING, ALLIGNING, RAISINGFRONT, LOWERINGFRONT, RAISINGMID, RAISINGREAR, IDLE, WAITING, DRIVING};
        RobotState botState = IDLE;
        
    private:
        PIDController* stagePID;
        PIDController* allignPID;
        PIDController* distPID;
        int delay = 2000;
};

#endif
