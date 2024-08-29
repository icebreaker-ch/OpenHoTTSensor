#include <Arduino.h>
#include <SoftwareSerial.h>

#define HOTT_BAUDRATE 19200
#define BINARY_SIZE 45
#define BINARY_DATA_START 0x7C
#define BINARY_DATA_STOP 0x7D
#define HOTT_ELECTRIC_AIR_MODULE_ID 0x8E
#define HOTT_ELECTRIC_AIR_SENSOR_ID 0xE0

SoftwareSerial serial(10, 11);

void setup() {
    serial.begin(HOTT_BAUDRATE);
}

uint8_t getCheckSum(uint8_t *buffer, int size) {
    uint8_t checkSum = 0;
    for (int pos = 0;pos < size; ++pos) {
        checkSum += buffer[pos];
    }
    return checkSum;
}

void loop() {
    uint8_t telemetry_data[BINARY_SIZE] = {
        0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00,
        0x00
    };

    uint16_t voltage1 = 1234; // 12.34 V

    telemetry_data[0] = BINARY_DATA_START;
    telemetry_data[1] = HOTT_ELECTRIC_AIR_MODULE_ID;
    telemetry_data[2] = 0x00; // Warnings
    telemetry_data[3] = HOTT_ELECTRIC_AIR_SENSOR_ID;

    telemetry_data[30] = voltage1 & 0xFF;
    telemetry_data[31] = (voltage1 >> 8) & 0xFF;
    telemetry_data[43] = BINARY_DATA_STOP;

    telemetry_data[BINARY_SIZE - 1] = getCheckSum(telemetry_data, BINARY_SIZE - 1);

    for (int i = 0; i < BINARY_SIZE; ++i) {
        serial.write(telemetry_data[i]);
        delay(2);
    }

}
