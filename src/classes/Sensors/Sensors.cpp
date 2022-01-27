#include "Sensors.h"
#include <Arduino.h>

Sensors::Sensors(){
}

void Sensors::init(){
    Wire.begin();
    sensorR.init();
    sensorR.configureDefault();
    sensorR.writeReg(VL6180X::SYSRANGE__MAX_CONVERGENCE_TIME, 30);
    sensorR.writeReg16Bit(VL6180X::SYSALS__INTEGRATION_PERIOD, 50);
    sensorR.setTimeout(0);
    sensorR.startRangeContinuous(15);
}

void Sensors::readRange(){
    Serial.print("Range: ");
    Serial.print(sensorR.readRangeContinuousMillimeters());
    if(sensorR.timeoutOccurred()) { Serial.print(" TIMEOUT"); }
    Serial.println();
}

uint16_t Sensors::getRange(){
    return sensorR.readRangeContinuousMillimeters();
}