#include "Encoder.h"
#include <Arduino.h>

byte statPinA;
byte statPinB;
uint16_t statCounts;

Encoder::Encoder(uint8_t pinA,uint8_t pinB){
    this->pinA = pinA;
    statPinA = pinA;
    this->pinB = pinB;
    statPinB = pinB;
}
void Encoder:: init(){
    pinMode(this->pinA, INPUT);
    pinMode(this->pinB, INPUT);
    attachInterrupt(this->pinA, encoderISR, RISING);
}
uint16_t Encoder::getCounts(){
    return counts;
}
void Encoder::updateCounts(){
    this->counts = statCounts;
}
float Encoder::calcDist(uint16_t start, uint16_t end){
    //64 Counts per revolution
    //Wheel OD is 40mm
    // 1 count = 1.9635mm
    return (end-start)*1.9635;
}
void encoderISR(){
    //turn off interrupts... I think
    if(digitalRead(statPinA) && !digitalRead(statPinB)){
        statCounts++;
    }
    if(digitalRead(statPinA) && digitalRead(statPinB)){
        statCounts--;
    } 
}
