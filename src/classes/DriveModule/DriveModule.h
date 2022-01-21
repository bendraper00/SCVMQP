#ifndef DRIVEMODULE_H_
#define DRIVEMODULE_H_
#include <L298N.h>
#include <stdint.h>
#include "..\PIDController\PIDcontroller.h"
#include "..\Encoder\Encoder.h"

class DriveModule{
    public:
      DriveModule(const unsigned int EN,
                  const unsigned int IN1,
                  const unsigned int IN2,
                  uint8_t chA,
                  uint8_t chB);
      void init();
      void driveSpeed(uint8_t target, boolean dir);
      boolean setWheelAngle(uint8_t target);
      boolean driveDist(float target);
      //rotate wheels
      //turn robot (can only occur when wheel axis colinear)
      Encoder* enc;
      uint16_t targetCounts;
    
    private:
      L298N* motor;
      boolean moving = false;
      enum State{DRIVING, WAITING};
      
};
#endif
