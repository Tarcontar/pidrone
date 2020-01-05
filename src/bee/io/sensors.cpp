#include "sensors.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include <cstdio>
#include <cmath>

#include <bme280.h>
#include <bmp3.h>
#include <bmi08x.h>
#include <bmi088.h>

#include "../hat_pcb.h"
#include "serial.h"
#include "../sys/clock.h"
#include "usart.h"


static const float RAD_TO_DEG = 57.2957795;

struct SPI_DEVICE
{
    uint32_t port;
    uint32_t pin;
};

static const uint32_t BME280_DEVICE_ID = 0;
static const uint32_t BMP388_DEVICE_ID = 1;
static const uint32_t BMI088_ACCEL_DEVICE_ID = 2;
static const uint32_t BMI088_GYRO_DEVICE_ID = 3;
static const uint32_t ORG1510_GPS_DEVICE_ID = 4;

SPI_DEVICE devices[] =
{
    {BME280_CS_PORT, BME280_CS_PIN},
    {BMP388_CS_PORT, BMP388_CS_PIN},
    {BMI088_ACCEL_CS_PORT, BMI088_ACCEL_CS_PIN},
    {BMI088_GYRO_CS_PORT, BMI088_GYRO_CS_PIN},
    {ORG1510_CS_PORT, ORG1510_CS_PIN}
};

// BME280
struct bme280_dev dev_bme;
bool init_bme = false;
// BMP388
struct bmp3_dev dev_bmp;
bool init_bmp = false;
// BMI088
struct bmi08x_dev dev_bmi;
bool init_bmi = false;

