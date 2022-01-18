#ifndef ENCODER_H_
#define ENCODER_H_
class Encoder{
    public:
        void init();
        uint16_t getCounts();
        Encoder();
    private:
        uint16_t counts;
};
#endif