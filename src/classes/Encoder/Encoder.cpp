#include "Encoder.h"
uint8_t statPinFA;
uint8_t statPinFB;
uint8_t statPinRA;
uint8_t statPinRB;
volatile uint32_t statFrontCounts = 0;
volatile uint32_t statRearCounts = 0;

Encoder::Encoder(uint8_t pinFA, uint8_t pinFB, uint8_t pinRA, uint8_t pinRB){
    this->pinFA = pinFA;
    statPinFA = pinFA;
    this->pinFB = pinFB;
    statPinFB = pinFB;

    this->pinRA = pinRA;
    statPinRA = pinRA;
    this->pinRB = pinRB;
    statPinRB = pinRB;
}
void Encoder:: init(){
    pinMode(this->pinFA, INPUT);
    pinMode(this->pinFB, INPUT);
    pinMode(this->pinRA, INPUT);
    pinMode(this->pinRB, INPUT);
    attachInterrupt(digitalPinToInterrupt(this->pinFA), Encoder::encoderFISR, RISING);
    attachInterrupt(digitalPinToInterrupt(this->pinRA), Encoder::encoderRISR, RISING);
}
int16_t Encoder::getFrontCounts(){
    return this->frontCounts;
}
int16_t Encoder::getRearCounts(){
    return this->rearCounts;
}
void Encoder::updateCounts(){
    this->frontCounts = statFrontCounts;
    this->rearCounts = statRearCounts;
}
float Encoder::calcDist(int16_t start, int16_t end){
    //800 Counts per revolution
    //Wheel OD is 40mm
    // 1 count = 0.15708mm
    return (end-start)*ENCODER_COUNTS_TO_MM;
}
int16_t Encoder::distToCounts(float dist){
    uint16_t counts = dist/ENCODER_COUNTS_TO_MM; //possibly problematic idk if this will round to nearest count
    return counts;
}

void Encoder::encoderFISR(){
    //turn off interrupts... I think
    if(!digitalRead(statPinFB)){
        statFrontCounts++;
    }
    else{
        statFrontCounts--;
    } 
}
void Encoder::encoderRISR(){
    //turn off interrupts... I think
    if(!digitalRead(statPinRB)){
        statRearCounts++;
    }
    else{
        statRearCounts--;
    } 
}
