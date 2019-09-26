#include "sensors.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>

#include <cstdio>
#include <bme280.h>
#include <bmp3.h>

#define BMI08X_ENABLE_BMI085 0
#define BMI08X_ENABLE_BMI088 1
#include <bmi08x.h> //needed?
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
    {BMI088_ACCEL_CS_PORT,  BMI088_ACCEL_CS_PIN},
    {BMI088_GYRO_CS_PORT,   BMI088_GYRO_CS_PIN}
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

    init(BME280_DEVICE_ID, 0xD0, 0x60);
    init(BMP388_DEVICE_ID, 0x00, 0x50);
    init(BMI088_ACCEL_DEVICE_ID, 0x00, 0x1E);
    init(BMI088_GYRO_DEVICE_ID, 0x00, 0x0F);
    //initializeBME();
    //initializeBMP();
    //initializeBMI();

    return true;
}

int8_t Sensors::init(uint8_t device_id, uint8_t reg_addr, uint8_t chip_id)
{
    uint8_t id = 0;
    int8_t rslt = get_reg(device_id, reg_addr, &id, 1);
    if (rslt == 0 && id == chip_id) return 5;
    return rslt;
}

int8_t Sensors::get_reg(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    uint8_t temp[len + 1];
    int8_t rslt = spi_transfer(device_id, reg_addr | 0x80, temp, len);
    if (rslt != 0) return -123;
    if (device_id > 0) //bmp and bmi -> dummy byte stuff
    {
        for (int i = 0; i < len; i++) reg_data[i] = temp[i + 1];
    }
    return rslt;
}

int8_t Sensors::spi_transfer(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    gpio_clear(devices[device_id].port, devices[device_id].pin);
    SysTick::sleep(5);

    reg_data[0] = spi_xfer(SPI, reg_addr);

    for (uint8_t i = 0; i < len; i++)
    {
        reg_data[i] = spi_xfer(SPI, reg_data[i]);
    }

    gpio_set(devices[device_id].port, devices[device_id].pin);

    ser << "dev: " << (uint32_t)device_id << " reg: " << (uint32_t)reg_addr << " rec: ";
    for (uint8_t i = 0; i < len; i++)
    {
        ser << (uint32_t)reg_data[i] << " ";
    }
    ser << "\n";
    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    SysTick::sleep(milliseconds);
}

bool Sensors::initializeBMI(int count)
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
        if (count < 5) return initializeBMI(++count);

        ser << "Could not initialize BMI088: " << (int32_t)rslt << "\n";
        return false;
    }
    init_bmi = true;

    ser << "BMI088 ready\n";
    return true;
}

bool Sensors::initializeBMP(int count)
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
        ser << "Could not initialize BMP388: " << (int32_t)rslt << "\n";
        if (++count < 5) return initializeBMP(count);
        return false;
    }
    init_bme = true;

    /* Used to select the settings user needs to change */
    uint16_t settings_sel;

    /* Select the pressure and temperature sensor to be enabled */
    dev_bmp.settings.press_en = BMP3_ENABLE;
    dev_bmp.settings.temp_en = BMP3_ENABLE;
    /* Sele. the output data rate and oversampling settings for pressure and temperature */
    dev_bmp.settings.odr_filter.press_os = BMP3_NO_OVERSAMPLING;
    dev_bmp.settings.odr_filter.temp_os = BMP3_NO_OVERSAMPLING;
    dev_bmp.settings.odr_filter.odr = BMP3_ODR_200_HZ;
    /* Assign the settings which needs to be set in the sensor */
    settings_sel = BMP3_PRESS_EN_SEL | BMP3_TEMP_EN_SEL | BMP3_PRESS_OS_SEL | BMP3_TEMP_OS_SEL | BMP3_ODR_SEL;
    rslt = bmp3_set_sensor_settings(settings_sel, &dev_bmp);

    /* Set the power mode to normal mode */
    dev_bmp.settings.op_mode = BMP3_NORMAL_MODE;
    rslt = bmp3_set_op_mode(&dev_bmp);

    if (rslt != BMP3_OK)
    {
        ser << "Could not set BMP3 sensor settings: " << (int32_t)rslt << "\n";
        return false;
    }

    ser << "BMP388 ready\n";
    return true;
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
        ser << "Could not initialize BME280: " << (int32_t)rslt << "\n";
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
        ser << "Could not set BME280 sensor settings: " << (int32_t)rslt << "\n";
        return false;
    }

    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "Could not set BME280 sensor mode: " << (int32_t)rslt << "\n";
        return false;
    }

    ser << "BME280 ready\n";
    return true;
}

void Sensors::readBMI()
{
    /*
    bmi160_sensor_data accel;
    bmi160_sensor_data gyro;
    int8_t rslt = BMI160_OK;

    // To read both Accel and Gyro data
    rslt = bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL),
                            &accel, &gyro, &dev_bmi);

	if (rslt != BMI160_OK)
	{
		return;
	}

	// float gyroX = convertRawGyro(gyro.x);
	// float gyroY = convertRawGyro(gyro.y);
	// float gyroZ = convertRawGyro(gyro.z);

	float accX = convertRawAccel(accel.x);
	float accY = convertRawAccel(accel.y);
	float accZ = convertRawAccel(accel.z);

	m_roll = atan(accY / sqrt(accX * accX + accZ * accZ)) * RAD_TO_DEG;
	m_pitch = atan2(-accX, accZ) * RAD_TO_DEG;
    m_yaw = 0.0; //how to calculate?
	*/
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

void Sensors::readBMP()
{
    int8_t rslt;
    /* Variable used to select the sensor component */
    uint8_t sensor_comp;
    /* Variable used to store the compensated data */
    struct bmp3_data data;

    /* Sensor component selection */
    sensor_comp = BMP3_PRESS | BMP3_TEMP;
    /* Temperature and Pressure data are read and stored in the bmp3_data instance */
    rslt = bmp3_get_sensor_data(sensor_comp, &data, &dev_bmp);

    /* Print the temperature and pressure data */
    printf("Temperature in deg celsius\t Pressure in Pascal\t\n");
    printf("%0.2f\t\t %0.2f\t\t\n", data.temperature, data.pressure);
}

void Sensors::readBME()
{
    if (!init_bme)
        return;

    bme280_data comp_data;
    int8_t rslt = BME280_OK;
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev_bme);
    if (rslt != BME280_OK)
    {
        ser << "Could not read BME280: " << (int32_t)rslt;
        return;
    }

    ser << "Temp: " << comp_data.temperature << "\n";
    ser << "Humidity: " << comp_data.temperature << "\n";
    ser << "Pressure: " << comp_data.pressure << "\n";
}

float Sensors::convertRawGyro(int gRaw)
{
    //since we are using 250 degrees/second range
    float g = (gRaw * 250.0) / 32768.0;
    return g;
}

float Sensors::convertRawAccel(int aRaw)
{
    // since we are using 2G range
    // -2g maps to a raw value of -32768
    // +2g maps to a raw value of 32767

    float a = (aRaw * 2.0) / 32768.0;
    return a;
}

void Sensors::update()
{
    readBME();
    readBMP();
    readBMI();
    //readGPS();
}
