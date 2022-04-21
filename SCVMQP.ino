#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"
#include <IRremote.h>
#include <L298N.h>

//Objects
Robot bot;
IRrecv ir(IR_REMOTE_SIGNAL);
int testState = 0;

void setup()
{
  Serial.begin(115200);
  Serial.println("---Initializing Systems---");
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(0);
  bot.rearDrive->setWheelAngle(0);

  Serial.println("HOMING");
  while(!bot.home());
  Serial.println("HOMED");
}

void loop(){
  static bool flag = false;
  switch(testState){
    case 0:
      if(bot.ascendStep()){testState = 1;}
      // if(bot.scissors->stepRearDown()){
      //     bot.scissors->rearSpeed(0);
      //     testState = 1;
      // }
      break;

    case 1:
      break;
  }
}


//ROBOT GETS STUCK HOMING AND CALIBRATING 