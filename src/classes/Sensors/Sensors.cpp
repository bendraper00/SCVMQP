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

    //Boot MPU
    while(!this->mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G)){
        Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
        delay(500);
    }
    //this->mpu.calibrateGyro();
    this->mpu.setThreshold(3);

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

void Sensors::getRangeData(int& mag, int& diff){
    static uint8_t range;
    static int16_t _magnitudeState = 0; //0 - waiting on sensor 1 data, 1 = waiting on sensor 2 data then update magnitude
    static int16_t magnitude = 0;
    static int16_t difference = 0;
    static int16_t tempMag = 0;
    static int16_t tempDif = 0;

    switch(_magnitudeState){
        case 0:
            if(this->sens[0]->readRange(range)){
                tempMag += range;
                tempDif = range;
                _magnitudeState = 1;
            }
            break;
        case 1:
            if(this->sens[1]->readRange(range)){
                tempMag += range;
                tempDif -= range;
                magnitude = tempMag/TOF_SENSOR_COUNT;
                difference = tempDif;
                tempDif = 0;
                tempMag = 0;
                _magnitudeState = 0;
            }
            break;
    }
    mag = magnitude;
    diff = difference;
}

float Sensors::getPitch(){
    static unsigned long prev = 0;

    Vector normG = mpu.readNormalizeGyro();
    unsigned long t = millis();
    double dt = (t - prev)/1000.0;
    prev = t;
    
    // Calculate Pitch and Roll
    this->pitchG = this->pitch + (normG.YAxis * dt);

    Vector normAccel = mpu.readNormalizeAccel();
    this->pitchA = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/PI;

    this->pitch = (GYRO_WEIGHT * this->pitchG) + (ACCEL_WEIGHT * this->pitchA);

    
    Serial.print("Angle: ");
    Serial.print(this->pitch);

    return this->pitch;
}

void Sensors::gyroCalibrate(){
    this->mpu.calibrateGyro();
}


