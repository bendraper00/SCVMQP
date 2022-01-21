#include <Arduino.h>
#include <stdint.h>
#include "DriveModule\DriveModule.h"

uint8_t chA = 2;
uint8_t chB = 3;
const unsigned int EN = 4;
const unsigned int IN1 = 5;
const unsigned int IN2 = 6;

DriveModule mod(EN, IN1, IN2, chA, chB);

void setup()
{
  Serial.begin(9600);
  mod.init();
}

void loop()
{
  mod.driveDist(40*3.1415);
}
