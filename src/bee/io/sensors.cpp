#include "sensors.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/spi.h>
// #include <bmi160.h>
// #include <bme280.h>

#include "../hat_pcb.h"
#include "serial.h"

/*
#include <TinyGPS.h>
// set up the speed, data order and data mode
SPISettings set_gps(4000, MSBFIRST, SPI_MODE1);
TinyGPS gps;
*/

enum DEVICES
{
	BMI = 0,
	BME = 1
};

struct SPI_DEVICE
{
	uint32_t port;
	uint32_t pin;
};

SPI_DEVICE devices[2] = {{GPIOA, GPIO1}, {GPIOA, GPIO2}};

// struct bmi160_dev dev_bmi; //1000000 msbfirst, spimode0
// struct bme280_dev dev_bme;

bool Sensors::setup()
{
	//setup spi_transfer
	rcc_periph_clock_enable(SPI_RCC_PORT);
	rcc_periph_clock_enable(SPI_RCC_SPI_PORT);

	gpio_set_mode(SPI_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, SPI_SS | SPI_SCK | SPI_MOSI);
	gpio_set_mode(SPI_PORT, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, SPI_MISO);

	spi_reset(SPI);
	spi_init_master(SPI, SPI_CR1_BAUDRATE_FPCLK_DIV_64, SPI_CR1_CPOL_CLK_TO_1_WHEN_IDLE,
			SPI_CR1_CPHA_CLK_TRANSITION_2, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);

	//needed even if we handle the slave selects ourselves
	spi_enable_software_slave_management(SPI);
	spi_set_nss_high(SPI);
	
	spi_enable(SPI);
	
	//TODO: iterate over devices and set pins high

    if (!initializeBMI())
		return false;
	/*
    if (!initializeBME())
		return false;
	*/
	return true;
}

int8_t Sensors::spi_transfer(uint8_t device_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len)
{
	gpio_clear(devices[device_id].port, devices[device_id].pin);
	
	spi_write(SPI, reg_addr);

    for(uint16_t i = 0; i < len; i++)
        reg_data[i] = spi_xfer(_SPI, reg_data[i]);

	gpio_set(devices[device_id].port, devices[device_id].pin);
    return 0;
}

void Sensors::user_delay_ms(uint32_t milliseconds)
{
	uint32_t prescaler = 24000; //depends on clockrate
    for (uint32_t i = 0; i < milliseconds * prescaler; i++)
			__asm__("NOP");
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
	/*
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
	*/
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
    Serial.println("Read GPS");
    SPI.beginTransaction(set_gps);
    digitalWrite (GPS_CS, LOW);

    //read until new data is available
    bool receivedData = false;

    while(!receivedData)
    {
	//Serial.println("Transfer");
        char c = SPI.transfer(0);
        Serial.print(c); // uncomment this line if you want to see the GPS data flowing
        if (gps.encode(c)) // Did a new valid sentence come in?
            receivedData = true;
    }

    digitalWrite (GPS_CS, HIGH);
    SPI.endTransaction();
    Serial.println("Received sth");
	*/
}

void Sensors::readBME()
{
	/*
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
	*/
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
    //readBME();
    //readBMI();
    //readGPS();
}
