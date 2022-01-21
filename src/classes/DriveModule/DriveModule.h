#ifndef DRIVEMODULE_H_
#define DRIVEMODULE_H_
#include <L298N.h>
#include <stdint.h>
#include "..\PIDController\PIDcontroller.h"
#include "..\Encoder\Encoder.h"
#include <Servo.h>

class DriveModule{
    public:
      DriveModule(const unsigned int EN,
                  const unsigned int IN1,
                  const unsigned int IN2,
                  uint8_t chA,
                  uint8_t chB,
                  uint8_t servoPin);
      void init();
      void driveSpeed(uint8_t target, boolean dir);
      boolean setWheelAngle(int8_t target);
      boolean driveDist(float target);
      //turn robot or Skid Steer (can only occur when wheel axis colinear)
    
    private:
      L298N* motor;
      uint8_t servoPin;
      Servo servo;
      boolean moving = false;
      Encoder* enc;
      int16_t targetCounts;
      enum State{DRIVING, WAITING};
      
};
#endif
