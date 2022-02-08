#include "Sensors.h"
#include <Arduino.h>

Sensors::Sensors(){
}

void Sensors::init(){
    this->sensA = Adafruit_VL6180X();
    this->sensB = Adafruit_VL6180X();

    digitalWrite(LOX1_SHT, LOW);
    digitalWrite(LOX2_SHT, LOW);
    delay(10);
    digitalWrite(LOX1_SHT, HIGH);
    digitalWrite(LOX2_SHT, HIGH);
    delay(10);
    digitalWrite(LOX1_SHT, HIGH);
    digitalWrite(LOX2_SHT, LOW);
    if (!sensA.begin()) {
        Serial.println(F("Failed to boot first VL6180X"));
    }
    lox1.setAddress(LOX1_ADDRESS);
    delay(10);
    digitalWrite(LOX2_SHT, HIGH);
    delay(10);
    if (!sensB.begin()) {
        Serial.println(F("Failed to boot second VL6180X"));
    }
    sensB.setAddress(LOX2_ADDRESS);
}

void Sensors::printRange(){

}

uint16_t Sensors::getRange(){
    return 
}