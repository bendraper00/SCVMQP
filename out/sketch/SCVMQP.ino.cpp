#line 1 "c:\\Users\\bendr\\Documents\\GitHub\\SCVMQP\\SCVMQP.ino"
#include <Arduino.h>
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"

uint8_t chA = 2;
uint8_t chB = 3;
const unsigned int EN = 4;
const unsigned int IN1 = 5;
const unsigned int IN2 = 6;

DriveModule *mod;

void setup()
{
  Serial.begin(9600);
  mod = new DriveModule(EN, IN1, IN2, chA, chB);
  mod->init();
}

void loop()
{
  mod->driveDist(40*3.1415);
  Serial.println(mod->enc->getCounts());
}

