#ifndef SENSORS_H_
#define SENSORS_H_
#include <Adafruit_VL6180X.h> //Time of Flight sensor
#include <L3G4200D.h>
#include <button.h>
#include <Wire.h>
#include "../../../config.h"

class Sensors{
    public:
        Sensors();
        void init();
        void printRange();
        uint16_t getRange();

    private:
        Adafruit_VL6180X sensA;
        Adafruit_VL6180X sensB;
};
#endif