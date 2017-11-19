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
    pinMode (BMI_PIN, OUTPUT);
    pinMode (BME_PIN, OUTPUT);
    SPI.begin();

    /* You may assign a chip select identifier to be handled later */
    dev_bmi.id = ID_BMI;
    dev_bmi.interface = BMI160_SPI_INTF;
    dev_bmi.read = user_spi_read;
    dev_bmi.write = user_spi_write;
    dev_bmi.delay_ms = user_delay_ms;


    int8_t rslt = BMI160_OK;
    rslt = bmi160_init(&dev_bmi);

    rslt = BME280_OK;

    /* Sensor_0 interface over SPI with native chip select line */
    dev_bme.dev_id = ID_BME;
    dev_bme.intf = BME280_SPI_INTF;
    dev_bme.read = user_spi_read;
    dev_bme.write = user_spi_write;
    dev_bme.delay_ms = user_delay_ms;

    rslt = bme280_init(&dev_bme);
}

void Sensors::update()
{

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
            break;
    }

    //send all bytes
    SPI.beginTransaction(set);
    digitalWrite (pin, LOW);

    for(int i=0;i<len;++i)
        SPI.transfer(0); //where to store the bytes

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
        break;
    }

    //send all bytes
    SPI.beginTransaction(set);
    digitalWrite (pin, LOW);

    for(int i=0;i<len;++i)
        SPI.transfer(reg_data[i]);

    digitalWrite (pin, HIGH);
    SPI.endTransaction();
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    delay(milliseconds);
}