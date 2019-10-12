#pragma once
#include <stdint.h>

#define MID 1500

class PWMReceiver
{
public:
    enum Channel 
    {
            THRUST = 0,
            ROLL,
            PITCH,
            YAW,
            MODE,
            UNUSED
    };

    PWMReceiver();

    uint16_t getChannel(const Channel c);
};
