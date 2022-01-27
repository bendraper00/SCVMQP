#ifndef DRIVEMODULE_H_
#define DRIVEMODULE_H_
#include <L298N.h>
#include <stdint.h>
#include "..\PIDController\PIDcontroller.h"
#include "..\Encoder\Encoder.h"
#include <Servo.h>

class DriveModule{
    public:
      DriveModule(const uint8_t EN,
                  const uint8_t IN1,
                  const uint8_t IN2,
                  const uint8_t chA,
                  const uint8_t chB,
                  const uint8_t servoPin);
      void init();
      void driveSpeed(uint8_t target);
      void pidSpeed(uint8_t target);
      boolean setWheelAngle(int8_t target);
      boolean driveDist(float target);
      //turn robot or Skid Steer (can only occur when wheel axis colinear)
      boolean moving = false;
      Encoder* enc;
      Servo servo;

    private:
      L298N* motor;
      uint8_t servoPin;
      int32_t targetCounts;
      enum State{DRIVING, WAITING};
      PIDController* speedPID;
      
};
#endif
