#include "ElectricAirModule.h"

void ElectricAirModule::setMainVoltage(double mainVoltage) {
    this->mainVoltage = mainVoltage;
}

void ElectricAirModule::setCurrent(double current) {
    this->current = current;
}

void ElectricAirModule::setAltitude(double altitude) {
    this->altitude = altitude;
}

void ElectricAirModule::setVSpeed(double vSpeed) {
    this->vSpeed = vSpeed;
}

void ElectricAirModule::setCapacity(double capacity) {
    this->capacity = capacity;
}

uint8_t *ElectricAirModule::getFrame() {
    memset(frame, 0x00, FRAME_SIZE);

    frame[0] = HOTT_BINARY_DATA_START;
    frame[1] = MODULE_ID;
    frame[2] = 0x00; // Warnings
    frame[3] = SENSOR_ID;

    uint16_t telemetryAltitude = static_cast<uint16_t>(round(ALTITUDE_OFFSET + altitude));
    frame[26] = lsb(telemetryAltitude);
    frame[27] = msb(telemetryAltitude);

    uint16_t telemetryCurrent = static_cast<uint16_t>(round(current * CURRENT_FACTOR));
    frame[28] = lsb(telemetryCurrent);
    frame[29] = msb(telemetryCurrent);

    uint16_t telemetryVoltage = static_cast<uint16_t>(round(mainVoltage * VOLTAGE_FACTOR));
    frame[30] = lsb(telemetryVoltage);
    frame[31] = msb(telemetryVoltage);

    uint16_t telemetryCapacity = static_cast<uint16_t>(round(capacity * CAPACITY_FACTOR));
    frame[32] = lsb(telemetryCapacity);
    frame[33] = msb(telemetryCapacity);

    uint16_t telemetryVSpeed = static_cast<uint16_t>(VSPEED_OFFSET + round(vSpeed) * VSPEED_FACTOR);
    frame[34] = lsb(telemetryVSpeed);
    frame[35] = msb(telemetryVSpeed);

    frame[43] = HOTT_BINARY_DATA_END;
    frame[44] = calcCheckSum(frame, 0, 43);

    return frame;
}
