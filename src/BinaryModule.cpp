#include "BinaryModule.h"

uint8_t *BinaryModule::getFrame() {
    return frame;
}

uint8_t BinaryModule::calcCheckSum(uint8_t buffer[], int fromPos, int toPos) {
    uint8_t checkSum = 0;
    for (unsigned pos = 0; pos < FRAME_SIZE - 1; ++pos) {
        checkSum += frame[pos];
    }
    return checkSum;
}