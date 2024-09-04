#include "RxCommunication.h"

RxCommunication::RxCommunication(uint8_t pin, long speed) :
    pin(pin),
    serial(pin, pin) {
        pinMode(pin, INPUT);
        serial.begin(speed);
        serial.listen();
}

void RxCommunication::listen() {
    pinMode(pin, INPUT);
    serial.listen();
}

void RxCommunication::stopListening() {
    pinMode(pin, OUTPUT);
    serial.stopListening();
}

int RxCommunication::available() {
    serial.available();
}

int RxCommunication::read() {
    return serial.read();
}

void RxCommunication::write(uint8_t byte) {
    serial.write(byte);
}