bool Sensors::setup()
{
     Clock::sleep(3000);
    ser << "Setting up SPI\n";
    rcc_periph_clock_enable(SPI_RCC_PORT);
    rcc_periph_clock_enable(SPI_RCC_SPI_PORT);

    gpio_mode_setup(SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_af(SPI_PORT, SPI_AF, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_output_options(SPI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, SPI_SCK | SPI_MOSI);


    for(const auto& device : devices)
    {
        gpio_mode_setup(device.port, GPIO_MODE_INPUT, GPIO_PUPD_NONE, device.pin);
        //gpio_set(device.port, device.pin);
    }

    spi_reset(SPI);
    spi_set_baudrate_prescaler(SPI, SPI_CR1_BR_FPCLK_DIV_256);

    spi_set_standard_mode(SPI, 1);
    spi_set_data_size(SPI, SPI_CR2_DS_8BIT);
    spi_fifo_reception_threshold_8bit(SPI);
    spi_enable_software_slave_management(SPI);
    spi_set_nss_high(SPI);
    spi_set_full_duplex_mode(SPI);
	spi_set_unidirectional_mode(SPI);
    spi_send_msb_first(SPI);
    spi_set_master_mode(SPI);
    spi_enable(SPI);

    ser << "SPI enabled\n";

    // if (!initializeBME()) return false;
    // if (!initializeBMP()) return false;
    // if (!initializeBMI()) return false;

    gps.millis = Clock::millis;

    ser << "SPI setup done\n";
    Clock::sleep(3000);

    for(const auto& device : devices)
    {
        gpio_mode_setup(device.port, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, device.pin);
        gpio_set(device.port, device.pin);
    }

    return true;
}

void enableDevice(uint8_t device_id)
{
    gpio_clear(devices[device_id].port, devices[device_id].pin);
}

void disableDevice(uint8_t device_id)
{
    gpio_set(devices[device_id].port, devices[device_id].pin);
}

int8_t Sensors::spi_transfer(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    enableDevice(device_id);
    spi_set_standard_mode(SPI, 0);
    spi_xfer(SPI, reg_addr);

    for (uint8_t i = 0; i < len; i++)
    {
        while (SPI_SR(SPI) & SPI_SR_BSY);
        while (!(SPI_SR(SPI) & SPI_SR_TXE));
        reg_data[i] = spi_xfer(SPI, reg_data[i]);
    }

    while (SPI_SR(SPI) & SPI_SR_BSY);
    while (SPI_SR(SPI) & SPI_SR_RXNE) spi_read(SPI);
    disableDevice(device_id);
    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    Clock::sleep(milliseconds);
}

bool Sensors::initializeBMI()
{
    ser << "Initializing BMI088...\n";

    dev_bmi.accel_id = BMI088_ACCEL_DEVICE_ID;
    dev_bmi.gyro_id = BMI088_GYRO_DEVICE_ID;
    dev_bmi.intf = BMI08X_SPI_INTF;
    dev_bmi.read = spi_transfer;
    dev_bmi.write = spi_transfer;
    dev_bmi.delay_ms = user_delay_ms;

    int8_t rslt = bmi088_init(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not initialize -> " << rslt << "\n";
        return false;
    }

    dev_bmi.accel_cfg.bw = BMI08X_ACCEL_BW_NORMAL;
    dev_bmi.accel_cfg.odr = BMI08X_ACCEL_ODR_100_HZ;
    dev_bmi.accel_cfg.range = BMI088_ACCEL_RANGE_24G;
    dev_bmi.accel_cfg.power = BMI08X_ACCEL_PM_ACTIVE;

    rslt = bmi08a_set_power_mode(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set accel power mode -> " << rslt << "\n";
        return false;
    }
    rslt = bmi08a_set_meas_conf(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set accel config -> " << rslt << "\n";
        return false;
    }

    dev_bmi.gyro_cfg.power = BMI08X_GYRO_PM_NORMAL;

    rslt = bmi08g_set_power_mode(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set gyro power mode -> " << rslt << "\n";
        return false;
    }

    dev_bmi.gyro_cfg.odr = BMI08X_GYRO_BW_23_ODR_200_HZ;
    dev_bmi.gyro_cfg.range = BMI08X_GYRO_RANGE_1000_DPS;
    dev_bmi.gyro_cfg.bw = BMI08X_GYRO_BW_23_ODR_200_HZ;

    rslt = bmi08g_set_meas_conf(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set gyro config -> " << (int32_t)rslt << "\n";
        return false;
    }

    //TODO: find meaningful config values
    struct bmi08x_data_sync_cfg sync_cfg;
    /*! Mode (0 = off, 1 = 400Hz, 2 = 1kHz, 3 = 2kHz) */
    sync_cfg.mode = 3;

    rslt = bmi088_configure_data_synchronization(sync_cfg, &dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not configure data synchronization -> " << rslt << "\n";
        return false;
    }

    init_bmi = true;

    ser << "BMI088: ready\n";
    return true;
}

void Sensors::readBMI()
{
    uint32_t user_sampling_time;

    int8_t rslt = bmi08a_get_sensor_time(&dev_bmi, &user_sampling_time);
    if (rslt != BMI08X_OK) ser << "BMI088: could not read sensor time -> " << rslt << "\n";

    struct bmi08x_sensor_data accel_data;
    struct bmi08x_sensor_data gyro_data;
    rslt = bmi088_get_synchronized_data(&accel_data, &gyro_data, &dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: could not read sensor data -> " << rslt << "\n";
        return;
    }

    float gyroX = convertRawGyro(gyro_data.x);
    float gyroY = convertRawGyro(gyro_data.y);
    float gyroZ = convertRawGyro(gyro_data.z);

    float accX = convertRawAccel(accel_data.x);
    float accY = convertRawAccel(accel_data.y);
    float accZ = convertRawAccel(accel_data.z);

    ser << "gyro: " << gyroX << " " << gyroY << " " << gyroZ << "\n";
    ser << "accel: " << accX << " " << accY << " " << accZ << "\n";
    m_roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    m_pitch = atan2(-accX, accZ) * RAD_TO_DEG;
    m_yaw = 0.0; //how to calculate?

    ser << "roll: " << m_roll << " pitch: " << m_pitch << " yaw: " << m_yaw << "\n";
}

bool Sensors::initializeBMP()
{
    ser << "Initializing BMP388...\n";

    dev_bmp.dev_id = BMP388_DEVICE_ID;
    dev_bmp.intf = BMP3_SPI_INTF;
    dev_bmp.read = spi_transfer;
    dev_bmp.write = spi_transfer;
    dev_bmp.delay_ms = user_delay_ms;

    int8_t rslt = bmp3_init(&dev_bmp);
    if (rslt != BMP3_OK)
    {
        ser << "BMP388: Could not initialize -> " << rslt << "\n";
        return false;
    }
    init_bme = true;

    uint16_t settings_sel;

    dev_bmp.settings.press_en = BMP3_ENABLE;
    dev_bmp.settings.temp_en = BMP3_ENABLE;

    dev_bmp.settings.odr_filter.press_os = BMP3_NO_OVERSAMPLING;
    dev_bmp.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    dev_bmp.settings.odr_filter.odr = BMP3_ODR_200_HZ;

    settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL;
    rslt = bmp3_set_sensor_settings(settings_sel, &dev_bmp);

    dev_bmp.settings.op_mode = BMP3_NORMAL_MODE;
    rslt = bmp3_set_op_mode(&dev_bmp);

    if (rslt != BMP3_OK)
    {
        ser << "BMP388: Could not set sensor settings -> " << rslt << "\n";
        return false;
    }

    ser << "BMP388: ready\n";
    return true;
}

void Sensors::readBMP()
{
    struct bmp3_data data;

    uint8_t sensor_comp = BMP3_PRESS | BMP3_TEMP;
    int8_t rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev_bmp);
    if (rslt != BMP3_OK)
    {
        ser << "BMP388: unable to read sensor data -> " << rslt << "\n";
        return;
    }

    ser << "Temp: " << data.temperature << "\n";
    ser << "Pressure: " << data.pressure << "\n";
}

bool Sensors::initializeBME()
{
    ser << "Initializing BME280...\n";

    dev_bme.dev_id = BME280_DEVICE_ID;
    dev_bme.intf = BME280_SPI_INTF;
    dev_bme.read = spi_transfer;
    dev_bme.write = spi_transfer;
    dev_bme.delay_ms = user_delay_ms;

    int8_t rslt = bme280_init(&dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "BME280: Could not initialize -> " << rslt << "\n";
        return false;
    }

    init_bme = true;

    dev_bme.settings.osr_h = BME280_OVERSAMPLING_1X;
    dev_bme.settings.osr_p = BME280_OVERSAMPLING_16X;
    dev_bme.settings.osr_t = BME280_OVERSAMPLING_2X;
    dev_bme.settings.filter = BME280_FILTER_COEFF_16;
    dev_bme.settings.standby_time = BME280_STANDBY_TIME_62_5_MS;

    uint8_t settings_sel;
    settings_sel = BME280_OSR_PRESS_SEL;
    settings_sel |= BME280_OSR_TEMP_SEL;
    settings_sel |= BME280_OSR_HUM_SEL;
    settings_sel |= BME280_STANDBY_SEL;
    settings_sel |= BME280_FILTER_SEL;
    rslt = bme280_set_sensor_settings(settings_sel, &dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "BME280: Could not set sensor settings -> " << rslt << "\n";
        return false;
    }

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "BME280: Could not set sensor mode -> " << rslt << "\n";
        return false;
    }

    ser << "BME280: ready\n";
    return true;
}

