#ifndef RX_COMMUNICATION_H
#define RX_COMMUNICATION_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class RxCommunication {
    public:
        RxCommunication(uint8_t rxPin, uint8_t txPin, long speed);
        void listen();
        void stopListening();
        int available();
        int read();
        void write(uint8_t byte);
        void write(const uint8_t *buffer, size_t size);

    private:
        uint8_t rxPin;
        uint8_t txPin;
        SoftwareSerial serial;
};

#endif
