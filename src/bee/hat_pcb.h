#pragma once
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

//TODO: make struct LED / PIN
static const rcc_periph_clken LED_STATUS_RCC_PORT = RCC_GPIOD;
static const uint32_t LED_STATUS_PORT = GPIOD;
static const uint32_t LED_STATUS_PIN = GPIO7;

static const rcc_periph_clken LED_BLUE_RCC_PORT = RCC_GPIOA;
static const uint32_t LED_BLUE_PORT = GPIOA;
static const uint32_t LED_BLUE_PIN = GPIO2;

static const rcc_periph_clken LED_ORANGE_RCC_PORT = RCC_GPIOE;
static const uint32_t LED_ORANGE_PORT = GPIOE;
static const uint32_t LED_ORANGE_PIN = GPIO15;


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

static const uint32_t BMP388_CS_PORT = GPIOD;
static const uint32_t BMP388_CS_PIN = GPIO13;
static const uint32_t BMP388_INT_PORT = GPIOD;
static const uint32_t BMP388_INT_PIN = GPIO12;

static const uint32_t BMM150_CS_PORT = GPIOD;
static const uint32_t BMM150_CS_PIN = GPIO9;
static const uint32_t BMM150_INT_PORT = GPIOD;
static const uint32_t BMM150_INT_PIN = GPIO10;
static const uint32_t BMM150_DRY_PORT = GPIOD;
static const uint32_t BMM150_DRY_PIN = GPIO8;

static const uint32_t BMI088_GYRO_CS_PORT = GPIOE;
static const uint32_t BMI088_GYRO_CS_PIN = GPIO14;
static const uint32_t BMI088_ACCEL_CS_PORT = GPIOE;
static const uint32_t BMI088_ACCEL_CS_PIN = GPIO12;
static const uint32_t BMI088_GYRO_INT_PORT = GPIOE;
static const uint32_t BMI088_GYRO_INT_PIN = GPIO11;
static const uint32_t BMI088_ACCEL_INT_PORT = GPIOE;
static const uint32_t BMI088_ACCEL_INT_PIN = GPIO13;

static const uint32_t ORG1510_CS_PORT = GPIOE;
static const uint32_t ORG1510_CS_PIN = GPIO8;


static const uint32_t ULTRASONIC_TRIGG_PORT = GPIOC;
static const uint32_t ULTRASONIC_TRIGG_PIN = GPIO13;
static const uint32_t ULTRASONIC_ECHO_PORT = GPIOE;
static const uint32_t ULTRASONIC_ECHO_PIN = GPIO6;


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


static const uint32_t REC_CHANNEL_1_EXTI = EXTI2;
static const uint32_t REC_CHANNEL_1_EXTI_IRQ = NVIC_EXTI2_IRQ;
static const rcc_periph_clken REC_CHANNEL_1_RCC_PORT = RCC_GPIOE;
static const uint32_t REC_CHANNEL_1_IDR = GPIOE_IDR;
static const uint32_t REC_CHANNEL_1_PORT = GPIOE;
static const uint32_t REC_CHANNEL_1_PIN = GPIO2;

static const uint32_t REC_CHANNEL_2_EXTI = EXTI3;
static const uint32_t REC_CHANNEL_2_EXTI_IRQ = NVIC_EXTI3_IRQ;
static const rcc_periph_clken REC_CHANNEL_2_RCC_PORT = RCC_GPIOE;
static const uint32_t REC_CHANNEL_2_IDR = GPIOE_IDR;
static const uint32_t REC_CHANNEL_2_PORT = GPIOE;
static const uint32_t REC_CHANNEL_2_PIN = GPIO3;

static const uint32_t REC_CHANNEL_3_EXTI = EXTI4;
static const uint32_t REC_CHANNEL_3_EXTI_IRQ = NVIC_EXTI4_IRQ;
static const rcc_periph_clken REC_CHANNEL_3_RCC_PORT = RCC_GPIOE;
static const uint32_t REC_CHANNEL_3_IDR = GPIOE_IDR;
static const uint32_t REC_CHANNEL_3_PORT = GPIOE;
static const uint32_t REC_CHANNEL_3_PIN = GPIO4;

//static const uint32_t REC_CHANNEL_4_EXTI = EXTI5;
//static const uint32_t REC_CHANNEL_4_EXTI_IRQ = NVIC_EXTI5_IRQ; // nvic_exti5_irq not found
//static const rcc_periph_clken REC_CHANNEL_4_RCC_PORT = RCC_GPIOE;
//static const uint32_t REC_CHANNEL_4_IDR = GPIOE_IDR;
//static const uint32_t REC_CHANNEL_4_PORT = GPIOE;
//static const uint32_t REC_CHANNEL_4_PIN = GPIO5;

static const uint32_t REC_CHANNEL_5_EXTI = EXTI1;
static const uint32_t REC_CHANNEL_5_EXTI_IRQ = NVIC_EXTI1_IRQ;
static const rcc_periph_clken REC_CHANNEL_5_RCC_PORT = RCC_GPIOE;
static const uint32_t REC_CHANNEL_5_IDR = GPIOE_IDR;
static const uint32_t REC_CHANNEL_5_PORT = GPIOE;
static const uint32_t REC_CHANNEL_5_PIN = GPIO1;

static const uint32_t REC_CHANNEL_6_EXTI = EXTI0;
static const uint32_t REC_CHANNEL_6_EXTI_IRQ = NVIC_EXTI0_IRQ;
static const rcc_periph_clken REC_CHANNEL_6_RCC_PORT = RCC_GPIOE;
static const uint32_t REC_CHANNEL_6_IDR = GPIOE_IDR;
static const uint32_t REC_CHANNEL_6_PORT = GPIOE;
static const uint32_t REC_CHANNEL_6_PIN = GPIO0;
