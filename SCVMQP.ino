#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"
#include <IRremote.h>

//Objects
Robot bot;
IRrecv ir(IR_REMOTE_SIGNAL);

//Temp Variables for testing
uint32_t prevt = 0;

enum command{RAISEFRONT, LOWERFRONT, FORWARD, BACKWARD, LEFTSKID, RIGHTSKID, LATERAL, LEFT, RIGHT, STOP};
command state = STOP;
command prevState = STOP;
bool lateral = false;

void setup()
{
  Serial.begin(9600);
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
}

void loop()
{
  if(millis()-prevt > 115){
    state = STOP;
  }
  if(ir.decode()){
    prevt = millis();
    int comm = ir.decodedIRData.command;
    if(comm == 22){state = RAISEFRONT;}
    else if(comm == 25){state = LOWERFRONT;}
    else if(comm == 70 && lateral == false){state = FORWARD;}
    else if(comm == 68 && lateral == false){state = LEFTSKID;}
    else if(comm == 21 && lateral == false){state = BACKWARD;}
    else if(comm == 67 && lateral == false){state = RIGHTSKID;}
    else if(comm == 64 && prevState != LATERAL){state = LATERAL;}
    else if(comm == 68 && lateral == true){state = LEFT;}
    else if(comm == 21 && lateral == true){state = STOP;}
    else if(comm == 70 && lateral == true){state = STOP;}
    else if(comm == 67 && lateral == true){state = RIGHT;}
    else{state = STOP;}
    ir.resume();
  }

  Serial.println(state);
  switch(state){
    case RAISEFRONT:
      bot.scissors->raiseFront(230);
      bot.frontDrive->driveSpeed(0);
      bot.rearDrive->driveSpeed(0);
      prevState = RAISEFRONT;
      break;
    
    case LOWERFRONT:
      bot.scissors->lowerFront(230);
      bot.frontDrive->driveSpeed(0);
      bot.rearDrive->driveSpeed(0);
      prevState = LOWERFRONT;
      break;

    case FORWARD:
      bot.frontDrive->driveSpeed(120);
      bot.rearDrive->driveSpeed(120);
      bot.scissors->frontSpeed(0);
      prevState = FORWARD;
      break;

    case BACKWARD:
      bot.frontDrive->driveSpeed(-120);
      bot.rearDrive->driveSpeed(-120);
      bot.scissors->frontSpeed(0);
      prevState = BACKWARD;
      break;

    case LEFTSKID:
      bot.frontDrive->driveSpeed(120);
      bot.rearDrive->driveSpeed(-120);
      bot.scissors->frontSpeed(0);
      prevState = LEFTSKID;
      break;

    case RIGHTSKID:
      bot.frontDrive->driveSpeed(-120);
      bot.rearDrive->driveSpeed(120);
      bot.scissors->frontSpeed(0);
      prevState = RIGHTSKID;
      break;

    case LATERAL:
      if(lateral == true){
        bot.frontDrive->setWheelAngle(90);
        bot.rearDrive->setWheelAngle(90);
        bot.scissors->frontSpeed(0);
        lateral = false;
      }
      else{
        bot.frontDrive->setWheelAngle(0);
        bot.rearDrive->setWheelAngle(0);
        bot.scissors->frontSpeed(0);
        lateral = true;
      }
      prevState = LATERAL;
      break;

    case LEFT:
      bot.frontDrive->driveSpeed(-120);
      bot.rearDrive->driveSpeed(-120);
      bot.scissors->frontSpeed(0);
      prevState = LEFT;
      break;

    case RIGHT:
      bot.frontDrive->driveSpeed(120);
      bot.rearDrive->driveSpeed(120);
      bot.scissors->frontSpeed(0);
      prevState = RIGHT;
      break;

    case STOP:
      bot.frontDrive->driveSpeed(0);
      bot.rearDrive->driveSpeed(0);
      bot.scissors->frontSpeed(0);
      prevState = STOP;
      break;

    default:
      bot.frontDrive->driveSpeed(0);
      bot.rearDrive->driveSpeed(0);
      bot.scissors->frontSpeed(0);
      prevState = STOP;
      break;
  }

}
