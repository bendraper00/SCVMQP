#include <Arduino.h>
#include <stdint.h>
#include "src\classes\DriveModule\DriveModule.h"
#include <TimerInterrupt.h>

//PIN DEFINITIONS
uint8_t chA = 2;
uint8_t chB = 51;
const unsigned int EN = 10;
const unsigned int IN1 = 53;
const unsigned int IN2 = 52;
uint8_t servoPin = 9;

//Objects
DriveModule *mod;

//Temp Variables for testing
int state = 1;
volatile uint8_t readyToPID = 0;
volatile int32_t counts = 0;
volatile int32_t startCounts = 0;

void setup()
{
  Serial.begin(9600);
  
  noInterrupts();
  //USE TIMER 3 (16 bit) (Had to comment out block in Servo.h, this timer also controls PWM for pins 2,3, and 5)
  TCCR3A = 0;
  TCCR3B &= ~(1<<WGM33); //CTC Mode
  TCCR3B |= (1<<WGM32); //CTC Mode
  TCCR3B |= (1<<CS32); //Prescalar
  TCCR3B &= ~(1<<CS31); //Prescalar
  TCCR3B &= ~(1<<CS30); //Prescalar
  TCNT3 = 0;
  OCR3A = 6249;
  TIMSK3 = (1<<OCIE3A);
  interrupts();
  sei();

  mod = new DriveModule(EN, IN1, IN2, chA, chB, servoPin);
  mod->init();
  mod->driveSpeed(255);
}


void loop()
{
  CalcPID();
}

ISR(TIMER3_COMPA_vect)
{
  mod->enc->updateCounts();
  counts = mod->enc->getCounts();
  readyToPID = 1;
}

void CalcPID()
{

  if(readyToPID){
    readyToPID = 0;
    static int16_t prev = 0;
    static int16_t sum = 0;

    noInterrupts();
    int16_t speed = counts - prev;
    prev = counts;
    interrupts();

   Serial.println(speed);
  }

}
