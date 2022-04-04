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
int nextState;
unsigned long t = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("---Initializing Systems---");
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(0);
  bot.rearDrive->setWheelAngle(0);
}

void loop()
{
  //-------------STAIR SWEEPING----------------
  // switch(tempState){
  //   case 0:
  //     if(!digitalRead(BUMP_RIGHT)){tempState = 1;}
  //     bot.stairFollow(100, 80);
  //     break;
    
  //   case 1:
  //   if(!digitalRead(BUMP_LEFT)){tempState = 0;}
  //     bot.stairFollow(-100, 80);
  //     break;
  // }

  
  //------------RAISE MID TEST----------------
  switch(tempState){
    case 0:
      if(bot.home()){
        tempState = 1;
        nextState = 2;
        t = millis();
        Serial.println("CALIBRATING");
        bot.sensors->gyroCalibrate();
      }
      break;
    
    case 1:
      if(millis() - t >= 3000){
        tempState = nextState;
      }
      break;
    
    case 2:
      if(bot.raiseFront(MAX_DRIVE_SPEED)){
        tempState = 1;
        nextState = 3;
        t = millis();

      }
      break;
    case 3:
      if(bot.raiseMid(MAX_DRIVE_SPEED)){
        tempState = 4;
      }
      break;
    case 4:
      break;
  }

  //---------------IMU TEST----------------------
  // float angle;
  // angle = bot.sensors->getPitch();
  // Serial.println(angle);
}

