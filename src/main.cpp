#include <Arduino.h>
#include <SoftwareSerial.h>

#define HOTT_BAUDRATE 19200
#define BINARY_SIZE 45
#define BINARY_DATA_START 0x7C
#define BINARY_DATA_STOP 0x7D
#define HOTT_GENERAL_AIR_MODULE_ID 0x8D
#define HOTT_GENERAL_AIR_SENSOR_ID 0xD0
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

    uint16_t voltage1 = 123; // 12.3 V

    telemetry_data[0] = BINARY_DATA_START;
    telemetry_data[1] = HOTT_GENERAL_AIR_MODULE_ID;
    telemetry_data[2] = 0x00; // Warnings
    telemetry_data[3] = HOTT_GENERAL_AIR_SENSOR_ID;

    telemetry_data[6] = 1800 & 0xFF; // Cell Voltage 1
    telemetry_data[13] = (1800 >> 8) & 0xFF;

    telemetry_data[20] = 50 & 0xFF; // Voltage 1
    telemetry_data[21] = (50 >> 8)  & 0xFF;

    telemetry_data[22] = 50 & 0xFF; // Voltage 1
    telemetry_data[23] = (50 >> 8)  & 0xFF;

    telemetry_data[24] = 32; // Temperature 1
    telemetry_data[25] = 35; // Temperature 2

    int altitude = 500 + 222; // Altitude
    telemetry_data[26] = altitude & 0xFF; // Altitude
    telemetry_data[27] = (altitude >> 8) & 0xFF; // Altitude

    telemetry_data[28] = 234 & 0xFF; // Current
    telemetry_data[29] = (234 >> 8) & 0xFF; // Current

    telemetry_data[30] = voltage1 & 0xFF;
    telemetry_data[31] = (voltage1 >> 8) & 0xFF;

    int climbRate = 30000 + 123; // 1.23 m/s
    telemetry_data[34] = climbRate & 0xFF; // climb rate
    telemetry_data[35] = (climbRate >> 8) & 0xFF;

    int rpm = 3300 / 10;
    telemetry_data[37] = rpm & 0xFF; // RPM
    telemetry_data[38] = (rpm >> 8) & 0xFF;

    telemetry_data[41] = 85; // Speed km/h

    telemetry_data[43] = BINARY_DATA_STOP;

    telemetry_data[BINARY_SIZE - 1] = getCheckSum(telemetry_data, BINARY_SIZE - 1);

    for (int i = 0; i < BINARY_SIZE; ++i) {
        serial.write(telemetry_data[i]);
        delay(2);
    }

}
