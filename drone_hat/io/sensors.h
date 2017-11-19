#pragma once
#include <stdint.h>

class Sensors 
{
    public:
    void setup();
    void update();

    private:
    enum Identifier
    {
        ID_BMI = 0,
        ID_BME = 1
    };

    static int8_t user_spi_read(uint8_t dev_id, uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t len);

    static int8_t user_spi_write(uint8_t dev_id, uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t len);

    static void user_delay_ms(uint32_t milliseconds);
    private:
    const int BMI_PIN = 8;
    const int BME_PIN = 7;
};