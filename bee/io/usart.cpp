#include "usart.h"

#include "../hat_pcb.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

bool USART::m_ready = false;

void USART::begin(int baud)
{
	if (!m_ready)
	{
		rcc_periph_clock_enable(RCC_GPIOA); 
		rcc_periph_clock_enable(RCC_USART1);

		gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

		usart_set_databits(USART1, 8);
		usart_set_stopbits(USART1, USART_STOPBITS_1);
		usart_set_mode(USART1, USART_MODE_TX);
		usart_set_parity(USART1, USART_PARITY_NONE);
		usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
		
		m_ready = true;
	}
	
	usart_set_baudrate(USART1, baud);
	usart_enable(USART1);
}

void USART::write(uint16_t data)
{
	usart_send_blocking(USART1, data);
}