void Sensors::readBME()
{
    if (!init_bme) return;

    struct bme280_data data;
    int8_t rslt = bme280_get_sensor_data(BME280_ALL, &data, &dev_bme);
    if (rslt != BME280_OK)
    {
        ser << "BME280: Could not read data -> " << rslt;
        return;
    }

    ser << "Temp: " << data.temperature << "\n";
    ser << "Humidity: " << data.humidity << "\n";
    ser << "Pressure: " << data.pressure << "\n";
}

void Sensors::readGPS()
{
    enableDevice(ORG1510_GPS_DEVICE_ID);
    spi_set_standard_mode(SPI, 1);
	ser << "Reading GPS1510\n";

    //read until new data is available
    bool receivedData = false;

    // Wait that we are no busy anympre
    while (SPI_SR(SPI) & SPI_SR_BSY);

    while(!receivedData)
    {
        spi_send8(SPI, 0);
        char c = spi_read8(SPI);
        USART::write(c);
        ser << "\n";
        if (gps.encode(c)) // Did a new valid sentence come in?
            receivedData = true;
    }

    ser << "GPS1510 received " << receivedData << "\n";
    disableDevice(ORG1510_GPS_DEVICE_ID);
}

float Sensors::convertRawGyro(int gRaw)
{
    //since we are using 250 degrees/second range
    return (gRaw * 250.0) / 32768.0;
}

float Sensors::convertRawAccel(int aRaw)
{
    // since we are using 2G range
    // -2g maps to a raw value of -32768
    // +2g maps to a raw value of 32767

    return (aRaw * 2.0) / 32768.0;
}

void Sensors::update()
{
    // readBME();
    // readBMP();
    // readBMI();
    ser << "GPS1510 blaa sth\n";
    readGPS();
}
