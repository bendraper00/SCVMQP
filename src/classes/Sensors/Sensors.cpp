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

    delay(10);
    digitalWrite(LOX2_SHT, HIGH);
    delay(10);

    //Boot Sensor B
    if (!this->sensB.begin()) {
        Serial.println(F("Failed to boot second VL6180X"));
    }
    this->sensB.setAddress(LOX2_ADDRESS);
}

void Sensors::printRange(){
    uint16_t mask = 1;
    for(uint8_t i = 0; i < TOF_SENSOR_COUNT; i++) {
        bool range_complete = false;
        if (sensors_pending & mask) {
            range_complete = this->sens[i]->isRangeComplete();
            if(range_complete) {
                sensor_ranges[i] = this->sens[i]->readRangeResult();
                sensor_status[i] = this->sens[i]->readRangeStatus();
                sensors_pending ^= mask;
            }
        }

        mask <<= 1; // setup to test next one
    }
    // See if we have all of our sensors read OK
    uint32_t delta_time = millis() - sensor_last_cycle_time;
    if (!sensors_pending || (delta_time > 1000)) {
        Serial.print(delta_time, DEC);
        Serial.print("(");
        Serial.print(sensors_pending, HEX);
        Serial.print(")");
        mask = 1;
        for (uint8_t i = 0; i < TOF_SENSOR_COUNT; i++) {
            Serial.print(" : ");
            if (sensors_pending & mask) Serial.print("TTT");  // show timeout in this one
            else if (sensor_status[i] == VL6180X_ERROR_NONE) Serial.print(sensor_ranges[i], DEC);
            else {
                Serial.print("#");
                Serial.print(sensor_status[i], DEC);
            }
        }
    // setup for next pass
    Serial.println();
    sensor_last_cycle_time = millis();
    sensors_pending = ALL_SENSORS_PENDING;
  }
}

uint16_t Sensors::getRange(){
    return 1;
}

void Sensors::startRanging(){
    for (uint8_t i = 0; i < TOF_SENSOR_COUNT; i++){
        this->sens[i]->startRangeContinuous(TOF_PERIOD);
    }
    sensors_pending = ALL_SENSORS_PENDING;
    sensor_last_cycle_time = millis();
}

void Sensors::stopRanging() {
    for (uint8_t i = 0; i < TOF_SENSOR_COUNT; i++) {
        this->sens[i]->stopRangeContinuous();
    }
    delay(10); // give time for it to complete.
}