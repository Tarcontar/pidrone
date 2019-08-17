#pragma once
#include <stdint.h>

class SysTick
{
    public:
    static void setup();
    static void sleep(uint32_t delay);
};
