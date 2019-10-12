#pragma once
#include <stdint.h>

class SysTick
{
public:
    static void setup();
    static void sleep(uint32_t delay);
    static volatile uint32_t system_millis;
};
