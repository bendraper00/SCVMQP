#include "Encoder.h"
byte statPinA;
byte statPinB;
uint16_t statCounts = 0;

Encoder::Encoder(uint8_t pinA,uint8_t pinB){
    this->pinA = pinA;
    statPinA = pinA;
    this->pinB = pinB;
    statPinB = pinB;
}
void Encoder:: init(){
    pinMode(this->pinA, INPUT);
    pinMode(this->pinB, INPUT);
    attachInterrupt(digitalPinToInterrupt(this->pinA), Encoder::encoderISR, RISING);
}
uint16_t Encoder::getCounts(){
    return this->counts;
}
void Encoder::updateCounts(){
    this->counts = statCounts;
}
float Encoder::calcDist(uint16_t start, uint16_t end){
    //800 Counts per revolution
    //Wheel OD is 40mm
    // 1 count = 0.15708mm
    return (end-start)*0.15708;
}
static void Encoder::encoderISR(){
    //turn off interrupts... I think
    if(digitalRead(statPinA) && !digitalRead(statPinB)){
        statCounts++;
    }
    if(digitalRead(statPinA) && digitalRead(statPinB)){
        statCounts--;
    } 
}
