#include "VarioModule.h"

void VarioModule::setAltitude(double altitude) {
    this->altitude = altitude;
}

void VarioModule::setVSpeed(double vSpeed) {
    this->vSpeed = vSpeed;
}

uint8_t *VarioModule::getFrame()
{
    memset(frame, 0x00, FRAME_SIZE);

    frame[0] = HOTT_BINARY_DATA_START;
    frame[1] = MODULE_ID;
    frame[2] = 0x00; // Warnings
    frame[3] = SENSOR_ID;

    uint16_t telemetryAltitude = static_cast<uint16_t>(round(ALTITUDE_OFFSET + altitude));
    frame[5] = lsb(telemetryAltitude);
    frame[6] = msb(telemetryAltitude);

    uint16_t telemetryVSpeed = static_cast<uint16_t>(round(VSPEED_OFFSET + vSpeed * VSPEED_FACTOR));
    frame[11] = lsb(telemetryVSpeed);
    frame[12] = msb(telemetryVSpeed);

    frame[43] = HOTT_BINARY_DATA_END;
    frame[44] = calcCheckSum(frame, 0, 43);

    return frame;
}