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

//FORWARD MOVES BOTH STAGES DOWN

enum command{MOVING, STOP};
command state = STOP;
command prevState = STOP;
bool lateral = false;
L298N worm(FRONT_SCISSOR_EN, FRONT_SCISSOR_IN1, FRONT_SCISSOR_IN2);

void setup()
{
  Serial.begin(115200);
  bot.init();
  //ir.enableIRIn();
  bot.frontDrive->setWheelAngle(90);
  bot.rearDrive->setWheelAngle(90);
}

void loop()
{
  bot.frontDrive->pidFSpeed(10);
  bot.rearDrive->pidRSpeed(10);
}
