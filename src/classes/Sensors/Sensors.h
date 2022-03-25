#ifndef SENSORS_H_
#define SENSORS_H_
#include <arduino.h>
#include <Adafruit_VL6180X.h> //Time of Flight sensor
#include <L3G.h>
#include <Wire.h>
#include "../../../config.h"

class Sensors{
    public:
        Sensors();
        void init();

        uint8_t sensor_ranges[TOF_SENSOR_COUNT];
        uint8_t sensor_status[TOF_SENSOR_COUNT];
        uint16_t getRangeMagnitude();
        int16_t getDifference();
        int16_t getPitch(float& observedAngle);
        uint8_t ranges[TOF_SENSOR_COUNT];

    private:
        Adafruit_VL6180X sensA;
        Adafruit_VL6180X sensB;
        Adafruit_VL6180X* sens[TOF_SENSOR_COUNT];
        L3G gyro;
};

#endif