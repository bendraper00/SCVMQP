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

enum command{MOVING, STOP};
command state = STOP;
command prevState = STOP;
bool lateral = false;
L298N worm(12, 38, 39);

void setup()
{
  Serial.begin(115200);
  bot.init();
  ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
  
  void stopMotor();
  state = MOVING;
  pinMode(19, INPUT);
  attachInterrupt(digitalPinToInterrupt(19), stopMotor, RISING);
}


void loop()
{
  switch(state){
    case MOVING:
      worm.setSpeed(150);
      worm.forward();
    break;

    case STOP:
      worm.stop();
    break;
  }
}

void stopMotor(){
  state = STOP;
}
  