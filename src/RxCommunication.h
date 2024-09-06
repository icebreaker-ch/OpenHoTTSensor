#ifndef RX_COMMUNICATION_H
#define RX_COMMUNICATION_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class RxCommunication {
    public:
        RxCommunication(uint8_t pin, long speed);
        RxCommunication(uint8_t rxPin, uint8_t txPin, long speed);
        void setDelayBetweenChars(unsigned long delayBetweenChars);
        void listen();
        void stopListening();
        int available();
        int read();
        void write(uint8_t byte);
        void write(const uint8_t *buffer, size_t size);

    private:
        boolean fullDuplex;
        uint8_t rxPin;
        uint8_t txPin;
        SoftwareSerial serial;
        unsigned long delayBetweenChars; // ms
};

#endif
