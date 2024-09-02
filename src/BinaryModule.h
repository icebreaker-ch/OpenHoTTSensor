#ifndef BINARY_MODULE_H
#define BINARY_MODULE_H

#include <Arduino.h>

class BinaryModule {
    public:
        static const unsigned int FRAME_SIZE = 45;
        static const unsigned int HOTT_BINARY_DATA_START = 0x7C;
        static const unsigned int HOTT_BINARY_DATA_END  = 0x7D;
        static const unsigned int HOTT_BINARY_MODE_REQUEST_ID = 0x80;


        virtual uint8_t *getFrame() = 0;

    protected:
        uint8_t frame[FRAME_SIZE];
        uint8_t calcCheckSum(uint8_t buffer[], int fromPos, int toPos);
        uint8_t lsb(uint16_t val) { return val & 0xFF; }
        uint8_t msb(uint16_t val) { return (val >> 8 ) & 0xFF; }
};

#endif