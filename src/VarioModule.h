#ifndef VARIO_MODULE_H
#define VARIO_MODULE_H

#include "BinaryModule.h"

class VarioModule : BinaryModule {
    public:
        static const unsigned int MODULE_ID = 0x89;
        static const unsigned int SENSOR_ID = 0x90;

        static constexpr double ALTITUDE_OFFSET = 500.0;
        static constexpr double VSPEED_FACTOR = 100.0;
        static constexpr double VSPEED_OFFSET = 30000.0;

        void setAltitude(double altitude);
        void setVSpeed(double vSpeed);
        uint8_t *getFrame();

    private:
        double altitude;
        double vSpeed;
};

#endif
