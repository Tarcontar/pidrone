#pragma once
#include <stdint.h>

class SysTick
{
    public:
    static void setup();
    static void sleep_mills(uint32_t delay);
    static void sleep_mics(uint32_t delay);
};
