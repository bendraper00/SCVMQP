#ifndef ENCODER_H_
#define ENCODER_H_
class Encoder{
    public:
        void init();
        uint16_t getCounts();
        void updateCounts();
        float calcDist(uint16_t start, uint16_t end);
        Encoder(uint8_t pinA, uint8_t pinB);
        
    private:
        uint8_t pinA;
        uint8_t pinB;
        uint16_t counts;
};
#endif