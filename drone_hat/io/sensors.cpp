#include "sensors.h"
#include <Arduino.h>
#include <SPI.h>
#include <bmi160.h>
#include <bme280.h>

// set up the speed, data order and data mode
//SPISettings set_bmi(1000000, MSBFIRST, SPI_MODE0);
//SPISettings set_bme(1000000, MSBFIRST, SPI_MODE0);

SPISettings set(1000000, MSBFIRST, SPI_MODE0);

struct bmi160_dev dev_bmi;
struct bme280_dev dev_bme;

void Sensors::setup(uint8_t bmi_cs, uint8_t bme_cs)
{
    // set the Slave Select Pins as outputs:
    pinMode (8, OUTPUT);
    pinMode (7, OUTPUT);
    SPI.begin();
    initializeBMI();
    //initializeBME();
}

void Sensors::update()
{
    readBMI();
    //readBME();
}

int8_t Sensors::spi_transfer(uint8_t cs, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t len)
{
    SPI.beginTransaction(set);
    digitalWrite (8, LOW);

    SPI.transfer(reg_addr); 

    for(uint16_t i = 0; i < len; i++)
        reg_data[i] = SPI.transfer(reg_data[i]);

    digitalWrite (8, HIGH);
    SPI.endTransaction();

    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    delay(milliseconds);
}

void Sensors::initializeBMI()
{
    /* You may assign a chip select identifier to be handled later */
    dev_bmi.id = 8;
    dev_bmi.interface = BMI160_SPI_INTF;
    dev_bmi.read = spi_transfer;
    dev_bmi.write = spi_transfer;
    dev_bmi.delay_ms = user_delay_ms;

    int8_t rslt = BMI160_OK;
    rslt = bmi160_init(&dev_bmi);

    if(rslt != BMI160_OK)
    {
        Serial.print("1Could not initialize BMI160: ");
        Serial.println(rslt);
        //return;
    }

    rslt = BMI160_OK;

    /* Select the Output data rate, range of accelerometer sensor */
    dev_bmi.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ; //1600HZ
    dev_bmi.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    dev_bmi.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    dev_bmi.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    dev_bmi.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ; //3200HZ
    dev_bmi.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    dev_bmi.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    dev_bmi.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE; 

    /* Set the sensor configuration */
    rslt = bmi160_set_sens_conf(&dev_bmi);

    if(rslt != BMI160_OK)
    {
        Serial.print("2Could not initialize BMI160: ");
        Serial.println(rslt);
    }

    rslt = bmi160_perform_self_test((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &dev_bmi);

    if(rslt != BMI160_OK)
    {
        Serial.print("BMI160 self test failed: ");
        Serial.println(rslt);
    }
	else
	{
		Serial.println("BMI160 ready");
	}
}

void Sensors::initializeBME()
{
    /* Sensor_0 interface over SPI with native chip select line */
    dev_bme.dev_id = m_bme_cs;
    dev_bme.intf = BME280_SPI_INTF;
    dev_bme.read = spi_transfer;
    dev_bme.write = spi_transfer;
    dev_bme.delay_ms = user_delay_ms;

    int8_t rslt = BME280_OK;
    rslt = bme280_init(&dev_bme);

    if(rslt != BME280_OK)
    {
        Serial.print("Could not initialize BME280: ");
        Serial.println(rslt);
    }

    /* Recommended mode of operation: Indoor navigation */
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
	rslt = bme280_set_sensor_mode(BME280_NORMAL_MODE, &dev_bme);

    if(rslt != BME280_OK)
    {
       Serial.print("Could not initialize BME280: ");
       Serial.println(rslt);
    }
	else
	{
		Serial.println("BME280 ready");
	}
}

void Sensors::readBMI()
{
    bmi160_sensor_data accel;
    bmi160_sensor_data gyro;
    int8_t rslt = BMI160_OK;

    /* To read both Accel and Gyro data */
    rslt = bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL),
                            &accel, &gyro, &dev_bmi);						
}

void Sensors::readBME()
{
    bme280_data comp_data;
    int8_t rslt = BMI160_OK;
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev_bme);
}
