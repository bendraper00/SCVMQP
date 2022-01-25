#include <Arduino.h>
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"
#include <TimerInterrupt.h>


uint8_t chA = 2;
uint8_t chB = 3;
const unsigned int EN = 4;
const unsigned int IN1 = 5;
const unsigned int IN2 = 6;
uint8_t servoPin = 9;

int state = 1;
volatile uint8_t readyToPID = 0;
volatile int32_t counts = 0;
volatile int32_t startCounts = 0;

DriveModule *mod;


void setup()
{
  Serial.begin(9600);
  
  noInterrupts();
  //USE TIMER 4 (16 bit)
  //TCCR4B = 0x54
  TCCR4A = 0x00;
  TCCR4B = 0x0C; //sets the prescaler (256) and CTC mode
  TCCR4A = 0x00;
  OCR4A = 12499;
  TIMSK4 = 0x01; //enable overflow interrupt
  interrupts();

  mod = new DriveModule(EN, IN1, IN2, chA, chB, servoPin);
  mod->init();
}

void loop()
{
  // switch(state){
  //   Serial.println(state);
  //   case 1:
  //     mod->driveDist(40*3.1415);
  //     if(mod->moving == false){
  //       state = 2;
  //     }
  //     break;
  //   case 2:
  //     mod->setWheelAngle(90);
  //     state = 3;
  //     break;
  //   case 3:
  //     mod->driveDist(40*3.1415);
  //     if(mod->moving == false){
  //       state = 4;
  //     }
  //     break;
  //   case 4:
  //     mod->setWheelAngle(0);
  //     state = 1;
  //     break;
    
  // }
  mod->driveSpeed(255);
  CalcPID();
}

ISR(TIMER4_OVF_vect){
  mod->enc->updateCounts();
  counts = mod->enc->getCounts();
  readyToPID = 1;
}

void CalcPID(){
  if(readyToPID){
    readyToPID = 0;
    static int16_t prev = 0;
    static int16_t sum = 0;

    noInterrupts();
    int16_t speed = counts - prev;
    prev = counts;
    interrupts();

   Serial.println(micros()); 
  }
}
