#include "Sensors.h"
#include <Arduino.h>

Sensors::Sensors(){
}

void Sensors::init(){
    //Initialize sensors
    this->sensA = Adafruit_VL6180X();
    this->sensB = Adafruit_VL6180X();
    this->sens[0] = &this->sensA;
    this->sens[1] = &this->sensB;

    pinMode(LOX1_SHT, OUTPUT);
    pinMode(LOX2_SHT, OUTPUT);
    
    //Reset Sensors
    digitalWrite(LOX1_SHT, LOW);
    digitalWrite(LOX2_SHT, LOW);
    delay(10); //Don't know if there is any way to avoid this
    digitalWrite(LOX1_SHT, HIGH);
    digitalWrite(LOX2_SHT, HIGH);
    delay(10); 
    digitalWrite(LOX1_SHT, HIGH);
    digitalWrite(LOX2_SHT, LOW);

    //Boot Sensor A
    if (!this->sensA.begin()) {
        Serial.println(F("Failed to boot first VL6180X"));
    }
    this->sensA.setAddress(LOX1_ADDRESS);
    Serial.println("Left ToF sensor initialized");

    delay(10);
    digitalWrite(LOX2_SHT, HIGH);
    delay(10);

    //Boot Sensor B
    if (!this->sensB.begin()) {
        Serial.println(F("Failed to boot second VL6180X"));
    }
    this->sensB.setAddress(LOX2_ADDRESS);
    Serial.println("Right ToF sensor initialized");

    //Boot gyro
    if (!this->gyro.init()){
        Serial.println("Failed to autodetect gyro type!");
    }
    Serial.println("Gyro initialized");
    this->gyro.enableDefault();

}

uint16_t Sensors::getRangeMagnitude(){
    static uint8_t range;
    static uint16_t magnitude = 0;
    static uint16_t tempMag = 0;
    static uint8_t _magnitudeState = 0; //0 - waiting on sensor 1 data, 1 = waiting on sensor 2 data then update magnitude

    switch(_magnitudeState){
        case 0:
            if(this->sens[0]->readRange(range)){
                tempMag += range;
                _magnitudeState = 1;
            }
            break;
        case 1:
            if(this->sens[1]->readRange(range)){
                tempMag += range;
                magnitude = tempMag/TOF_SENSOR_COUNT;
                tempMag = 0;
                _magnitudeState = 0;
            }
            break;
    }
    return magnitude;
}

int16_t Sensors::getDifference(){
    static uint8_t range;
    static uint16_t diff = 0;
    static uint16_t tempDif = 0;
    static uint8_t _difState = 0; //0 - waiting on sensor 1 data, 1 = waiting on sensor 2 data then update magnitude

    switch(_difState){
        case 0:
            if(this->sens[0]->readRange(range)){
                tempDif = range;
                _difState = 1;
            }
            break;
        case 1:
            if(this->sens[1]->readRange(range)){
                tempDif -= range;
                diff = tempDif;
                tempDif = 0;
                _difState = 0;
            }
            break;
    }
    return diff;
}

int16_t Sensors::getPitch(float& observedAngle){
    this->gyro.read();
    int16_t vel = (int)this->gyro.g.y;
    uint32_t t = millis();
    static uint32_t prevTime = t;
    static int16_t prevVel = vel;
    static int16_t angle = 0;
    angle = angle + (vel - prevVel)/(millis()-prevTime);
    prevTime = t;
    prevVel = vel;
    return angle;
}



