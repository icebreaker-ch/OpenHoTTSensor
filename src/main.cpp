#include <Arduino.h>
#include <SoftwareSerial.h>

#define HOTT_BAUDRATE 19200
#define BINARY_SIZE 45
#define BINARY_DATA_START 0x7C
#define BINARY_DATA_STOP 0x7D
#define HOTT_BINARY_MODE_REQUEST_ID 0x80
#define HOTT_ELECTRIC_AIR_MODULE_ID 0x8E
#define HOTT_ELECTRIC_AIR_SENSOR_ID 0xE0

#define HOTT_GENERAL_AIR_MODULE_ID 0x8D
#define HOTT_GENERAL_AIR_SENSOR_ID 0xD0

#define OFFSET_ALTITUDE 500
#define OFFSET_VSPEED 30000

SoftwareSerial serial(4, 5);

uint8_t module = 0;

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

void sendData() {
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

    static uint16_t alt = 1;

    uint16_t voltage = 225; // 22.5 V
    uint16_t current = 666; // 66.6 A
    uint16_t altitude = OFFSET_ALTITUDE + alt++; // altitude m
    uint16_t vspeed = OFFSET_VSPEED + 235; // 2.35 m/s
    uint16_t capacity = module; //72; // 720 mAh

    telemetry_data[0] = BINARY_DATA_START;
    telemetry_data[1] = HOTT_GENERAL_AIR_MODULE_ID;
    telemetry_data[2] = 0x00; // Warnings
    telemetry_data[3] = HOTT_GENERAL_AIR_SENSOR_ID;

    telemetry_data[23] = altitude & 0xFF; // Altitude
    telemetry_data[24] = (altitude >> 8) & 0xFF;

    telemetry_data[28] = current & 0xFF; // Current
    telemetry_data[29] = (current >> 8) & 0xFF;

    telemetry_data[12] = voltage & 0xFF;
    telemetry_data[13] = (voltage >> 8) & 0xFF;

    telemetry_data[32] = capacity & 0xFF;
    telemetry_data[33] = (capacity >> 8) & 0xFF;

    telemetry_data[25] = vspeed & 0xFF; // climb rate
    telemetry_data[26] = (vspeed >> 8) & 0xFF;

    telemetry_data[43] = BINARY_DATA_STOP;

    telemetry_data[BINARY_SIZE - 1] = getCheckSum(telemetry_data, BINARY_SIZE - 1);

    //serial.write(telemetry_data, BINARY_SIZE);
    for (int i = 0; i < BINARY_SIZE; ++i) {
        serial.write(telemetry_data[i]);
        delay(2);
    }
}

void setup() {
    serial.begin(HOTT_BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    static State state = WAIT_START;

    sendData();

// Seems it does not work when waiting for request!
#if 0
    switch(state) {
        case WAIT_START:
            Serial.println("Wait");
            if ((serial.available()) && (serial.read() == HOTT_BINARY_MODE_REQUEST_ID))
                state = WAIT_ID;
            break;

        case WAIT_ID:
            Serial.println("Wait ID");
            if (serial.available()) {
                if (serial.read() == HOTT_GENERAL_AIR_MODULE_ID)
                    state = SEND_DATA;
                else
                    state = WAIT_START;
            }
            break;

        case SEND_DATA:
            Serial.println("Send");
            digitalWrite(LED_BUILTIN, LOW);
            sendData();
            state = WAIT_START;
            digitalWrite(LED_BUILTIN, HIGH);
            break;
    }
#endif
}
