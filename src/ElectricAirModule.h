#ifndef ELECTRIC_AIR_MODULE_H
#define ELECTRIC_AIR_MODULE_H

#include "BinaryModule.h"

class ElectricAirModule : BinaryModule {
    public:
        static const unsigned int MODULE_ID = 0x8E;
        static const unsigned int SENSOR_ID = 0xE0;

        void setMainVoltage(double mainVoltage);
        void setCurrent(double current);
        void setAltitude(double altitude);
        void setVSpeed(double vSpeed);
        void setCapacity(double capacity);

        uint8_t *getFrame();

    private:
        static constexpr double VOLTAGE_FACTOR = 10.0;
        static constexpr double CURRENT_FACTOR = 10.0;
        static constexpr double VSPEED_FACTOR = 100.0;
        static constexpr double VSPEED_OFFSET = 30000.0;
        static constexpr double CAPACITY_FACTOR = 0.1;
        static constexpr double ALTITUDE_OFFSET = 500.0;

        double mainVoltage = 0.0;
        double current = 0.0;
        double altitude = 0.0;
        double vSpeed = 0.0;
        double capacity = 0.0;
};

#endif
