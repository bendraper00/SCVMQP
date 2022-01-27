#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"
#include "src\classes\Sensors\Sensors.h"

//Objects
DriveModule mod(FRONT_DRIVE_EN, 
                FRONT_DRIVE_IN1, 
                FRONT_DRIVE_IN2, 
                FRONT_ENCODER_A, 
                FRONT_ENCODER_B, 
                FRONT_DRIVE_SERVO);
Sensors sens;

//Temp Variables for testing

void setup()
{
  Serial.begin(9600);
  mod.init();
  sens.init();
  delay(300);
}


void loop()
{
  //mod->pidSpeed(100);
  sens.readRange();
}
