#pragma once
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>

static const rcc_periph_clken 		_LED_STATUS_RCC_PORT 	= RCC_GPIOC;
static const uint32_t 				_LED_STATUS_PORT 		= GPIOC;
static const uint32_t 				_LED_STATUS_PIN 		= GPIO8;

static const rcc_periph_clken 		_USART_RCC_PORT 		= RCC_GPIOA;
static const rcc_periph_clken 		_USART_RCC_USART_PORT 	= RCC_USART1;
static const uint32_t 				_USART_PORT 			= GPIOA;
static const uint32_t 				_USART_TX 				= GPIO_USART1_TX;
static const uint32_t 				_USART 					= USART1;


static const rcc_periph_clken _MOTORS_RCC_TIMER = RCC_TIM3;
static const uint32_t _MOTORS_TIMER = TIM3;

static const rcc_periph_clken _MOTOR_1_RCC_PORT = RCC_GPIOA;
static const uint32_t _MOTOR_1_PORT = GPIOA;
static const uint32_t _MOTOR_1_PIN = GPIO6;
static const tim_oc_id _MOTOR_1_CHANNEL = TIM_OC1;

static const rcc_periph_clken _MOTOR_2_RCC_PORT = RCC_GPIOA;
static const uint32_t _MOTOR_2_PORT = GPIOA;
static const uint32_t _MOTOR_2_PIN = GPIO7;
static const tim_oc_id _MOTOR_2_CHANNEL = TIM_OC2;

static const rcc_periph_clken _MOTOR_3_RCC_PORT = RCC_GPIOB;
static const uint32_t _MOTOR_3_PORT = GPIOB;
static const uint32_t _MOTOR_3_PIN = GPIO0;
static const tim_oc_id _MOTOR_3_CHANNEL = TIM_OC3;

static const rcc_periph_clken _MOTOR_4_RCC_PORT = RCC_GPIOB;
static const uint32_t _MOTOR_4_PORT = GPIOB;
static const uint32_t _MOTOR_4_PIN = GPIO1;
static const tim_oc_id _MOTOR_4_CHANNEL = TIM_OC4;


/*
static const uint8_t RPI_SPI_CS_PORT = GPIOB;
static const uint8_t RPI_SPI_CS_PIN = GPIO12;
static const uint8_t RPI_SPI_SCK_PORT = GPIOB;
static const uint8_t RPI_SPI_SCK_PIN = GPIO13;
static const uint8_t RPI_SPI_MISO_PORT = GPIOB;
static const uint8_t RPI_SPI_MISO_PIN = GPIO14;
static const uint8_t RPI_SPI_MOSI_PORT = GPIOB;
static const uint8_t RPI_SPI_MOSI_PIN = GPIO15;

static const uint8_t SPI_SCK_PORT = GPIOA;
static const uint8_t SPI_SCK_PIN = GPIO5;
static const uint8_t SPI_MISO_PORT = GPIOA;
static const uint8_t SPI_MISO_PIN = GPIO6;
static const uint8_t SPI_MOSI_PORT = GPIOA;
static const uint8_t SPI_MOSI_PIN = GPIO7;

static const uint8_t BMI_GYRO_CS_PORT = GPIOE;
static const uint8_t BMI_GYRO_CS_PIN = GPIO11;
static const uint8_t BMI_GYRO_INT_PORT = GPIOE;
static const uint8_t BMI_GYRO_INT_PIN = GPIO13;

static const uint8_t BMI_ACCEL_CS_PORT = GPIOE;
static const uint8_t BMI_ACCEL_CS_PIN = GPIO12;
static const uint8_t BMI_ACCEL_INT_PORT = GPIOE;
static const uint8_t BMI_ACCEL_INT_PIN = GPIO14;

static const uint8_t BME_CS_PORT = GPIOA;
static const uint8_t BME_CS_PIN = GPIO3;

static const uint8_t BMM_CS_PORT = GPIOB;
static const uint8_t BMM_CS_PIN = GPIO10;
static const uint8_t BMM_INT_PORT = GPIOB;
static const uint8_t BMM_INT_PIN = GPIO11;
static const uint8_t BMM_DRY_PORT = GPIOE;
static const uint8_t BMM_DRY_PIN = GPIO15;

static const uint8_t BMP_CS_PORT = GPIOB;
static const uint8_t BMP_CS_PIN = GPIO2;
static const uint8_t BMP_INT_PORT = GPIOE;
static const uint8_t BMP_INT_PIN = GPIO7;


static const uint8_t CH1_PORT = GPIOE;
static const uint8_t CH1_PIN = GPIO2;
static const uint8_t CH2_PORT = GPIOE;
static const uint8_t CH2_PIN = GPIO3;
static const uint8_t CH3_PORT = GPIOE;
static const uint8_t CH3_PIN = GPIO4;
static const uint8_t CH4_PORT = GPIOE;
static const uint8_t CH4_PIN = GPIO5;
static const uint8_t CH5_PORT = GPIOE;
static const uint8_t CH5_PIN = GPIO1;
static const uint8_t CH6_PORT = GPIOE;
static const uint8_t CH6_PIN = GPIO0;

static const uint8_t US_TRIGG_PORT = GPIOC;
static const uint8_t US_TRIGG_PIN = GPIO13;
static const uint8_t US_ECHO_PORT = GPIOE;
static const uint8_t US_ECHO_PIN = GPIO5;
*/
