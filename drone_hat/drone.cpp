#include <Arduino.h>

#include <SPI.h>
#include <bmi160.h>

/* Helper functions */
int8_t spiTransfer(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length);
void delay_ms(uint32_t period)
{
  delay(period);
}

/* Constants and variables */
SPISettings set_bmi(1000000, MSBFIRST, SPI_MODE0);
const uint8_t bmi160_cs = 8;
struct bmi160_dev bmi_dev;
String output;

/* Application code execution starts here */
void setup() {
  /* Initialize debug serial communication at 9600 bits per second: */
  Serial.begin(9600);

  /* Initialize sensor interface */
  pinMode(bmi160_cs, OUTPUT);
  SPI.begin();

  /* Set the inferace configurations */
  bmi_dev.id = bmi160_cs;
  bmi_dev.read = spiTransfer; // Duplex
  bmi_dev.write = spiTransfer; // Duplex
  bmi_dev.delay_ms = delay_ms;
  bmi_dev.interface = BMI160_SPI_INTF;

  /* Initialize the BMI160 */
  output = "BMI160 init "  + String(bmi160_init(&bmi_dev)) + " , chip ID: " + String(bmi_dev.chip_id, HEX);
  Serial.println(output);

  /* Select the Output data rate, range of accelerometer sensor */
  bmi_dev.accel_cfg.odr = BMI160_ACCEL_ODR_100HZ;
  bmi_dev.accel_cfg.range = BMI160_ACCEL_RANGE_2G;
  bmi_dev.accel_cfg.bw = BMI160_ACCEL_BW_NORMAL_AVG4;

  /* Select the power mode of accelerometer sensor */
  bmi_dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

  /* Select the Output data rate, range of Gyroscope sensor */
  bmi_dev.gyro_cfg.odr = BMI160_GYRO_ODR_100HZ;
  bmi_dev.gyro_cfg.range = BMI160_GYRO_RANGE_2000_DPS;
  bmi_dev.gyro_cfg.bw = BMI160_GYRO_BW_NORMAL_MODE;

  /* Select the power mode of Gyroscope sensor */
  bmi_dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

  /* Set the sensor configuration */
  output = "BMI160 set sensor config:  " + String(bmi160_set_sens_conf(&bmi_dev));
  Serial.println(output);
}

/* The loop routine runs over and over again forever after the setup */
void loop() {
  bmi160_sensor_data accel;
  bmi160_sensor_data gyro;

  /* To read both Accel and Gyro data */
  output = "R: " + String(bmi160_get_sensor_data((BMI160_ACCEL_SEL | BMI160_GYRO_SEL), &accel, &gyro, &bmi_dev));
  output += " A: " + String(accel.x) + ", "  + String(accel.y) + ", " + String(accel.z);
  output += " G: " + String(gyro.x) + ", "  + String(gyro.y) + ", " + String(gyro.z);
  Serial.println(output);
  delay(100);
}

int8_t spiTransfer(uint8_t cs, uint8_t reg_addr, uint8_t *reg_data, uint16_t length)
{
  SPI.beginTransaction(set_bmi);
  digitalWrite(cs, LOW);
  SPI.transfer(reg_addr);
  for (uint8_t i = 0; i < length; i++)
    reg_data[i] = SPI.transfer(reg_data[i]);
  digitalWrite(8, HIGH);
  SPI.endTransaction();

  return 0;
}