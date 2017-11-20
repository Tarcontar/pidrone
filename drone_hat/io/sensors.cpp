#include "sensors.h"
#include <Arduino.h>
#include <SPI.h>
#include <bmi160.h>
#include <bme280.h>

// set up the speed, data order and data mode
SPISettings set_bmi(2000000, MSBFIRST, SPI_MODE1);
SPISettings set_bme(16000000, LSBFIRST, SPI_MODE3);

struct bmi160_dev dev_bmi;
struct bme280_dev dev_bme;

void Sensors::setup()
{
    // set the Slave Select Pins as outputs:
    pinMode (BMI_CS, OUTPUT);
    pinMode (BME_CS, OUTPUT);
    SPI.begin();
    initializeBMI();
    initializeBME();
}

void Sensors::initializeBMI()
{
    /* You may assign a chip select identifier to be handled later */
    dev_bmi.id = ID_BMI;
    dev_bmi.interface = BMI160_SPI_INTF;
    dev_bmi.read = user_spi_read;
    dev_bmi.write = user_spi_write;
    dev_bmi.delay_ms = user_delay_ms;

    int8_t rslt = BMI160_OK;
    rslt = bmi160_init(&dev_bmi);

    if(rslt!=BMI160_OK)
    {
        Serial.print("Could not initialize BMI160");
        return;
    }

    rslt = BMI160_OK;

    /* Select the Output data rate, range of accelerometer sensor */
    dev_bmi.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
    dev_bmi.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
    dev_bmi.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

    /* Select the power mode of accelerometer sensor */
    dev_bmi.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

    /* Select the Output data rate, range of Gyroscope sensor */
    dev_bmi.gyro_cfg.odr = BMI160_GYRO_ODR_3200HZ;
    dev_bmi.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
    dev_bmi.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

    /* Select the power mode of Gyroscope sensor */
    dev_bmi.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE; 

    /* Set the sensor configuration */
    rslt = bmi160_set_sens_conf(&dev_bmi);

    if(rslt!=BMI160_OK)
    {
        Serial.print("Could not initialize BMI160");
    }
}

void Sensors::initializeBME()
{
    /* Sensor_0 interface over SPI with native chip select line */
    dev_bme.dev_id = ID_BME;
    dev_bme.intf = BME280_SPI_INTF;
    dev_bme.read = user_spi_read;
    dev_bme.write = user_spi_write;
    dev_bme.delay_ms = user_delay_ms;

    int8_t rslt = BME280_OK;
    rslt = bme280_init(&dev_bme);

    if(rslt!=BME280_OK)
    {
        Serial.print("Could not initialize BME280");
    }
}

void Sensors::update()
{
    readBMI();
}

int8_t Sensors::user_spi_read(uint8_t dev_id, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t len)
{
    SPISettings set;
    int pin = 0;
    switch(dev_id)
    {
        case ID_BMI:
            set = set_bmi;
            pin = BMI_CS; // Gpio used as a chip select
            break;
        case ID_BME:
            set = set_bme;
            pin = BME_CS;
            break;
    }

    //send all bytes
    SPI.beginTransaction(set);
    digitalWrite (pin, LOW);

    SPI.transfer(reg_addr);

    for(int i=0;i<len;++i)
        reg_data[i] = SPI.transfer(0); //where to store the bytes

    digitalWrite (pin, HIGH);
    SPI.endTransaction();
}

int8_t Sensors::user_spi_write(uint8_t dev_id, uint8_t reg_addr,
                            uint8_t *reg_data, uint16_t len)
{
    SPISettings set;
    int pin = 0;
    switch(dev_id)
    {
        case ID_BMI:
            set = set_bmi;
            pin = BMI_CS; // Gpio used as a chip select
            break;
        case ID_BME:
            set = set_bme;
            pin = BME_CS;
            break;
    }

    //send all bytes
    SPI.beginTransaction(set);
    digitalWrite (pin, LOW);

    SPI.transfer(reg_addr); // Write the register address, ignore the return

    for(int i=0;i<len;++i)
        SPI.transfer(reg_data[i]);

    digitalWrite (pin, HIGH);
    SPI.endTransaction();
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    delay(milliseconds);
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

}