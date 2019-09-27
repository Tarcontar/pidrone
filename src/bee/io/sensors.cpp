#include "sensors.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include <cstdio>

#include <bme280.h>
#include <bmp3.h>
#include <bmi08x.h>
#include <bmi088.h>

#include "../hat_pcb.h"
#include "serial.h"
#include "../sys/systick.h"

//#include <TinyGPS.h>
// set up the speed, data order and data mode
//SPISettings set_gps(4000, MSBFIRST, SPI_MODE1);
//TinyGPS gps;

struct SPI_DEVICE
{
    uint32_t port;
    uint32_t pin;
};

static const uint32_t BME280_DEVICE_ID = 0;
static const uint32_t BMP388_DEVICE_ID = 1;
static const uint32_t BMI088_ACCEL_DEVICE_ID = 2;
static const uint32_t BMI088_GYRO_DEVICE_ID = 3;

SPI_DEVICE devices[] =
{
    {BME280_CS_PORT, BME280_CS_PIN},
    {BMP388_CS_PORT, BMP388_CS_PIN},
    {BMI088_ACCEL_CS_PORT, BMI088_ACCEL_CS_PIN},
    {BMI088_GYRO_CS_PORT, BMI088_GYRO_CS_PIN}
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
    ser << "Setting up SPI\n";
    rcc_periph_clock_enable(SPI_RCC_PORT);
    rcc_periph_clock_enable(SPI_RCC_SPI_PORT);

    gpio_mode_setup(SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_af(SPI_PORT, SPI_AF, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_output_options(SPI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, SPI_SCK | SPI_MOSI);

    gpio_mode_setup(BME280_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BME280_CS_PIN);
    gpio_set(BME280_CS_PORT, BME280_CS_PIN);
    gpio_mode_setup(BMP388_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BMP388_CS_PIN);
    gpio_set(BMP388_CS_PORT, BMP388_CS_PIN);

    gpio_mode_setup(BMI088_GYRO_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BMI088_GYRO_CS_PIN);
    gpio_set(BMI088_GYRO_CS_PORT, BMI088_GYRO_CS_PIN);
    gpio_mode_setup(BMI088_ACCEL_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BMI088_ACCEL_CS_PIN);
    gpio_set(BMI088_ACCEL_CS_PORT, BMI088_ACCEL_CS_PIN);

    spi_reset(SPI);
    spi_init_master(SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_256, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_MSBFIRST);

    spi_set_data_size(SPI, SPI_CR2_DS_8BIT);
    spi_fifo_reception_threshold_8bit(SPI);

    spi_set_master_mode(SPI);
    spi_enable(SPI);

    ser << "SPI enabled\n";

    if (!initializeBME()) return false;
    if (!initializeBMP()) return false;
    if (!initializeBMI()) return false;

    ser << "SPI setup done\n";

    return true;
}

int8_t Sensors::spi_transfer(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    gpio_clear(devices[device_id].port, devices[device_id].pin);

    spi_xfer(SPI, reg_addr);

    for (uint8_t i = 0; i < len; i++)
    {
        while (SPI_SR(SPI) & SPI_SR_BSY);
        while (!(SPI_SR(SPI) & SPI_SR_TXE));
        reg_data[i] = spi_xfer(SPI, reg_data[i]);
    }

    while (SPI_SR(SPI) & SPI_SR_BSY);
    while (SPI_SR(SPI) & SPI_SR_RXNE) spi_read(SPI);

    gpio_set(devices[device_id].port, devices[device_id].pin);
    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    SysTick::sleep_mills(milliseconds);
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
        ser << "BMI088: Could not initialize -> " << (int32_t)rslt << "\n";
        return false;
    }

    dev_bmi.accel_cfg.bw = BMI08X_ACCEL_BW_NORMAL;
    dev_bmi.accel_cfg.odr = BMI08X_ACCEL_ODR_100_HZ;
    dev_bmi.accel_cfg.range = BMI088_ACCEL_RANGE_24G;
    dev_bmi.accel_cfg.power = BMI08X_ACCEL_PM_ACTIVE;

    rslt = bmi08a_set_power_mode(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set accel power mode -> " << (int32_t)rslt << "\n";
        return false;
    }
    rslt = bmi08a_set_meas_conf(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set accel config -> " << (int32_t)rslt << "\n";
        return false;
    }

    dev_bmi.gyro_cfg.power = BMI08X_GYRO_PM_NORMAL;

    rslt = bmi08g_set_power_mode(&dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not set gyro power mode -> " << (int32_t)rslt << "\n";
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
        ser << "BMI088: Could not configure data synchronization -> " << (int32_t)rslt << "\n";
        return false;
    }

    /*
    //TODO: find meaningful config values
    struct bmi08x_anymotion_cfg anymotion_cfg;
    anymotion_cfg.threshold = ?;
    anymotion_cfg.nomotion_sel = ?;
    anymotion_cfg.duration = ?;
    anymotion_cfg.x_en = 1; ?
    anymotion_cfg.y_en = 1; ?
    anymotion_cfg.z_en = 1; ?

    rslt = bmi088_configure_anymotion(anymotion_cfg, &dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: Could not configure anymotion -> " << (int32_t)rslt << "\n";
        return false;
    }
    */

    init_bmi = true;

    ser << "BMI088: ready\n";
    return true;
}

void Sensors::readBMI()
{
    uint32_t user_sampling_time;

    int8_t rslt = bmi08a_get_sensor_time(&dev_bmi, &user_sampling_time);
    if (rslt != BMI08X_OK) ser << "BMI088: could not read sensor time -> " << (int32_t)rslt << "\n";

    struct bmi08x_sensor_data accel_data;
    struct bmi08x_sensor_data gyro_data;
    rslt = bmi088_get_synchronized_data(&accel_data, &gyro_data, &dev_bmi);
    if (rslt != BMI08X_OK)
    {
        ser << "BMI088: could not read sensor data -> " << (int32_t)rslt << "\n";
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
    //m_roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
    //m_pitch = atan2(-accX, accZ) * RAD_TO_DEG;
    //m_yaw = 0.0; //how to calculate?
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
        ser << "BMP388: Could not initialize -> " << (int32_t)rslt << "\n";
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
        ser << "BMP388: Could not set sensor settings -> " << (int32_t)rslt << "\n";
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
        ser << "BMP388: unable to read sensor data -> " << (uint32_t)rslt << "\n";
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
        ser << "BME280: Could not initialize -> " << (int32_t)rslt << "\n";
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
        ser << "BME280: Could not set sensor settings -> " << (int32_t)rslt << "\n";
        return false;
    }

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "BME280: Could not set sensor mode -> " << (int32_t)rslt << "\n";
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
        ser << "BME280: Could not read data -> " << (int32_t)rslt;
        return;
    }

    ser << "Temp: " << data.temperature << "\n";
    ser << "Humidity: " << data.humidity << "\n";
    ser << "Pressure: " << data.pressure << "\n";
}

void Sensors::readGPS()
{
    /*
	ser << "Reading GPS1510";

    //read until new data is available
    bool receivedData = false;

    while(!receivedData)
    {
        char d = 0xff;
        char c = spi_transfer(1, 0, &d, 1);
        if (gps.encode(c)) // Did a new valid sentence come in?
            receivedData = true;
    }

    ser << "GPS1510 received sth\n";
	*/
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
    readBME();
    readBMP();
    readBMI();
    //readGPS();
}
