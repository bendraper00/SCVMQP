#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"
#include <IRremote.h>
#include <L298N.h>

//Objects
Robot bot;
IRrecv ir(IR_REMOTE_SIGNAL);

void setup()
{
  Serial.begin(9600);
  Serial.println("---Initializing Systems---");
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
}

void loop()
{
  bot.stairFollow(125, 80);
}
