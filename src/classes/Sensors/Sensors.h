#ifndef SENSORS_H_
#define SENSORS_H_
#include <Adafruit_VL6180X.h> //Time of Flight sensor
#include <L3G4200D.h>
#include <Wire.h>
#include "../../../config.h"

class Sensors{
    public:
        Sensors();
        void init();
        void printRange();
        uint16_t getRange();
        void startRanging();
        void stopRanging();

        const uint16_t  ALL_SENSORS_PENDING = ((1 << TOF_SENSOR_COUNT) - 1);
        uint16_t sensors_pending = ALL_SENSORS_PENDING;
        uint32_t sensor_last_cycle_time;
        uint8_t sensor_ranges[TOF_SENSOR_COUNT];
        uint8_t sensor_status[TOF_SENSOR_COUNT];

    private:
        Adafruit_VL6180X sensA;
        Adafruit_VL6180X sensB;
        Adafruit_VL6180X* sens[];
};
#endif