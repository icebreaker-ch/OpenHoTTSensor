#ifndef RX_COMMUNICATION_H
#define RX_COMMUNICATION_H

#include <Arduino.h>
#include <SoftwareSerial.h>

class RxCommunication {
    public:
        RxCommunication(uint8_t pin, long speed);
        void listen();
        void stopListening();
        int available();
        int read();
        void write(uint8_t byte);

    private:
        uint8_t pin;
        SoftwareSerial serial;
};

#endif
