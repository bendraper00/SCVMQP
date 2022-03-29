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
        void getRangeData(int& mag, int& diff);
        void getPitch(float& observedAngle);
        uint8_t ranges[TOF_SENSOR_COUNT];
        void gyroCalibrate(); //Maybe add dynamic bias calibration later
        L3G gyro;

    private:
        Adafruit_VL6180X sensA;
        Adafruit_VL6180X sensB;
        Adafruit_VL6180X* sens[TOF_SENSOR_COUNT];
        double angle = 0.0;
        double bias = 0.0;
};

#endif