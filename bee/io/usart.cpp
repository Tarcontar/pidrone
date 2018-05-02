#include "usart.h"

#include "../hat_pcb.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

bool USART::m_ready = false;

void USART::setup()
{
	rcc_periph_clock_enable(_USART_RCC_PORT);
	rcc_periph_clock_enable(_USART_RCC_USART_PORT);

	gpio_set_mode(_USART_PORT, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, _USART_TX);

	usart_set_databits(_USART, 8);
	usart_set_stopbits(_USART, USART_STOPBITS_1);
	usart_set_mode(_USART, USART_MODE_TX);
	usart_set_parity(_USART, USART_PARITY_NONE);
	usart_set_flow_control(_USART, USART_FLOWCONTROL_NONE);

	usart_set_baudrate(_USART, _USART_BAUD);
	usart_enable(_USART);

	m_ready = true;
}

void USART::write(uint16_t data)
{
	if (!m_ready)
		setup();
	usart_send_blocking(_USART, data);
}
