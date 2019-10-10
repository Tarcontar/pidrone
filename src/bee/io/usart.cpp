#include "usart.h"

#include "../hat_pcb.h"

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

void USART::setup()
{
    rcc_periph_clock_enable(USART_RCC_PORT);
    rcc_periph_clock_enable(USART_RCC_USART_PORT);

    gpio_mode_setup(USART_PORT, GPIO_MODE_AF, GPIO_PUPD_NONE, USART_TX | USART_RX);
    gpio_set_af(USART_PORT, GPIO_AF8, GPIO10);

    usart_set_baudrate(USART_, 9600);
    usart_set_databits(USART_, 8);
    usart_set_stopbits(USART_, USART_STOPBITS_1);
    usart_set_mode(USART_, USART_MODE_TX);
    usart_set_parity(USART_, USART_PARITY_NONE);
    usart_set_flow_control(USART_, USART_FLOWCONTROL_NONE);

    usart_enable(USART_);
}

void USART::write(uint16_t data)
{
    usart_send_blocking(USART_, data);
}

extern "C"
{
    int _write(int file, char *ptr, int len)
    {
        int i;

        if (file == STDOUT_FILENO || file == STDERR_FILENO)
        {
            for (i = 0; i < len; i++)
            {
                if (ptr[i] == '\n')
                {
                    USART::write('\r');
                }
                USART::write(ptr[i]);
            }
            return i;
        }
        errno = EIO;
        return -1;
    }
}
