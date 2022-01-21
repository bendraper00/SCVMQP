#ifndef ENCODER_H_
#define ENCODER_H_
#include <stdint.h>
#include <Arduino.h>
class Encoder{
    public:
        Encoder(uint8_t pinA, uint8_t pinB);
        void init();
        uint16_t getCounts();
        void updateCounts();
        void updatePrevCounts();
        float calcDist(uint16_t start, uint16_t end);
        static void encoderISR();
        uint16_t distToCounts(float);
    private:
        uint8_t pinA;
        uint8_t pinB;
        uint16_t counts;
        uint16_t prevCounts;
};
#endif
