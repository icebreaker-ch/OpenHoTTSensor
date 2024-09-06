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
    DELAY,
    SEND_DATA
} State;


static ElectricAirModule eam;

void setup() {
    Serial.begin(9600);
    pRxCommunication = new RxCommunication(8, 9, HOTT_BAUDRATE);
    pRxCommunication->listen();
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
    static uint8_t requestId;
    static uint8_t moduleId;

    switch(state) {
        case WAIT_START:
            if (pRxCommunication->available())
            {
                requestId = pRxCommunication->read();
                Serial.println(requestId, 16);
                if (requestId == BinaryModule::HOTT_BINARY_MODE_REQUEST_ID) {
                    state = WAIT_ID;
                }
            }
            break;

        case WAIT_ID:
            if (pRxCommunication->available()) {
                moduleId = pRxCommunication->read();
                Serial.println(moduleId, 16);
                if (moduleId == ElectricAirModule::MODULE_ID) {
                    state = DELAY;
                    state = SEND_DATA;
                }
                else {
                    state = WAIT_START;
                }
            }
            break;

        case DELAY:
            Serial.println("DELAY");
            delay(50); // Grace time
            if (pRxCommunication->available() == 0)
                state = SEND_DATA;
            else
                state = WAIT_START;

        case SEND_DATA:
            Serial.println("SEND");
            digitalWrite(LED_BUILTIN, HIGH);
            pRxCommunication->stopListening();

            pRxCommunication->write(BinaryModule::HOTT_BINARY_MODE_REQUEST_ID);
            pRxCommunication->write(moduleId);

            // Example values
            altitude = altitude + 0.1;
            eam.setAltitude(altitude);
            capacity = capacity + 0.1;
            eam.setCapacity(capacity);
            pRxCommunication->write(eam.getFrame(), BinaryModule::FRAME_SIZE);

            pRxCommunication->listen();
            state = WAIT_START;
            digitalWrite(LED_BUILTIN, LOW);
            break;
    }
}
