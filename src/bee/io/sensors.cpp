#include "sensors.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
// #include <bmi160.h>
#include <bme280.h>

#include "../hat_pcb.h"
#include "serial.h"
#include "../sys/systick.h"

//#include <TinyGPS.h>
// set up the speed, data order and data mode
//SPISettings set_gps(4000, MSBFIRST, SPI_MODE1);
//TinyGPS gps;

enum DEVICES
{
    BME = 0
};

struct SPI_DEVICE
{
    uint32_t port;
    uint32_t pin;
};

static const uint32_t BME280_DEVICE_ID = 0;
SPI_DEVICE devices[1] = {{BME280_CS_PORT, BME280_CS_PIN}};

// struct bmi160_dev dev_bmi; //1000000 msbfirst, spimode0
struct bme280_dev dev_bme;
bool init_bme=false;

bool Sensors::setup()
{
    ser << "Setting up SPI\n";
    rcc_periph_clock_enable(SPI_RCC_PORT);
    rcc_periph_clock_enable(SPI_RCC_SPI_PORT);

    gpio_mode_setup(SPI_PORT, GPIO_MODE_AF, GPIO_PUPD_PULLDOWN, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_af(SPI_PORT, SPI_AF, SPI_SCK | SPI_MISO | SPI_MOSI);
    gpio_set_output_options(SPI_PORT, GPIO_OTYPE_PP, GPIO_OSPEED_HIGH, SPI_SCK | SPI_MOSI);
    gpio_set(SPI_PORT, SPI_SS);
    gpio_mode_setup(SPI_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, SPI_MISO);

    spi_reset(SPI);
    spi_init_master(SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_16, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                    SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_MSBFIRST);

    //needed even if we handle the slave selects ourselves
    spi_enable_software_slave_management(SPI);
    spi_set_nss_high(SPI);

    // The terminology around directionality can be a little confusing here -
    // unidirectional mode means that this is the only chip initiating
    // transfers, not that it will ignore any incoming data on the MISO pin.
    // Enabling duplex is required to read data back however.
    spi_set_unidirectional_mode(SPI);

    // We're using 8 bit, not 16 bit, transfers
    spi_fifo_reception_threshold_8bit(SPI);
    spi_set_data_size(SPI, SPI_CR2_DS_8BIT);

    spi_enable(SPI);

    ser << "SPI enabled\n"; 

    //TODO: iterate over devices and set pins high

    // if (!initializeBMI())
    // 	return false;

    if (!initializeBME())
        return false;

    return true;
}

int8_t Sensors::spi_transfer(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    ser << "SPI transfer: dev=" << (uint32_t)device_id << " reg=" << (uint32_t)reg_addr << " len=" << (uint32_t)len << "\n"; 
    //gpio_clear(devices[device_id].port, devices[device_id].pin);
    gpio_clear(BME280_CS_PORT, BME280_CS_PIN);

	SPI_DR8(SPI) = reg_addr;

	ser << "wrote reg\n";

	while(!(SPI_SR(SPI) & SPI_SR_TXE));
	ser << "ready to transmit\n";
	while(!(SPI_SR(SPI) & SPI_SR_RXNE));

	ser << "ready to write data \n";

    // For each byte of data we want to transmit
    for (uint8_t i = 0; i < len; i++) {
        // Wait for the peripheral to become ready to transmit (transmit buffer
        while (!(SPI_SR(SPI) & SPI_SR_TXE));

	ser << "ready to transmit\n";
        // Place the next data in the data register for transmission
	SPI_DR8(SPI) = reg_data[i];

	ser << "wrote data\n";
        while (!(SPI_SR(SPI) & SPI_SR_RXNE));

	ser << "ready to receive\n";
      	reg_data[i] = SPI_DR8(SPI);
	ser << "received: " << (uint32_t)reg_data[i] << "\n";
    }

    // Putting data into the SPI_DR register doesn't block - it will start
    // sending the data asynchronously with the main CPU. To make sure that the
    // data is finished sending before we pull the register clock high again,
    // we wait here until the busy flag is cleared on the SPI peripheral.
    while (SPI_SR(SPI2) & SPI_SR_BSY);

    //gpio_set(devices[device_id].port, devices[device_id].pin);
    gpio_set(BME280_CS_PORT, BME280_CS_PIN);
	return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    ser << "Doing a user delay: " << milliseconds << "\n";
    SysTick::sleep(milliseconds);
}

bool Sensors::initializeBMI()
{
    // dev_bmi.id = BMI;
    // dev_bmi.interface = BMI160_SPI_INTF;
    // dev_bmi.read = spi_transfer;
    // dev_bmi.write = spi_transfer;
    // dev_bmi.delay_ms = user_delay_ms;

    // int8_t rslt = BMI160_OK;
    // rslt = bmi160_init(&dev_bmi);

    // if(rslt != BMI160_OK)
    // {
    //     ser << "Could not initialize BMI160: " << rslt << ser.endl;
    //     return false;
    // }

    // rslt = BMI160_OK;

    // // Select the Output data rate, range of accelerometer sensor
    // dev_bmi.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ; //1600HZ
    // dev_bmi.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    // dev_bmi.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    // // Select the power mode of accelerometer sensor
    // dev_bmi.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    // // Select the Output data rate, range of Gyroscope sensor
    // dev_bmi.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ; //3200HZ
    // dev_bmi.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    // dev_bmi.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    // // Select the power mode of Gyroscope sensor
    // dev_bmi.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

    // // Set the sensor configuration
    // rslt = bmi160_set_sens_conf(&dev_bmi);

    // if(rslt != BMI160_OK)
    // {
    // 	ser << "Could not configurate BMI160: " << rslt << ser.endl;
    // 	return false;
    // }

    // rslt = bmi160_perform_self_test((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &dev_bmi);

    // if(rslt != BMI160_OK)
    // {
    //     ser << "BMI160 self test failed: " << rslt << ser.endl;
    // 	return false;
    // }
    // else
    // {
    // 	ser << "BMI160 ready" << ser.endl;
    // }
    return true;
}

bool Sensors::initializeBME()
{
    ser << "Initializing BME280...\n"; 
    gpio_set(BME280_CS_PORT, BME280_CS_PIN);
    gpio_mode_setup(BME280_CS_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, BME280_CS_PIN);

    dev_bme.dev_id = BME280_DEVICE_ID;
    dev_bme.intf = BME280_SPI_INTF;
    dev_bme.read = spi_transfer;
    dev_bme.write = spi_transfer;
    dev_bme.delay_ms = user_delay_ms;

    int8_t rslt = BME280_OK;
    do
    {
    rslt = bme280_init(&dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "Could not initialize BME280: " << (int32_t)rslt << "\n";
        //Serial.println(rslt);
        //return false;
    }
    }
    while(rslt!=BME280_OK);

    init_bme = true;
    ser << "Done\n";

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
    rslt = bme280_set_sensor_mode(BME280_FORCED_MODE, &dev_bme);

    if (rslt != BME280_OK)
    {
        ser << "Could not setup BME280: " << (int32_t)rslt << "\n";
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

void Sensors::readBME()
{
    if(!init_bme)
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
    //readBMI();
    //readGPS();
}
