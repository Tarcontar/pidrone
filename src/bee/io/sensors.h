#pragma once
#include <stdint.h>
#include <TinyGPS.h>

class Sensors
{
public:
    bool setup();

    void update();

    inline float getRoll()  { return m_roll; }
    inline float getYaw()   { return m_yaw; }
    inline float getPitch() { return m_pitch; }
private:
    bool initializeBMI();
    bool initializeBME();
    bool initializeBMP();

    void readBMI();
    void readBME();
    void readBMP();
    void readGPS();

    static int8_t spi_transfer(uint8_t device_id, uint8_t reg_addr,
                                uint8_t *reg_data, uint16_t len);

    static void user_delay_ms(uint32_t milliseconds);

    float convertRawGyro(int gRaw);
    float convertRawAccel(int aRaw);

    // set up the speed, data order and data mode
    //SPISettings set_gps(4000, MSBFIRST, SPI_MODE1);
    TinyGPS gps;

    float m_roll;
    float m_pitch;
    float m_yaw;
};
