#pragma once
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

static const rcc_periph_clken LED_STATUS_RCC_PORT = RCC_GPIOC;
static const uint32_t LED_STATUS_PORT = GPIOC;
static const uint32_t LED_STATUS_PIN = GPIO12;


static const uint32_t USART_BAUD = 9600;
static const rcc_periph_clken USART_RCC_PORT = RCC_GPIOC;
static const rcc_periph_clken USART_RCC_USART_PORT = RCC_UART4;
static const uint32_t USART_PORT = GPIOC;
static const uint32_t USART_TX = GPIO10;
static const uint32_t USART_RX = GPIO11;
static const uint32_t USART_ = UART4_BASE;
static const uint32_t USART_AF = GPIO_AF8;

static const rcc_periph_clken SPI_RCC_SPI_PORT = RCC_SPI1;
static const rcc_periph_clken SPI_RCC_PORT = RCC_GPIOA;
static const uint32_t SPI_PORT = GPIOA;
static const uint32_t SPI_SS = GPIO4;
static const uint32_t SPI_SCK = GPIO5;
static const uint32_t SPI_MISO = GPIO6;
static const uint32_t SPI_MOSI = GPIO7;
static const uint32_t SPI = SPI1;
static const uint32_t SPI_AF = GPIO_AF5;

static const uint32_t BME280_CS_PORT = GPIOA;
static const uint32_t BME280_CS_PIN = GPIO3;

static const uint32_t ORG1510_CS_PORT = GPIOD;
static const uint32_t ORG1510_CS_PIN = GPIO8;

static const uint32_t BMP388_CS_PORT = GPIOB;
static const uint32_t BMP388_CS_PIN = GPIO2;

static const uint32_t BMI088_GYRO_CS_PORT = GPIOE;
static const uint32_t BMI088_GYRO_CS_PIN = GPIO11;
static const uint32_t BMI088_ACCEL_CS_PORT = GPIOE;
static const uint32_t BMI088_ACCEL_CS_PIN = GPIO12;

static const rcc_periph_clken MOTORS_RCC_TIMER = RCC_TIM3;
static const uint32_t MOTORS_TIMER = TIM3;

static const rcc_periph_clken MOTOR_1_RCC_PORT = RCC_GPIOB;
static const uint32_t MOTOR_1_PORT = GPIOB;
static const uint32_t MOTOR_1_PIN = GPIO6;
static const tim_oc_id MOTOR_1_CHANNEL = TIM_OC1;

static const rcc_periph_clken MOTOR_2_RCC_PORT = RCC_GPIOB;
static const uint32_t MOTOR_2_PORT = GPIOB;
static const uint32_t MOTOR_2_PIN = GPIO7;
static const tim_oc_id MOTOR_2_CHANNEL = TIM_OC2;

static const rcc_periph_clken MOTOR_3_RCC_PORT = RCC_GPIOB;
static const uint32_t MOTOR_3_PORT = GPIOB;
static const uint32_t MOTOR_3_PIN = GPIO9;
static const tim_oc_id MOTOR_3_CHANNEL = TIM_OC3;

static const rcc_periph_clken MOTOR_4_RCC_PORT = RCC_GPIOB;
static const uint32_t MOTOR_4_PORT = GPIOB;
static const uint32_t MOTOR_4_PIN = GPIO8;
static const tim_oc_id MOTOR_4_CHANNEL = TIM_OC4;
