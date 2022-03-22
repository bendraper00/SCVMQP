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

uint8_t Sensors::getRangeMagnitude(){
    uint16_t magnitude = 0;
    for (uint8_t i = 0; i < TOF_SENSOR_COUNT; i++) {
        magnitude += this->sens[i]->readRange();
    }
    return magnitude/TOF_SENSOR_COUNT;
}

int16_t Sensors::getDifference(){
    for (uint8_t i = 0; i < TOF_SENSOR_COUNT; i++) {
        ranges[i] = sens[i]->readRange();
    }
    return ranges[0]-ranges[1]; //Application specific function
}

int16_t Sensors::getPitch(float& observedAngle){
    int16_t vel = (int)this->gyro.g.y;
    uint32_t t = millis();
    static uint32_t prevTime = t;
    static int16_t prevVel = vel;
    static int16_t angle = 0;
    angle = angle + ((int)this->gyro.g.y - prevVel)/(millis()-prevTime);
    prevTime = t;
    prevVel = vel;
    return angle;
}



