#include <Arduino.h>
#include <L298N.h>
#include <stdint.h>
#include "Encoder.h"

uint8_t chA = 2;
uint8_t chB = 3;
const unsigned int EN = 4;
const unsigned int IN1 = 5;
const unsigned int IN2 = 6;

L298N motor(EN, IN1, IN2);
Encoder enc(chA, chB);

void setup()
{
  Serial.begin(9600);
  enc.init();
  motor.setSpeed(65);
  motor.forward();

  pinMode(chA, INPUT);
  pinMode(chB, INPUT);
  
}

void loop()
{
  enc.updateCounts();
  Serial.println(enc.getCounts());
  while(enc.calcDist(0, enc.getCounts()) > (40*3.14)){
    motor.stop();
    enc.updateCounts();
    Serial.println("ONE WHEEL ROTATION COMPLETE");
    Serial.println(enc.getCounts());
  }

  
}
