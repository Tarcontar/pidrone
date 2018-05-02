#pragma once
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>

static const rcc_periph_clken 	_LED_STATUS_RCC_PORT 	= RCC_GPIOC;
static const uint32_t 		_LED_STATUS_PORT 	= GPIOC;
static const uint32_t 		_LED_STATUS_PIN 	= GPIO8;

static const uint32_t		_USART_BAUD		= 9600;
static const rcc_periph_clken 	_USART_RCC_PORT 	= RCC_GPIOA;
static const rcc_periph_clken 	_USART_RCC_USART_PORT 	= RCC_USART1;
static const uint32_t 		_USART_PORT 		= GPIOA;
static const uint32_t 		_USART_TX 		= GPIO_USART1_TX;
static const uint32_t 		_USART 			= USART1;

static const rcc_periph_clken	_SPI_RCC_SPI_PORT 	= RCC_SPI1;
static const rcc_periph_clken	_SPI_RCC_PORT 		= RCC_GPIOA;
static const uint32_t		_SPI_PORT 		= GPIOA;
static const uint32_t		_SPI_SS 		= GPIO4;
static const uint32_t		_SPI_SCK 		= GPIO5;
static const uint32_t		_SPI_MISO 		= GPIO6;
static const uint32_t		_SPI_MOSI 		= GPIO7;
static const uint32_t 		_SPI 			= SPI1;

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
