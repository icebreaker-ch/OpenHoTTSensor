#include <Arduino.h>
#include <SoftwareSerial.h>

#define HOTT_BAUDRATE 19400
#define BINARY_SIZE 45
#define BINARY_DATA_START 0x7C
#define BINARY_DATA_STOP 0x7D
#define HOTT_BINARY_MODE_REQUEST_ID 0x80
#define HOTT_ELECTRIC_AIR_MODULE_ID 0x8E
#define HOTT_ELECTRIC_AIR_SENSOR_ID 0xE0

#define HOTT_GENERAL_AIR_MODULE_ID 0x8D
#define HOTT_GENERAL_AIR_SENSOR_ID 0xD0

#define HOTT_VARIO_MODULE_ID 0x89
#define HOTT_VARIO_SENSOR_ID 0x90

#define OFFSET_ALTITUDE 500
#define OFFSET_VSPEED 30000

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

void sendData(uint8_t *data, int size) {
    for (int i = 0; i < size; ++i) {
        serial.write(data[i]);
//        Serial.print(data[i], 16);
//        Serial.print(" ");
//        delay(2);
    }
//    Serial.println();
}

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

void sendEAM() {
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
    uint16_t capacity = 72; // 720 mAh

    telemetry_data[0] = BINARY_DATA_START;
    telemetry_data[1] = HOTT_ELECTRIC_AIR_MODULE_ID;
    telemetry_data[2] = 0x00; // Warnings
    telemetry_data[3] = HOTT_ELECTRIC_AIR_SENSOR_ID;

    telemetry_data[26] = altitude & 0xFF; // Altitude
    telemetry_data[27] = (altitude >> 8) & 0xFF;

    telemetry_data[28] = current & 0xFF; // Current
    telemetry_data[29] = (current >> 8) & 0xFF;

    telemetry_data[30] = voltage & 0xFF;
    telemetry_data[31] = (voltage >> 8) & 0xFF;

    telemetry_data[32] = capacity & 0xFF;
    telemetry_data[33] = (capacity >> 8) & 0xFF;

    telemetry_data[34] = vspeed & 0xFF; // climb rate
    telemetry_data[35] = (vspeed >> 8) & 0xFF;

    telemetry_data[43] = BINARY_DATA_STOP;

    telemetry_data[BINARY_SIZE - 1] = getCheckSum(telemetry_data, BINARY_SIZE - 1);

    sendData(telemetry_data, BINARY_SIZE);
}

void setup() {
    Serial.begin(9600);
    serial.begin(HOTT_BAUDRATE);
    pinMode(LED_BUILTIN, OUTPUT);
}


void loop() {
    static State state = WAIT_START;

#if 0
    if (serial.available() >= 2) {        
        uint8_t b1 = serial.read();
        if (b1 == HOTT_BINARY_MODE_REQUEST_ID) {
            uint8_t b2 = serial.read();
        }
    }
    sendEAM();
 #endif

#if 1
    switch(state) {
        case WAIT_START:
            int count = Serial1.available();
            Serial.println(count);
            Serial.print("Count: ");
            if (count > 0) {
                uint8_t requestId = Serial1.read();
                Serial.println(requestId, 16);
                if (requestId == HOTT_BINARY_MODE_REQUEST_ID) {
                    state = WAIT_ID;
                    Serial.println("WAIT_ID");
                }
            }
            break;

        case WAIT_ID:
            if (Serial1.available()) {
                uint8_t moduleId = Serial1.read();
                Serial.println(moduleId, 16);
                if (moduleId == HOTT_VARIO_MODULE_ID) {
                    state = SEND_DATA;
                    Serial.println("SEND_DATA");
                }
                else {
                    state = WAIT_START;
                    Serial.println("WAIT_START");
                }
            }
            break;

        case SEND_DATA:
            digitalWrite(LED_BUILTIN, HIGH);
            sendVario();
            state = WAIT_START;
            Serial.println("WAIT_START");
            digitalWrite(LED_BUILTIN, LOW);
            break;
    }

    //delay(100);
#endif
}
