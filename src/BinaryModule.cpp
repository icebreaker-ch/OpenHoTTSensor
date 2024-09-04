#include "BinaryModule.h"

uint8_t *BinaryModule::getFrame() {
    return frame;
}

uint8_t BinaryModule::calcCheckSum(uint8_t buffer[], unsigned int fromPos, unsigned int toPos) {
    uint8_t checkSum = 0;
    for (unsigned int pos = fromPos; pos <= toPos; ++pos) {
        checkSum += frame[pos];
    }
    return checkSum;
}