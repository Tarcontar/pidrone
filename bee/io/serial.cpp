#include "serial.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

bool Serial::m_ready = false;

Serial::Serial(int baud)
{
	//replace this with a singleton
	if (!m_ready)
	{
		m_usart = new USART();
		m_ready = true;
	}
	m_usart->begin(baud);
}

void Serial::operator<<(char c) const
{
	putc(c);
}

void Serial::operator<<(const char *str) const
{
	char *it = const_cast<char *>(str);
	while (it)
	{
		putc(*it);
		++it;
	}
}

void Serial::putc(char c) const
{
	m_usart->write(c);
}