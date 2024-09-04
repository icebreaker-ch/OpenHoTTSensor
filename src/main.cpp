#include <Arduino.h>
#include "BinaryModule.h"
#include "ElectricAirModule.h"
#include "RxCommunication.h"

#define HOTT_BAUDRATE 19200

//#define HOTT_GENERAL_AIR_MODULE_ID 0x8D
//#define HOTT_GENERAL_AIR_SENSOR_ID 0xD0


RxCommunication *pRxCommunication;

typedef enum {
    WAIT_START,
    WAIT_ID,
    SEND_DATA
} State;


void sendByte(uint8_t byte) {
    pRxCommunication->write(byte);
    Serial.print(byte, 16);
    Serial.print(" ");
}

void sendData(uint8_t *data, int size) {
    for (int i = 0; i < size; ++i) {
        sendByte(data[i]);
    }
    Serial.println();
}

static ElectricAirModule eam;

void setup() {
    Serial.begin(9600);
    pRxCommunication = new RxCommunication(9, HOTT_BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);

    // Example values
    eam.setAltitude(0);
    eam.setCapacity(0);
    eam.setCurrent(66.6);
    eam.setMainVoltage(22.5);
    eam.setVSpeed(3.2);
}

void loop() {
    static double altitude = 0;
    static double capacity = 0;

    static State state = WAIT_START;

    switch(state) {
        case WAIT_START:
            pRxCommunication->listen();
            if (pRxCommunication->available())
            {
                uint8_t requestId = pRxCommunication->read();
                Serial.println(requestId, 16);
                if (requestId == BinaryModule::HOTT_BINARY_MODE_REQUEST_ID) {
                    state = WAIT_ID;
                }
            }
            break;

        case WAIT_ID:
            if (pRxCommunication->available()) {
                uint8_t moduleId = pRxCommunication->read();
                Serial.println(moduleId, 16);
                if (moduleId == ElectricAirModule::MODULE_ID) {
                    // Echo RID and MID since we do not use the special diode cable
                    sendByte(BinaryModule::HOTT_BINARY_MODE_REQUEST_ID);
                    sendByte(moduleId);
                    state = SEND_DATA;
                }
                else {
                    state = WAIT_START;
                }
            }
            break;

        case SEND_DATA:
            digitalWrite(LED_BUILTIN, HIGH);
            sendData(eam.getFrame(), BinaryModule::FRAME_SIZE);

            // Example values
            altitude = altitude + 0.1;
            eam.setAltitude(altitude);
            capacity = capacity + 0.1;
            eam.setCapacity(capacity);

            state = WAIT_START;
            digitalWrite(LED_BUILTIN, LOW);
            break;
    }
}
