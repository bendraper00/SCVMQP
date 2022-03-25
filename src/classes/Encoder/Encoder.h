#ifndef ENCODER_H_
#define ENCODER_H_
#include <stdint.h>
#include <Arduino.h>
#include "../../../config.h"
class Encoder{
    public:
        Encoder(uint8_t pinFA, uint8_t pinFB, uint8_t pinRA, uint8_t pinRB);
        void init();
        int32_t getFrontCounts();
        int32_t getRearCounts();
        void updateCounts();
        float calcDist(int16_t start, int16_t end);
        static void encoderFISR();
        static void encoderRISR();
        int16_t distToCounts(float);
    private:
        uint8_t pinFA;
        uint8_t pinFB;
        uint8_t pinRA;
        uint8_t pinRB;
        int32_t frontCounts;
        int32_t rearCounts;
};
#endif
