#ifndef ENCODER_H_
#define ENCODER_H_
#include <stdint.h>
#include <Arduino.h>
class Encoder{
    public:
        Encoder(uint8_t pinA, uint8_t pinB);
        void init();
        int16_t getCounts();
        void updateCounts();
        void updatePrevCounts();
        float calcDist(int16_t start, int16_t end);
        static void encoderISR();
        int16_t distToCounts(float);
    private:
        uint8_t pinA;
        uint8_t pinB;
        int16_t counts;
        int16_t prevCounts;
};
#endif
