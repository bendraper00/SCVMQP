#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"

//Objects
Robot bot;

//Temp Variables for testing

void setup()
{
  Serial.begin(9600);
  bot.init();
  delay(300);
}


void loop()
{
bot.frontDrive->setWheelAngle(0);
}
