#include "RxCommunication.h"

RxCommunication::RxCommunication(uint8_t rxPin, uint8_t txPin, long speed) :
    rxPin(rxPin),
    txPin(txPin),
    serial(rxPin, txPin) {
        if (rxPin == txPin)
            pinMode(rxPin, INPUT);
        serial.begin(speed);
        serial.listen();
}

void RxCommunication::listen() {
    if (rxPin == txPin)
        pinMode(rxPin, INPUT);
    serial.listen();
}

void RxCommunication::stopListening() {
    if (rxPin == txPin)
        pinMode(txPin, OUTPUT);
    serial.stopListening();
}

int RxCommunication::available() {
    return serial.available();
}

int RxCommunication::read() {
    return serial.read();
}

void RxCommunication::write(uint8_t byte) {
    serial.write(byte);
    delay(2);
}

void RxCommunication::write(const uint8_t *buffer, size_t size) {
    for (unsigned int pos = 0; pos < size; ++pos)
        write(buffer[pos]);
}
