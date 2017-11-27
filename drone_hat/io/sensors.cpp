#include "sensors.h"
#include "../hat_pcb.h"
#include <Arduino.h>
#include <SPI.h>
#include <bmi160.h>
#include <bme280.h>
#include <TinyGPS.h>

// set up the speed, data order and data mode
SPISettings set_bmi(1000000, MSBFIRST, SPI_MODE0);
SPISettings set_bme(1000000, MSBFIRST, SPI_MODE0);
SPISettings set_gps(1000000, MSBFIRST, SPI_MODE0);

struct bmi160_dev dev_bmi;
struct bme280_dev dev_bme;

TinyGPS gps;

bool Sensors::setup()
{
    pinMode (BMI_CS, OUTPUT);
    digitalWrite(BMI_CS,HIGH);
    pinMode (BME_CS, OUTPUT);
    digitalWrite(BME_CS,HIGH);
    pinMode(GPS_CS,OUTPUT);
    digitalWrite(BME_CS,HIGH);

    SPI.begin();
    if (!initializeBMI())
		return false;
    if (!initializeBME())
		return false;
	return true;
}

int8_t Sensors::spi_transfer(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
    SPISettings set;
    switch(cs)
    {
        case BMI_CS:
            set = set_bmi;
            break;
        case BME_CS:
            set = set_bme;
            break;
    }

    SPI.beginTransaction(set);
    digitalWrite (cs, LOW);

    SPI.transfer(reg_addr); 

    for(uint16_t i = 0; i < len; i++)
        reg_data[i] = SPI.transfer(reg_data[i]);

    digitalWrite (cs, HIGH);
    SPI.endTransaction();

    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
    delay(milliseconds);
}

bool Sensors::initializeBMI()
{
    /* You may assign a chip select identifier to be handled later */
    dev_bmi.id = BMI_CS;
    dev_bmi.interface = BMI160_SPI_INTF;
    dev_bmi.read = spi_transfer;
    dev_bmi.write = spi_transfer;
    dev_bmi.delay_ms = user_delay_ms;

    int8_t rslt = BMI160_OK;
    rslt = bmi160_init(&dev_bmi);

    if(rslt != BMI160_OK)
    {
        Serial.print("Could not initialize BMI160: ");
        Serial.println(rslt);
        return false;
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
        Serial.print("Could not initialize BMI160: ");
        Serial.println(rslt);
		return false;
    }

    rslt = bmi160_perform_self_test((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &dev_bmi);

    if(rslt != BMI160_OK)
    {
        Serial.print("BMI160 self test failed: ");
        Serial.println(rslt);
		//return false;
    }
	else
	{
		Serial.println("BMI160 ready");
	}
	return true;
}

bool Sensors::initializeBME()
{
    dev_bme.dev_id = BME_CS;
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
		return false;
    }

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
	   return false;
    }
	else
	{
		Serial.println("BME280 ready");
	}
	return true;
}

void Sensors::readBMI()
{
    bmi160_sensor_data accel;
    bmi160_sensor_data gyro;
    int8_t rslt = BMI160_OK;

    /* To read both Accel and Gyro data */
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
}

void Sensors::readGPS()
{

    SPI.beginTransaction(set_gps);
    digitalWrite (GPS_CS, LOW);

    SPI.transfer(reg_addr); 

    //read until new data is available
    bool receivedData = false;

    while(!receivedData)
    {
        char c = SP.transfer(0);
        Serial.write(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
            receivedData = true;
    }

    digitalWrite (GPS_CS, HIGH);
    SPI.endTransaction();

}

void Sensors::readBME()
{
    bme280_data comp_data;
    int8_t rslt = BMI160_OK;
    rslt = bme280_get_sensor_data(BME280_ALL, &comp_data, &dev_bme);
	if(rslt != BMI160_OK)
    {
       Serial.print("Could not read BME280: ");
       Serial.println(rslt);
	   return;
    }

	Serial.print("\n Temp: ");
	Serial.print(comp_data.temperature);
	Serial.print(" humidity: ");
	Serial.print(comp_data.humidity);
	Serial.print(" pressure: ");
	Serial.print(comp_data.pressure);
	Serial.println();
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
    readBMI();
    readGPS();
}
