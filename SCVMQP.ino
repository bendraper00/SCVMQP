#include <Arduino.h>
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"
#include "src\classes\Sensors\Sensors.h"

//PIN DEFINITIONS
uint8_t chA = 2;
uint8_t chB = 51;
const unsigned int EN = 10;
const unsigned int IN1 = 53;
const unsigned int IN2 = 52;
uint8_t servoPin = 9;

//Objects
DriveModule mod(EN, IN1, IN2, chA, chB, servoPin);
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
