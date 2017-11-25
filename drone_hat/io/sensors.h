#pragma once
#include <stdint.h>

class Sensors 
{
public:
    void setup(uint8_t bmi_cs = -1, uint8_t bme_cs = -1);
    void update();

private:
    void initializeBMI();
    void initializeBME();
	
    void readBMI();
    void readBME();

    static int8_t spi_transfer(uint8_t cs, uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t len);

    static void user_delay_ms(uint32_t milliseconds);
	
private:
	uint8_t m_bmi_cs;
	uint8_t m_bme_cs;
};