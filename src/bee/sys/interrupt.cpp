#include "interrupt.h"

#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/nvic.h>

#include "clock.h"

//static unsigned long rising_start[] = { 0, 0, 0, 0, 0, 0 };
//static long channel_length[6];

//bool isRising(int idr)
//{
//    return (idr & 1) == 1;
//}

//void gpioe_isr(void)
//{
    /*
    uint8_t serviced_irqs = 0;

    if (gpio_is_interrupt_source(GPIOE, GPIO0))
    {
        //do sth
        serviced_irqs |= GPIO0;
    }

    gpio_clear_interrupt_flag(GPIOE, serviced_irqs);


    /*
    if (isRising(GPIOE_IDR)) rising_start[0] = SysTick::millis();
    else channel_length[0] = SysTick::millis() - rising_start[0];
    exti_reset_request(EXTI0);
    */
//}

Interrupt::Interrupt()
{
    //gpio_configure_trigger(GPIOE, GPIO_TRIG_BOTH, GPIO0);
    //gpio_enable_interrupts(GPIOE, GPIO0);
    //nvic_enable_irq(NVIC_GPIOE_IRQ);

    //rcc_periph_clock_enable(RCC_GPIOE);
    //gpio_mode_setup(GPIOE, GPIO_MODE_INPUT, GPIO_PUPD_NONE, GPIO0);
    //exti_select_source(EXTI0, GPIOE);
    //exti_set_trigger(EXTI0, EXTI_TRIGGER_BOTH);
    //exti_enable_request(EXTI0);
}
