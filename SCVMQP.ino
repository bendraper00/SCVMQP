#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"
#include <IRremote.h>
#include <L298N.h>

//Objects
Robot bot;
IRrecv ir(IR_REMOTE_SIGNAL);
int tempState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("---Initializing Systems---");
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
  delay(2000);
  bot.sensors->gyroCalibrate();
}

void loop()
{
  switch(tempState){
    case 0:
      if(!digitalRead(BUMP_RIGHT)){tempState = 1;}
      bot.stairFollow(100, 80);
      break;
    
    case 1:
    if(!digitalRead(BUMP_LEFT)){tempState = 0;}
      bot.stairFollow(-100, 80);
      break;
  }
  

  /*
  //DISCUSS AT MEETING
  float angle;
  bot.sensors->getPitch(angle);
  Serial.println(angle);
  */

}

