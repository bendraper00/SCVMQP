#include <Arduino.h>
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"

uint8_t chA = 2;
uint8_t chB = 3;
const unsigned int EN = 4;
const unsigned int IN1 = 5;
const unsigned int IN2 = 6;
uint8_t servoPin = 9;

DriveModule *mod;


void setup()
{
  Serial.begin(9600);
  //while(!Serial){}
  mod = new DriveModule(EN, IN1, IN2, chA, chB, servoPin);
  mod->init();
}

void loop()
{
  //mod->driveDist(40*3.1415);
  mod->setWheelAngle(90);
}
