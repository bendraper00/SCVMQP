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
  Serial.begin(9600);
  Serial.println("---Initializing Systems---");
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
  delay(5000);
}

void loop()
{
  //bot.scissors->lowerFront(200);
  
  switch(tempState){
    case 0:
      if(bot.raiseFront(200)){
        tempState = 1;
      }
      break;
    case 1:
      break;
  }

}
