#include <Arduino.h>
#include "config.h"
#include <stdint.h>
#include "src\classes\Robot\Robot.h"
#include <IRremote.h>

//Objects
Robot bot;
IRrecv ir(IR_REMOTE_SIGNAL);

//Temp Variables for testing
uint32_t t = 0;
uint32_t pt = 0;
uint32_t delta = 0;

void setup()
{
  Serial.begin(9600);
  bot.init();
  delay(300);
  ir.enableIRIn();
}


void loop()
{
  // delta = millis() - pt;
  // if (ir.decode()){
  //   pt = millis();
  //   ir.resume();
  // }
  // Serial.println(delta);

  bot.frontDrive->driveSpeed(-100);
}
