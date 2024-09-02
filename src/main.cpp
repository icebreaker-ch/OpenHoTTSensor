#include <Arduino.h>
#include "BinaryModule.h"
#include <SoftwareSerial.h>
#include "ElectricAirModule.h"

#define HOTT_BAUDRATE 19200

//#define HOTT_GENERAL_AIR_MODULE_ID 0x8D
//#define HOTT_GENERAL_AIR_SENSOR_ID 0xD0

//#define HOTT_VARIO_MODULE_ID 0x89
//#define HOTT_VARIO_SENSOR_ID 0x90

SoftwareSerial serial(8, 9);

typedef enum {
    WAIT_START,
    WAIT_ID,
    SEND_DATA
} State;


uint8_t getCheckSum(uint8_t *buffer, int size) {
    uint8_t checkSum = 0;
    for (int pos = 0;pos < size; ++pos) {
        checkSum += buffer[pos];
    }
    return checkSum;
}

void sendByte(uint8_t byte) {
    serial.write(byte);
    Serial.print(byte, 16);
    Serial.print(" ");
}

void sendData(uint8_t *data, int size) {
    for (int i = 0; i < size; ++i) {
        sendByte(data[i]);
    }
    Serial.println();
}

#if 0
void sendVario() {
    static uint16_t alt = 0;

    uint8_t telemetry_data[BINARY_SIZE];
    memset(telemetry_data, 0x00, BINARY_SIZE);

    uint16_t altitude = OFFSET_ALTITUDE + alt++; // 123 m
    uint16_t vspeed = OFFSET_VSPEED + 321; // 3.21 m/s

    telemetry_data[0] = BINARY_DATA_START;
    telemetry_data[1] = HOTT_VARIO_MODULE_ID;
    telemetry_data[3] = HOTT_VARIO_SENSOR_ID;
    telemetry_data[5] = altitude & 0xFF;
    telemetry_data[6] = (altitude >> 8) & 0xFF;
    telemetry_data[11] = vspeed & 0xFF;
    telemetry_data[12] = (vspeed >> 8) & 0xFF;
    telemetry_data[43] = BINARY_DATA_STOP;
    telemetry_data[44] = getCheckSum(telemetry_data, BINARY_SIZE - 1);

    sendData(telemetry_data, BINARY_SIZE);
}

#endif

static ElectricAirModule eam;

void setup() {
    Serial.begin(9600);
    serial.begin(HOTT_BAUDRATE);
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
            serial.listen();
            if (serial.available())
            {
                uint8_t requestId = serial.read();
                Serial.println(requestId, 16);
                if (requestId == BinaryModule::HOTT_BINARY_MODE_REQUEST_ID) {
                    state = WAIT_ID;
                }
            }
            break;

        case WAIT_ID:
            if (serial.available()) {
                uint8_t moduleId = serial.read();
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
