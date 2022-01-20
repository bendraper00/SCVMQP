#ifndef DRIVEMODULE_H_
#define DRIVEMODULE_H_
#include <L298N.h>
#include <stdint.h>
#include "PIDcontroller.h"
class DriveModule{
    public:
      DriveModule(void);
      void init();
      void driveSpeed(uint8_t target, boolean dir);
      boolean setWheelAngle(uint8_t target);
      boolean driveDist(float target, float speed);
      //rotate wheels
      //turn robot (can only occur when wheel axis colinear)
    
    private:
      L298N motor;
      PIDController pid;
    
};
#endif
