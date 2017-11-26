#pragma once
#include <stdint.h>

class Sensors 
{
public:
    bool setup(uint8_t bmi_cs = -1, uint8_t bme_cs = -1);
    
	void readBMI(double *roll, double *pitch, double * yaw);
    void readBME();

private:
    bool initializeBMI();
    bool initializeBME();

    static int8_t spi_transfer(uint8_t cs, uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t len);

    static void user_delay_ms(uint32_t milliseconds);
	
	float convertRawGyro(int gRaw);
	float convertRawAccel(int aRaw);
private:
	uint8_t m_bmi_cs;
	uint8_t m_bme_cs;
};