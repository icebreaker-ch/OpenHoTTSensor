#include "RxCommunication.h"

RxCommunication::RxCommunication(uint8_t pin, long speed) :
    fullDuplex(false),
    rxPin(pin),
    txPin(pin),
    serial(pin, pin),
    delayBetweenChars(0) {
        pinMode(pin, INPUT);
        serial.begin(speed);
        serial.listen(); // Start listening
}

RxCommunication::RxCommunication(uint8_t rxPin, uint8_t txPin, long speed) :
    fullDuplex(true),
    rxPin(rxPin),
    txPin(txPin),
    serial(rxPin, txPin),
    delayBetweenChars(0) {
    serial.begin(speed);
}

void RxCommunication::setDelayBetweenChars(unsigned long delayBetweenChars) {
    this->delayBetweenChars = delayBetweenChars;
}

void RxCommunication::listen() {
    if (! fullDuplex) {
        pinMode(rxPin, INPUT);
        serial.listen();
    }
}

void RxCommunication::stopListening() {
    if (! fullDuplex) {
        pinMode(txPin, OUTPUT);
        serial.stopListening();
    }
}

int RxCommunication::available() {
    return serial.available();
}

int RxCommunication::read() {
    return serial.read();
}

void RxCommunication::write(uint8_t byte) {
    serial.write(byte);
    if (delayBetweenChars > 0)
        delay(delayBetweenChars);
}

void RxCommunication::write(const uint8_t *buffer, size_t size) {
    for (unsigned int pos = 0; pos < size; ++pos)
        write(buffer[pos]);
}
