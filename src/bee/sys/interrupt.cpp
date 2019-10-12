#include "interrupt.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/exti.h>

static unsigned long rising_start[] = { 0, 0, 0, 0, 0, 0 };
static uint8_t channel_pin[6];
static long channel_length[6];

bool isRising(int idr) return (idr & 1) == 1;

void exti0_isr(void)
{
    if (isRising(GPIOE_IDR)) rising_start[0] = SysTick::system_millis;
    else channel_length[0] = SysTick::system_millis - rising_start[0];
    exti_reset_request(EXTI0);
}

Interrupt::Interrupt(InterruptPin iPin)
{
    rcc_periph_clock_enable(RCC_AFIO);
    
    nvic_enable_irq(NVIC_EXTI0_IRQ);
    rcc_periph_clock_enable(RCC_GPIOE);
    gpio_set_mode(GPIOE, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO0);
    exti_select_source(EXTI0, port);
    exti_set_trigger(EXTI0, EXTI_TRIGGER_BOTH);
    exti_enable_request(EXTI0);
}