#ifndef SENSORS_H_
#define SENSORS_H_
#include <VL6180X.h> //Time of Flight sensor
#include <L3G4200D.h>
#include <button.h>
#include <Wire.h>
class Sensors{
    public:
        Sensors();
        void init();
        void readRange();
    private:
    VL6180X sensorR;
};
#endif